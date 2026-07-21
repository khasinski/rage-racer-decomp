#!/usr/bin/env python3
from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
REAL_MASPSX = ROOT / "build" / "toolchain" / "maspsx" / "maspsx.py"


def replace_once(text: str, old: str, new: str, label: str) -> str:
    count = text.count(old)
    if count != 1:
        sys.stderr.write(f"maspsx_38D80_quad_wrapper_schedule: expected {label} once, found {count}\n")
        sys.exit(1)
    return text.replace(old, new, 1)


def strip_debug_labels(text: str) -> str:
    lines = []
    for line in text.splitlines():
        if line.startswith(".loc\t"):
            continue
        if re.match(r"^LM[0-9]+:$", line):
            continue
        if line.startswith("#nop # DEBUG"):
            continue
        lines.append(line)
    return "\n".join(lines) + "\n"


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

    out = strip_debug_labels(proc.stdout)
    out = replace_once(
        out,
        """mult\t$4,$2
mflo\t$2

srl\t$7,$2,5
addu\t$7,$5,$7

lh\t$5,6($6)

bgez\t$3,$L5
lui\t$2,(-65536 >> 16) & 0xFFFF


sra\t$3,$3,16

j\t$L6
or\t$2,$3,$2

$L5:

sra\t$2,$3,16
andi\t$2,$2,0x7fff
$L6:

mult\t$4,$2

lh\t$12,8($6)

mflo\t$2

srl\t$11,$2,5

andi\t$2,$9,0x8000
beq\t$2,$0,$L7
addu\t$11,$5,$11


lui\t$2,(-65536 >> 16) & 0xFFFF

j\t$L8
or\t$2,$9,$2

$L7:

andi\t$2,$9,0x7fff
$L8:

mult\t$4,$2

lh\t$6,10($6)

mflo\t$2

srl\t$2,$2,5

bgez\t$9,$L9
addu\t$12,$12,$2


sra\t$9,$9,16
lui\t$2,(-65536 >> 16) & 0xFFFF

j\t$L10
or\t$2,$9,$2

$L9:

sra\t$9,$9,16
andi\t$2,$9,0x7fff
$L10:

mult\t$4,$2

lbu\t$3,13($8)
nop # DEBUG: Reuse of '$3'. 'andi\t$3,$3,0x0003' does not use $at
andi\t$3,$3,0x0003

mflo\t$2

srl\t$2,$2,5
addu\t$6,$6,$2

ori\t$2,$zero,1
beq\t$3,$2,$L13
slt\t$2,$3,2
""",
        """mult\t$4,$2
mflo\t$2

srl\t$2,$2,5
addu\t$5,$5,$2
addu\t$7,$5,$zero

lh\t$5,6($6)

bgez\t$3,$L5
sra\t$2,$3,16

lui\t$3,(-65536 >> 16) & 0xFFFF

j\t$L6
or\t$2,$2,$3

$L5:

andi\t$2,$2,0x7fff
$L6:

mult\t$4,$2
mflo\t$2

srl\t$2,$2,5
addu\t$2,$5,$2
addu\t$11,$2,$zero

lh\t$3,8($6)

andi\t$2,$9,0x8000
beq\t$2,$0,$L7
lui\t$2,(-65536 >> 16) & 0xFFFF

j\t$L8
or\t$2,$9,$2

$L7:

andi\t$2,$9,0x7fff
$L8:

mult\t$4,$2
mflo\t$2

srl\t$2,$2,5
addu\t$2,$3,$2

lh\t$6,10($6)

bgez\t$9,$L9
addu\t$12,$2,$zero

sra\t$2,$9,16
lui\t$3,(-65536 >> 16) & 0xFFFF

j\t$L10
or\t$2,$2,$3

$L9:

sra\t$2,$9,16
andi\t$2,$2,0x7fff
$L10:

mult\t$4,$2
mflo\t$2

srl\t$2,$2,5
addu\t$6,$6,$2

lbu\t$2,13($8)
nop
andi\t$3,$2,0x0003

ori\t$2,$zero,1
beq\t$3,$2,$L13
addu\t$9,$6,$zero
slti\t$2,$3,2
""",
        "interpolation schedule",
    )
    out = replace_once(
        out,
        "beq\t$3,$0,$L12\nsll\t$3,$11,16",
        "beq\t$3,$0,$L12\nsll\t$6,$11,16",
        "zero mode y0 delay",
    )
    out = replace_once(
        out,
        "beq\t$3,$2,$L15\nsll\t$3,$11,16",
        "beq\t$3,$2,$L15\nsll\t$6,$11,16",
        "mode 3 y0 delay",
    )
    out = replace_once(
        out,
        "$L11:\n\nsll\t$3,$11,16",
        "$L11:\n\nsll\t$6,$11,16",
        "fallthrough y0 setup",
    )
    out = replace_once(
        out,
        """sra\t$3,$3,16
sll\t$5,$7,16
sra\t$5,$5,16

addu\t$6,$11,$6

sll\t$6,$6,16
sra\t$6,$6,16

addu\t$7,$7,$12

sll\t$7,$7,16

lbu\t$4,13($8)

sra\t$7,$7,16
sw\t$3,16($sp)
sw\t$5,20($sp)
sw\t$6,24($sp)
sw\t$7,28($sp)
sw\t$6,32($sp)
""",
        """sra\t$6,$6,16
sll\t$5,$7,16
sra\t$5,$5,16

addu\t$2,$11,$9

sll\t$2,$2,16
sra\t$2,$2,16

addu\t$7,$7,$12

sll\t$7,$7,16

lbu\t$4,13($8)

sra\t$7,$7,16
sw\t$6,16($sp)
sw\t$5,20($sp)
sw\t$2,24($sp)
sw\t$7,28($sp)
sw\t$2,32($sp)
""",
        "final y2 temp",
    )
    out = replace_once(
        out,
        "lbu\t$2,12($8)\naddu\t$6,$3,$zero\nsw\t$2,76($sp)",
        "lbu\t$2,12($8)\nnop\nsw\t$2,76($sp)",
        "final a2 move slot",
    )

    sys.stdout.write(out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
