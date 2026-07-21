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
        sys.stderr.write(f"maspsx_func_8003F700: expected {label} once, found {count}\n")
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
        """lw\t$2,D_801E408C
#nop # DEBUG: 'lw\t$4,D_801E4BA4' does not load from $2

.loc\t1 44
LM4:
lw\t$4,D_801E4BA4
#nop # DEBUG: 'lw\t$6,D_801E4174' does not load from $4

.loc\t1 45
LM5:
lw\t$6,D_801E4174
#nop # DEBUG: 'sll\t$2,$2,1' does not load from $6

.loc\t1 44
LM6:
sll\t$2,$2,1
addu\t$3,$2,$4

.loc\t1 45
LM7:
addu\t$2,$2,$6

.loc\t1 44
LM8:
lh\t$3,0($3)
#nop # DEBUG: 'lh\t$5,0($2)' does not load from $3

.loc\t1 45
LM9:
lh\t$5,0($2)
#nop # DEBUG: 'sh\t$0,D_801E4DB2' does not load from $5

.loc\t1 46
LM10:
sh\t$0,D_801E4DB2

.loc\t1 47
LM11:
sh\t$0,D_801E4DB2-2

.loc\t1 49
LM12:
sll\t$2,$3,2
addu\t$2,$2,$3
sll\t$2,$2,2
addu\t$2,$2,$4
addu\t$3,$2,4

.loc\t1 50
LM13:
sll\t$4,$5,1
addu\t$4,$4,$5
sll\t$4,$4,2
addu\t$4,$4,$6

.loc\t1 49
LM14:
sw\t$3,D_801E42DC

.loc\t1 50
LM15:
addu\t$3,$4,4
sw\t$3,D_801E42E8

.loc\t1 52
LM16:
lw\t$3,4($2)
#nop # DEBUG: 'lw\t$5,8($2)' does not load from $3

.loc\t1 53
LM17:
lw\t$5,8($2)
#nop # DEBUG: 'lw\t$6,12($2)' does not load from $5

.loc\t1 54
LM18:
lw\t$6,12($2)
#nop # DEBUG: 'lw\t$2,16($2)' does not load from $6

.loc\t1 55
LM19:
lw\t$2,16($2)
#nop # DEBUG: 'sw\t$3,D_801E4DB8' does not load from $2

.loc\t1 52
LM20:
sw\t$3,D_801E4DB8

.loc\t1 53
LM21:
sw\t$5,D_801E4DB8+4

.loc\t1 54
LM22:
sw\t$6,D_801E4DB8+8

.loc\t1 55
LM23:
sw\t$2,D_801E4DB8+12""",
        """lw\t$2,D_801E408C

.loc\t1 44
LM4:
lw\t$6,D_801E4BA4

.loc\t1 45
LM5:
lw\t$7,D_801E4174

.loc\t1 44
LM6:
sll\t$2,$2,1
addu\t$3,$2,$6

.loc\t1 45
LM7:
addu\t$2,$2,$7

.loc\t1 44
LM8:
lh\t$4,0($3)

.loc\t1 45
LM9:
lh\t$5,0($2)

.loc\t1 46
LM10:
la\t$8,D_801E4DB2
sh\t$0,0($8)
sh\t$0,-2($8)

.loc\t1 49
LM12:
sll\t$3,$4,2
addu\t$3,$3,$4
sll\t$3,$3,2
addu\t$3,$3,4
addu\t$3,$3,$6

.loc\t1 50
LM13:
sll\t$2,$5,1
addu\t$2,$2,$5
sll\t$2,$2,2
addu\t$2,$2,4
addu\t$2,$2,$7

.loc\t1 49
LM14:
sw\t$3,D_801E42DC

.loc\t1 50
LM15:
sw\t$2,D_801E42E8

.loc\t1 52
LM16:
la\t$6,D_801E4DB8
lw\t$2,0($3)
lw\t$4,4($3)
lw\t$5,8($3)
sw\t$2,0($6)
sw\t$4,4($6)
sw\t$5,8($6)
lw\t$2,12($3)
nop
sw\t$2,12($6)""",
        "entry records and first copy",
    )
    out = replace_once(
        out,
        """la\t$7,D_801E4DC8
lwl\t$2,7($4)
#nop # DEBUG: 'lwr\t$2,4($4)' does not load from $2
lwr\t$2,4($4)
#nop # DEBUG: 'lwl\t$5,11($4)' does not load from $2
lwl\t$5,11($4)
#nop # DEBUG: 'lwr\t$5,8($4)' does not load from $5
lwr\t$5,8($4)
#nop # DEBUG: 'swl\t$2,3($7)' does not load from $5
swl\t$2,3($7)
swr\t$2,0($7)
swl\t$5,7($7)
swr\t$5,4($7)""",
        """lw\t$2,D_801E42E8
lw\t$3,D_801E42DC
la\t$7,D_801E4DC8
lwl\t$4,3($2)
lwr\t$4,0($2)
lwl\t$5,7($2)
lwr\t$5,4($2)
swl\t$4,3($7)
swr\t$4,0($7)
swl\t$5,7($7)
swr\t$5,4($7)""",
        "unaligned copy",
    )
    out = replace_once(
        out,
        """lw\t$3,D_801E42DC
#nop # DEBUG: 'lw\t$4,D_801E42E8' does not load from $3

.loc\t1 62
LM26:
lw\t$4,D_801E42E8
#nop # DEBUG: 'sh\t$0,D_801E4DE0' does not load from $4

.loc\t1 59
LM27:
sh\t$0,D_801E4DE0

.loc\t1 60
LM28:
sh\t$0,D_801E4DE2

.loc\t1 61
LM29:
lhu\t$2,16($3)
sh\t$2,D_801E4DE4

.loc\t1 62
LM30:
lhu\t$2,8($4)
sh\t$2,D_801E4DE6

.loc\t1 63
LM31:
lhu\t$2,18($3)
sh\t$2,D_801E4DE8

.loc\t1 64
LM32:
lhu\t$2,10($4)
sh\t$2,D_801E4DEA

.loc\t1 70
LM33:
lh\t$3,18($3)
nop # DEBUG: Reuse of '$3'. 'bgez\t$3,$L2' does not use $at

.loc\t1 72
LM34:
bgez\t$3,$L2
addu\t$2,$3,$zero""",
        """sh\t$0,D_801E4DE0
sh\t$0,D_801E4DE2
lhu\t$2,16($3)
lw\t$4,D_801E42E8
sh\t$2,D_801E4DE4
lhu\t$2,8($4)
sh\t$2,D_801E4DE6
lhu\t$2,18($3)
sh\t$2,D_801E4DE8
lhu\t$2,10($4)
sh\t$2,D_801E4DEA
lh\t$3,18($3)
addu\t$sp,$sp,-16

.loc\t1 72
LM34:
bgez\t$3,$L2
addu\t$2,$3,$zero""",
        "halfword setup and stack open",
    )
    out = replace_once(
        out,
        """j\t$L3
nop  # DEBUG: branch/jump""",
        """j\t$L3
sh\t$2,2($8)""",
        "first sign jump delay",
    )
    out = replace_once(
        out,
        """ori\t$2,$zero,1
sh\t$2,D_801E4DB4

.loc\t1 76
LM38:
j\t$L3
sh\t$2,2($8)""",
        """ori\t$2,$zero,1

.loc\t1 76
LM38:
j\t$L3
sh\t$2,2($8)""",
        "first sign duplicate flag store",
    )
    out = replace_once(
        out,
        """lw\t$4,D_801E42DC
#nop # DEBUG: 'sw\t$0,D_801E4DF0' does not load from $4

.loc\t1 104
LM57:
sw\t$0,D_801E4DF0

.loc\t1 105
LM58:
sh\t$0,D_801E4DEC

.loc\t1 106
LM59:
sh\t$0,D_801E4DEE

.loc\t1 108
LM60:
lw\t$3,20($4)
#nop # DEBUG: 'lw\t$2,0($4)' does not load from $3
lw\t$2,0($4)
#nop # DEBUG: 'lw\t$5,24($4)' does not load from $2

.loc\t1 110
LM61:
lw\t$5,24($4)
#nop # DEBUG: 'subu\t$6,$3,$2' does not load from $5

.loc\t1 108
LM62:
subu\t$6,$3,$2

.loc\t1 109
LM63:
srl\t$3,$6,31
addu\t$3,$6,$3

.loc\t1 110
LM64:
lw\t$2,4($4)
#nop # DEBUG: 'sra\t$3,$3,1' does not load from $2

.loc\t1 109
LM65:
sra\t$3,$3,1
sh\t$3,D_801E4DD8

.loc\t1 110
LM66:
subu\t$6,$5,$2

.loc\t1 111
LM67:
srl\t$2,$6,31
addu\t$2,$6,$2

.loc\t1 112
LM68:
lw\t$5,28($4)
#nop # DEBUG: 'lw\t$4,8($4)' does not load from $5
lw\t$4,8($4)
#nop # DEBUG: 'sra\t$2,$2,1' does not load from $4

.loc\t1 111
LM69:
sra\t$2,$2,1
sh\t$2,D_801E4DDA

.loc\t1 112
LM70:
subu\t$6,$5,$4

.loc\t1 113
LM71:
srl\t$2,$6,31
addu\t$2,$6,$2

.loc\t1 115
LM72:
lw\t$4,D_801E42E8
#nop # DEBUG: 'sra\t$2,$2,1' does not load from $4

.loc\t1 113
LM73:
sra\t$2,$2,1
sh\t$2,D_801E4DDC

.loc\t1 115
LM74:
lh\t$3,12($4)
#nop # DEBUG: 'lh\t$2,0($4)' does not load from $3
lh\t$2,0($4)
nop # DEBUG: Reuse of '$2'. 'subu\t$6,$3,$2' does not use $at
subu\t$6,$3,$2

.loc\t1 116
LM75:
srl\t$2,$6,31
addu\t$2,$6,$2
sra\t$2,$2,1
sh\t$2,D_801E4DD0

.loc\t1 117
LM76:
lh\t$3,14($4)
#nop # DEBUG: 'lh\t$2,2($4)' does not load from $3
lh\t$2,2($4)
nop # DEBUG: Reuse of '$2'. 'subu\t$6,$3,$2' does not use $at
subu\t$6,$3,$2

.loc\t1 118
LM77:
srl\t$2,$6,31
addu\t$2,$6,$2
sra\t$2,$2,1
sh\t$2,D_801E4DD2

.loc\t1 119
LM78:
lh\t$3,16($4)
#nop # DEBUG: 'lh\t$2,4($4)' does not load from $3
lh\t$2,4($4)
nop # DEBUG: Reuse of '$2'. 'subu\t$6,$3,$2' does not use $at
subu\t$6,$3,$2

.loc\t1 120
LM79:
srl\t$2,$6,31
addu\t$2,$6,$2
sra\t$2,$2,1
sh\t$2,D_801E4DD4""",
        """lw\t$4,D_801E42DC
sw\t$0,D_801E4DF0
sh\t$0,D_801E4DEC
sh\t$0,D_801E4DEE
lw\t$2,20($4)
lw\t$3,0($4)
nop
subu\t$2,$2,$3
srl\t$3,$2,31
addu\t$2,$2,$3
sra\t$2,$2,1
sh\t$2,D_801E4DD8
lw\t$2,24($4)
lw\t$3,4($4)
nop
subu\t$2,$2,$3
srl\t$3,$2,31
addu\t$2,$2,$3
sra\t$2,$2,1
sh\t$2,D_801E4DDA
lw\t$2,28($4)
lw\t$3,8($4)
lw\t$4,D_801E42E8
subu\t$2,$2,$3
srl\t$3,$2,31
addu\t$2,$2,$3
sra\t$2,$2,1
sh\t$2,D_801E4DDC
lh\t$2,12($4)
lh\t$3,0($4)
nop
subu\t$2,$2,$3
srl\t$3,$2,31
addu\t$2,$2,$3
sra\t$2,$2,1
sh\t$2,D_801E4DD0
lh\t$2,14($4)
lh\t$3,2($4)
nop
subu\t$2,$2,$3
srl\t$3,$2,31
addu\t$2,$2,$3
sra\t$2,$2,1
sh\t$2,D_801E4DD2
lh\t$2,16($4)
lh\t$3,4($4)
nop
subu\t$2,$2,$3
srl\t$3,$2,31
addu\t$2,$2,$3
sra\t$2,$2,1
sh\t$2,D_801E4DD4""",
        "midpoint tail",
    )
    out = replace_once(
        out,
        """j\t$31
nop  # DEBUG: branch/jump""",
        """addu\t$sp,$sp,16
j\t$31
nop  # DEBUG: branch/jump""",
        "stack close",
    )
    sys.stdout.write(out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
