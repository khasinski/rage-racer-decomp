#!/usr/bin/env python3
"""Measure how far a candidate C body is from the retail function, in words.

    tools/scripts/measure_candidate.py func_8005D050 path/to/candidate.c
    tools/scripts/measure_candidate.py func_8005D050 build/.../unit.c.o --verbose

Reports three numbers, and the distinction between them has repeatedly decided
wrong conclusions in this project, so they are printed separately and labelled:

  exact    positional differing words. **This is the only number that maps to
           byte-exactness.** Zero here means the function is done.
  aligned  differing instructions after aligning the two streams. Tolerates an
           inserted or deleted instruction, so it says "how close is the shape".
           An edit that fixed an entire stack frame once *raised* this by 3.
  equal    words matching under that same alignment; useful as a progress guide
           while `exact` is still large.

Three measurement bugs this replaces, each of which produced a wrong answer that
was acted on:

  * Relocations were not resolved, so an address that links to the same word as
    retail counted as a difference. Both sides are linked here, with every
    undefined `D_`/`func_` symbol defined at the address encoded in its name.
  * The retail extent was truncated at an interior `.globl`. Several stubs carry
    mid-function labels, so one 345-word function was being compared against 158
    words of itself. The whole `.text` of the target file is used.
  * `aligned` was quoted as if it were a byte difference.

Never compare `objdump` text instead: the `LM<n>` COFF debug labels differ
harmlessly between builds and have twice been mistaken for real differences.
"""
import argparse
import difflib
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
PREFIX = "mipsel-none-elf-"

# branches are PC-relative, so their target field moves when anything shifts;
# `j`/`jal` inside the function likewise. Normalise them for the alignment pass.
BRANCH_OPS = {0x01, 0x04, 0x05, 0x06, 0x07, 0x14, 0x15, 0x16, 0x17}


def run(cmd: list[str]) -> str:
    return subprocess.run(cmd, capture_output=True, text=True, check=True).stdout


def find_target_asm(func: str) -> Path:
    """The generated stub for this function, wherever make split put it."""
    hits = list((ROOT / "asm").rglob(f"{func}.s"))
    if not hits:
        raise SystemExit(f"no assembly found for {func}; run `make split VERSION=PAL` first")
    if len(hits) > 1:
        raise SystemExit(f"{func} is ambiguous: {', '.join(str(h) for h in hits)}")
    return hits[0]


def symbol_offset(obj: Path, func: str) -> int:
    for line in run([PREFIX + "nm", str(obj)]).splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[2] == func:
            return int(parts[0], 16)
    raise SystemExit(f"{func} is not defined in {obj}")


def link_and_extract(obj: Path, func: str, vaddr: int, tmp: Path, tag: str) -> bytes:
    """Link at the real address with every undefined address-symbol satisfied."""
    offset = symbol_offset(obj, func)
    defines: list[str] = []
    for line in run([PREFIX + "nm", "-u", str(obj)]).splitlines():
        name = line.split()[-1]
        m = re.fullmatch(r"(?:D|func|jtbl)_([0-9A-Fa-f]{8})", name)
        # an address-encoded name defines itself; anything else just needs to exist
        defines += ["--defsym", f"{name}=0x{m.group(1)}" if m else f"{name}=0x80000000"]

    elf = tmp / f"{tag}.elf"
    binary = tmp / f"{tag}.bin"
    subprocess.run(
        [PREFIX + "ld", "-EL", "-e", "0", "--defsym", "_gp=0x80000000",
         "-Ttext", hex(vaddr - offset), *defines, "-o", str(elf), str(obj)],
        check=True, capture_output=True,
    )
    subprocess.run(
        [PREFIX + "objcopy", "-O", "binary", "--only-section=.text", str(elf), str(binary)],
        check=True,
    )
    return binary.read_bytes()[offset:]


def words(data: bytes) -> list[int]:
    return [int.from_bytes(data[i:i + 4], "little") for i in range(0, len(data) - 3, 4)]


def normalise(word: int) -> int:
    op = word >> 26
    if op in BRANCH_OPS:
        return word & 0xFFFF0000
    if op == 0x02:  # j, whose target shifts with the function
        return 0x08000000
    return word


def function_words(obj: Path, func: str) -> int | None:
    for line in run([PREFIX + "nm", "-S", str(obj)]).splitlines():
        parts = line.split()
        if len(parts) == 4 and parts[3] == func:
            return int(parts[1], 16) // 4
    return None


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("func", help="func_XXXXXXXX")
    ap.add_argument("candidate", type=Path, help=".c to compile, or a prebuilt .o")
    ap.add_argument("--target", type=Path, help="override the retail .s")
    ap.add_argument("--verbose", "-v", action="store_true", help="list the differing words")
    args = ap.parse_args()

    if not re.fullmatch(r"func_[0-9A-Fa-f]{8}", args.func):
        raise SystemExit("function must be spelled func_XXXXXXXX")
    vaddr = int(args.func.split("_")[1], 16)
    target_asm = args.target or find_target_asm(args.func)

    with tempfile.TemporaryDirectory() as td:
        tmp = Path(td)

        if args.candidate.suffix == ".o":
            cand_obj = args.candidate
        else:
            cand_obj = tmp / "cand.o"
            subprocess.run([str(ROOT / "tools/scripts/cc.sh"), str(args.candidate), str(cand_obj)],
                           check=True, cwd=ROOT)

        tgt_obj = tmp / "tgt.o"
        subprocess.run([PREFIX + "as", "-EL", "-G0", "-march=r3000", "-mtune=r3000",
                        "-no-pad-sections", "-I", str(ROOT / "include"),
                        "-o", str(tgt_obj), str(target_asm)], check=True, cwd=ROOT)

        target = words(link_and_extract(tgt_obj, args.func, vaddr, tmp, "tgt"))
        cand_all = words(link_and_extract(cand_obj, args.func, vaddr, tmp, "cand"))
        # must happen before the temporary directory goes away
        cand_len = function_words(cand_obj, args.func) or len(cand_all)

    n = len(target)
    cand = cand_all[:cand_len]

    exact = sum(1 for i in range(n) if i >= len(cand_all) or cand_all[i] != target[i])
    first = next((i for i in range(n) if i >= len(cand_all) or cand_all[i] != target[i]), None)

    matcher = difflib.SequenceMatcher(None, [normalise(w) for w in target],
                                      [normalise(w) for w in cand], autojunk=False)
    aligned = equal = 0
    for tag, i1, i2, j1, j2 in matcher.get_opcodes():
        if tag == "equal":
            equal += i2 - i1
            continue
        aligned += max(i2 - i1, j2 - j1)
        if args.verbose:
            for k in range(max(i2 - i1, j2 - j1)):
                want = f"{target[i1+k]:08X} @{vaddr + (i1+k)*4:08X}" if i1 + k < i2 else "-" * 8
                got = f"{cand[j1+k]:08X}" if j1 + k < j2 else "-" * 8
                print(f"  {tag:9} want {want:22} got {got}")

    print(f"{args.func}: retail {n} words, candidate {len(cand)} words")
    print(f"  exact   {exact:5}   <- the only number that means byte-exactness")
    print(f"  aligned {aligned:5}   shape difference, tolerates insertions")
    print(f"  equal   {equal:5}   of {n}")
    if first is not None:
        print(f"  first differing word at +0x{first * 4:X} (0x{vaddr + first * 4:08X})")
    return 0 if exact == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
