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
        register u32 timer asm("$2");
        register u32 magic asm("$3");
        register u32 hi asm("$2");
        register s32 tableY asm("$3");

        __asm__ volatile(
            "lui $3,0xAAAA\n"
            "lui $2,%%hi(D_801E40B8)\n"
            "lw $2,%%lo(D_801E40B8)($2)\n"
            "ori $3,$3,0xAAAB\n"
            "multu $2,$3"
            : "=r"(timer), "=r"(magic));
        tableY = *(s16 *)((u8 *)D_8007D6DE + offset);
        __asm__ volatile("mfhi %0" : "=r"(hi));
        adjusted = (hi >> 1) - 0xD0;
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
        register u8 *base asm("v0");

        camera = D_801E42E0;
        __asm__ volatile("lui $17,0x1F80" : "=r"(scratch));
        __asm__ volatile(
            "sll $2,%1,3\n"
            "subu $2,$2,%1\n"
            "sll %0,$2,5"
            : "=r"(scale_a)
            : "r"(camera)
            : "$2");
        __asm__ volatile(
            "lui $2,%%hi(D_8019C900)\n"
            "lw $2,%%lo(D_8019C900)($2)"
            : "=r"(base));
        ptr = base + 0xD0;
        arg1 = *scratch;
        arg7 = (scale_a / 0x100) + 0x20;
        scale_b = ((camera << 1) + camera) << 6;
        arg8 = (scale_b / 0x100) + 0x40;

        next = func_8001720C(ptr, arg1, 0, 0, 0x140, 0xF0, camera, arg7, arg8);
        *scratch = func_80017390(ptr, next, 0x49);
    }
}
