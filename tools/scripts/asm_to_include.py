#!/usr/bin/env python3
"""Replace baked assembly bodies in C files with INCLUDE_ASM stubs."""

from __future__ import annotations

import argparse
import re
from pathlib import Path


GLOBL_RE = re.compile(r"\.globl\s+(\w+)")
WRAPPER_RE = re.compile(
    r"(?:void\s+__maspsx_include_asm_hack_\w+\s*\(void\)\s*;\s*)?"
    r"void\s+__maspsx_include_asm_hack_\w+\s*\(void\)\s*\{\s*"
    r"__asm__\s*\((?:\s*\"(?:[^\"\\]|\\.)*\"\s*)+\)\s*;\s*\}",
    re.S,
)
TOPLEVEL_RE = re.compile(
    r"(?:__asm__|asm)\s*\(\s*(?:\"(?:[^\"\\]|\\.)*\"\s*)+\)\s*;",
    re.S,
)


def tu_name(path: Path, src_root: Path) -> str:
    return path.relative_to(src_root).with_suffix("").as_posix()


def include_asm_path(version: str, basename: str, tu: str, symbol: str) -> str:
    return f'INCLUDE_ASM("asm/nonmatchings/{version}/{basename}/{tu}", {symbol});'


def convert(path: Path, src_root: Path, version: str, basename: str) -> list[str]:
    text = path.read_text(errors="ignore")
    original = text
    tu = tu_name(path, src_root)
    converted: list[str] = []

    def replace(match: re.Match[str]) -> str:
        block = match.group(0)
        labels = GLOBL_RE.findall(block)
        has_body = ".word" in block or "maspsx-keep" in block or ".ent" in block
        if not labels or not has_body:
            return block

        converted.extend(labels)
        return "\n\n".join(include_asm_path(version, basename, tu, label) for label in labels)

    text = WRAPPER_RE.sub(replace, text)
    text = TOPLEVEL_RE.sub(replace, text)
    if not converted:
        return []

    if '#include "include_asm.h"' not in text:
        match = re.search(r"\A(?:\s*/\*.*?\*/\s*)*", text, re.S)
        insert_at = match.end() if match else 0
        text = text[:insert_at] + '#include "include_asm.h"\n' + text[insert_at:]

    if text != original:
        path.write_text(text)
    return converted


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--version", default="PAL")
    parser.add_argument("--basename", default="main")
    args = parser.parse_args()

    root = Path.cwd()
    src_root = root / "src" / args.basename / args.version
    if not src_root.exists():
        return 0

    total_files = 0
    total_funcs = 0
    for path in sorted(src_root.rglob("*.c")):
        converted = convert(path, src_root, args.version, args.basename)
        if converted:
            total_files += 1
            total_funcs += len(converted)

    print(f"converted {total_funcs} asm bodies in {total_files} files")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
