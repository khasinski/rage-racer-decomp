#include "common.h"
#include "psyq/cd.h"

extern s32 D_8009AF6C;
extern s32 D_8009AF70;
extern s32 D_8009AF74;
extern s32 D_8019CA1C;
extern u16 D_8019CE96;
extern volatile u16 D_8019CE98;
extern volatile u16 D_8019CE9A;
extern u16 D_801C067E;
extern volatile u16 D_801C0680;
extern volatile u16 D_801C0682;

s32 func_8006D0EC(StRingEventRecord **arg0, StRingEventRecord **arg1);
void func_80065A90(void *arg0, u32 arg1, u32 arg2, u32 arg3);

void *func_8001EDC4(s32 *arg0) {
    StRingEventRecord *slot[2];
    u16 rect[4];
    s32 count;
    u16 *dst;
    StRingEventRecord *entry;
    s32 w;
    s32 h;
    s32 h32;
    register s32 w32 asm("$4");
    register u16 hgt16 asm("$4");
    register u16 wid16 asm("$3");
    u16 wdraw;
    s32 half;
    s32 c067e;
    register void *ret asm("$2");

    for (count = 0x800000; count != 0; count--) {
        if (func_8006D0EC(&slot[0], &slot[1]) == 0) {
            goto process;
        }
    }
    return 0;

process:
    entry = slot[1];
    if (*(u32 *)((char *)entry + 8) >= (u32)D_8019CA1C) {
        D_8009AF74 = 1;
    }
    w = *(u16 *)((char *)entry + 0x10);
    if ((D_8009AF6C != w) || (D_8009AF70 != *(u16 *)((char *)entry + 0x12))) {
        h = *(u16 *)((char *)entry + 0x12);
        rect[0] = 0;
        rect[1] = 0;
        rect[2] = w * 3 / 2;
        rect[3] = 0x1E0;
        D_8009AF6C = w;
        D_8009AF70 = h;
        func_80065A90(rect, 0, 0, 0);
    }

    dst = (u16 *)arg0;
    ret = slot[0];
    h32 = D_8009AF70;
    w32 = D_8009AF6C;
    half = (0xF0 - h32) / 2;
    dst[0xD] = D_8019CE96 + half;
    __asm__ __volatile__("" ::: "memory");
    {
        register u32 wsgn asm("$4");
        u32 wm = (u32)w32 * 3;
        wsgn = wm >> 31;
        wdraw = (u16)((wm + wsgn) >> 1);
    }
    dst[0x12] = wdraw;
    dst[0xE] = wdraw;
    wid16 = D_8009AF6C;
    c067e = D_801C067E;
    hgt16 = D_8009AF70;
    dst[0x13] = hgt16;
    dst[0xF] = hgt16;
    dst[0x19] = hgt16;
    c067e += half;
    dst[0x11] = c067e;
    D_801C0680 = wid16;
    D_8019CE98 = wid16;
    D_801C0682 = hgt16;
    D_8019CE9A = hgt16;
    return ret;
}
