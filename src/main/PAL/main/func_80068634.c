#include "common.h"

extern s16 D_80092B08[];
extern s16 D_80093B08[];
extern s16 D_80094308[];

s32 rcos(s32 arg0) asm("func_80068634");

s32 rcos(s32 arg0) {
    if (arg0 < 0) {
        arg0 = -arg0;
    }

    arg0 &= 0xFFF;

    if (arg0 < 0x801) {
        if (arg0 < 0x401) {
            return D_80094308[0x400 - arg0];
        }

        return -D_80093B08[arg0];
    }

    if (arg0 < 0xC01) {
        return -D_80094308[0xC00 - arg0];
    }

    return D_80092B08[arg0];
}
