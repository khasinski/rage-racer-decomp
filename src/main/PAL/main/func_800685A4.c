#include "common.h"

extern s16 D_80093308[];
extern s16 D_80094308[];

s32 func_800685A4(s32 arg0) {
    if (arg0 < 0x801) {
        if (arg0 < 0x401) {
            return D_80094308[arg0];
        }

        return D_80094308[0x800 - arg0];
    }

    if (arg0 < 0xC01) {
        return -D_80093308[arg0];
    }

    return -D_80094308[0x1000 - arg0];
}
