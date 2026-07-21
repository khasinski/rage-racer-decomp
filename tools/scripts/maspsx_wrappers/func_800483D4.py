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
        sys.stderr.write(f"maspsx_38BD4_poly_wrapper_schedule: expected {label} once, found {count}\n")
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
        """lw\t$7,0($6)

lw\t$3,8($6)

lw\t$13,528482308
""",
        """lw\t$7,0($6)

lw\t$13,528482308

lw\t$3,8($6)
""",
        "initial load order",
    )
    out = replace_once(
        out,
        """mult\t$4,$2

lh\t$6,6($6)

mflo\t$2

srl\t$2,$2,5

bgez\t$3,$L5
addu\t$9,$7,$2
""",
        """mult\t$4,$2

mflo\t$2

srl\t$2,$2,5
addu\t$2,$7,$2

lh\t$6,6($6)

bgez\t$3,$L5
addu\t$9,$2,$zero
""",
        "first product schedule",
    )
    out = replace_once(
        out,
        """sra\t$3,$3,16
lui\t$2,(-65536 >> 16) & 0xFFFF

j\t$L6
or\t$2,$3,$2

$L5:

sra\t$3,$3,16
andi\t$2,$3,0x7fff
""",
        """sra\t$2,$3,16
lui\t$3,(-65536 >> 16) & 0xFFFF

j\t$L6
or\t$2,$2,$3

$L5:

sra\t$2,$3,16
andi\t$2,$2,0x7fff
""",
        "high-half register schedule",
    )
    out = replace_once(
        out,
        """mult\t$4,$2

lhu\t$7,0($5)

lhu\t$3,4($5)

lhu\t$11,2($5)

lhu\t$12,6($5)

addu\t$7,$7,$9

addu\t$3,$3,$9

mflo\t$2

srl\t$2,$2,5
addu\t$6,$6,$2

addu\t$11,$11,$6

lbu\t$2,11($5)
nop # DEBUG: Reuse of '$2'. 'andi\t$4,$2,0x0003' does not use $at


andi\t$4,$2,0x0003
ori\t$2,$zero,1
beq\t$4,$2,$L9
addu\t$12,$12,$6

slt\t$2,$4,2
beq\t$2,$0,$L14
nop  # DEBUG: branch/jump
beq\t$4,$0,$L8
nop  # DEBUG: branch/jump
j\t$L7
nop  # DEBUG: branch/jump
$L14:
ori\t$2,$zero,2
beq\t$4,$2,$L10
ori\t$2,$zero,3

beq\t$4,$2,$L11
nop  # DEBUG: branch/jump
j\t$L7
nop  # DEBUG: branch/jump
""",
        """mult\t$4,$2

mflo\t$2

srl\t$2,$2,5
addu\t$6,$6,$2

lhu\t$2,2($5)

lhu\t$3,6($5)
addu\t$11,$2,$6
addu\t$12,$3,$6

lhu\t$2,0($5)
lhu\t$3,4($5)
addu\t$2,$9,$2
addu\t$7,$2,$zero

lbu\t$2,11($5)
nop # DEBUG: Reuse of '$2'. 'andi\t$4,$2,0x0003' does not use $at
andi\t$4,$2,0x0003
ori\t$2,$zero,1
beq\t$4,$2,$L9
addu\t$3,$9,$3

slt\t$2,$4,2
beq\t$2,$0,$L14
nop  # DEBUG: branch/jump
beq\t$4,$0,$L8
nop  # DEBUG: branch/jump
j\t$L7
nop  # DEBUG: branch/jump
$L14:
ori\t$2,$zero,2
beq\t$4,$2,$L10
ori\t$2,$zero,3

beq\t$4,$2,$L11
nop  # DEBUG: branch/jump
j\t$L7
nop  # DEBUG: branch/jump
""",
        "second product and mode switch schedule",
    )
    out = replace_once(
        out,
        """lbu\t$2,11($5)
nop # DEBUG: Reuse of '$2'. 'andi\t$8,$2,0x0004' does not use $at
andi\t$8,$2,0x0004


beq\t$8,$0,$L15
ori\t$15,$zero,128


andi\t$15,$2,0x0060
$L15:
""",
        """lbu\t$2,11($5)
nop # DEBUG: Reuse of '$2'. 'andi\t$8,$2,0x0004' does not use $at
andi\t$8,$2,0x0004

beq\t$8,$0,$L16
andi\t$2,$2,0x0060

j\t$L17
andi\t$4,$2,0x00ff

$L16:

ori\t$4,$zero,128
$L17:
""",
        "alpha branch schedule",
    )
    out = replace_once(
        out,
        """sll\t$9,$9,16

lbu\t$2,8($5)

lbu\t$4,9($5)

lbu\t$14,10($5)

sra\t$5,$9,16
sll\t$6,$6,16
sra\t$6,$6,16
sll\t$7,$7,16
sra\t$7,$7,16

sll\t$11,$11,16
sra\t$11,$11,16

sll\t$3,$3,16
sra\t$3,$3,16

sll\t$12,$12,16
sra\t$12,$12,16

sw\t$11,16($sp)
sw\t$3,20($sp)
sw\t$12,24($sp)
sw\t$8,40($sp)
sw\t$15,44($sp)
sw\t$4,32($sp)
sll\t$4,$10,2
addu\t$4,$13,$4
sw\t$2,28($sp)
jal\tfunc_80046BA0
sw\t$14,36($sp)
""",
        """sll\t$2,$11,16
sra\t$2,$2,16
sw\t$2,16($sp)
sll\t$2,$3,16
sra\t$2,$2,16
sw\t$2,20($sp)
sll\t$2,$12,16
sra\t$2,$2,16
sll\t$6,$6,16
sw\t$2,24($sp)
lbu\t$2,8($5)
sll\t$7,$7,16
sw\t$2,28($sp)
lbu\t$2,9($5)
sra\t$6,$6,16
sw\t$2,32($sp)
lbu\t$2,10($5)
sra\t$7,$7,16
sw\t$4,44($sp)
sll\t$4,$10,2
sll\t$5,$9,16
addu\t$4,$13,$4
sra\t$5,$5,16
sw\t$8,40($sp)
jal\tfunc_80046BA0
sw\t$2,36($sp)
""",
        "final argument setup schedule",
    )

    sys.stdout.write(out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
