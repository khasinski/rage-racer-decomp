#include "common.h"
#include "game/render.h"

extern volatile s32 D_8007D790;
extern volatile s32 D_8007D794;
extern volatile s32 D_8007D79C;
extern volatile s32 D_8007D7A0;
extern volatile s32 D_8007D7AC;
extern volatile s32 D_8007D7B4;
extern volatile s32 D_8007D7B8;
s32 func_8006A574(s32 arg0);
s32 func_8006A58C(s32 arg0);
s32 func_8006DD30(s32 arg0);
s32 func_8006A3E8(void);
s32 func_8006A808(s32 arg0, void *arg1, s32 arg2);
s32 func_8002745C(s32 arg0);
void func_8006A554(s32 arg0, s32 arg1);
extern s32 D_8007D78C;
extern u8 D_8007D7BC[];
extern u8 D_8007D7BD[];
extern u8 D_8007D87C[];
extern u8 D_8007BED0[];
void func_80064FA8(u8 *prim);
void func_80064EB8(u8 *prim, s32 enabled);
void func_80064DDC(void *ot, void *prim);
void *func_800666F4(void *prim, s32 a, s32 b, s32 c, void *d);
s32 func_80032F34(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

s32 func_80027688(s32 arg0, s32 arg1, s32 arg2) {
    s32 savedArg0;
    s32 mode;
    s32 value;

    value = (s32)&D_8007D79C;
    *(volatile s32 *)value = arg2;
    value = *(volatile s32 *)value;
    mode = value & 0x30;

    switch (mode) {
    case 0:
        value = 0x200;
        D_8007D7A0 = value;
        break;
    case 0x20:
        value = 0x249;
        D_8007D7A0 = value;
        break;
    default:
        value = 0x246;
        mode = (s32)&D_8007D7A0;
        *(volatile s32 *)mode = value;
        break;
    }

    mode = (s32)&D_8007D79C;
    savedArg0 = arg0;
    value = *(volatile s32 *)mode;
    value |= 0x20;
    *(volatile s32 *)mode = value;
    D_8007D794 = arg1;
    D_8007D790 = savedArg0;
    D_8007D7B4 = func_8006A574(0);
    D_8007D7B8 = func_8006A58C(0);
    D_8007D7AC = func_8006DD30(-1);

    if ((func_8006A3E8() & 0xE0) != 0) {
        func_8006A808(9, 0, 0);
    }

    return func_8002745C(0) > 0;
}

s32 func_80027790(s32 arg0, s32 arg1) {
    s32 savedArg0;
    s32 savedArg1;
    volatile s32 *state;
    s32 result;

    savedArg0 = arg0;
    savedArg1 = arg1;
    state = &D_8007D7AC;

    do {
        s32 now;

        now = func_8006DD30(-1);
        if (state[0] + 0x4B0 < now) {
            result = -1;
            goto loop_check;
        }

        if (state[-2] < 0) {
            goto reset_read;
        }

        now = func_8006DD30(-1);
        if (state[-1] + 0x3C < now) {
reset_read:
            func_8002745C(1);
            result = state[-7];
            goto loop_check;
        }

        result = state[-2];

loop_check:
        if (savedArg0 != 0) {
            break;
        }
    } while (result > 0);

    func_8006A554(1, savedArg1);
    return result;
}

s32 func_8002785C(s32 arg0) {
    s32 old = D_8007D78C;

    D_8007D78C = arg0;
    return old;
}

void func_80027874(s32 x, s32 y, u8 *str, s32 arg3) {
    register u8 *packet __asm("$18");
    s32 idx;
    register u8 *next __asm("$20");
    register u8 *sr __asm("$21");
    register u8 *tableA __asm("$23");
    register s32 ga __asm("$16");
    register s32 gb __asm("$17");
    register s32 w __asm("$3");
    register u8 *oldPacket __asm("$5");
    register u8 *otv __asm("$4");
    s16 a3;
    u8 *tableB;

    sr = str;
    next = *(u8 **) 0x1F800000;
    if (*sr != 0) {
        tableA = D_8007D7BC;
        tableB = D_8007D7BD;
        packet = next;
        do {
            idx = *sr++ - 0x20;
            if (idx != 0) {
                ga = tableA[idx * 2];
                gb = tableB[idx * 2];
                func_80064FA8(next);
                func_80064EB8(next, 1);
                next += 0x14;
                oldPacket = packet;
                __asm__ __volatile__("" : : "r"(oldPacket));
                *(volatile s16 *)(packet + 0x8) = x;
                *(volatile s16 *)(packet + 0xA) = y;
                *(volatile u8 *)(packet + 0xC) = ga;
                *(volatile u8 *)(packet + 0xD) = gb;
                w = D_8007D87C[idx];
                *(volatile s16 *)(packet + 0x12) = 0x18;
                a3 = arg3;
                __asm__ __volatile__(
                    "lui %0, %%hi(D_8019C900)\n\t"
                    "lw %0, %%lo(D_8019C900)(%0)\n\t"
                    "sh %2, 0xE(%1)"
                    : "=&r"(otv) : "r"(packet), "r"(a3) : "memory");
                *(volatile s16 *)(packet + 0x10) = w;
                packet += 0x14;
                __asm__ __volatile__("" : "=r"(otv) : "0"(otv));
                func_80064DDC(otv + 0xCC, oldPacket);
            }
            x += D_8007D87C[idx];
        } while (*sr != 0);
    }
    func_800666F4(next, 0, 1, 0x1D, D_8007BED0);
    func_80064DDC(g_DrawBuffer + 0xCC, next);
    *(u8 **) 0x1F800000 = next + 0xC;
    __asm__ __volatile__("" : : "r"(next));
}

void func_800279EC(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp;

    temp = func_80032F34(arg0, arg1, arg2 + 1, arg3 + 2, 0xC2, 0x1C, 0, 0, 0);
    func_80032F34(arg0, temp, arg2, arg3, 0xC4, 0x20, 0xFF, 0xFF, 0xFF);
}
