#!/usr/bin/env python3
"""Build a decomp-permuter rig with settings that actually work on this project.

    tools/scripts/make_permuter_rig.py func_8005D050 candidate.c --out /tmp/perm-5D050

Why this exists: rigs here have been copied from one another for months, and the
inherited settings carried `perm_reorder_decls = 100`. On any base with
block-scoped or duplicate-named locals that makes roughly 94% of permutations
fail to compile. The iteration counter still climbs, so a run looks busy while
almost nothing is being tried — which means historical "N iterations, no
improvement" results from this project are not evidence of anything. Zeroing it
and hoisting declarations to function scope took the observed error rate to 0%.

Two further things this project has paid for, both encoded here:

  * Every fabrication weight is zero. That is necessary but **not sufficient**:
    a run with them zeroed still emitted `if (((!p) && (!p)) && (!p)) {}`. Audit
    every winner by hand before applying it.
  * The permuter's score is an aligned diff. It has been observed to be
    anti-correlated with byte-exactness here, hill-climbing by adding
    instructions the differ then aligns around. Gate every candidate through
    `tools/scripts/measure_candidate.py` and look at `exact`, not at the score.

The base must be self-contained: decomp-permuter preprocesses with plain
`cpp -P -nostdinc` and parses with pycparser, so no `#include`, no file-scope
`__asm__(".include ...")`, and no `asm("symbol")` aliases — spell every global
and callee with its raw `D_`/`func_` name. This script checks that and refuses
rather than letting you discover it after a wasted run.
"""
import argparse
import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

# everything that can invent code rather than rearrange it
FABRICATION = [
    "perm_condition", "perm_ins_block", "perm_empty_stmt", "perm_var_cond_block",
    "perm_dummy_comma_expr", "perm_add_self_assignment", "perm_duplicate_assignment",
    "perm_mult_zero", "perm_xor_zero", "perm_remove_ast", "perm_float_literal",
    "perm_pad_var_decl", "perm_randomize_external_type", "perm_randomize_function_type",
]

# rearrangements that are legitimate, with the weights that have worked here
REARRANGEMENT = {
    "perm_temp_for_expr": 100,
    "perm_expand_expr": 80,
    "perm_reorder_stmts": 70,
    "perm_associative": 40,
    "perm_commutative": 40,
    "perm_add_mask": 40,
    "perm_cast_simple": 50,
    "perm_refer_to_var": 90,
    "perm_sameline": 20,
    "perm_split_assignment": 70,
    # deliberately 0: at 100 it makes ~94% of permutations fail to compile on any
    # base with block-scoped locals, silently wasting the whole run
    "perm_reorder_decls": 0,
}

COMPILE_SH = """#!/usr/bin/env bash
INPUT="$(readlink -f "$1")"; OUTPUT="$3"
case "$OUTPUT" in /*) ;; *) OUTPUT="$PWD/$3";; esac
export PATH="$HOME/Projects/rage-pc/tbin:$HOME/.local/bin:$PATH"
# the assembler resolves include/macro.inc relative to the working directory
cd "$HOME/Projects/rage-pc" || exit 1
exec "$HOME/Projects/rage-pc/tools/scripts/cc.sh" "$INPUT" "$OUTPUT"
"""


def check_base(path: Path) -> list[str]:
    text = path.read_text()
    problems = []
    if "#include" in text:
        problems.append("contains #include; the permuter preprocesses with -nostdinc")
    if re.search(r'__asm__\s*\(\s*"\.include', text):
        problems.append('contains a file-scope __asm__(".include ...")')
    if re.search(r'\)\s*asm\s*\(\s*"', text):
        problems.append('contains asm("symbol") aliases; use the raw D_/func_ names instead')
    if re.search(r'register\s+[^;]*\basm\s*\(\s*"\$', text):
        problems.append('contains register pins asm("$N"); most rig builds cannot parse them')
    return problems


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("func", help="func_XXXXXXXX")
    ap.add_argument("base", type=Path, help="self-contained single-function C source")
    ap.add_argument("--out", type=Path, required=True, help="rig directory to create")
    ap.add_argument("--target", type=Path, help="retail .s (default: found under asm/)")
    ap.add_argument("--force", action="store_true", help="build the rig despite base warnings")
    args = ap.parse_args()

    if not re.fullmatch(r"func_[0-9A-Fa-f]{8}", args.func):
        raise SystemExit("function must be spelled func_XXXXXXXX")

    problems = check_base(args.base)
    if problems:
        for p in problems:
            print(f"base problem: {p}", file=sys.stderr)
        if not args.force:
            raise SystemExit("refusing to build a rig the permuter cannot parse; use --force to override")

    target = args.target
    if target is None:
        hits = list((ROOT / "asm").rglob(f"{args.func}.s"))
        if len(hits) != 1:
            raise SystemExit(f"could not find a unique retail .s for {args.func}")
        target = hits[0]

    out = args.out
    out.mkdir(parents=True, exist_ok=True)
    shutil.copy(args.base, out / "base.c")

    subprocess.run(["mipsel-none-elf-as", "-EL", "-G0", "-march=r3000", "-mtune=r3000",
                    "-no-pad-sections", "-I", str(ROOT / "include"),
                    "-o", str(out / "target.o"), str(target)], check=True, cwd=ROOT)

    (out / "compile.sh").write_text(COMPILE_SH)
    (out / "compile.sh").chmod(0o755)

    lines = [f'func_name = "{args.func}"', 'compiler_type = "gcc"', "", "[weight_overrides]"]
    lines += [f"{k} = {v}" for k, v in REARRANGEMENT.items()]
    lines += [f"{k} = 0" for k in FABRICATION]
    (out / "settings.toml").write_text("\n".join(lines) + "\n")

    print(f"rig ready: {out}")
    print(f"  base.c      {args.base}")
    print(f"  target.o    {target}")
    print("  run:  timeout 1800 python3 <permuter>/permuter.py --best-only --stop-on-zero -j 7 "
          f"{out}")
    print("  then: tools/scripts/measure_candidate.py "
          f"{args.func} <winner.c>   # trust `exact`, not the score")
    return 0


if __name__ == "__main__":
    sys.exit(main())
