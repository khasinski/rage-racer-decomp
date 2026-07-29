# GTE and Handwritten Assembly Policy

This document defines how to handle GTE/COP2 code and suspected handwritten
assembly during matching decompilation.

The goal is still C decompilation. Assembly is allowed only when it represents
the original source-level interface to hardware or when there is concrete
evidence that the code was not compiler-generated C.

## Default Rule

A function should be decompiled as C. Do not use inline assembly to replace
ordinary MIPS instructions, control flow, pointer arithmetic, struct access,
bit packing, or register scheduling.

Acceptable matching work should follow this order:

1. Fix prototypes, types, signedness, storage width, and struct windows.
2. Reshape C control flow and expression order.
3. Adjust variable lifetime and local scopes.
4. Use register pinning only for the last small scheduling/register issues.
5. Use small local barriers only when the C is otherwise understood.
6. Use non-empty inline assembly only for documented hardware/compiler cases.

Large inline assembly blocks are not decompilation. If a function cannot be
matched as C yet, keep the generated asm stub or document it as a handwritten
exception.

## GTE Code

GTE/COP2 operations should be represented with PSYQ-style macros, not ad hoc
inline assembly blocks inside each function.

The original source for typical PlayStation code usually looked like C plus
GTE macros from PSYQ headers such as `inline_c.h`, `inline_n.h`, or GTE macro
headers. The surrounding logic was C, while the macro emitted COP2 transfers
and commands.

Good shape:

```c
gte_ldv0(vertex);
gte_rtps();
gte_stsxy(screen);
gte_stsz(&z);
```

Bad shape:

```c
asm volatile(
    "lwc2 $0, 0(%0)\n"
    "lwc2 $1, 4(%0)\n"
    "cop2 0x180001\n"
    "swc2 $14, 0(%1)\n"
    /* plus ordinary MIPS logic */
);
```

Centralize GTE operations in a PSYQ-compatible header such as
`include/psyq/gte.h` or `include/psyq/gte_macros.h`. Function bodies should use
named macros for:

- GTE data/control register transfers: `lwc2`, `swc2`, `mfc2`, `mtc2`,
  `cfc2`, `ctc2`.
- GTE commands: `rtps`, `rtpt`, `nclip`, `avsz3`, `avsz4`, `mvmva`, `ncs`,
  `nct`, `ncds`, `ncdt`, `gpf`, `gpl`, and similar COP2 ops.
- Required hazard nops when the original PSYQ macro would have emitted them.

Do not hide normal CPU work in GTE macros. Pointer math, table indexing,
primitive setup, OT insertion, branch logic, and field packing should stay in C
unless they are proven handwritten assembly.

## Macro Requirements

GTE macros should be small and named after the original PSYQ convention where
possible.

Each macro should have a clear hardware meaning. Examples:

```c
#define gte_rtps()  __asm__ volatile(".word 0x4A180001")
#define gte_nclip() __asm__ volatile(".word 0x4B400006")
#define gte_stotz(v) __asm__ volatile("mfc2 %0, $7" : "=r"(v))
```

Avoid macros like:

```c
#define MATCH_RENDER_LOOP(...) asm volatile("...")
```

Those macros only disguise an asm rewrite. They do not preserve the original C
structure.

## Handwritten Assembly Evidence

Do not mark code as handwritten only because GCC did not immediately emit it.
That is not enough.

Strong evidence includes at least one of:

- Non-standard entry/exit behavior that violates the C ABI.
- Use of registers unavailable or invalid for compiler-generated C in that
  context, such as kernel-only exception state or deliberate `$k0/$k1` use.
- Exception, interrupt, syscall, BIOS trampoline, or return-from-exception
  behavior.
- Self-modifying code, copied instruction stubs, or runtime-generated branch
  targets.
- Instruction sequences that depend on exact machine encodings not exposed by
  C or PSYQ macros.
- Multiple entry points into the middle of a routine with real control-flow
  semantics, not just linker labels.
- Proven library/kernel routines distributed as assembly in PSYQ or BIOS
  sources.

Weak evidence is not enough:

- "GCC would not do this."
- "The register allocation is strange."
- "The delay slot is hard to match."
- "The function has many gotos."
- "The function contains GTE instructions."

Those are matching problems first.

## Assembly Kept Deliberately, For Relocation

There is a second reason to leave a block as assembly, distinct from "the
original was hand-written": **decompiling it would freeze the image**.

Two blocks are in this category, `D_800630B4` in `main/boot/_start.c` and
`func_80069FA8` in `main/sdk/Gte_PatchExceptionHandler.c`. Both used to be
`u32[]` arrays placed in `.text`. An array of literal words carries no
relocation records, so the addresses inside it did not move when code size
changed, and a padded build jumped to the old entry point and hung at
`0x8006317x`. Rewritten as real assembly they gained relocations, and a build
with `. += 0x40` inserted mid-`.text` was proved to run: every sampled pc was the
baseline's plus exactly `0x40`, with an identical BIOS banner.

Turning either back into C, or back into a word array, would undo that. They are
marked `HANDWRITTEN_ASM` and excluded from the totals for the same reason the
GTE family is: the metric should measure work left to do, and there is none here.

The evidence bar is the same as above. "This function manipulates addresses"
alone is not enough — the test is whether a source spelling exists that keeps the
relocations, and for these two it does not, because the entry stub runs before
`$gp`, `$sp` and `$fp` exist and the handler copies its own code into the BIOS
exception table.

## How To Document Exceptions

If a function must stay as assembly or use non-empty inline assembly, add a
short note near the stub or in a matching notes document with:

```text
Symbol:
Address:
Reason:
Evidence:
Why C+PSYQ macros are insufficient:
Current representation:
Revisit condition:
```

The evidence must be concrete. Include exact instructions, ABI behavior,
symbol layout, external library comparison, or original SDK evidence.

## Preferred Representations

Use these representations in order:

1. Plain C.
2. Plain C plus PSYQ-style GTE macros.
3. Plain C plus empty barriers or register pinning for local matching issues.
4. Small, documented inline assembly for a single hardware/compiler operation.
5. Generated `INCLUDE_ASM` stub with a note explaining why it is not C yet.
6. Dedicated handwritten `.s` only when there is strong evidence it was
   assembly in the original program.

Do not use top-level `asm(".globl ...")` to manufacture symbol layout in C
files. Prefer config/linker symbols, proper declarations with `asm("symbol")`,
or a dedicated assembly file when the symbol really is assembly.

## Review Checklist

Before accepting a GTE or asm-heavy change:

- Does the function body still express the algorithm in C?
- Are all GTE operations named through central macros?
- Are ordinary MIPS instructions absent from inline asm?
- Are hazard nops part of a known GTE macro or explicitly justified?
- Does the function match after `make clean build check`?
- Does progress count only functions without non-empty inline assembly?
- If assembly remains, is there written evidence stronger than register
  scheduling difficulty?

If any answer is no, treat the function as unfinished decompilation work.
