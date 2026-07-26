#ifndef RAGE_PC_COMMON_H
#define RAGE_PC_COMMON_H

#include "include_asm.h"
#include "asm_macros.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

/*
 * The one trace/printf entry point (0x8001674C). Every surviving PSY-Q debug
 * format string in the image is passed to it, from libgpu, libcd, libspu and
 * the game alike, which is what labels most of the arguments in this repo.
 */
/* Stubbed out: does nothing and returns 1. */
s32 GameDebugPrintf() asm("func_8001674C");

#endif
