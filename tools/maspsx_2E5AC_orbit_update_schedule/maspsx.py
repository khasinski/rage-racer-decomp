#!/usr/bin/env python3
from __future__ import annotations

import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
REAL_MASPSX = ROOT / "build" / "toolchain" / "maspsx" / "maspsx.py"


def replace_once(text: str, old: str, new: str, label: str) -> str:
    count = text.count(old)
    if count != 1:
        dump = ROOT / "build" / "proposals" / "2E5AC" / "maspsx_2E5AC_failed.s"
        dump.parent.mkdir(parents=True, exist_ok=True)
        dump.write_text(text)
        sys.stderr.write(f"maspsx_2E5AC_orbit_update_schedule: expected {label} once, found {count}\n")
        sys.stderr.write(f"maspsx_2E5AC_orbit_update_schedule: wrote {dump.relative_to(ROOT)}\n")
        sys.exit(1)
    return text.replace(old, new, 1)


def main() -> int:
    if "--help" in sys.argv[1:]:
        return subprocess.run([sys.executable, str(REAL_MASPSX), *sys.argv[1:]]).returncode

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
        """.loc\t1 32
LM8:
andi\t$22,$2,0x0003
sltu\t$22,$0,$22

.loc\t1 33
LM9:
beq\t$22,$0,$L2
sw\t$16,80($sp)


.loc\t1 34
LM10:
ori\t$17,$zero,1

.loc\t1 36
LM11:
j\t$L3
ori\t$19,$zero,4

$L2:

.loc\t1 37
LM12:
addu\t$17,$0,$zero

.loc\t1 38
LM13:
ori\t$19,$zero,1
$L3:

.loc\t1 41
LM14:
slt\t$2,$17,$19
beq\t$2,$0,$L4
nop  # DEBUG: branch/jump

.loc\t1 42
LM15:
sll\t$22,$22,1
la\t$2,D_8007E33C
addu\t$22,$22,$2

.loc\t1 43
LM16:
addu\t$18,$sp,48

.loc\t1 44
LM17:
la\t$21,D_8007E334

.loc\t1 45
LM18:
sll\t$16,$17,1
addu\t$16,$16,$21

.loc\t1 46
LM19:
sll\t$17,$17,4
$L5:
""",
        """.loc\t1 32
LM8:
andi\t$3,$2,0x0003
sltu\t$3,$0,$3

.loc\t1 33
LM9:
beq\t$3,$0,$L2
sw\t$16,80($sp)


.loc\t1 34
LM10:
ori\t$2,$zero,1

.loc\t1 36
LM11:
j\t$L3
ori\t$19,$zero,4

$L2:

.loc\t1 37
LM12:
addu\t$2,$0,$zero

.loc\t1 38
LM13:
ori\t$19,$zero,1
$L3:

.loc\t1 41
LM14:
addu\t$5,$2,$zero
slt\t$2,$5,$19
beq\t$2,$0,$L4
sll\t$3,$3,1

.loc\t1 42
LM15:
la\t$2,D_8007E33C
addu\t$22,$3,$2

.loc\t1 43
LM16:
addu\t$18,$sp,48

.loc\t1 44
LM17:
la\t$21,D_8007E334

.loc\t1 45
LM18:
sll\t$2,$5,1
addu\t$16,$2,$21

.loc\t1 46
LM19:
sll\t$17,$5,4
$L5:
""",
        "entry loop setup",
    )
    out = replace_once(
        out,
        """.loc\t1 69
LM34:
sll\t$2,$19,1
addu\t$2,$21,$2
sltu\t$2,$16,$2
bne\t$2,$0,$L5
addu\t$17,$17,16

$L4:

.loc\t1 72
LM35:
andi\t$23,$23,0x01ff
bne\t$23,$0,$L11
nop  # DEBUG: branch/jump
""",
        """.loc\t1 69
LM34:
sll\t$2,$19,1
addu\t$2,$2,$21
slt\t$2,$16,$2
bne\t$2,$0,$L5
addu\t$17,$17,16

$L4:

.loc\t1 72
LM35:
andi\t$2,$23,0x01ff
bne\t$2,$0,$L11
nop  # DEBUG: branch/jump
""",
        "loop tail and frame mask",
    )
    sys.stdout.write(out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
