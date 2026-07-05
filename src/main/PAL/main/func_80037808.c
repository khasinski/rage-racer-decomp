#include "common.h"

typedef struct UnkFunc80037808 {
    u8 pad0[4];
    s32 x;
    u8 pad8[4];
    s32 y;
} UnkFunc80037808;

extern s32 D_8009E6D4;
extern s32 D_8009E6DC;

s32 func_80037808(UnkFunc80037808 *arg0) {
    register s32 center_x asm("v1") = D_8009E6D4;
    s32 x = arg0->x;
    register s32 ret asm("a2") = 0;

    if ((center_x - 0x40) < x) {
        register s32 max_x asm("v0") = center_x + 0x40;

        if (x < max_x) {
            register s32 center_y asm("v1") = D_8009E6DC;
            s32 y = arg0->y;

            if ((center_y - 0x40) < y) {
                register s32 max_y asm("v0") = center_y + 0x40;

                ret = y < max_y;
            }
        }
    }

    return ret;
}
