#include "common.h"

extern u8 *D_801E4150;
extern s32 D_801E408C;
extern u8 *D_8009E688;

extern s32 func_80030EB4(u8 *ent, s32 arg);
extern void func_8002BF68(u8 *ent, s32 arg);
extern void func_80031298(u8 *ent, s32 arg, void *arg2);

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} Vec4;

void func_800383A8(u8 *ent, s32 pos, s32 *arr) {
    u8 *base;
    s32 sub;
    u8 *p;
    u16 val122;
    s32 scene;
    u16 av;

    *(s32 *)(ent + 0xB0) = 1;
    av = *(u16 *)&arr[pos];
    sub = (pos + 1) * 12;
    {
        u8 *baseValue = D_801E4150;
        base = baseValue;
    }
    *(s16 *)(ent + 0x8A) = 0;
    *(s32 *)(ent + 0xBC) = 1;
    *(s16 *)(ent + 0xAE) = av;
    val122 = *(u16 *)&arr[pos];
    scene = D_801E408C;
    *(s16 *)(ent + 0x122) = val122;
    {
        u8 *p1;

        p1 = base + (sub + scene * 144);
        *(s32 *)(ent + 0x30) = *(s16 *)(p1 + 0x35C);
        *(s32 *)(ent + 0x00) = *(s32 *)(p1 + 0x354);
        *(s32 *)(ent + 0x08) = *(s32 *)(p1 + 0x358);
        *(s32 *)(ent + 0x04) = 0;
    }
    {
        s32 ret = func_80030EB4(ent, *(s32 *)(ent + 0x30));
        s32 lev = D_801E408C;
        s32 idx;
        s32 levShift;
        s32 acc;
        s32 angle;

        *(s32 *)(ent + 0x30) = ret;
        *(s32 *)(ent + 0x20) = 0;
        idx = *(s32 *)(ent + 0x30);
        acc = 0xC00;
        levShift = lev << 11;
        angle = *(s16 *)(D_8009E688 + idx * 24 + 0xA);
        acc -= levShift;
        *(s32 *)(ent + 0x24) = (acc - angle) & 0xFFF;

        *(s32 *)(ent + 0x28) = 0;
        *(s32 *)(ent + 0x64) = 0;
        *(s32 *)(ent + 0x6C) = 0;
        *(s32 *)(ent + 0x68) = 0;
        *(s32 *)(ent + 0x70) = 0;
        *(s32 *)(ent + 0xA4) = 0;
        *(s32 *)(ent + 0xA8) = 0;
        *(s32 *)(ent + 0xD0) = 0;
        *(s32 *)(ent + 0xCC) = 0;
        *(s32 *)(ent + 0xC8) = 0;
        *(s32 *)(ent + 0xE0) = 0;
        *(s32 *)(ent + 0xDC) = 0;
        *(s32 *)(ent + 0xD8) = 0;
        *(s32 *)(ent + 0x18) = 0;
        *(s32 *)(ent + 0x14) = 0;
        *(s32 *)(ent + 0x10) = 0;
        *(s32 *)(ent + 0x100) = 0;
        *(s16 *)(ent + 0x116) = 0;
        *(s16 *)(ent + 0x110) = 0;
        *(s32 *)(ent + 0xF4) = 0;
        *(s16 *)(ent + 0x13A) = 0;
        *(s32 *)(ent + 0xF0) = 0;
        *(s32 *)(ent + 0x108) = *(s32 *)(ent + 0x24);
        p = base + (sub + lev * 144);
        *(s32 *)(ent + 0xEC) = *(s32 *)(ent + 0x24);
        *(s32 *)(ent + 0xA0) = *(s32 *)(ent + 0x24);
        *(s32 *)(ent + 0xF8) = 0;
        *(s16 *)(ent + 0x104) = 0;
        *(s32 *)(ent + 0xC4) = 0;
        *(s16 *)(ent + 0x138) = 0;
        func_8002BF68(ent, *(s16 *)(p + 0x35E));
    }

    sub += D_801E408C * 144;
    base += sub;
    {
        u16 model;

        model = *(u16 *)(base + 0x35E);
        *(s16 *)(ent + 0xAC) = model;
        if ((s16)model != -1) {
            struct {
                s32 pad[4];
                s16 a;
                s16 b;
            } pair;

            pair.a = 20;
            pair.b = -20;
            func_80031298(ent, *(s32 *)(ent + 0x30), &pair.a);
            *(s32 *)(ent + 0x60) = *(s32 *)(ent + 0x04);
            *(s32 *)(ent + 0x74) = *(s32 *)(ent + 0x70);
        }
    }

    {
        s32 height;

        height = *(s32 *)(ent + 0x34);
        *(s16 *)(ent + 0x120) = 0;
        *(s32 *)(ent + 0xFC) = height;
        *(s16 *)(ent + 0x11E) = height;
        *(s16 *)(ent + 0x11C) = height;
    }
    *(Vec4 *)(ent + 0x50) = *(Vec4 *)(ent + 0x20);
    {
        s32 lateral;

        lateral = *(s32 *)(ent + 0x04);
        *(s32 *)(ent + 0x40) = 0;
        *(s32 *)(ent + 0x44) = 0;
        *(s32 *)(ent + 0x48) = 0;
        *(s32 *)(ent + 0x60) = lateral;
    }
}
