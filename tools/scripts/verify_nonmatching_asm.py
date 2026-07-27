#!/usr/bin/env python3
"""Prove that every generated nonmatching stub still assembles to the retail bytes.

`make check` proves it for the image as a whole, but only after a full build and
only as one hash.  This assembles each stub on its own, links it at the address
it belongs to with every symbol it references defined at the address the retail
image encoded, and compares the result with the target EXE.  A stub that came
out wrong is named, with the offending word.

Usage: verify_nonmatching_asm.py [--version PAL] [--basename main] [pattern...]
"""

from __future__ import annotations

import argparse
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

from dis_sym import FILE_TO_VRAM, address_from_name  # noqa: E402

AS = "mipsel-none-elf-as"
LD = "mipsel-none-elf-ld"
OBJCOPY = "mipsel-none-elf-objcopy"

ADDR_COMMENT_RE = re.compile(r"^\s*/\* (?P<vaddr>[0-9A-Fa-f]{8}) (?P<word>[0-9A-Fa-f]{8}) \*/")


def stub_range(path: Path) -> tuple[int, int] | None:
    addresses = [
        int(match.group("vaddr"), 16)
        for match in (ADDR_COMMENT_RE.match(line) for line in path.read_text().splitlines())
        if match is not None
    ]
    if not addresses:
        return None
    return min(addresses), max(addresses) + 4


def check_stub(path: Path, target: bytes, symbols: dict[str, int], workdir: Path) -> str | None:
    span = stub_range(path)
    if span is None:
        return "no addresses"
    start, end = span

    result = subprocess.run(
        [AS, "-EL", "-G0", "-march=r3000", "-mtune=r3000", "-no-pad-sections",
         f"-I{path.parent}", "-o", str(workdir / "unit.o"), str(path)],
        capture_output=True, text=True,
    )
    if result.returncode != 0:
        return "assembler: " + result.stderr.strip().splitlines()[-1]

    undefined = subprocess.run(
        ["mipsel-none-elf-nm", "-u", str(workdir / "unit.o")],
        capture_output=True, text=True, check=True,
    ).stdout.split()
    script_lines = []
    for name in sorted(set(undefined) - {"U"}):
        address = symbols.get(name)
        if address is None:
            address = address_from_name(name)
        if address is None:
            return f"cannot place symbol {name}"
        script_lines.append(f"{name} = 0x{address:08X};")
    # .MIPS.abiflags is an allocated orphan section: left in, ld puts it first
    # and every absolute address in the function comes out 0x1C too high.
    script_lines.append(
        f"SECTIONS {{ /DISCARD/ : {{ *(.MIPS.abiflags) *(.reginfo) *(.pdr) *(.mdebug*) "
        f"*(.comment) }} . = 0x{start:08X}; .all : {{ *(.text*) *(.rodata*) *(.data*) }} }}"
    )
    (workdir / "link.ld").write_text("\n".join(script_lines) + "\n")

    result = subprocess.run(
        [LD, "-EL", "-T", str(workdir / "link.ld"), "-o", str(workdir / "unit.elf"),
         str(workdir / "unit.o")],
        capture_output=True, text=True,
    )
    if result.returncode != 0:
        return "linker: " + result.stderr.strip().splitlines()[-1]

    subprocess.run(
        [OBJCOPY, "-O", "binary", "--only-section=.all",
         str(workdir / "unit.elf"), str(workdir / "unit.bin")],
        check=True, capture_output=True,
    )
    built = (workdir / "unit.bin").read_bytes()
    want = target[start - FILE_TO_VRAM : end - FILE_TO_VRAM]
    if len(built) != len(want):
        return f"size {len(built):#x} != {len(want):#x}"
    for offset in range(0, len(want), 4):
        got = struct.unpack_from("<I", built, offset)[0]
        expected = struct.unpack_from("<I", want, offset)[0]
        if got != expected:
            return f"0x{start + offset:08X}: got {got:08X} want {expected:08X}"
    return None


def linker_symbols(root: Path, version: str, basename: str) -> dict[str, int]:
    """Every symbol the real link defines, so the standalone link can too."""
    symbols: dict[str, int] = {}
    paths = list((root / "linkers" / version).glob("*.txt"))
    paths += list((root / "configs" / version).glob("sym.*.txt"))
    assign = re.compile(r"^\s*(?P<name>[A-Za-z_][A-Za-z0-9_]*)\s*=\s*0x(?P<value>[0-9A-Fa-f]+);")
    for path in paths:
        for line in path.read_text(errors="ignore").splitlines():
            match = assign.match(line)
            if match:
                symbols.setdefault(match.group("name"), int(match.group("value"), 16))
    labels = root / "configs" / version / f"nonmatching_labels.{basename}.txt"
    if labels.exists():
        for raw in labels.read_text().splitlines():
            parts = raw.split()
            if len(parts) == 3 and not raw.startswith("#"):
                symbols.setdefault(parts[2], int(parts[1], 16))
    data_root = root / "asm" / version / basename / "data"
    pending: list[str] = []
    for path in sorted(data_root.rglob("*.s")) if data_root.exists() else []:
        for line in path.read_text(errors="ignore").splitlines():
            label = re.match(r"^\s*(?:glabel|dlabel|jlabel)\s+(\S+)", line)
            if label:
                pending.append(label.group(1))
                continue
            addr = re.match(r"^\s*/\* [0-9A-Fa-f]+ ([0-9A-Fa-f]{8}) \*/", line)
            if addr and pending:
                for name in pending:
                    symbols.setdefault(name, int(addr.group(1), 16))
                pending.clear()
    return symbols


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--version", default="PAL")
    parser.add_argument("--basename", default="main")
    parser.add_argument("patterns", nargs="*")
    args = parser.parse_args()

    root = Path.cwd()
    target = (root / "assets" / args.version / f"{args.basename}.exe").read_bytes()
    symbols = linker_symbols(root, args.version, args.basename)
    stub_root = root / "asm" / args.version / args.basename / "nonmatchings"

    failures: list[tuple[str, str]] = []
    checked = 0
    with tempfile.TemporaryDirectory() as tmp:
        for path in sorted(stub_root.rglob("*.s")):
            if args.patterns and not any(pattern in str(path) for pattern in args.patterns):
                continue
            workdir = Path(tmp)
            problem = check_stub(path, target, symbols, workdir)
            checked += 1
            if problem is not None:
                failures.append((path.stem, problem))

    print(f"verified {checked - len(failures)}/{checked} stubs")
    for name, problem in failures:
        print(f"  {name}: {problem}")
    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
