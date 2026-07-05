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
ASM_WRAP_RE = re.compile(r"RAGE_ASM_WRAP\([^,]+,\s*([A-Za-z0-9_]+)\)")
RODATA_WRAP_RE = re.compile(r"RAGE_RODATA_WRAP\([^,]+,\s*([A-Za-z0-9_]+)\)")


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


def parse_wrappers(src_root: Path) -> tuple[set[str], set[str]]:
    asm: set[str] = set()
    rodata: set[str] = set()
    for path in src_root.rglob("*.c"):
        text = path.read_text()
        asm.update(match.group(1) for match in ASM_WRAP_RE.finditer(text))
        rodata.update(match.group(1) for match in RODATA_WRAP_RE.finditer(text))
    return asm, rodata


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
    out_dir = root / "asm" / "nonmatchings" / args.version / args.basename
    labels = parse_labels(root / "configs" / args.version / f"nonmatching_labels.{args.basename}.txt")

    if not config.exists() or not target.exists() or not src_root.exists():
        return 0

    asm_wrappers, rodata_wrappers = parse_wrappers(src_root)
    target_bytes = target.read_bytes()

    generated = 0
    for start, kind, name, end in parse_subsegments(config):
        if end <= start:
            continue
        vram = BASE_VRAM + (start - BASE_OFF)
        data = target_bytes[start:end]
        stem = Path(name).name

        if kind == "c" and stem in asm_wrappers:
            output = out_dir / f"{stem}.s"
            write_words(
                output,
                '.section .text, "ax"',
                data,
                vram,
                with_entry_label(labels.get(output.name, {}), vram, stem),
            )
            generated += 1
        elif kind == ".rodata" and f"{stem}_rodata" in rodata_wrappers:
            output = out_dir / f"{stem}_rodata.s"
            write_words(output, '.section .rodata, "a"', data, vram, labels.get(output.name, {}))
            generated += 1

    print(f"generated {generated} local nonmatching asm files for {args.version}/{args.basename}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
