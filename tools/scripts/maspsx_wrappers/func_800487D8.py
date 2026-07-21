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
        sys.stderr.write(f"maspsx_38FD8_draw_command_dispatch_schedule: expected {label} once, found {count}\n")
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
subu\t$sp,$sp,40
sw\t$17,20($sp)

.loc\t1 26
LM2:
addu\t$17,$0,$zero

.loc\t1 17
LM3:
sw\t$20,32($sp)

.loc\t1 18
LM4:
addu\t$20,$4,$zero

.loc\t1 17
LM5:
sw\t$18,24($sp)

.loc\t1 19
LM6:
addu\t$18,$5,$zero

.loc\t1 17
LM7:
sw\t$19,28($sp)

.loc\t1 20
LM8:
addu\t$19,$6,$zero

.loc\t1 17
LM9:
sw\t$31,36($sp)
""",
        """$Lb0:
subu\t$sp,$sp,40
sw\t$20,32($sp)
addu\t$20,$4,$zero
sw\t$18,24($sp)
addu\t$18,$5,$zero
sw\t$19,28($sp)
addu\t$19,$6,$zero
sw\t$17,20($sp)
addu\t$17,$0,$zero
sw\t$31,36($sp)
""",
        "prologue saved-register setup",
    )
    out = replace_once(out, "lw\t$2,0($5)", "lw\t$2,0($18)", "negative cursor load")
    out = replace_once(out, "sw\t$2,0($5)", "sw\t$2,0($18)", "negative cursor positive store")
    out = replace_once(out, "sw\t$0,0($5)", "sw\t$0,0($18)", "negative cursor zero store")
    out = replace_once(out, "addu\t$3,$20,$2", "addu\t$3,$2,$20", "initial command pointer add")
    out = replace_once(
        out,
        """$L37:
lw\t$4,0($18)
nop # DEBUG: Reuse of '$4'. 'subu\t$4,$4,$2' does not use $at
subu\t$4,$4,$2

.loc\t1 44
LM20:
bltz\t$4,$L9
nop  # DEBUG: branch/jump

.loc\t1 45
LM21:
$Lb2:
lh\t$3,2($16)
nop # DEBUG: Reuse of '$3'. 'move\t$7,$3' does not use $at
addu\t$7,$3,$zero

.loc\t1 47
LM22:
sltu\t$2,$3,40
beq\t$2,$0,$L9
sll\t$2,$3,2
""",
        """$L37:
lh\t$3,0($16)
lw\t$2,0($18)
nop # DEBUG: Reuse of '$2'. 'subu\t$4,$2,$3' does not use $at
subu\t$4,$2,$3

.loc\t1 44
LM20:
bltz\t$4,$L9
nop  # DEBUG: branch/jump

.loc\t1 45
LM21:
$Lb2:
lh\t$7,2($16)
nop # DEBUG: Reuse of '$7'. 'sltu\t$2,$7,40' does not use $at

.loc\t1 47
LM22:
sltu\t$2,$7,40
beq\t$2,$0,$L9
sll\t$2,$7,2
""",
        "loop time/kind register schedule",
    )
    out = replace_once(out, "addu\t$2,$20,$2", "addu\t$2,$2,$20", "tail command pointer add")
    sys.stdout.write(out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
