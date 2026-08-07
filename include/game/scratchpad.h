#ifndef GAME_SCRATCHPAD_H
#define GAME_SCRATCHPAD_H

#include "common.h"

/* Shared render scratchpad slots. These macros intentionally expand to direct
 * l-value accesses: using extern symbols changes GCC 2.6.3 code generation. */
#define SCRATCH_PRIMITIVE_CURSOR (*(void **)0x1F800000)
#define SCRATCH_OT_BASE          (*(void **)0x1F800004)
#define SPAD_OT_SHIFT            (*(s32 *)0x1F800064)

#endif
