#!/usr/bin/env python3
"""Turn raw address words in the split data into symbol references.

The jump and handler tables come out of splat as `.word 0x80049900`, which
freezes the function they point at: the linker has nothing to relocate, so
nothing either side of that address may change size.  `.word func_800496F0 +
0x210` is the same four bytes today and a relocation tomorrow.

Only words the symbol table can account for are rewritten, and only ever to the
address they already encode, so the image is unchanged by construction.  A word
that looks like an address but belongs to no known object stays as it is.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import dis_sym
from gen_nonmatching_asm import build_plan

DATA_WORD_RE = re.compile(
    r"^(?P<head>\s*/\* [0-9A-Fa-f]+ (?P<vaddr>[0-9A-Fa-f]{8}) [0-9A-Fa-f]{8} \*/\s*)"
    r"\.word (?P<value>0x[0-9A-Fa-f]{8})\s*$"
)


def rewrite(path: Path, table: dis_sym.SymbolTable) -> tuple[int, int]:
    """Rewrite one file; returns (converted, left as raw addresses)."""
    converted = 0
    remaining = 0
    lines = path.read_text().splitlines()
    for index, line in enumerate(lines):
        match = DATA_WORD_RE.match(line)
        if match is None:
            continue
        value = int(match.group("value"), 16)
        if not dis_sym.RAM_START <= value < dis_sym.RAM_END:
            continue
        # No invented names here: a word in a data table is only a pointer if
        # something already known lives where it points.  Inventing a symbol
        # for the rest would dress up a guess as a fact.
        reference = table.reference(value, synthesize=False)
        if reference is None:
            remaining += 1
            continue
        lines[index] = f"{match.group('head')}.word {reference}"
        converted += 1
    if converted:
        path.write_text("\n".join(lines) + "\n")
    return converted, remaining


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--version", default="PAL")
    parser.add_argument("--basename", default="main")
    parser.add_argument("--verbose", action="store_true")
    args = parser.parse_args()

    root = Path.cwd()
    plan = build_plan(root, args.version, args.basename)
    if plan is None:
        return 0

    data_root = root / "asm" / args.version / args.basename / "data"
    if not data_root.exists():
        return 0

    total = 0
    left = 0
    for path in sorted(data_root.rglob("*.s")):
        converted, remaining = rewrite(path, plan.table)
        total += converted
        left += remaining
        if args.verbose and converted:
            print(f"  {path.relative_to(root)}: {converted} words")
    print(
        f"symbolised {total} address words in {args.version}/{args.basename} data "
        f"({left} left as literals)"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
