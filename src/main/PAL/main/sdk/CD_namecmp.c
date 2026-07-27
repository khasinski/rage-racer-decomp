#include "common.h"

s32 LibcStrncmp(s32 arg0, s32 arg1, s32 arg2) asm("func_8006CC8C");

s32 CD_namecmp(s32 arg0, s32 arg1) asm("func_8006C53C");
s32 CD_namecmp(s32 arg0, s32 arg1) {
    return LibcStrncmp(arg0, arg1, 0xC) == 0;
}
