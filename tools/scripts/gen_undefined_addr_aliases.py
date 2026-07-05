#!/usr/bin/env python3
from __future__ import annotations

import argparse
import re
import subprocess
from pathlib import Path


ADDR_SYM_RE = re.compile(r"^D_([0-9A-Fa-f]{8})$")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate linker assignments for undefined D_<address> symbols."
    )
    parser.add_argument("objects", nargs="+", type=Path)
    parser.add_argument("--nm", default="mipsel-none-elf-nm")
    parser.add_argument("--output", required=True, type=Path)
    return parser.parse_args()


def undefined_symbols(nm: str, objects: list[Path]) -> set[str]:
    result = subprocess.run(
        [nm, "-u", *(str(obj) for obj in objects)],
        check=True,
        stdout=subprocess.PIPE,
        text=True,
    )
    syms: set[str] = set()
    for line in result.stdout.splitlines():
        parts = line.split()
        if parts:
            syms.add(parts[-1])
    return syms


def main() -> int:
    args = parse_args()
    aliases: list[tuple[str, int]] = []
    for sym in undefined_symbols(args.nm, args.objects):
        match = ADDR_SYM_RE.match(sym)
        if match is not None:
            aliases.append((sym, int(match.group(1), 16)))

    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", encoding="ascii") as out:
        out.write("/* Generated address aliases for split data labels. */\n")
        for sym, addr in sorted(aliases, key=lambda item: item[1]):
            out.write(f"{sym} = 0x{addr:08X};\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
