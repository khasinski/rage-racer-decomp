#include "common.h"

extern s16 D_8009AFA4;
extern s16 D_8009AFA8;
extern s16 D_8009AFAC;
extern s16 D_8009E83C;
extern s32 D_8009AF78;
extern s32 D_8009AF7C;
extern s32 D_8009AF80[];
extern s32 D_8009AF8C;
extern s32 D_8009AF90[];
extern s32 D_8009AFB0;
extern s32 D_801E408C;
extern s32 D_801E40D8;
extern s32 D_801E4148;
extern s32 D_801E428C;
extern s32 D_801E4364;
extern s32 D_801E4BCC;
extern s32 D_801E4D64;
extern s32 D_801E4D98[];
extern s32 D_801E41E8[][4][3];
extern s32 D_8019C70C[][4][2];

void func_80033308(s32 arg0, s32 arg1);
void func_80033D50(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_8005D6EC(s32 arg0);

void func_800352B8(void *arg0, s32 arg1, s32 arg2) {
    s32 slot;
    s32 nextSlot;
    s32 delta;
    s32 value;
    s32 tile;
    s32 timeout;
    s32 threshold;
    u8 *route;

    route = (u8 *)arg0 + 0xBC;

    if (arg2 == 2 || arg1 != 0) {
        return;
    }

    slot = D_801E4148;
    if (slot >= 0) {
        if ((*(s16 *)((u8 *)arg0 + 0x168) - 1) * D_801E40D8 + D_801E4D98[slot] <=
                (*(s32 *)((u8 *)arg0 + 0x6C) + *(s32 *)((u8 *)arg0 + 0x68)) ||
            arg2 != 0) {
            D_8009AF80[slot] = D_801E4D64;
            if (D_801E4D64 <= 0x927BE) {
                if (arg2 != 0) {
                    delta = D_8009AF8C - D_801E4D64;
                } else {
                    delta = D_8009AF90[slot] - D_801E4D64;
                }

                D_8009AFAC = 1;
                if (delta < 0) {
                    D_8009AFAC = -1;
                    delta = -delta;
                    if (arg2 == 0) {
                        func_8005D6EC(0x3F);
                    }
                } else if (delta > 0 && arg2 == 0) {
                    func_8005D6EC(0x3E);
                }
                D_8009AF7C = delta;
            } else {
                D_8009AFAC = 0;
            }

            D_8009AFA8 = 0;
            nextSlot = D_801E4148;
            nextSlot++;
            nextSlot %= 3;
            D_801E4148 = nextSlot;

            if (arg2 != 0) {
                D_8009AFA4 = 2;
                D_8009AFB0 = D_8009AF8C;
                D_8009AF8C = D_801E4BCC;
            } else {
                nextSlot += 2;
                nextSlot %= 3;
                D_8009AFA4 = nextSlot;
                D_8009AFB0 = D_8009AF90[nextSlot];
            }

            nextSlot = D_801E4148;
            nextSlot += 2;
            nextSlot %= 3;
            nextSlot <<= 2;
            D_8009AF78 = *(s32 *)((u8 *)D_8009AF80 + nextSlot);
            goto draw;
        }
    }

    if (D_801E4148 == -2 && arg2 != 0) {
        D_801E4148 = 0;
        D_8009AFAC = 0;
        D_8009AFB0 = D_801E41E8[D_801E408C][D_801E428C][0];
        D_8009AFA8 = 0x3C;
        D_8009AFA4 = (u16)D_801E4148;
        goto draw;
    }

    nextSlot = D_801E4148;
    if (nextSlot >= 0 && D_801E4364 >= *(s16 *)(route + 0xAC)) {
        if (D_8009AFA8 < 0x3C) {
            D_8009AFA8++;
            if (D_8009AFA8 == 0x3C) {
                D_8009AFB0 = D_8009AF90[nextSlot];
                D_8009AFAC = 0;
                D_8009AFA4 = (u16)D_801E4148;
            }
        }
    } else {
        D_8009AFA4 = 0;
        D_8009AFA8 = 0;
        D_8009AFAC = 0;
        D_8009AFB0 = D_8009AF90[0];
    }

draw:
    if (D_8009AFA8 >= 0x3C) {
        threshold = 0x927BE;
        value = D_801E4D64;
        goto compare_first;
    } else if (D_801E4148 >= 0) {
        if (D_8009AFAC != 0) {
            if (D_801E4364 >= D_8009E83C) {
                value = D_8009AF7C;
                if (D_8009AFAC > 0) {
                    tile = 0x7810;
                } else {
                    tile = 0x780F;
                }
                func_80033D50(0x80, 0x50, value, tile, 0x3E8);
            }
        }
        threshold = 0x927BE;
        value = D_8009AF78;
    } else {
        goto skip_first;
    }

compare_first:
    if (value <= threshold) {
        tile = 0x78CC;
    } else {
        tile = 0x7890;
    }
    func_80033D50(0x12, 0x2A, value, tile, 0x3E8);

skip_first:
    timeout = 0x3E8;
    func_80033D50(0x12, 0x20, D_8009AFB0, 0x78CC, timeout);
    func_80033308(D_8009AFA4, D_8009AFAC);

    func_80033D50(
        0xFA,
        0x7C,
        D_8019C70C[D_801E408C][D_801E428C][arg1],
        0x78CC,
        timeout);
}
