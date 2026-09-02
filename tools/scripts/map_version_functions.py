#!/usr/bin/env python3
"""Map PAL function names into another Rage Racer executable.

The regional builds keep almost all functions in the same order but relocate
them as code and localized data change size.  A raw byte search therefore
misses every body containing an address.  This tool compares an instruction
skeleton instead: R-type and COP instructions are retained, while jump targets
and I-type immediates are ignored.  Only complete, same-sized function bodies
are mapped.  Functions whose structure changed are intentionally left for the
regional assembly split.
"""

from __future__ import annotations

import argparse
import json
import re
import struct
import subprocess
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path


LOAD_ADDRESS = 0x80010000
HEADER_SIZE = 0x800
READELF_FUNC = re.compile(
    r"^\s*\d+:\s+([0-9a-fA-F]+)\s+(\d+)\s+FUNC\s+"
    r"(?:LOCAL|GLOBAL|WEAK)\s+DEFAULT\s+\S+\s+(\S+)\s*$"
)
SYMBOL_ASSIGNMENT = re.compile(
    r"^([A-Za-z_$][A-Za-z0-9_$]*)\s*=\s*(0x[0-9a-fA-F]+)"
)


@dataclass
class Function:
    address: int
    size: int
    names: list[str]
    exact: bool = False
    candidates: list[int] | None = None
    target: int | None = None


def normalize_word(word: int) -> int:
    opcode = word >> 26
    if opcode == 0 or opcode in (16, 17, 18, 19):
        return word
    if opcode in (2, 3):
        return opcode << 26
    return word & 0xFFFF0000


def normalize(data: bytes) -> bytes:
    size = len(data) & ~3
    return b"".join(
        struct.pack("<I", normalize_word(word))
        for (word,) in struct.iter_unpack("<I", data[:size])
    )


def read_functions(readelf: str, elf: Path) -> list[Function]:
    result = subprocess.run(
        [readelf, "-sW", str(elf)], check=True, text=True, capture_output=True
    )
    grouped: dict[tuple[int, int], list[str]] = defaultdict(list)
    for line in result.stdout.splitlines():
        match = READELF_FUNC.match(line)
        if not match:
            continue
        address, size, name = int(match.group(1), 16), int(match.group(2)), match.group(3)
        if size < 8 or name.startswith(".L") or not (LOAD_ADDRESS <= address):
            continue
        grouped[(address, size)].append(name)
    return [
        Function(address, size, sorted(set(names), key=name_rank))
        for (address, size), names in sorted(grouped.items())
    ]


def name_rank(name: str) -> tuple[int, int, str]:
    raw = name.startswith(("func_", "D_"))
    return (raw, len(name), name)


def occurrences(haystack: bytes, needle: bytes) -> list[int]:
    found: list[int] = []
    start = 0
    while True:
        pos = haystack.find(needle, start)
        if pos < 0:
            return found
        if pos % 4 == 0:
            found.append(LOAD_ADDRESS + pos)
        start = pos + 4


def expected_address(functions: list[Function], index: int) -> int | None:
    before = next(
        (functions[i] for i in range(index - 1, -1, -1) if functions[i].target is not None),
        None,
    )
    after = next(
        (functions[i] for i in range(index + 1, len(functions)) if functions[i].target is not None),
        None,
    )
    if before and after and after.address != before.address:
        source_span = after.address - before.address
        target_span = after.target - before.target
        return before.target + (functions[index].address - before.address) * target_span // source_span
    if before:
        return before.target + functions[index].address - before.address
    if after:
        return after.target - (after.address - functions[index].address)
    return None


def map_functions(source: bytes, target: bytes, functions: list[Function]) -> None:
    target_normalized = normalize(target[HEADER_SIZE:])
    for function in functions:
        offset = HEADER_SIZE + function.address - LOAD_ADDRESS
        body = source[offset : offset + function.size]
        if len(body) != function.size:
            continue
        skeleton = normalize(body)
        function.candidates = occurrences(target_normalized, skeleton)
        function.exact = target.find(body, HEADER_SIZE) >= 0
        if len(function.candidates) == 1:
            function.target = function.candidates[0]

    # Resolve repeated short helpers against already unique, monotonic anchors.
    for _ in range(3):
        changed = False
        for i, function in enumerate(functions):
            if function.target is not None or not function.candidates:
                continue
            expected = expected_address(functions, i)
            if expected is None:
                continue
            before = next((functions[j].target for j in range(i - 1, -1, -1) if functions[j].target is not None), None)
            after = next((functions[j].target for j in range(i + 1, len(functions)) if functions[j].target is not None), None)
            valid = [
                candidate
                for candidate in function.candidates
                if (before is None or candidate >= before)
                and (after is None or candidate <= after)
            ]
            if valid:
                function.target = min(valid, key=lambda candidate: abs(candidate - expected))
                changed = True
        if not changed:
            break


def read_named_addresses(paths: list[Path]) -> dict[int, list[str]]:
    grouped: dict[int, list[str]] = defaultdict(list)
    for path in paths:
        for line in path.read_text().splitlines():
            match = SYMBOL_ASSIGNMENT.match(line)
            if match and "type:func" not in line:
                grouped[int(match.group(2), 16)].append(match.group(1))
    return {
        address: sorted(set(names), key=name_rank)
        for address, names in grouped.items()
    }


def read_named_functions(paths: list[Path]) -> dict[int, list[str]]:
    grouped: dict[int, list[str]] = defaultdict(list)
    for path in paths:
        for line in path.read_text().splitlines():
            match = SYMBOL_ASSIGNMENT.match(line)
            if match and "type:func" in line:
                grouped[int(match.group(2), 16)].append(match.group(1))
    return {
        address: sorted(set(names), key=name_rank)
        for address, names in grouped.items()
    }


def signed_half(value: int) -> int:
    return value if value < 0x8000 else value - 0x10000


def infer_data_addresses(
    source: bytes,
    target: bytes,
    functions: list[Function],
    named_addresses: dict[int, list[str]],
) -> tuple[dict[int, tuple[int, int]], dict[int, dict[int, int]]]:
    """Infer regional data addresses from corresponding address loads.

    GCC emits absolute references as a ``lui`` followed shortly by an
    instruction consuming its register. Since mapped functions have identical
    instruction structure, the two immediate pairs give the PAL and regional
    addresses directly. An address is accepted only when every observation
    agrees; conflicts are reported and left to assembly.
    """

    votes: dict[int, dict[int, int]] = defaultdict(lambda: defaultdict(int))
    signed_low_opcodes = {
        9,   # addiu
        32, 33, 35, 36, 37,  # lb/lh/lw/lbu/lhu
        40, 41, 43,          # sb/sh/sw
        48, 49, 50, 51,      # lwc0..lwc3
        56, 57, 58,          # swc0..swc2
    }

    for function in functions:
        if function.target is None:
            continue
        source_offset = HEADER_SIZE + function.address - LOAD_ADDRESS
        target_offset = HEADER_SIZE + function.target - LOAD_ADDRESS
        count = function.size // 4
        source_words = struct.unpack(
            f"<{count}I", source[source_offset : source_offset + count * 4]
        )
        target_words = struct.unpack(
            f"<{count}I", target[target_offset : target_offset + count * 4]
        )
        for index, (source_hi, target_hi) in enumerate(zip(source_words, target_words)):
            if source_hi >> 26 != 15 or target_hi >> 26 != 15:
                continue
            register = (source_hi >> 16) & 31
            if register != (target_hi >> 16) & 31:
                continue
            for low_index in range(index + 1, min(index + 13, count)):
                source_low = source_words[low_index]
                target_low = target_words[low_index]
                opcode = source_low >> 26
                if opcode != target_low >> 26:
                    continue
                if (source_low >> 21) & 31 != register:
                    continue
                if (target_low >> 21) & 31 != register:
                    continue
                if opcode in signed_low_opcodes:
                    source_address = (
                        ((source_hi & 0xFFFF) << 16)
                        + signed_half(source_low & 0xFFFF)
                    )
                    target_address = (
                        ((target_hi & 0xFFFF) << 16)
                        + signed_half(target_low & 0xFFFF)
                    )
                elif opcode == 13:  # ori
                    source_address = (
                        ((source_hi & 0xFFFF) << 16) | (source_low & 0xFFFF)
                    )
                    target_address = (
                        ((target_hi & 0xFFFF) << 16) | (target_low & 0xFFFF)
                    )
                else:
                    continue
                if source_address in named_addresses:
                    votes[source_address][target_address] += 1
                    break

    mapped: dict[int, tuple[int, int]] = {}
    ambiguous: dict[int, dict[int, int]] = {}
    for source_address, candidates in votes.items():
        if len(candidates) == 1:
            target_address, references = next(iter(candidates.items()))
            mapped[source_address] = (target_address, references)
        else:
            ambiguous[source_address] = dict(candidates)
    return mapped, ambiguous


def infer_called_functions(
    source: bytes,
    target: bytes,
    functions: list[Function],
    named_functions: dict[int, list[str]],
) -> tuple[dict[int, tuple[int, int]], dict[int, dict[int, int]]]:
    """Map changed or zero-sized callees through corresponding J/JAL sites."""

    votes: dict[int, dict[int, int]] = defaultdict(lambda: defaultdict(int))
    for function in functions:
        if function.target is None:
            continue
        source_offset = HEADER_SIZE + function.address - LOAD_ADDRESS
        target_offset = HEADER_SIZE + function.target - LOAD_ADDRESS
        count = function.size // 4
        source_words = struct.unpack(
            f"<{count}I", source[source_offset : source_offset + count * 4]
        )
        target_words = struct.unpack(
            f"<{count}I", target[target_offset : target_offset + count * 4]
        )
        for source_word, target_word in zip(source_words, target_words):
            opcode = source_word >> 26
            if opcode not in (2, 3) or target_word >> 26 != opcode:
                continue
            source_address = (
                (function.address & 0xF0000000) | ((source_word & 0x03FFFFFF) << 2)
            )
            target_address = (
                (function.target & 0xF0000000) | ((target_word & 0x03FFFFFF) << 2)
            )
            if source_address in named_functions:
                votes[source_address][target_address] += 1

    mapped: dict[int, tuple[int, int]] = {}
    ambiguous: dict[int, dict[int, int]] = {}
    for source_address, candidates in votes.items():
        if len(candidates) == 1:
            target_address, references = next(iter(candidates.items()))
            mapped[source_address] = (target_address, references)
        else:
            ambiguous[source_address] = dict(candidates)
    return mapped, ambiguous


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--source-exe", type=Path, default=Path("assets/PAL/main.exe"))
    parser.add_argument("--source-elf", type=Path, default=Path("build/PAL/main.elf"))
    parser.add_argument("--target-exe", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--report", type=Path)
    parser.add_argument(
        "--source-symbols",
        type=Path,
        action="append",
        help="PAL symbol file used to infer regional data addresses (repeatable)",
    )
    parser.add_argument("--readelf", default="mipsel-none-elf-readelf")
    args = parser.parse_args()

    functions = read_functions(args.readelf, args.source_elf)
    source = args.source_exe.read_bytes()
    target = args.target_exe.read_bytes()
    map_functions(source, target, functions)
    mapped = [function for function in functions if function.target is not None]
    symbol_paths = args.source_symbols or [
        Path("configs/PAL/sym.main.txt"),
        Path("configs/PAL/sym.bss.main.txt"),
    ]
    named_addresses = read_named_addresses(symbol_paths)
    named_functions = read_named_functions(symbol_paths)
    # Some hand-written callable stubs are stored as words in .text and are
    # consequently typed as data in the PAL symbols. A J/JAL target proves
    # their callable role for regional mapping.
    for address, address_names in named_addresses.items():
        names = named_functions.setdefault(address, [])
        names.extend(name for name in address_names if name not in names)
        names.sort(key=name_rank)
    # The linked PAL ELF also contains semantic function names which predate
    # the hand-maintained symbol file. Include them so a changed callee can be
    # recovered from matching call sites even when its own body cannot map.
    for function in functions:
        names = named_functions.setdefault(function.address, [])
        names.extend(name for name in function.names if name not in names)
        names.sort(key=name_rank)
    inferred_functions, ambiguous_functions = infer_called_functions(
        source, target, functions, named_functions
    )
    mapped_data, ambiguous_data = infer_data_addresses(
        source, target, functions, named_addresses
    )

    lines = [
        "// Generated from structurally identical PAL function bodies.",
        "// Changed regional functions deliberately remain in assembly.",
    ]
    used_names: set[str] = set()
    used_addresses: set[int] = set()
    for function in mapped:
        name = next((name for name in function.names if name not in used_names), None)
        if name is None or function.target in used_addresses:
            continue
        lines.append(f"{name} = 0x{function.target:08X}; // type:func")
        used_names.add(name)
        used_addresses.add(function.target)
    lines.extend(
        [
            "",
            "// Changed callees inferred from corresponding PAL J/JAL sites.",
            "// Conflicting observations deliberately remain unnamed assembly.",
        ]
    )
    for source_address, (target_address, references) in sorted(inferred_functions.items()):
        name = next(
            (name for name in named_functions[source_address] if name not in used_names),
            None,
        )
        if name is None or target_address in used_addresses:
            continue
        lines.append(
            f"{name} = 0x{target_address:08X}; // type:func inferred from {references} call(s)"
        )
        used_names.add(name)
        used_addresses.add(target_address)
    lines.extend(
        [
            "",
            "// Data addresses inferred from matching PAL address-load pairs.",
            "// Conflicting observations deliberately remain unnamed assembly.",
        ]
    )
    for source_address, (target_address, references) in sorted(mapped_data.items()):
        name = next(
            (name for name in named_addresses[source_address] if name not in used_names),
            None,
        )
        if name is None or target_address in used_addresses:
            continue
        lines.append(
            f"{name} = 0x{target_address:08X}; // inferred from {references} reference(s)"
        )
        used_names.add(name)
        used_addresses.add(target_address)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text("\n".join(lines) + "\n")

    report = {
        "source_functions": len(functions),
        "mapped_functions": len(mapped),
        "exact_byte_functions": sum(function.exact for function in mapped),
        "inferred_called_functions": len(inferred_functions),
        "ambiguous_called_functions": len(ambiguous_functions),
        "mapped_data_symbols": len(mapped_data),
        "ambiguous_data_symbols": len(ambiguous_data),
        "unmapped": [
            {"name": function.names[0], "address": f"0x{function.address:08X}", "size": function.size}
            for function in functions
            if function.target is None
        ],
    }
    if args.report:
        args.report.parent.mkdir(parents=True, exist_ok=True)
        args.report.write_text(json.dumps(report, indent=2) + "\n")
    print(
        f"mapped {report['mapped_functions']}/{report['source_functions']} PAL functions "
        f"into {args.target_exe} ({report['exact_byte_functions']} exact-byte bodies)"
    )


if __name__ == "__main__":
    main()
