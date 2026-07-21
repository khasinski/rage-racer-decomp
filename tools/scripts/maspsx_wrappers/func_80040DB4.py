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
        sys.stderr.write(f"maspsx_func_80040DB4: expected {label} once, found {count}\n")
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
        """$Lb0:

.loc\t1 30
LM2:
lui\t$2,(1717960704 >> 16) & 0xFFFF
lw\t$5,D_8009E6A4
#nop # DEBUG: 'ori\t$2,$2,0x6667' does not load from $5
ori\t$2,$2,0x6667
mult\t$5,$2

.loc\t1 17
LM3:
subu\t$sp,$sp,24

.loc\t1 29
LM4:
lw\t$6,D_801E4150
#nop # DEBUG: 'move\t$9,$4' does not load from $6

.loc\t1 28
LM5:
addu\t$9,$4,$zero

.loc\t1 17
LM6:
sw\t$31,16($sp)

.loc\t1 30
LM7:
sra\t$3,$5,31
mfhi\t$2
sra\t$2,$2,1
subu\t$3,$2,$3
sll\t$2,$3,2
addu\t$2,$2,$3
subu\t$3,$5,$2
""",
        """$Lb0:
addu\t$9,$4,$zero
lui\t$2,(1717960704 >> 16) & 0xFFFF
lw\t$4,D_8009E6A4
ori\t$2,$2,0x6667
mult\t$4,$2
lw\t$2,D_801E4150
subu\t$sp,$sp,24
addu\t$6,$2,7324
sra\t$2,$4,31
mfhi\t$3
sra\t$3,$3,1
subu\t$3,$3,$2
sll\t$2,$3,2
addu\t$2,$2,$3
subu\t$4,$4,$2
""",
        "prologue selector schedule",
    )
    local_table = "\n".join(
        [
            ".section .rodata",
            ".align\t3",
            "$L10:",
            ".word\t$L5",
            ".word\t$L7",
            ".word\t$L7",
            ".word\t$L9",
            ".word\t$L9",
            ".word\t$L5",
            ".text",
            "",
        ]
    )
    out = replace_once(
        out,
        """sltu\t$2,$3,6
beq\t$2,$0,$L5
addu\t$6,$6,7324

sll\t$2,$3,2
# EXPAND_AT START
.set\tnoat
lui\t$at,%hi($L10)
addu\t$at,$at,$2
lw\t$2,%lo($L10)($at)
.set\tat
# EXPAND_AT END
nop # DEBUG: Reuse of '$2'. 'j\t$2' does not use $at
j\t$2
nop  # DEBUG: branch/jump
"""
        + local_table,
        """sltu\t$2,$4,6
beq\t$2,$0,$L5
sw\t$31,16($sp)

sll\t$2,$4,2
# EXPAND_AT START
.set\tnoat
lui\t$at,%hi(jtbl_800114D4)
addu\t$at,$at,$2
lw\t$2,%lo(jtbl_800114D4)($at)
.set\tat
# EXPAND_AT END
nop # DEBUG: Reuse of '$2'. 'j\t$2' does not use $at
j\t$2
nop  # DEBUG: branch/jump
""",
        "switch jump table ownership",
    )
    out = replace_once(
        out,
        """bne\t$2,$0,$L22
nop  # DEBUG: branch/jump
$Lb3:
$Le4:
$L17:""",
        """bgtz\t$2,$L24
subu\t$2,$9,$6
$Lb3:
$Le4:
$L17:""",
        "leading edge branch delay",
    )
    out = replace_once(
        out,
        """$L22:

.loc\t1 73
LM13:
subu\t$6,$9,$6

.loc\t1 75
LM14:
j\t$L24
mult\t$10,$6

$L23:

.loc\t1 77
LM15:
mult\t$10,$8
$L24:
mflo\t$2
lui\t$3,(1374355456 >> 16) & 0xFFFF
ori\t$3,$3,0x851f
mult\t$2,$3
sra\t$2,$2,31
mfhi\t$5
sra\t$5,$5,8

.loc\t1 78
LM16:
j\t$L13
subu\t$5,$5,$2
""",
        """$L22:

.loc\t1 73
LM13:

.loc\t1 75
LM14:

$L23:

.loc\t1 77
LM15:
$L24:
mult\t$10,$2
mflo\t$3
lui\t$2,(1374355456 >> 16) & 0xFFFF
ori\t$2,$2,0x851f
mult\t$3,$2
sra\t$3,$3,31
mfhi\t$2
sra\t$2,$2,8

.loc\t1 78
LM16:
j\t$L13
subu\t$5,$2,$3
""",
        "blend quotient register",
    )
    out = replace_once(
        out,
        """beq\t$2,$0,$L19
andi\t$3,$3,0x0002

bne\t$3,$0,$L23
subu\t$8,$8,$9

$Lb5:
$Le6:
$L19:

.loc\t1 81
LM32:
j\t$L13
addu\t$5,$10,$zero
""",
        """beq\t$2,$0,$L19
addu\t$5,$10,$zero

andi\t$2,$3,0x0002
bgtz\t$2,$L24
subu\t$2,$8,$9

$Lb5:
$Le6:
$L19:

.loc\t1 81
LM32:
j\t$L13_DONE
sll\t$5,$5,16
""",
        "trailing edge default delay slot",
    )
    out = replace_once(
        out,
        """$L13:

.loc\t1 87
LM36:
sll\t$5,$5,16
sra\t$5,$5,16
jal\tfunc_8005C31C
addu\t$6,$5,$zero
""",
        """$L13:

.loc\t1 87
LM36:
sll\t$5,$5,16
$L13_DONE:
sra\t$5,$5,16
jal\tfunc_8005C31C
addu\t$6,$5,$zero
""",
        "shared sign extension label",
    )
    sys.stdout.write(out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
