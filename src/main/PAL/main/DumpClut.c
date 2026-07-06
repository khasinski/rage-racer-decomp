#include "common.h"

extern char D_8001338C[];
extern void (*D_800941E4)(char *, ...);

void func_80064D64(s32 arg0) {
    D_800941E4(D_8001338C, (arg0 & 0x3F) << 4, (arg0 & 0xFFFF) >> 6);
}
