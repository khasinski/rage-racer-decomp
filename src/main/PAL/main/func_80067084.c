#include "common.h"

extern volatile u16 D_800941EC;
extern volatile u16 D_800941EE;
extern volatile u32 *D_800942B8;
extern volatile u32 *D_800942BC;
extern volatile u32 *D_800942C0;
extern volatile u32 *D_800942C4;
extern volatile u32 *D_800942C8;

void func_80067F04(void);
s32 func_80067F38(void);

typedef struct {
    u32 xy;
    s16 w;
    s16 h;
} Prim;

s32 func_80067084(Prim *rect, u32 *data) {
    s16 win;
    s16 wout;
    s16 hin;
    s16 hout;
    s32 raw;
    s32 count;
    s32 blocks;
    s32 rem;
    s32 tmp;
    s32 flag;

    flag = 0;
    asm("" : "+r"(flag));

    func_80067F04();

    win = rect->w;
    if (win >= 0) {
        volatile u16 *pw = &D_800941EC;
        if ((s16)*pw < win) {
            wout = *pw;
        } else {
            wout = win;
        }
    } else {
        wout = 0;
    }
    rect->w = wout;

    hin = rect->h;
    if (hin >= 0) {
        volatile u16 *ph = &D_800941EE;
        if ((s16)*ph < hin) {
            hout = *ph;
        } else {
            hout = hin;
        }
    } else {
        hout = 0;
    }
    rect->h = hout;

    raw = rect->w * hout + 1;
    count = raw / 2;
    rem = count >> 4;
    if (count <= 0) {
        return -1;
    }
    tmp = rem;
    rem = count - tmp * 16;
    blocks = tmp;

    while ((*D_800942BC & 0x04000000) == 0) {
        if (func_80067F38() != 0) {
            return -1;
        }
    }

    *D_800942BC = 0x04000000;
    *D_800942B8 = 0x01000000;
    *D_800942B8 = flag ? 0xB0000000 : 0xA0000000;
    *D_800942B8 = rect->xy;
    *D_800942B8 = *(u32 *)&rect->w;

    rem--;
    if (rem != -1) {
        do {
            *D_800942B8 = *data;
            data++;
            rem--;
        } while (rem != -1);
    }

    if (blocks != 0) {
        *D_800942BC = 0x04000002;
        *D_800942C0 = (u32)data;
        *D_800942C4 = (blocks << 16) | 0x10;
        *D_800942C8 = 0x01000201;
    }
    return 0;
}
