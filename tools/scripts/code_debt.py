#!/usr/bin/env python3
from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SOURCE_ROOT = ROOT / "src/main/PAL/main"
BASELINE = ROOT / "tools/code_debt_baseline.json"

PATTERNS = {
    "byte_pointer_arithmetic": re.compile(
        r"\(\s*(?:const\s+|volatile\s+)?(?:u8|s8|char)\s*\*\s*\)[^;\n]*\+"
    ),
    "raw_offset_dereferences": re.compile(
        r"\*\s*\([^)]*\*\s*\)\s*\([^;\n]*\+\s*0x[0-9A-Fa-f]+"
    ),
    "pointer_integer_casts": re.compile(
        r"\(\s*(?:s32|u32|long|unsigned\s+long)\s*\)\s*(?:[A-Za-z_&]|\()"
    ),
    "field_macros": re.compile(r"\b(?:FIELD|RAW_FIELD)\w*\s*\("),
    "register_pins": re.compile(
        r"\bregister\b[^;\n]*\basm\s*\(\s*\"(?:\$\d+|zero|at|v[01]|a[0-3]|t[0-9]|s[0-8]|k[01]|gp|sp|fp|ra)\""
    ),
    "empty_barriers": re.compile(
        r"\b(?:__asm__|asm)\s*(?:volatile\s*)?\(\s*\"\""
    ),
    "statement_expressions": re.compile(r"\(\s*\{"),
    "asm_aliases": re.compile(r"\.globl\s+func_[0-9A-Fa-f]+"),
    "unknown_fields": re.compile(r"\b(?:field_[0-9A-Fa-f]+|unk[0-9A-Fa-f]+)\b"),
    "externs_in_c": re.compile(r"^\s*extern\b", re.MULTILINE),
    "declaration_overrides": re.compile(
        r"^\s*#define\s+GAME_[A-Z0-9_]+_(?:TYPE|QUALIFIER|DECL)\b", re.MULTILINE
    ),
}


def strip_comments(text: str) -> str:
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)
    return re.sub(r"//[^\n]*", "", text)


def count_debt(source_root: Path = SOURCE_ROOT) -> dict[str, int]:
    totals = {name: 0 for name in PATTERNS}
    for path in sorted(source_root.rglob("*.c")):
        text = strip_comments(path.read_text(errors="ignore"))
        for name, pattern in PATTERNS.items():
            totals[name] += len(pattern.findall(text))
    return totals


def main() -> int:
    parser = argparse.ArgumentParser(description="Measure decompilation scaffolding in game C")
    parser.add_argument("--check", action="store_true", help="fail if any count exceeds baseline")
    parser.add_argument("--json", action="store_true", help="print machine-readable output")
    args = parser.parse_args()

    current = count_debt()
    if args.json:
        print(json.dumps(current, indent=2, sort_keys=True))
    else:
        for name, count in current.items():
            print(f"{name:28} {count:5}")

    if not args.check:
        return 0

    baseline = json.loads(BASELINE.read_text())
    increases = {
        name: (baseline.get(name, 0), count)
        for name, count in current.items()
        if count > baseline.get(name, 0)
    }
    if increases:
        for name, (before, after) in increases.items():
            print(f"increased: {name}: {before} -> {after}")
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
