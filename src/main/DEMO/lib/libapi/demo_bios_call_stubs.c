#include "common.h"

/* Additional PsyQ kernel call trampolines this pre-release build needs that
 * retail's src/main/PAL/lib/libapi/bios_call_stubs.c does not include (this
 * early build calls a slightly different set of BIOS functions). Same raw
 * jump-through-a-register-with-the-call-number-in-a-delay-slot protocol C
 * cannot express; see that file's own comment for the full rationale. The
 * last entry's bytes are ExitCriticalSection (already pinned by address in
 * linkers/DEMO/undefined_syms_manual.txt; this wrapper does not redeclare
 * that name to avoid a duplicate-definition conflict). */
HANDWRITTEN_ASM("src/main/DEMO/lib/libapi", demo_bios_call_stubs);
