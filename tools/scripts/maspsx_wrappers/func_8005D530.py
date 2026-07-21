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
        sys.stderr.write(f"maspsx_func_8005D530: expected {label} once, found {count}\n")
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

    out = replace_once(
        proc.stdout,
        """addu\t$2,$2,$5

.loc\t1 52
LM27:
lh\t$5,4($2)
#nop # DEBUG: 'sll\t$6,$6,16' does not load from $5

.loc\t1 53
LM28:

.loc\t1 54
LM29:
sll\t$6,$6,16
sra\t$6,$6,16

.loc\t1 55
LM30:
sll\t$7,$7,16
sra\t$7,$7,16

.loc\t1 57
LM31:
sll\t$2,$3,9
sra\t$2,$2,16

.loc\t1 58
LM32:
ori\t$4,$zero,19
ori\t$3,$zero,60
sw\t$3,16($sp)
""",
        """addu\t$2,$5,$2

.loc\t1 52
LM27:
lh\t$5,4($2)
ori\t$2,$zero,60
sw\t$2,16($sp)
sll\t$2,$3,9
sra\t$2,$2,16
sll\t$6,$6,16
sll\t$7,$7,16

.loc\t1 58
LM32:
ori\t$4,$zero,19
sra\t$6,$6,16
sra\t$7,$7,16
""",
        "single cue argument schedule",
    )

    out = replace_once(
        out,
        """sw\t$31,60($sp)
sw\t$22,56($sp)
sw\t$21,52($sp)
sw\t$19,44($sp)
sw\t$18,40($sp)
sw\t$17,36($sp)
sw\t$16,32($sp)

.loc\t1 86
LM41:
# EXPAND_AT START
.set\tnoat
lui\t$at,%hi(D_80011F5C)
addu\t$at,$at,$2
lw\t$7,%lo(D_80011F5C)($at)
.set\tat
# EXPAND_AT END
nop # DEBUG: Reuse of '$7'. 'mult\t$7,$5' does not use $at

.loc\t1 91
LM42:
mult\t$7,$5

.loc\t1 87
LM43:
# EXPAND_AT START
.set\tnoat
lui\t$at,%hi(D_80011F5C+4)
addu\t$at,$at,$2
lw\t$18,%lo(D_80011F5C+4)($at)
.set\tat
# EXPAND_AT END
#nop # DEBUG: 'lw\t$19,D_80011F5C+8($2)' does not load from $18

.loc\t1 88
LM44:
# EXPAND_AT START
.set\tnoat
lui\t$at,%hi(D_80011F5C+8)
addu\t$at,$at,$2
lw\t$19,%lo(D_80011F5C+8)($at)
.set\tat
# EXPAND_AT END
#nop # DEBUG: 'lw\t$22,D_80011F5C+12($2)' does not load from $19

.loc\t1 89
LM45:
# EXPAND_AT START
.set\tnoat
lui\t$at,%hi(D_80011F5C+12)
addu\t$at,$at,$2
lw\t$22,%lo(D_80011F5C+12)($at)
.set\tat
# EXPAND_AT END
#nop # DEBUG: 'mflo\t$3' does not load from $22
""",
        """# EXPAND_AT START
.set\tnoat
lui\t$at,%hi(D_80011F5C)
addu\t$at,$at,$2
lw\t$7,%lo(D_80011F5C)($at)
.set\tat
# EXPAND_AT END
sw\t$18,40($sp)
# EXPAND_AT START
.set\tnoat
lui\t$at,%hi(D_80011F60)
addu\t$at,$at,$2
lw\t$18,%lo(D_80011F60)($at)
.set\tat
# EXPAND_AT END
mult\t$7,$5
sw\t$19,44($sp)
# EXPAND_AT START
.set\tnoat
lui\t$at,%hi(D_80011F64)
addu\t$at,$at,$2
lw\t$19,%lo(D_80011F64)($at)
.set\tat
# EXPAND_AT END
sw\t$22,56($sp)
# EXPAND_AT START
.set\tnoat
lui\t$at,%hi(D_80011F68)
addu\t$at,$at,$2
lw\t$22,%lo(D_80011F68)($at)
.set\tat
# EXPAND_AT END
sw\t$16,32($sp)
addu\t$16,$6,$zero
sw\t$31,60($sp)
sw\t$21,52($sp)
""",
        "prologue/table-load schedule",
    )

    out = replace_once(
        out,
        """bgez\t$3,$L6
nop  # DEBUG: branch/jump
""",
        """bgez\t$3,$L6
sw\t$17,36($sp)
""",
        "left product rounding delay slot",
    )

    out = replace_once(
        out,
        """bgez\t$17,$L7
mult\t$7,$6
""",
        """bgez\t$17,$L7
mult\t$7,$16
""",
        "right input multiply operand",
    )

    out = replace_once(
        out,
        """mflo\t$16

.loc\t1 106
LM59:
bgez\t$16,$L9
nop  # DEBUG: branch/jump

.loc\t1 107
LM60:
addu\t$16,$16,127
""",
        """mflo\t$6

.loc\t1 106
LM59:
bgez\t$6,$L9
nop  # DEBUG: branch/jump

.loc\t1 107
LM60:
addu\t$6,$6,127
""",
        "right product result register",
    )

    out = replace_once(
        out,
        """jal\tfunc_8007B088
sra\t$16,$16,7

beq\t$2,$0,$L10
ori\t$2,$zero,61

beq\t$20,$2,$L10
ori\t$2,$zero,43

bne\t$20,$2,$L11
addu\t$2,$21,$zero

$L10:

.loc\t1 115
LM64:
ori\t$4,$zero,22
""",
        """jal\tfunc_8007B088
sra\t$16,$6,7

beq\t$2,$0,$L10
ori\t$4,$zero,22

ori\t$2,$zero,61
beq\t$20,$2,$L10
ori\t$2,$zero,43

bne\t$20,$2,$L11
addu\t$2,$21,$zero

$L10:

.loc\t1 115
LM64:
""",
        "cue gate branch schedule",
    )

    out = replace_once(
        out,
        """addu\t$7,$22,1
sll\t$7,$7,16
lh\t$5,0($18)
#nop # DEBUG: 'sra\t$7,$7,16' does not load from $5
sra\t$7,$7,16

.loc\t1 117
LM66:
sw\t$21,D_801E4D90

.loc\t1 118
LM67:
sw\t$20,16($sp)
""",
        """addu\t$7,$22,1
sll\t$7,$7,16
sw\t$21,D_801E4D90
lh\t$5,0($18)
sra\t$7,$7,16
sw\t$20,16($sp)
""",
        "second cue argument/store schedule",
    )

    out = replace_once(
        out,
        """sra\t$21,$2,16

.loc\t1 121
LM68:
addu\t$2,$21,$zero

.loc\t1 120
LM69:
sw\t$2,D_801E4D94
""",
        """sra\t$21,$2,16
sw\t$21,D_801E4D94
addu\t$2,$21,$zero
""",
        "return handle store schedule",
    )

    sys.stdout.write(out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
