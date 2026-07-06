#include "common.h"

extern char D_80013504[];
extern void (*D_800941E4)(char *, ...);
extern u32 D_800941F4;
extern u8 D_800941EA;

u32 func_80065804(u32 arg0) {
    u32 ret;

    if (D_800941EA >= 2) {
        D_800941E4(D_80013504, arg0);
    }

    ret = D_800941F4;
    D_800941F4 = arg0;
    return ret;
}
