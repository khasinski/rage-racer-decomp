#include "common.h"
#include "game/render.h"
#include "psyq/cd.h"
#include "game/race.h"

typedef struct {
    u8 pad0[0x18];
    u16 field_18;
    u16 field_1A;
    u8 pad1[0xC];
    s32 field_28;
    u16 field_2C;
    u16 field_2E;
    u8 pad2[0x4];
    s32 field_34;
} Unk8001EF54;

void *func_8001EDC4(s32 *arg0);
void func_80064588(void *arg0, s32 arg1);
s32 func_8006CFF0(void *arg0);
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
void func_8001674C(const char *fmt);
extern char D_80010D34[];
s32 func_8006A534(s32 arg0, s32 arg1);
s32 func_8006A5A4(s32 arg0, void *arg1, s32 arg2);
s32 func_8006DD30(s32 arg0);
s32 func_8006CD0C(s32 arg0);
extern u8 D_801E8AFC;
extern u8 *D_8009F0A4;
extern u8 *D_8019C7A4;
extern u32 D_801E4BB0;
extern u32 D_8019CB6C;
extern u32 D_8009EC8C;
extern s16 D_8009E782;
extern s32 D_801E4D8C;
extern s32 D_801E4BC0;

s32 func_8001ED3C(s32 *arg0) {
    void *p;
    s32 retry;
    for (retry = 1; retry != 0; retry--) {
        p = func_8001EDC4(arg0);
        if (p != 0) {
            arg0[2] = (arg0[2] == 0);
            func_80064588(p, arg0[arg0[2]]);
            return func_8006CFF0(p);
        }
    }
    return -1;
}

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

void func_8001EF54(Unk8001EF54 *arg0) {
    volatile s32 timeout = 0x800000;
    s32 one;
    u16 x;

    if (arg0->field_34 == 0) {
        one = 1;
        do {
            timeout = timeout - 1;
            if (timeout == 0) {
                func_8001674C(D_80010D34);
                arg0->field_34 = one;
                arg0->field_28 = arg0->field_28 < 1U;
                x = ((Unk8001EF54 *)((u8 *)arg0 + (arg0->field_28 << 3)))->field_18;
                arg0->field_2C = x;
                arg0->field_2E = ((Unk8001EF54 *)((u8 *)arg0 + (arg0->field_28 << 3)))->field_1A;
            }
        } while (arg0->field_34 == 0);
    }

    arg0->field_34 = 0;
}

void func_8001F018(void *arg0) {
    u8 byte;

outer:
    while (func_8006A534(1, 0) == 0) {
    }

    byte = 0x80;
    while (func_8006A5A4(0xE, &byte, 0) == 0) {
    }

pollFirst:
    switch (func_8006A534(1, 0)) {
    case 0:
        goto pollFirst;
    case 2:
        func_8006DD30(3);
        break;
    }

send:
    while (func_8006A5A4(0x15, arg0, 0) == 0) {
    }

pollNext:
    switch (func_8006A534(1, 0)) {
    case 0:
        goto pollNext;
    case 2:
        break;
    default:
        goto send;
    }

    if (func_8006CD0C(0x1E0) == 0) {
        goto outer;
    }
}

void func_8001F0E0(void) {
    D_8009F0A4 = &D_801E8AFC;
    D_8019C7A4 = &D_801E8AFC;
}

void func_8001F100(void) {
    u32 value;

    value = g_GrandPrixMode;
    D_801E4BB0 = 0;
    if (value != 0) {
        value = 0x5DC;
    } else {
        value = 0xA0A;
    }
    D_8019CB6C = value;
    D_8009EC8C = 0;
}

/*
 * Packs a GameRenderPairPoint (billboard/edge pair) from two
 * GameRenderSourcePoint records (srcA=first, srcB=second) into the pair-point
 * output ring D_8009F0A4, keyed by pairIndex>>1. Only even indices hold a pair
 * (odd indices are skipped). Stride is ((n<<1)+n)<<4 == n*0x30 (sizeof pair).
 */
void func_8001F134(s32 pairIndex, u8 *srcA, u8 *srcB) {
    GameRenderPairPoint *dst;
    u8 *base;
    GameRenderSourcePoint *src1;
    GameRenderSourcePoint *src2;
    s32 sourceField_AE;
    s32 current;
    s32 odd;
    u32 first;

    current = D_8009E782;
    src2 = (GameRenderSourcePoint *)srcB;
    sourceField_AE = src2->field_AE;
    D_801E4D8C = current;
    odd = pairIndex & 1;
    D_801E4BC0 = sourceField_AE;
    if (odd) {
        return;
    }

    pairIndex >>= 1;
    dst = (GameRenderPairPoint *)(((pairIndex << 1) + pairIndex) << 4);
    base = D_8009F0A4;
    src1 = (GameRenderSourcePoint *)srcA;
    first = src1->field_0;
    dst = (GameRenderPairPoint *)((s32)dst + (s32)base);
    dst->first_0 = first;
    dst->first_2 = src1->field_4;
    dst->first_4 = src1->field_8;
    dst->first_6 = src1->field_60;
    dst->first_8 = src1->field_20;
    dst->first_A = src1->field_24;
    dst->first_C = src1->field_28;
    dst->first_E = src1->field_48;
    dst->first_2C = src1->field_44;
    dst->second_10 = src2->field_0;
    dst->second_12 = src2->field_4;
    dst->second_14 = src2->field_8;
    dst->second_16 = src2->field_60;
    dst->second_18 = src2->field_20;
    dst->second_1A = src2->field_24;
    dst->second_1C = src2->field_28;
    dst->second_1E = src2->field_48;
    dst->second_2E = src2->field_44;
    dst->first_24 = src1->field_30;
    dst->second_28 = src2->field_30;
    dst->field_20 = src1->field_8C;
}

/*
 * Single-point variant of func_8001F134: packs a GameRenderSinglePoint from a
 * GameRenderSourcePoint into the single-point output buffer D_8019C7A4, keyed
 * by pointIndex>>1 (odd indices skipped). Stride ((n<<3)-n)<<2 == n*0x1C
 * (sizeof GameRenderSinglePoint).
 */
void func_8001F274(s32 pointIndex, u8 *srcPtr) {
    GameRenderSinglePoint *dst;
    u8 *base;
    GameRenderSourcePoint *src;
    u32 first;

    D_801E4D8C = D_8009E782;
    if (pointIndex & 1) {
        return;
    }

    pointIndex >>= 1;
    dst = (GameRenderSinglePoint *)(((pointIndex << 3) - pointIndex) << 2);
    base = D_8019C7A4;
    src = (GameRenderSourcePoint *)srcPtr;
    first = src->field_0;
    dst = (GameRenderSinglePoint *)((s32)dst + (s32)base);
    dst->field_0 = first;
    dst->field_2 = src->field_4;
    dst->field_4 = src->field_8;
    dst->field_6 = src->field_60;
    dst->field_8 = src->field_20;
    dst->field_A = src->field_24;
    dst->field_C = src->field_28;
    dst->field_E = src->field_48;
    dst->field_18 = src->field_44;
    dst->field_14 = src->field_30;
    dst->field_10 = src->field_8C;
}
