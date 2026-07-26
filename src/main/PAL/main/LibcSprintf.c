#include "common.h"

/*
 * PSY-Q libc sprintf: the whole formatter, with no vsprintf split. Identified by
 * its %X / %x digit tables at D_800131E4 / D_800131F8 and by its LibcMemchr /
 * LibcMemmove / LibcStrlen callees.
 */
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/LibcSprintf", func_800632F0);

