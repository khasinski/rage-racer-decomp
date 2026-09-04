#!/usr/bin/env python3
"""Reuse directly portable PAL C text units in a regional split.

The regional executables retain most compiler output in the same structural
form, but code and data move. This tool selects complete PAL translation units
whose text has a uniquely anchored structural match and whose relocatable
rodata and external symbols can be mapped unambiguously. Everything else is
emitted as regional assembly.
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
from dataclasses import dataclass
from pathlib import Path

try:
    from .map_version_functions import (
        HEADER_SIZE,
        LOAD_ADDRESS,
        normalize,
        occurrences,
        read_functions,
    )
except ImportError:  # Direct execution adds tools/scripts rather than the repo root.
    from map_version_functions import (
        HEADER_SIZE,
        LOAD_ADDRESS,
        normalize,
        occurrences,
        read_functions,
    )


SEGMENT = re.compile(
    r"^\s*- \[(0x[0-9A-Fa-f]+)(?:,\s*([^,\]]+))?(?:,\s*([^\]]+))?\]"
)
OUTPUT_SECTIONS = {".rodata", ".rdata", ".data", ".sdata", ".sbss", ".bss", ".lit4", ".lit8"}
SYMBOL_ASSIGNMENT = re.compile(
    r"^([A-Za-z_$][A-Za-z0-9_$]*)\s*=\s*(0x[0-9A-Fa-f]+)"
)
ALIASES_BEGIN = "// PAL text-unit aliases begin (generated)"
ALIASES_END = "// PAL text-unit aliases end (generated)"


@dataclass(frozen=True)
class PalTextUnit:
    start: int
    end: int
    path: str
    undefined: frozenset[str]
    rodata: tuple[int, int] | None = None


def parse_segments(config: Path) -> list[tuple[int, str, str]]:
    segments: list[tuple[int, str, str]] = []
    for line in config.read_text().splitlines():
        match = SEGMENT.match(line)
        if match:
            segments.append(
                (
                    int(match.group(1), 16),
                    (match.group(2) or "").strip(),
                    (match.group(3) or "").strip(),
                )
            )
    return segments


def section_sizes(size_tool: str, obj: Path) -> dict[str, int]:
    output = subprocess.run(
        [size_tool, "-A", str(obj)], check=True, text=True, capture_output=True
    ).stdout
    sizes: dict[str, int] = {}
    for line in output.splitlines():
        fields = line.split()
        if len(fields) >= 2 and fields[0].startswith(".") and fields[1].isdigit():
            sizes[fields[0]] = int(fields[1])
    return sizes


def undefined_symbols(nm: str, obj: Path) -> frozenset[str]:
    output = subprocess.run(
        [nm, "-u", str(obj)], check=True, text=True, capture_output=True
    ).stdout
    return frozenset(
        fields[-1]
        for line in output.splitlines()
        if (fields := line.split())
    )


def defined_text_symbols(nm: str, obj: Path, base_address: int) -> dict[str, int]:
    output = subprocess.run(
        [nm, "-g", "--defined-only", str(obj)],
        check=True,
        text=True,
        capture_output=True,
    ).stdout
    symbols: dict[str, int] = {}
    for line in output.splitlines():
        fields = line.split()
        if len(fields) == 3 and fields[1].upper() == "T":
            try:
                symbols[fields[2]] = base_address + int(fields[0], 16)
            except ValueError:
                pass
    return symbols


def global_defined_symbols(nm: str, elf: Path) -> dict[str, int]:
    output = subprocess.run(
        [nm, "-g", "--defined-only", str(elf)],
        check=True,
        text=True,
        capture_output=True,
    ).stdout
    symbols: dict[str, int] = {}
    for line in output.splitlines():
        fields = line.split()
        if len(fields) == 3:
            try:
                symbols[fields[2]] = int(fields[0], 16)
            except ValueError:
                pass
    return symbols


def read_pal_units(
    config: Path, build_dir: Path, size_tool: str, nm: str
) -> tuple[list[PalTextUnit], list[dict[str, object]]]:
    segments = parse_segments(config)
    rodata_segments = {
        path: (start, segments[index + 1][0])
        for index, (start, kind, path) in enumerate(segments[:-1])
        if kind == ".rodata"
    }
    units: list[PalTextUnit] = []
    rejected: list[dict[str, object]] = []
    for index, (start, kind, path) in enumerate(segments[:-1]):
        if kind != "c":
            continue
        end = segments[index + 1][0]
        obj = build_dir / "src/main" / f"{path}.c.o"
        if not obj.exists():
            rejected.append({"path": path, "reason": "missing PAL object"})
            continue
        sizes = section_sizes(size_tool, obj)
        if sizes.get(".text", 0) != end - start:
            rejected.append({"path": path, "reason": "text size differs from PAL split"})
            continue
        populated = sorted(section for section in OUTPUT_SECTIONS if sizes.get(section, 0))
        if populated and populated != [".rodata"]:
            rejected.append(
                {"path": path, "reason": "has non-text output", "sections": populated}
            )
            continue
        rodata = rodata_segments.get(path) if populated else None
        if rodata is not None and sizes[".rodata"] != rodata[1] - rodata[0]:
            rejected.append({"path": path, "reason": "rodata size differs from PAL split"})
            continue
        if populated and rodata is None:
            rejected.append({"path": path, "reason": "missing PAL rodata split"})
            continue
        units.append(PalTextUnit(start, end, path, undefined_symbols(nm, obj), rodata))
    return units, rejected


def symbol_assignments(paths: list[Path]) -> dict[str, int]:
    symbols: dict[str, int] = {}
    for path in paths:
        for line in path.read_text().splitlines():
            if match := SYMBOL_ASSIGNMENT.match(line):
                symbols[match.group(1)] = int(match.group(2), 16)
    return symbols


def assigned_names(paths: list[Path]) -> set[str]:
    names: set[str] = set()
    for path in paths:
        if not path.exists():
            continue
        for line in path.read_text().splitlines():
            if "=" in line and not line.lstrip().startswith(("//", "/*", "*")):
                names.add(line.split("=", 1)[0].strip())
    return names


def write_linker_aliases(path: Path, aliases: dict[str, int]) -> None:
    lines = [
        "/* PAL external names inferred from corresponding regional code. */",
        "/* Generated by tools/scripts/port_pal_text_units.py. */",
    ]
    lines.extend(
        f"{name} = 0x{address:08X};"
        for name, address in sorted(aliases.items(), key=lambda item: (item[1], item[0]))
    )
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(lines) + "\n")


def read_regional_units(path: Path) -> list[tuple[int, int, str, str]]:
    """Read hand-decompiled regional C ranges preserved across ``make port``."""

    if not path.exists():
        return []
    result: list[tuple[int, int, str, str]] = []
    for unit in json.loads(path.read_text()):
        start = int(unit["start"], 0)
        end = int(unit["end"], 0)
        if start >= end:
            raise ValueError(f"invalid regional unit range 0x{start:X}..0x{end:X}")
        result.append((start, end, "c", unit["path"]))
    return result


def read_regional_rodata(path: Path) -> list[tuple[int, int, str, str]]:
    """Read optional regional read-only-data ranges paired with C units."""

    if not path.exists():
        return []
    result: list[tuple[int, int, str, str]] = []
    for unit in json.loads(path.read_text()):
        if "rodata_start" not in unit and "rodata_end" not in unit:
            continue
        if "rodata_start" not in unit or "rodata_end" not in unit:
            raise ValueError("regional rodata requires both start and end")
        start = int(unit["rodata_start"], 0)
        end = int(unit["rodata_end"], 0)
        if start >= end:
            raise ValueError(f"invalid regional rodata range 0x{start:X}..0x{end:X}")
        result.append((start, end, ".rodata", unit.get("rodata_path", unit["path"])))
    return result


def append_text_aliases(
    target_symbols: Path,
    source_symbols: dict[str, int],
    selected: list[tuple[int, int, str, int, int]],
    extra_aliases: dict[str, int] | None = None,
) -> int:
    lines = target_symbols.read_text().splitlines()
    if ALIASES_BEGIN in lines:
        begin = lines.index(ALIASES_BEGIN)
        end = lines.index(ALIASES_END, begin)
        lines = lines[:begin] + lines[end + 1 :]
    existing = {
        match.group(1): int(match.group(2), 16)
        for line in lines
        if (match := SYMBOL_ASSIGNMENT.match(line))
    }
    aliases: dict[str, int] = {}
    for target_start, _, _, pal_start, pal_end in selected:
        pal_vram = LOAD_ADDRESS + pal_start - HEADER_SIZE
        for name, source_address in source_symbols.items():
            if not pal_vram <= source_address < pal_vram + pal_end - pal_start:
                continue
            target_address = LOAD_ADDRESS + target_start - HEADER_SIZE + source_address - pal_vram
            if name in existing and existing[name] != target_address:
                raise ValueError(
                    f"mapped symbol conflict for {name}: "
                    f"0x{existing[name]:08X} != 0x{target_address:08X}"
                )
            if name not in existing:
                aliases[name] = target_address
    for name, target_address in (extra_aliases or {}).items():
        if name in existing and existing[name] != target_address:
            raise ValueError(
                f"mapped symbol conflict for {name}: "
                f"0x{existing[name]:08X} != 0x{target_address:08X}"
            )
        if name not in existing:
            aliases[name] = target_address
    if aliases:
        while lines and not lines[-1]:
            lines.pop()
        lines.extend(["", ALIASES_BEGIN])
        lines.extend(
            f"{name} = 0x{address:08X}; // type:func"
            for name, address in sorted(aliases.items(), key=lambda item: (item[1], item[0]))
        )
        lines.append(ALIASES_END)
    target_symbols.write_text("\n".join(lines) + "\n")
    return len(aliases)


def infer_range_start(
    pal_data: bytes,
    target_data: bytes,
    pal_text_start: int,
    pal_text_end: int,
    target_text_vram: int,
    pal_range_start: int,
    pal_range_end: int,
) -> int | None:
    """Infer a relocated section start from address loads in matching text."""

    count = (pal_text_end - pal_text_start) // 4
    source_words = tuple(
        int.from_bytes(pal_data[offset : offset + 4], "little")
        for offset in range(pal_text_start, pal_text_start + count * 4, 4)
    )
    target_offset = HEADER_SIZE + target_text_vram - LOAD_ADDRESS
    target_words = tuple(
        int.from_bytes(target_data[offset : offset + 4], "little")
        for offset in range(target_offset, target_offset + count * 4, 4)
    )
    signed_opcodes = {
        9, 32, 33, 35, 36, 37, 40, 41, 43, 48, 49, 50, 51, 56, 57, 58
    }
    candidates: set[int] = set()
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
            if opcode in signed_opcodes:
                source_half = source_low & 0xFFFF
                target_half = target_low & 0xFFFF
                source_address = ((source_hi & 0xFFFF) << 16) + (
                    source_half if source_half < 0x8000 else source_half - 0x10000
                )
                target_address = ((target_hi & 0xFFFF) << 16) + (
                    target_half if target_half < 0x8000 else target_half - 0x10000
                )
            elif opcode == 13:
                source_address = ((source_hi & 0xFFFF) << 16) | (source_low & 0xFFFF)
                target_address = ((target_hi & 0xFFFF) << 16) | (target_low & 0xFFFF)
            else:
                continue
            if pal_range_start <= source_address < pal_range_end:
                candidates.add(target_address - (source_address - pal_range_start))
                break
    return next(iter(candidates)) if len(candidates) == 1 else None


def infer_exact_address(
    pal_data: bytes,
    target_data: bytes,
    pal_text_start: int,
    pal_text_end: int,
    target_text_vram: int,
    source_address: int,
) -> int | None:
    """Infer one external symbol from corresponding relocatable instructions."""

    count = (pal_text_end - pal_text_start) // 4
    source_words = tuple(
        int.from_bytes(pal_data[offset : offset + 4], "little")
        for offset in range(pal_text_start, pal_text_start + count * 4, 4)
    )
    target_offset = HEADER_SIZE + target_text_vram - LOAD_ADDRESS
    target_words = tuple(
        int.from_bytes(target_data[offset : offset + 4], "little")
        for offset in range(target_offset, target_offset + count * 4, 4)
    )
    candidates: set[int] = set()
    for source_word, target_word in zip(source_words, target_words):
        opcode = source_word >> 26
        if opcode in (2, 3) and target_word >> 26 == opcode:
            source_target = (LOAD_ADDRESS & 0xF0000000) | (
                (source_word & 0x03FFFFFF) << 2
            )
            if source_target == source_address:
                candidates.add(
                    (LOAD_ADDRESS & 0xF0000000) | ((target_word & 0x03FFFFFF) << 2)
                )

    signed_opcodes = {
        9, 32, 33, 35, 36, 37, 40, 41, 43, 48, 49, 50, 51, 56, 57, 58
    }
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
            if opcode in signed_opcodes:
                source_half = source_low & 0xFFFF
                target_half = target_low & 0xFFFF
                observed_source = ((source_hi & 0xFFFF) << 16) + (
                    source_half if source_half < 0x8000 else source_half - 0x10000
                )
                observed_target = ((target_hi & 0xFFFF) << 16) + (
                    target_half if target_half < 0x8000 else target_half - 0x10000
                )
            elif opcode == 13:
                observed_source = ((source_hi & 0xFFFF) << 16) | (source_low & 0xFFFF)
                observed_target = ((target_hi & 0xFFFF) << 16) | (target_low & 0xFFFF)
            else:
                continue
            if source_address <= observed_source < source_address + 0x100:
                candidates.add(observed_target - (observed_source - source_address))
                break
    return next(iter(candidates)) if len(candidates) == 1 else None


def rewrite_main_subsegments(
    config: Path,
    units: list[tuple[int, int, str, str]],
    payload_end: int,
    text_start: int,
) -> None:
    lines = config.read_text().splitlines()
    main_index = next(i for i, line in enumerate(lines) if line.strip() == "- name: main")
    subsegments_index = next(
        i for i in range(main_index + 1, len(lines)) if lines[i].strip() == "subsegments:"
    )
    block_end = subsegments_index + 1
    while block_end < len(lines) and lines[block_end].startswith("      - ["):
        block_end += 1

    generated: list[str] = []

    def append_gap(start: int, end: int) -> None:
        if start < text_start < end:
            append_gap(start, text_start)
            append_gap(text_start, end)
            return
        kind = "rodata" if end <= text_start else "asm"
        generated.append(
            f"      - [0x{start:X}, {kind}, main/{start:06X}_main]"
        )

    cursor = HEADER_SIZE
    for start, end, kind, path in units:
        if start < cursor:
            raise ValueError(f"overlapping target units at 0x{start:X}: {path}")
        if start > cursor:
            append_gap(cursor, start)
        generated.append(f"      - [0x{start:X}, {kind}, {path}]")
        cursor = end
    if cursor < payload_end:
        append_gap(cursor, payload_end)
    generated.append(f"      - [0x{payload_end:X}]")
    config.write_text("\n".join(lines[: subsegments_index + 1] + generated + lines[block_end:]) + "\n")


def validate_text_coverage(
    segments: list[tuple[int, int, str, str]], text_start: int
) -> tuple[int, int]:
    """Require source units to cover the regional executable text contiguously."""
    ordered = sorted(segments)
    if not ordered:
        raise ValueError("regional executable has no source-covered text units")
    if ordered[0][0] != text_start:
        raise ValueError(
            f"source coverage starts at 0x{ordered[0][0]:X}, expected 0x{text_start:X}"
        )

    cursor = text_start
    for start, end, _, path in ordered:
        if start != cursor:
            relation = "overlap" if start < cursor else "gap"
            raise ValueError(
                f"source coverage {relation} before {path}: "
                f"0x{cursor:X} -> 0x{start:X}"
            )
        if end <= start:
            raise ValueError(f"empty source-covered text unit: {path}")
        cursor = end
    return cursor, cursor - text_start


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("version", choices=("USA", "JAP10", "JAP11"))
    parser.add_argument("--pal-config", type=Path, default=Path("configs/PAL/main.yaml"))
    parser.add_argument("--pal-exe", type=Path, default=Path("assets/PAL/main.exe"))
    parser.add_argument("--pal-build", type=Path, default=Path("build/PAL"))
    parser.add_argument("--pal-elf", type=Path, default=Path("build/PAL/main.elf"))
    parser.add_argument("--target-config", type=Path)
    parser.add_argument("--target-exe", type=Path)
    parser.add_argument("--target-symbols", type=Path)
    parser.add_argument("--target-manual", type=Path)
    parser.add_argument("--target-aliases", type=Path)
    parser.add_argument(
        "--pal-symbols", type=Path, action="append",
        help="PAL symbol files used for labels inside reused text units",
    )
    parser.add_argument("--report", type=Path)
    parser.add_argument(
        "--regional-units",
        type=Path,
        help="JSON ranges for version-specific C units which make port must preserve",
    )
    parser.add_argument(
        "--exclude", type=Path,
        help="newline-separated PAL unit paths proven non-identical after linking",
    )
    parser.add_argument("--nm", default="mipsel-none-elf-nm")
    parser.add_argument("--size", default="mipsel-none-elf-size")
    parser.add_argument("--readelf", default="mipsel-none-elf-readelf")
    args = parser.parse_args()

    target_config = args.target_config or Path(f"configs/{args.version}/main.yaml")
    target_exe = args.target_exe or Path(f"assets/{args.version}/main.exe")
    target_symbols = args.target_symbols or Path(f"configs/{args.version}/sym.main.txt")
    target_manual = args.target_manual or Path(
        f"linkers/{args.version}/undefined_syms_manual.txt"
    )
    target_aliases = args.target_aliases or Path(
        f"configs/{args.version}/pal_text_aliases.txt"
    )
    report_path = args.report or Path(f"configs/{args.version}/portable_text.json")
    regional_units_path = args.regional_units or Path(
        f"configs/{args.version}/regional_text_units.json"
    )
    exclude_path = args.exclude or Path(f"configs/{args.version}/nonportable_pal_text.txt")
    excluded = {
        line.strip()
        for line in exclude_path.read_text().splitlines()
        if line.strip() and not line.lstrip().startswith("#")
    } if exclude_path.exists() else set()

    candidates, rejected = read_pal_units(
        args.pal_config, args.pal_build, args.size, args.nm
    )
    pal_data = args.pal_exe.read_bytes()
    target_data = target_exe.read_bytes()
    target_normalized = normalize(target_data[HEADER_SIZE:])
    target_assignments = symbol_assignments([target_symbols])
    names = set(target_assignments) | assigned_names([target_manual])
    pal_functions = read_functions(args.readelf, args.pal_elf)
    pal_symbol_paths = args.pal_symbols or [
        Path("configs/PAL/sym.main.txt"),
        Path("configs/PAL/sym.bss.main.txt"),
    ]
    source_assignments = symbol_assignments(pal_symbol_paths)
    for unit in candidates:
        obj = args.pal_build / "src/main" / f"{unit.path}.c.o"
        base_address = LOAD_ADDRESS + unit.start - HEADER_SIZE
        for name, address in defined_text_symbols(args.nm, obj, base_address).items():
            source_assignments.setdefault(name, address)
    for function in pal_functions:
        for name in function.names:
            source_assignments.setdefault(name, function.address)
    external_source_assignments = dict(source_assignments)
    needed_externals = set().union(*(unit.undefined for unit in candidates))
    for name, address in global_defined_symbols(args.nm, args.pal_elf).items():
        if name in needed_externals:
            external_source_assignments.setdefault(name, address)
    inferred_externals: dict[str, int] = {}
    unit_matches: dict[str, list[int]] = {}
    for unit in candidates:
        matches = occurrences(
            target_normalized, normalize(pal_data[unit.start : unit.end])
        )
        if len(matches) != 1:
            pal_vram = LOAD_ADDRESS + unit.start - HEADER_SIZE
            anchored_starts: set[int] = set()
            for function in pal_functions:
                if not pal_vram <= function.address < pal_vram + unit.end - unit.start:
                    continue
                mapped_targets = {
                    target_assignments[name]
                    for name in function.names
                    if name in target_assignments
                }
                if len(mapped_targets) == 1:
                    anchored_starts.add(
                        next(iter(mapped_targets)) - (function.address - pal_vram)
                    )
            if len(anchored_starts) == 1:
                anchored = next(iter(anchored_starts))
                matches = [match for match in matches if match == anchored]
        unit_matches[unit.path] = matches

    # Unique whole-unit matches can reveal callees which are themselves tiny
    # repeated stubs. Feed those inferred targets back as anchors before the
    # monotonic pass below.
    for unit in candidates:
        matches = unit_matches[unit.path]
        if len(matches) != 1:
            continue
        for name in unit.undefined:
            source_address = external_source_assignments.get(name)
            if source_address is None:
                continue
            target_address = infer_exact_address(
                pal_data,
                target_data,
                unit.start,
                unit.end,
                matches[0],
                source_address,
            )
            if target_address is not None:
                inferred_externals[name] = target_address
                target_assignments[name] = target_address
                names.add(name)

    for unit in candidates:
        matches = unit_matches[unit.path]
        if len(matches) == 1:
            continue
        pal_vram = LOAD_ADDRESS + unit.start - HEADER_SIZE
        anchored_starts = {
            target_assignments[name] - (source_address - pal_vram)
            for name, source_address in source_assignments.items()
            if pal_vram <= source_address < pal_vram + unit.end - unit.start
            and name in target_assignments
        }
        if len(anchored_starts) == 1:
            anchored = next(iter(anchored_starts))
            unit_matches[unit.path] = [match for match in matches if match == anchored]

    # Repeated tiny helpers become unambiguous once constrained between their
    # nearest uniquely placed translation units. Repeat because each newly
    # resolved unit can become an anchor for the next one.
    for _ in range(4):
        changed = False
        for index, unit in enumerate(candidates):
            matches = unit_matches[unit.path]
            if len(matches) <= 1:
                continue
            before = next(
                (
                    (candidates[i], unit_matches[candidates[i].path][0])
                    for i in range(index - 1, -1, -1)
                    if len(unit_matches[candidates[i].path]) == 1
                ),
                None,
            )
            after = next(
                (
                    unit_matches[candidates[i].path][0]
                    for i in range(index + 1, len(candidates))
                    if len(unit_matches[candidates[i].path]) == 1
                ),
                None,
            )
            valid = [
                match
                for match in matches
                if (
                    before is None
                    or match >= before[1] + before[0].end - before[0].start
                )
                and (after is None or match + unit.end - unit.start <= after)
            ]
            if len(valid) == 1:
                unit_matches[unit.path] = valid
                changed = True
        if not changed:
            break
    first_pal_text = min(candidates, key=lambda unit: unit.start)
    first_text_matches = unit_matches[first_pal_text.path]
    if len(first_text_matches) != 1:
        raise ValueError("could not locate the regional .rodata/.text boundary")
    target_text_start = HEADER_SIZE + first_text_matches[0] - LOAD_ADDRESS
    selected: list[tuple[int, int, str, int, int, int | None, int | None]] = []
    selected_segments: list[tuple[int, int, str, str]] = []

    for unit in candidates:
        if unit.path in excluded:
            rejected.append({"path": unit.path, "reason": "linked bytes differ"})
            continue
        matches = unit_matches[unit.path]
        if len(matches) != 1:
            rejected.append(
                {
                    "path": unit.path,
                    "reason": "structural match is not unique",
                    "matches": len(matches),
                }
            )
            continue
        raw_address_names = {
            name
            for name in unit.undefined
            if (match := re.fullmatch(r"D_([0-9A-Fa-f]{8})", name))
            and 0x1F800000 <= int(match.group(1), 16) < 0x1F801000
        }
        names.update(raw_address_names)
        for name in sorted(unit.undefined - names):
            source_address = external_source_assignments.get(name)
            if source_address is None and (
                raw_match := re.fullmatch(r"D_([0-9A-Fa-f]{8})", name)
            ):
                source_address = int(raw_match.group(1), 16)
            if source_address is None:
                continue
            target_address = infer_exact_address(
                pal_data,
                target_data,
                unit.start,
                unit.end,
                matches[0],
                source_address,
            )
            if target_address is not None:
                inferred_externals[name] = target_address
                target_assignments[name] = target_address
                names.add(name)
        missing = sorted(unit.undefined - names)
        if missing:
            rejected.append(
                {"path": unit.path, "reason": "unmapped externals", "symbols": missing}
            )
            continue
        target_start = HEADER_SIZE + matches[0] - LOAD_ADDRESS
        target_rodata_start: int | None = None
        target_rodata_end: int | None = None
        if unit.rodata is not None:
            pal_rodata_start = LOAD_ADDRESS + unit.rodata[0] - HEADER_SIZE
            pal_rodata_end = pal_rodata_start + unit.rodata[1] - unit.rodata[0]
            target_rodata_vram = infer_range_start(
                pal_data,
                target_data,
                unit.start,
                unit.end,
                matches[0],
                pal_rodata_start,
                pal_rodata_end,
            )
            if target_rodata_vram is None:
                rejected.append({"path": unit.path, "reason": "unmapped rodata"})
                continue
            target_rodata_start = HEADER_SIZE + target_rodata_vram - LOAD_ADDRESS
            target_rodata_end = target_rodata_start + unit.rodata[1] - unit.rodata[0]
        selected.append(
            (
                target_start,
                target_start + unit.end - unit.start,
                unit.path,
                unit.start,
                unit.end,
                target_rodata_start,
                target_rodata_end,
            )
        )
        selected_segments.append(
            (target_start, target_start + unit.end - unit.start, "c", unit.path)
        )
        if target_rodata_start is not None and target_rodata_end is not None:
            selected_segments.append(
                (target_rodata_start, target_rodata_end, ".rodata", unit.path)
            )

    selected.sort()
    selected_segments.sort()
    regional_units = read_regional_units(regional_units_path)
    regional_segments = regional_units + read_regional_rodata(regional_units_path)
    for regional_start, regional_end, _, regional_path in regional_segments:
        if any(
            regional_start < selected_end and selected_start < regional_end
            for selected_start, selected_end, _, _ in selected_segments
        ):
            raise ValueError(
                f"regional unit {regional_path} overlaps an automatically selected unit"
            )
    selected_segments.extend(regional_segments)
    selected_segments.sort()
    text_end, covered_text_bytes = validate_text_coverage(
        [segment for segment in selected_segments if segment[2] == "c"],
        target_text_start,
    )
    payload_end = len(target_data)
    rewrite_main_subsegments(
        target_config,
        selected_segments,
        payload_end,
        target_text_start,
    )
    aliases = append_text_aliases(
        target_symbols,
        source_assignments,
        [
            (start, end, path, pal_start, pal_end)
            for start, end, path, pal_start, pal_end, _, _ in selected
        ],
    )
    write_linker_aliases(target_aliases, inferred_externals)
    report = {
        "version": args.version,
        "portable_units": len(selected),
        "portable_text_bytes": sum(
            end - start for start, end, _, _, _, _, _ in selected
        ),
        "portable_rodata_bytes": sum(
            (rodata_end or 0) - (rodata_start or 0)
            for _, _, _, _, _, rodata_start, rodata_end in selected
        ),
        "regional_units": len(regional_units),
        "regional_text_bytes": sum(end - start for start, end, _, _ in regional_units),
        "source_units": len(selected) + len(regional_units),
        "source_text_bytes": covered_text_bytes,
        "text_start": f"0x{target_text_start:X}",
        "text_end": f"0x{text_end:X}",
        "source_coverage_percent": 100.0,
        "text_aliases": aliases,
        "inferred_externals": len(inferred_externals),
        "selected": [
            {
                "start": f"0x{start:X}",
                "end": f"0x{end:X}",
                "path": path,
                **(
                    {
                        "rodata_start": f"0x{rodata_start:X}",
                        "rodata_end": f"0x{rodata_end:X}",
                    }
                    if rodata_start is not None and rodata_end is not None
                    else {}
                ),
            }
            for start, end, path, _, _, rodata_start, rodata_end in selected
        ],
        "rejected": rejected,
    }
    report_path.write_text(json.dumps(report, indent=2) + "\n")
    print(
        f"{args.version}: reused {report['portable_units']} PAL C text units "
        f"({report['portable_text_bytes']} bytes); source coverage "
        f"{report['source_text_bytes']}/{text_end - target_text_start} bytes (100%)"
    )


if __name__ == "__main__":
    main()
