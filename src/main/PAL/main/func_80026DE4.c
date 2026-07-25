/*
 * Target-sized near-match attempt for asm/PAL/main/main/175E4.s.
 * This keeps the matched loop shape from the scale-inline attempt and only
 * pins the final D_8019C900 load into $v0.
 */
#include "common.h"

extern s16 D_8007D6DC[];
extern s16 D_8007D6DE[];
extern s32 D_8007D6E0[];
extern u32 D_801E40B8;
extern s32 D_801E42E0;
extern u8 *D_8019C900;

void func_800168AC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_8001720C(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(u8 *arg0, s32 arg1, s32 arg2);

void func_80026DE4(void) {
    s32 i;
    s32 offset;
    s32 adjusted;
    s32 delta;
    s32 value;
    s32 clamped;
    s32 next;
    s32 arg1;
    s32 arg7;
    s32 scale_b;
    s32 arg8;

    for (i = 0, offset = 0; i < 14; offset += 8) {
        s32 tableY;

        adjusted = (D_801E40B8 / 3) - 0xD0;
        tableY = *(s16 *)((u8 *)D_8007D6DE + offset);
        delta = tableY - adjusted;

        if (delta < 0x60) {
            value = (0x60 - delta) << 1;
        } else if (delta >= 0x91) {
            value = (delta - 0x90) << 1;
        } else {
            value = 0;
        }

        if (value >= 0) {
            clamped = value;
            if (clamped >= 0x80) {
                clamped = 0x7F;
            }
        } else {
            clamped = 0;
        }

        value = 0x7F - clamped;
        i++;
        if (value != 0) {
            func_800168AC(
                *(s16 *)((u8 *)D_8007D6DC + offset),
                delta,
                *(s32 *)((u8 *)D_8007D6E0 + offset),
                0x78CC,
                value);
        }
    }

    {
        register s32 camera asm("t1");
        register s32 *scratch asm("s1");
        register u8 *ptr asm("s0");
        register s32 scale_a asm("v1");
        register s32 tmp asm("v0");
        register u8 *base asm("v0");

        camera = D_801E42E0;
        scratch = (s32 *)0x1F800000;
        tmp = (camera << 3) - camera;
        scale_a = tmp << 5;
        base = D_8019C900;
        arg1 = *scratch;
        ptr = base + 0xD0;
        arg7 = (scale_a / 0x100) + 0x20;
        scale_b = ((camera << 1) + camera) << 6;
        arg8 = (scale_b / 0x100) + 0x40;

        next = func_8001720C(ptr, arg1, 0, 0, 0x140, 0xF0, camera, arg7, arg8);
        *scratch = func_80017390(ptr, next, 0x49);
    }
}
