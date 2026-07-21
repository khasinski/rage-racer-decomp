#!/usr/bin/env python3
from __future__ import annotations

import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
REAL_MASPSX = ROOT / "build" / "toolchain" / "maspsx" / "maspsx.py"


def replace_once(text: str, old: str, new: str, label: str) -> str:
    count = text.count(old)
    if count != 1:
        sys.stderr.write(f"maspsx_func_800383A8: expected {label} once, found {count}\n")
        sys.exit(1)
    return text.replace(old, new, 1)


def main() -> int:
    proc = subprocess.run(
        [sys.executable, str(REAL_MASPSX), *sys.argv[1:]],
        input=sys.stdin.read(),
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )
    if proc.stderr:
        sys.stderr.write(proc.stderr)
    if proc.returncode != 0:
        sys.stdout.write(proc.stdout)
        return proc.returncode

    out = proc.stdout
    out = replace_once(
        out,
        """sw\t$31,52($sp)
sw\t$17,44($sp)
sw\t$16,40($sp)

.loc\t1 18
LM5:
sw\t$4,176($18)

.loc\t1 19
LM6:
lhu\t$3,0($2)
#nop # DEBUG: 'addu\t$5,$5,1' does not load from $3

.loc\t1 21
LM7:
addu\t$5,$5,1

.loc\t1 20
LM8:
sh\t$3,174($18)

.loc\t1 22
LM9:
lw\t$17,D_801E4150
#nop # DEBUG: 'sll\t$16,$5,1' does not load from $17

.loc\t1 21
LM10:
sll\t$16,$5,1

.loc\t1 23
LM11:
sh\t$0,138($18)

.loc\t1 24
LM12:
sw\t$4,188($18)

.loc\t1 25
LM13:
lhu\t$2,0($2)
#nop # DEBUG: 'addu\t$16,$16,$5' does not load from $2

.loc\t1 21
LM14:
addu\t$16,$16,$5

.loc\t1 25
LM15:
sh\t$2,290($18)

.loc\t1 28
LM16:
lw\t$3,D_801E408C
#nop # DEBUG: 'sll\t$16,$16,2' does not load from $3

.loc\t1 21
LM17:
sll\t$16,$16,2""",
        """addu\t$5,$5,1
sw\t$16,40($sp)
sll\t$16,$5,1
sw\t$31,52($sp)
sw\t$17,44($sp)
sw\t$4,176($18)
lhu\t$3,0($2)
lw\t$17,D_801E4150
addu\t$16,$16,$5
sh\t$0,138($18)
sw\t$4,188($18)
sh\t$3,174($18)
lhu\t$2,0($2)
lw\t$3,D_801E408C
sll\t$16,$16,2
sh\t$2,290($18)""",
        "initial setup schedule",
    )
    out = replace_once(
        out,
        """sw\t$0,32($18)

.loc\t1 43
LM32:
lw\t$3,D_8009E688
#nop # DEBUG: 'sll\t$2,$4,1' does not load from $3
sll\t$2,$4,1
addu\t$2,$2,$4
sll\t$2,$2,3
addu\t$2,$2,$3

.loc\t1 41
LM33:
sll\t$4,$6,11
ori\t$3,$zero,3072

.loc\t1 43
LM34:
lh\t$2,10($2)
#nop # DEBUG: 'subu\t$3,$3,$4' does not load from $2

.loc\t1 41
LM35:
subu\t$3,$3,$4""",
        """ori\t$3,$zero,3072
sw\t$0,32($18)
sll\t$5,$6,11
sll\t$2,$4,1
addu\t$2,$2,$4
lw\t$4,D_8009E688
sll\t$2,$2,3
addu\t$2,$2,$4
lh\t$2,10($2)
subu\t$3,$3,$5""",
        "heading calculation schedule",
    )
    sys.stdout.write(out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
