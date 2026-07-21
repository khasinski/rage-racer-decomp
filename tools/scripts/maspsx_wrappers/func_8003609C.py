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
        sys.stderr.write(f"maspsx_func_8003609C: expected {label} once, found {count}\n")
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
    out = out.replace(".frame\t$sp,24,$31", ".frame\t$sp,32,$31")
    out = replace_once(out, "subu\t$sp,$sp,24", "subu\t$sp,$sp,32", "stack alloc")
    out = replace_once(out, "sw\t$31,20($sp)", "sw\t$31,28($sp)", "ra save")
    out = replace_once(out, "sw\t$16,16($sp)", "sw\t$16,24($sp)", "s0 save")
    out = replace_once(out, "lw\t$31,20($sp)", "lw\t$31,28($sp)", "ra restore")
    out = replace_once(
        out,
        """lw\t$31,28($sp)
#nop # DEBUG: 'lw\t$16,16($sp)' does not load from $31
lw\t$16,16($sp)
#nop # DEBUG: 'addu\t$sp,$sp,24' does not load from $16
addu\t$sp,$sp,24
""",
        """lw\t$31,28($sp)
#nop # DEBUG: 'lw\t$16,24($sp)' does not load from $31
lw\t$16,24($sp)
#nop # DEBUG: 'addu\t$sp,$sp,32' does not load from $16
addu\t$sp,$sp,32
""",
        "epilogue frame",
    )

    out = replace_once(
        out,
        """.loc\t1 112
LM35:
# EXPAND_AT START
.set\tnoat
lui\t$at,%hi(D_801E41EC)
addu\t$at,$at,$3
lw\t$7,%lo(D_801E41EC)($at)
.set\tat
# EXPAND_AT END
#nop # DEBUG: 'li\t$2,-2\t\t\t# 0xfffffffe' does not load from $7

.loc\t1 113
LM36:
addiu\t$2,$zero,-2
sw\t$2,D_801E4148

.loc\t1 116
LM37:
lh\t$2,D_801E4DAC
#nop # DEBUG: 'sll\t$5,$5,3' does not load from $2
sll\t$5,$5,3

.loc\t1 112
LM38:
sw\t$7,D_8009AF94

.loc\t1 114
LM39:
la\t$7,D_801E4408
addu\t$4,$4,$7

.loc\t1 116
LM40:
sll\t$2,$2,2
addu\t$5,$5,$4
addu\t$2,$2,$5

.loc\t1 115
LM41:
# EXPAND_AT START
.set\tnoat
lui\t$at,%hi(D_801E41F0)
addu\t$at,$at,$3
lw\t$3,%lo(D_801E41F0)($at)
.set\tat
# EXPAND_AT END
#nop # DEBUG: 'lw\t$5,D_801E4364' does not load from $3

.loc\t1 121
LM42:
lw\t$5,D_801E4364
#nop # DEBUG: 'sw\t$3,D_8009AF98' does not load from $5

.loc\t1 115
LM43:
sw\t$3,D_8009AF98

.loc\t1 116
LM44:
lw\t$3,0($2)
""",
        """.loc\t1 112
LM35:
# EXPAND_AT START
.set\tnoat
lui\t$at,%hi(D_801E41EC)
addu\t$at,$at,$3
lw\t$2,%lo(D_801E41EC)($at)
.set\tat
# EXPAND_AT END

.loc\t1 116
LM37B:
sll\t$5,$5,3

.loc\t1 112
LM38:
sw\t$2,D_8009AF94

.loc\t1 113
LM36:
addiu\t$2,$zero,-2
sw\t$2,D_801E4148

.loc\t1 114
LM39:
la\t$2,D_801E4408
addu\t$4,$4,$2

.loc\t1 116
LM40B:
addu\t$5,$5,$4

.loc\t1 116
LM37:
lh\t$2,D_801E4DAC

.loc\t1 115
LM41:
# EXPAND_AT START
.set\tnoat
lui\t$at,%hi(D_801E41F0)
addu\t$at,$at,$3
lw\t$3,%lo(D_801E41F0)($at)
.set\tat
# EXPAND_AT END

.loc\t1 116
LM40:
sll\t$2,$2,2
addu\t$2,$2,$5

.loc\t1 115
LM43:
sw\t$3,D_8009AF98

.loc\t1 116
LM44:
lw\t$3,0($2)

.loc\t1 121
LM42:
lw\t$5,D_801E4364
""",
        "table selection schedule",
    )

    out = replace_once(
        out,
        """.loc\t1 161
LM82:
ori\t$2,$zero,12
sw\t$2,D_801E42E4

.loc\t1 162
LM83:
ori\t$2,$zero,384

.loc\t1 158
LM84:
sh\t$0,D_801E6C90

.loc\t1 159
LM85:
sh\t$0,D_8019C9AC

.loc\t1 160
LM86:
sh\t$0,D_801E4CF8

.loc\t1 162
LM87:
sw\t$2,D_8019C768
""",
        """.loc\t1 158
LM84:
sh\t$0,D_801E6C90

.loc\t1 159
LM85:
sh\t$0,D_8019C9AC

.loc\t1 160
LM86:
sh\t$0,D_801E4CF8

.loc\t1 161
LM82:
ori\t$2,$zero,12
sw\t$2,D_801E42E4

.loc\t1 162
LM83:
ori\t$2,$zero,384

.loc\t1 162
LM87:
sw\t$2,D_8019C768
""",
        "final state clear schedule",
    )

    sys.stdout.write(out)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
