#!/usr/bin/env python3
"""Work out which addresses the linker still has to be told, and only those.

splat writes `undefined_syms_auto` / `undefined_funcs_auto` by taking every
symbol it saw referenced but could not place, and assigning it the literal
address encoded in its name.  A linker-script assignment *overrides* a real
definition, so feeding those files to the linker pins every one of those
addresses for good -- including the ones whose object is now laid out
properly, and including names nothing refers to any more.

So the linker gets this file instead.  It carries an assignment only when the
symbol is genuinely unresolvable:

  * defined by none of the objects being linked, and
  * not assigned relative to another symbol in the manual script
    (`func_X = func_Y + 0x40;`, the shiftable spelling of a mid-function
    entry point), and
  * actually referenced by something.

splat's own files stay untouched, because the disassembler reads them for
symbol *names*; they are simply no longer part of the link.  What ends up
here is the honest list of addresses that cannot move.
"""
from __future__ import annotations

import argparse
import re
import subprocess
from pathlib import Path


ADDR_SYM_RE = re.compile(r"^D_([0-9A-Fa-f]{8})$")
ASSIGN_RE = re.compile(r"^([\w.]+)\s*=")
ABS_ASSIGN_RE = re.compile(r"^([\w.]+)\s*=\s*(0x[0-9A-Fa-f]+|\d+)\s*;")
RHS_NAME_RE = re.compile(r"(?<![A-Za-z0-9_.$])[A-Za-z_.$][A-Za-z0-9_.$]*")
ADDRESS_NAME_RE = re.compile(r"^(D_|func_|jtbl_|\.L)")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("objects", nargs="+", type=Path)
    parser.add_argument("--nm", default="mipsel-none-elf-nm")
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument(
        "--source",
        action="append",
        default=[],
        type=Path,
        help="splat-generated pin file to take addresses from (read only)",
    )
    parser.add_argument(
        "--manual",
        action="append",
        default=[],
        type=Path,
        help="linker script whose assignments already resolve symbols",
    )
    return parser.parse_args()


def symbol_tables(nm: str, objects: list[Path]) -> tuple[set[str], set[str]]:
    """Return (referenced-but-undefined, defined) across all objects.

    `nm -u` reports per object, so a symbol another object defines still shows
    up there.  The two sets have to be built together and subtracted.
    """
    result = subprocess.run(
        [nm, *(str(obj) for obj in objects)],
        check=True,
        stdout=subprocess.PIPE,
        text=True,
    )
    undefined: set[str] = set()
    defined: set[str] = set()
    for line in result.stdout.splitlines():
        parts = line.split()
        if len(parts) == 2 and parts[0] == "U":
            undefined.add(parts[1])
        elif len(parts) == 3:
            defined.add(parts[2])
    return undefined, defined


def assigned_names(paths: list[Path]) -> set[str]:
    names: set[str] = set()
    for path in paths:
        if not path.exists():
            continue
        for line in path.read_text().splitlines():
            m = ASSIGN_RE.match(line)
            if m is not None:
                names.add(m.group(1))
    return names


def known_addresses(paths: list[Path]) -> dict[str, int]:
    known: dict[str, int] = {}
    for path in paths:
        if not path.exists():
            continue
        for line in path.read_text().splitlines():
            m = ABS_ASSIGN_RE.match(line)
            if m is not None:
                known.setdefault(m.group(1), int(m.group(2), 0))
    return known


def assigned_dependencies(paths: list[Path]) -> set[str]:
    """Names used on the right-hand side of linker-script assignments.

    These references do not occur in an object relocation, but GNU ld still
    needs their definitions while evaluating the script.  In particular, the
    regional terrain units derive encoded address halves from Splat-named jump
    tables that otherwise exist only in ``undefined_syms_auto``.
    """
    dependencies: set[str] = set()
    for path in paths:
        if not path.exists():
            continue
        for line in path.read_text().splitlines():
            code = line.split("//", 1)[0].split("/*", 1)[0]
            if "=" not in code:
                continue
            dependencies.update(RHS_NAME_RE.findall(code.split("=", 1)[1]))
    return dependencies


def main() -> int:
    args = parse_args()
    undefined, defined = symbol_tables(args.nm, args.objects)
    known = known_addresses(args.source)
    assigned = assigned_names(args.manual)
    unresolved = undefined - defined - assigned
    script_dependencies = assigned_dependencies(args.manual) & known.keys()
    # GNU ld on Ubuntu cannot use the object-defined jump-table labels while
    # evaluating the supplemental script.  Those tables still live in the
    # retained regional assembly blob, so bootstrap only those addresses.  Do
    # not pin ordinary object symbols merely because a script refers to them.
    script_bootstrap = {
        sym for sym in script_dependencies - assigned if sym.startswith("jtbl_")
    }
    required = unresolved | script_bootstrap
    pins: list[tuple[str, int]] = []
    missing: list[str] = []
    for sym in sorted(required):
        if sym in known:
            pins.append((sym, known[sym]))
            continue
        match = ADDR_SYM_RE.match(sym)
        if match is not None:
            pins.append((sym, int(match.group(1), 16)))
        else:
            # Left to the linker, which will say so far more clearly than a
            # guessed address would.
            missing.append(sym)

    out = [
        "/* Addresses with nothing to point at, so they stay absolute. */",
        "/* Generated by tools/scripts/gen_undefined_addr_aliases.py. */",
    ]
    out += [f"{sym} = 0x{addr:08X};" for sym, addr in sorted(pins, key=lambda i: i[1])]
    args.output.parent.mkdir(parents=True, exist_ok=True)
    text = "\n".join(out) + "\n"
    if not args.output.exists() or args.output.read_text() != text:
        args.output.write_text(text, encoding="ascii")

    # Names that are not addresses (the INCLUDE_ASM wrappers, C functions
    # renamed with `asm("...")`) turn up undefined without carrying a single
    # relocation, so the linker neither needs nor misses them.  An address-like
    # name reaching here is the interesting case: something referenced it and
    # nothing can say where it is.
    suspicious = [s for s in missing if ADDRESS_NAME_RE.match(s)]
    if suspicious:
        print(
            "gen_undefined_addr_aliases: no address for "
            + ", ".join(suspicious[:8])
            + (" ..." if len(suspicious) > 8 else "")
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
