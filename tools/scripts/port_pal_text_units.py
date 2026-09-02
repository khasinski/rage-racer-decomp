#!/usr/bin/env python3
"""Reuse directly portable PAL C text units in a regional split.

The regional executables retain most compiler output in the same structural
form, but code and data move. This tool selects PAL translation units whose
entire text section has one structural match in the target, whose other game
output sections are empty, and whose external symbols are all mapped in the
regional symbol file. Everything else is emitted as regional assembly.
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
from dataclasses import dataclass
from pathlib import Path

from map_version_functions import HEADER_SIZE, LOAD_ADDRESS, normalize, occurrences


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


def read_pal_units(
    config: Path, build_dir: Path, size_tool: str, nm: str
) -> tuple[list[PalTextUnit], list[dict[str, object]]]:
    segments = parse_segments(config)
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
        if populated:
            rejected.append(
                {"path": path, "reason": "has non-text output", "sections": populated}
            )
            continue
        units.append(PalTextUnit(start, end, path, undefined_symbols(nm, obj)))
    return units, rejected


def symbol_assignments(paths: list[Path]) -> dict[str, int]:
    symbols: dict[str, int] = {}
    for path in paths:
        for line in path.read_text().splitlines():
            if match := SYMBOL_ASSIGNMENT.match(line):
                symbols[match.group(1)] = int(match.group(2), 16)
    return symbols


def append_text_aliases(
    target_symbols: Path,
    source_symbols: dict[str, int],
    selected: list[tuple[int, int, str, int, int]],
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


def rewrite_main_subsegments(
    config: Path, units: list[tuple[int, int, str]], payload_end: int
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
    cursor = HEADER_SIZE
    for start, end, path in units:
        if start < cursor:
            raise ValueError(f"overlapping target units at 0x{start:X}: {path}")
        if start > cursor:
            generated.append(f"      - [0x{cursor:X}, asm, main/{cursor:06X}_main]")
        generated.append(f"      - [0x{start:X}, c, {path}]")
        cursor = end
    if cursor < payload_end:
        generated.append(f"      - [0x{cursor:X}, asm, main/{cursor:06X}_main]")
    generated.append(f"      - [0x{payload_end:X}]")
    config.write_text("\n".join(lines[: subsegments_index + 1] + generated + lines[block_end:]) + "\n")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("version", choices=("USA", "JAP10", "JAP11"))
    parser.add_argument("--pal-config", type=Path, default=Path("configs/PAL/main.yaml"))
    parser.add_argument("--pal-exe", type=Path, default=Path("assets/PAL/main.exe"))
    parser.add_argument("--pal-build", type=Path, default=Path("build/PAL"))
    parser.add_argument("--target-config", type=Path)
    parser.add_argument("--target-exe", type=Path)
    parser.add_argument("--target-symbols", type=Path)
    parser.add_argument(
        "--pal-symbols", type=Path, action="append",
        help="PAL symbol files used for labels inside reused text units",
    )
    parser.add_argument("--report", type=Path)
    parser.add_argument(
        "--exclude", type=Path,
        help="newline-separated PAL unit paths proven non-identical after linking",
    )
    parser.add_argument("--nm", default="mipsel-none-elf-nm")
    parser.add_argument("--size", default="mipsel-none-elf-size")
    args = parser.parse_args()

    target_config = args.target_config or Path(f"configs/{args.version}/main.yaml")
    target_exe = args.target_exe or Path(f"assets/{args.version}/main.exe")
    target_symbols = args.target_symbols or Path(f"configs/{args.version}/sym.main.txt")
    report_path = args.report or Path(f"configs/{args.version}/portable_text.json")
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
    names = set(target_assignments)
    selected: list[tuple[int, int, str, int, int]] = []

    for unit in candidates:
        if unit.path in excluded:
            rejected.append({"path": unit.path, "reason": "linked bytes differ"})
            continue
        matches = occurrences(
            target_normalized, normalize(pal_data[unit.start : unit.end])
        )
        missing = sorted(unit.undefined - names)
        if len(matches) != 1:
            rejected.append(
                {
                    "path": unit.path,
                    "reason": "structural match is not unique",
                    "matches": len(matches),
                }
            )
            continue
        if missing:
            rejected.append(
                {"path": unit.path, "reason": "unmapped externals", "symbols": missing}
            )
            continue
        target_start = HEADER_SIZE + matches[0] - LOAD_ADDRESS
        selected.append(
            (target_start, target_start + unit.end - unit.start, unit.path, unit.start, unit.end)
        )

    selected.sort()
    payload_end = len(target_data)
    rewrite_main_subsegments(
        target_config,
        [(start, end, path) for start, end, path, _, _ in selected],
        payload_end,
    )
    pal_symbol_paths = args.pal_symbols or [
        Path("configs/PAL/sym.main.txt"),
        Path("configs/PAL/sym.bss.main.txt"),
    ]
    aliases = append_text_aliases(
        target_symbols, symbol_assignments(pal_symbol_paths), selected
    )
    report = {
        "version": args.version,
        "portable_units": len(selected),
        "portable_text_bytes": sum(end - start for start, end, _, _, _ in selected),
        "text_aliases": aliases,
        "selected": [
            {"start": f"0x{start:X}", "end": f"0x{end:X}", "path": path}
            for start, end, path, _, _ in selected
        ],
        "rejected": rejected,
    }
    report_path.write_text(json.dumps(report, indent=2) + "\n")
    print(
        f"{args.version}: reused {report['portable_units']} PAL C text units "
        f"({report['portable_text_bytes']} bytes); the rest remains assembly"
    )


if __name__ == "__main__":
    main()
