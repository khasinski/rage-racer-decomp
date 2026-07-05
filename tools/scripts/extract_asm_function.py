#!/usr/bin/env python3
from __future__ import annotations

import argparse
import subprocess
import sys
import tempfile
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]


def extract_function(asm_path: Path, func_name: str) -> str:
    lines = asm_path.read_text().splitlines()
    start = None
    end = None

    for i, line in enumerate(lines):
        if line.strip() == f"glabel {func_name}":
            start = i
            break
    if start is None:
        raise SystemExit(f"{asm_path}: glabel {func_name} not found")

    for i in range(start + 1, len(lines)):
        if lines[i].strip() == f"endlabel {func_name}":
            end = i + 1
            break
    if end is None:
        raise SystemExit(f"{asm_path}: endlabel {func_name} not found")

    nonmatching = []
    for i in range(start - 1, -1, -1):
        stripped = lines[i].strip()
        if stripped.startswith("nonmatching "):
            nonmatching = [lines[i], ""]
            break
        if stripped and not stripped.startswith("/*"):
            break

    header = [
        '.include "macro.inc"',
        "",
        ".set noat",
        ".set noreorder",
        "",
        '.section .text, "ax"',
        "",
    ]
    return "\n".join(header + nonmatching + lines[start:end]) + "\n"


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Build a single-function target.o from a labelled asm file."
    )
    parser.add_argument("asm", type=Path)
    parser.add_argument("function")
    parser.add_argument("output", type=Path)
    parser.add_argument("--as", dest="assembler", default="mipsel-none-elf-as")
    args = parser.parse_args()

    source = extract_function(args.asm, args.function)
    args.output.parent.mkdir(parents=True, exist_ok=True)

    with tempfile.NamedTemporaryFile("w", suffix=".s", delete=False) as tmp:
        tmp.write(source)
        tmp_path = Path(tmp.name)

    try:
        subprocess.run(
            [
                args.assembler,
                "-EL",
                "-G0",
                "-march=r3000",
                "-mtune=r3000",
                "-no-pad-sections",
                "-I",
                str(ROOT / "include"),
                "-o",
                str(args.output),
                str(tmp_path),
            ],
            check=True,
        )
    finally:
        tmp_path.unlink(missing_ok=True)

    return 0


if __name__ == "__main__":
    sys.exit(main())
