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
        sys.stderr.write(f"maspsx_func_80038CE8: expected {label} once, found {count}\n")
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
        """subu\t$sp,$sp,40
sw\t$16,16($sp)

.loc\t1 7
LM2:
sw\t$17,20($sp)

.loc\t1 8
LM3:
addu\t$17,$4,$zero""",
        """subu\t$sp,$sp,40
sw\t$17,20($sp)
addu\t$17,$4,$zero
sw\t$16,16($sp)""",
        "prologue schedule",
    )
    out = replace_once(
        out,
        "sll\t$3,$4,16",
        "sll\t$3,$3,16",
        "base angle sign source",
    )
    out = replace_once(
        out,
        """addu\t$3,$3,-1024

$L3:

.loc\t1 28
LM14:
addu\t$3,$3,1024
$L20:
andi\t$18,$3,0x0fff

.loc\t1 31
LM15:
lw\t$3,164($17)
nop # DEBUG: Reuse of '$3'. 'slt\t$2,$3,801' does not use $at

.loc\t1 33
LM16:
slt\t$2,$3,801""",
        """addu\t$2,$3,-1024

$L3:

.loc\t1 28
LM14:
addu\t$2,$3,1024
$L20:
andi\t$2,$2,0x0fff

.loc\t1 31
LM15:
lw\t$3,164($17)
addu\t$18,$2,$zero

.loc\t1 33
LM16:
slt\t$2,$3,801""",
        "first angle materialization",
    )
    out = replace_once(
        out,
        """sll\t$16,$16,16
sra\t$16,$16,16
mult\t$2,$16
mflo\t$3""",
        """sll\t$3,$16,16
sra\t$3,$3,16
mult\t$3,$2
mflo\t$3""",
        "high speed multiply register",
    )
    out = replace_once(
        out,
        """j\t$L9
sra\t$3,$3,16""",
        """j\t$L9
sra\t$2,$3,16""",
        "high speed result register",
    )
    out = replace_once(
        out,
        """addu\t$3,$2,$zero

.loc\t1 47
LM26:
sll\t$2,$3,1
addu\t$2,$2,$3
sll\t$2,$2,3
addu\t$2,$2,$3
sll\t$3,$2,1

.loc\t1 48
LM27:
bgez\t$3,$L10
nop  # DEBUG: branch/jump

.loc\t1 49
LM28:
addu\t$3,$3,4095
$L10:

.loc\t1 51
LM29:
sra\t$3,$3,12
$L9:

.loc\t1 54
LM30:
addu\t$16,$3,10

.loc\t1 55
LM31:
addu\t$19,$18,$zero

.loc\t1 56
LM32:
sll\t$16,$16,16
sra\t$18,$16,16

.loc\t1 57
LM33:
jal\tfunc_80068568
addu\t$4,$19,$zero

mult\t$2,$18
mflo\t$3""",
        """sll\t$3,$2,1
addu\t$3,$3,$2
sll\t$3,$3,3
addu\t$3,$3,$2
sll\t$2,$3,1

.loc\t1 48
LM27:
bgez\t$2,$L10
nop  # DEBUG: branch/jump

.loc\t1 49
LM28:
addu\t$2,$2,4095
$L10:

.loc\t1 51
LM29:
sra\t$2,$2,12
$L9:

.loc\t1 54
LM30:
addu\t$16,$2,10

.loc\t1 55
LM31:
addu\t$19,$18,$zero

.loc\t1 57
LM33:
jal\tfunc_80068568
addu\t$4,$19,$zero

.loc\t1 56
LM32:
sll\t$3,$16,16
sra\t$18,$3,16
mult\t$2,$18
mflo\t$2""",
        "low speed multiply and sine call",
    )
    out = replace_once(
        out,
        """bgez\t$3,$L22
sra\t$16,$3,12


.loc\t1 59
LM35:
addu\t$3,$3,4095

.loc\t1 61
LM36:
sra\t$16,$3,12""",
        """bgez\t$2,$L22
sra\t$16,$2,12


.loc\t1 59
LM35:
addu\t$2,$2,4095

.loc\t1 61
LM36:
sra\t$16,$2,12""",
        "sine signed shift",
    )
    out = replace_once(
        out,
        """mult\t$2,$18
mflo\t$3

.loc\t1 63
LM38:
bgez\t$3,$L12
ori\t$2,$zero,30


.loc\t1 64
LM39:
addu\t$3,$3,4095
$L12:

.loc\t1 68
LM40:
j\t$L13
sra\t$6,$3,12""",
        """mult\t$2,$18
mflo\t$2

.loc\t1 63
LM38:
bgez\t$2,$L12
sra\t$6,$2,12


.loc\t1 64
LM39:
addu\t$2,$2,4095
sra\t$6,$2,12
$L12:

.loc\t1 68
LM40:
j\t$L13
ori\t$2,$zero,30""",
        "cosine signed shift",
    )
    out = replace_once(
        out,
        """$L21:
andi\t$18,$3,0x0fff

.loc\t1 78
LM48:
jal\tfunc_80068568
addu\t$4,$18,$zero""",
        """$L21:
andi\t$4,$3,0x0fff

.loc\t1 78
LM48:
jal\tfunc_80068568
addu\t$18,$4,$zero""",
        "fallback angle call",
    )
    out = replace_once(
        out,
        """addu\t$3,$3,-1024

$L15:

.loc\t1 75
LM47:
addu\t$3,$3,1024
$L21:
andi\t$4,$3,0x0fff""",
        """addu\t$2,$3,-1024

$L15:

.loc\t1 75
LM47:
addu\t$2,$3,1024
$L21:
andi\t$4,$2,0x0fff""",
        "fallback angle materialization",
    )
    out = replace_once(
        out,
        """sll\t$3,$2,2
addu\t$3,$3,$2
sll\t$3,$3,2

.loc\t1 79
LM49:
bgez\t$3,$L23
sra\t$16,$3,12


.loc\t1 80
LM50:
addu\t$3,$3,4095

.loc\t1 82
LM51:
sra\t$16,$3,12""",
        """sll\t$3,$2,2
addu\t$3,$3,$2
sll\t$2,$3,2

.loc\t1 79
LM49:
bgez\t$2,$L23
sra\t$16,$2,12


.loc\t1 80
LM50:
addu\t$2,$2,4095

.loc\t1 82
LM51:
sra\t$16,$2,12""",
        "fallback sine multiply register",
    )
    out = replace_once(
        out,
        """sll\t$3,$2,2
addu\t$3,$3,$2
sll\t$3,$3,2

.loc\t1 84
LM53:
bgez\t$3,$L18
ori\t$2,$zero,15


.loc\t1 85
LM54:
addu\t$3,$3,4095
$L18:

.loc\t1 88
LM55:
j\t$L13
sra\t$6,$3,12""",
        """sll\t$3,$2,2
addu\t$3,$3,$2
sll\t$2,$3,2

.loc\t1 84
LM53:
bgez\t$2,$Lmode15
sra\t$6,$2,12


.loc\t1 85
LM54:
addu\t$2,$2,4095

.loc\t1 88
LM55:
j\t$Lmode15
sra\t$6,$2,12
$L18:""",
        "fallback cosine multiply register",
    )
    out = replace_once(
        out,
        """
.loc\t1 92
LM58:
ori\t$2,$zero,15""",
        """$Lmode15:
.loc\t1 92
LM58:
ori\t$2,$zero,15""",
        "fallback mode label anchor",
    )
    out = replace_once(
        out,
        """srl\t$2,$16,31
addu\t$16,$16,$2
sra\t$16,$16,1

.loc\t1 90
LM57:
srl\t$2,$6,31
addu\t$6,$6,$2
sra\t$6,$6,1""",
        """srl\t$2,$16,31
addu\t$2,$16,$2
sra\t$16,$2,1

.loc\t1 90
LM57:
srl\t$2,$6,31
addu\t$2,$6,$2
sra\t$6,$2,1""",
        "signed half register schedule",
    )
    sys.stdout.write(out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
