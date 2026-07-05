#!/usr/bin/env python3
"""Regenerate local nonmatching asm wrappers from a user-supplied target EXE."""

from __future__ import annotations

import argparse
import re
from collections import defaultdict
from pathlib import Path


BASE_VRAM = 0x80010000
BASE_OFF = 0x800


SUBSEGMENT_RE = re.compile(r"\[0x([0-9A-Fa-f]+),\s*([^,\]]+),\s*([^,\]\s]+)")
ASM_WRAP_RE = re.compile(r"INCLUDE_ASM\([^,]+,\s*([A-Za-z0-9_]+)\)")
RODATA_WRAP_RE = re.compile(r"INCLUDE_RODATA\([^,]+,\s*([A-Za-z0-9_]+)\)")
C_FUNC_RE = re.compile(
    r"^\s*(?:[A-Za-z_][A-Za-z0-9_]*\s+)+(?P<name>func_[0-9A-Fa-f]{8})\s*\([^;]*\)\s*\{",
    re.MULTILINE,
)


def parse_subsegments(config: Path) -> list[tuple[int, str, str, int]]:
    rows: list[tuple[int, str, str]] = []
    for line in config.read_text().splitlines():
        match = SUBSEGMENT_RE.search(line)
        if match:
            rows.append((int(match.group(1), 16), match.group(2).strip(), match.group(3).strip()))

    result: list[tuple[int, str, str, int]] = []
    for index, (start, kind, name) in enumerate(rows):
        end = rows[index + 1][0] if index + 1 < len(rows) else start
        result.append((start, kind, name, end))
    return result


def parse_wrappers(
    src_root: Path, version: str
) -> tuple[dict[str, list[str]], dict[str, list[str]], dict[str, str]]:
    asm_by_unit: dict[str, list[str]] = {}
    c_funcs_by_unit: dict[str, list[str]] = {}
    rodata_by_name: dict[str, str] = {}
    for path in src_root.rglob("*.c"):
        text = path.read_text()
        rel = path.relative_to(src_root).with_suffix("").as_posix()
        asm_by_unit[f"{version}/{rel}"] = [match.group(1) for match in ASM_WRAP_RE.finditer(text)]
        c_funcs_by_unit[f"{version}/{rel}"] = [
            match.group("name") for match in C_FUNC_RE.finditer(text)
        ]
        for match in RODATA_WRAP_RE.finditer(text):
            rodata_by_name[match.group(1)] = rel
    return asm_by_unit, c_funcs_by_unit, rodata_by_name


def unit_output_path(name: str, version: str) -> str:
    prefix = f"{version}/"
    if name.startswith(prefix):
        return name[len(prefix) :]
    return name


def labels_for_asm(labels: dict[str, dict[int, list[str]]], unit: str, asm_name: str) -> dict[int, list[str]]:
    return labels.get(f"{asm_name}.s") or labels.get(f"{Path(unit).name}.s", {})


def parse_labels(path: Path) -> dict[str, dict[int, list[str]]]:
    labels: dict[str, dict[int, list[str]]] = defaultdict(lambda: defaultdict(list))
    if not path.exists():
        return labels

    for raw in path.read_text().splitlines():
        line = raw.strip()
        if not line or line.startswith("#"):
            continue
        output_file, address, label = line.split(maxsplit=2)
        labels[output_file][int(address, 16)].append(label)
    return labels


def parse_label_addresses(labels: dict[str, dict[int, list[str]]]) -> dict[str, int]:
    addresses: dict[str, int] = {}
    for output_labels in labels.values():
        for address, names in output_labels.items():
            for name in names:
                addresses.setdefault(name, address)
    return addresses


def fallback_function_address(name: str) -> int | None:
    match = re.fullmatch(r"func_([0-9A-Fa-f]{8})", name)
    if not match:
        return None
    return int(match.group(1), 16)


def function_address(
    name: str,
    label_addresses: dict[str, int],
    segment_vram: int,
    output_labels: dict[int, list[str]],
) -> int | None:
    return label_addresses.get(name) or fallback_function_address(name) or (
        min(output_labels) if output_labels else segment_vram
    )


def write_words(path: Path, section: str, data: bytes, vram: int, labels: dict[int, list[str]]) -> None:
    lines = [section, ""]
    if section.startswith(".section .text"):
        lines[:0] = [".set noreorder", ".set noat"]

    for offset in range(0, len(data), 4):
        address = vram + offset
        for label in labels.get(address, []):
            lines.append(f".globl {label}")
            lines.append(f"{label}:")
        word = int.from_bytes(data[offset : offset + 4], "little")
        lines.append(f".word 0x{word:08X}")

    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text("\n".join(lines) + "\n")


def with_entry_label(labels: dict[int, list[str]], address: int, label: str) -> dict[int, list[str]]:
    merged = {key: list(value) for key, value in labels.items()}
    if any(label in value for value in merged.values()):
        return merged
    merged.setdefault(address, [])
    merged[address].insert(0, label)
    return merged


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--version", default="PAL")
    parser.add_argument("--basename", default="main")
    args = parser.parse_args()

    root = Path.cwd()
    config = root / "configs" / args.version / f"{args.basename}.yaml"
    target = root / "assets" / args.version / f"{args.basename}.exe"
    src_root = root / "src" / args.basename / args.version
    out_dir = root / "asm" / args.version / args.basename / "nonmatchings"
    labels = parse_labels(root / "configs" / args.version / f"nonmatching_labels.{args.basename}.txt")

    if not config.exists() or not target.exists() or not src_root.exists():
        return 0

    asm_wrappers_by_unit, c_funcs_by_unit, rodata_wrappers = parse_wrappers(src_root, args.version)
    label_addresses = parse_label_addresses(labels)
    target_bytes = target.read_bytes()

    generated = 0
    for start, kind, name, end in parse_subsegments(config):
        if end <= start:
            continue
        vram = BASE_VRAM + (start - BASE_OFF)
        data = target_bytes[start:end]
        stem = Path(name).name

        if kind == "c":
            asm_wrappers = asm_wrappers_by_unit.get(name, [])
            c_func_boundaries = [
                function_address(c_func, label_addresses, vram, {})
                for c_func in c_funcs_by_unit.get(name, [])
            ]
            c_func_boundaries = [address for address in c_func_boundaries if address is not None]
            for index, asm_name in enumerate(asm_wrappers):
                output_labels = labels_for_asm(labels, name, asm_name)
                func_vram = function_address(asm_name, label_addresses, vram, output_labels)
                if func_vram is None:
                    continue

                next_boundaries = [address for address in c_func_boundaries if address > func_vram]
                if index + 1 < len(asm_wrappers):
                    next_name = asm_wrappers[index + 1]
                    next_asm_vram = function_address(
                        next_name,
                        label_addresses,
                        vram,
                        labels_for_asm(labels, name, next_name),
                    )
                    if next_asm_vram is not None:
                        next_boundaries.append(next_asm_vram)
                else:
                    next_boundaries.append(BASE_VRAM + (end - BASE_OFF))
                next_vram = min(next_boundaries) if next_boundaries else None
                if next_vram is None or next_vram <= func_vram:
                    continue

                func_start = start + (func_vram - vram)
                func_end = start + (next_vram - vram)
                output = out_dir / unit_output_path(name, args.version) / f"{asm_name}.s"
                write_words(
                    output,
                    '.section .text, "ax"',
                    target_bytes[func_start:func_end],
                    func_vram,
                    with_entry_label(output_labels, func_vram, asm_name),
                )
                generated += 1
        elif kind == ".rodata" and f"{stem}_rodata" in rodata_wrappers:
            output = out_dir / rodata_wrappers[f"{stem}_rodata"] / f"{stem}_rodata.s"
            write_words(output, '.section .rodata, "a"', data, vram, labels.get(output.name, {}))
            generated += 1

    print(f"generated {generated} local nonmatching asm files for {args.version}/{args.basename}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
