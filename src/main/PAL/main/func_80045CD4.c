#include "common.h"
#include "psyq/gpu.h"

typedef struct {
    s32 id;
    u8 pad[0x2C];
} Cmd;

extern u8 D_8019C8F4;
extern s32 D_8019C8FC;
extern s32 D_8019C774;

extern Cmd *D_801E40E8;
extern u32 *D_801E42F4;

extern s16 D_801E3FB4;
extern u8 D_801E3FB6;
extern u8 D_801E3FB7;
extern u8 D_801E3FB8;
extern u8 D_801E3FB9;
extern u32 D_801E3FBA;

extern s16 D_801E4022;
extern s16 D_801E4024;
extern s16 D_801E4026;
extern s16 D_801E4028;
extern s16 D_801E402A;
extern s16 D_801E402C;
extern s32 D_801E4FB0;
extern u8 *D_801E4140;
extern s32 D_801E428C;
extern s16 D_801E6DA4[];

extern s32 D_8009B24C;

void func_800455EC(void *arg0);
void func_8004554C(u8 *arg0, u8 *arg1, u8 *out, s32 arg3);
void func_80065B24();
void func_80069A38(s32 arg0, s32 arg1, s32 arg2);
void func_80069B14(void *arg0, s32 arg1, void *arg2);
void func_800686D4(s32 arg0, s32 arg1);

void func_80045CD4(void) {
    Rect rect;
    s32 local[3];
    u8 out[4];
    s32 i;
    s32 diff;
    s32 frac;
    u8 *p1;
    u8 *p2;
    Cmd *cur;
    u8 *pp;

    if (D_8019C8F4 == 0) {
        return;
    }

    cur = D_801E40E8;
    if (cur->id == D_8019C8FC) {
        D_801E4022 = 0;
        D_801E40E8 = cur + 1;
        func_800455EC(cur);
        if (D_801E40E8->id < 0) {
            D_801E40E8 = (Cmd *)D_801E42F4;
        }
    }

    D_8019C8FC = (D_8019C8FC < D_8019C774) ? D_8019C8FC + 1 : 0;

    if (D_801E3FB4 == 0) {
        return;
    }

    if (D_8019C8F4 != 0) {
        if (D_801E4022 < D_801E4024) {
            D_801E4022 = D_801E4022 + 1;
        }
    }

    diff = D_801E4024 - D_801E4022;
    frac = (D_801E4022 << 12) / D_801E4024;

    for (i = 0; i < 0x10; i++) {
        s16 *dst;
        p1 = (u8 *)(i * 3) + (D_801E4FB0 * 48 + (s32)D_801E4140);
        local[0] = p1[0] << 4;
        local[1] = p1[1] << 4;
        local[2] = p1[2] << 4;
        p2 = (u8 *)(i * 3) + (D_801E4026 * 48 + (s32)D_801E4140);
        func_80069A38(p2[0], p2[1], p2[2]);
        func_80069B14(local, frac, out);
        {
            u8 *idx = (u8 *)(i * 2);
            register s32 palo asm("$5");
            LA_ORDERED(palo, D_801E6DA4, idx);
            dst = (s16 *)(idx + palo);
        }
        *dst = 0;
        *dst = out[0];
        *dst |= out[1] << 5;
        *dst |= out[2] << 10;
    }

    rect.x = 0xE0;
    rect.y = 0x1E6;
    rect.w = 0x10;
    rect.h = 0x1;
    func_80065B24(&rect);

    pp = (u8 *)&D_801E3FBA;
    func_8004554C(pp + 0x0, pp + 0x4, pp - 0x4, frac);
    func_8004554C(pp + 0xC, pp + 0x10, pp + 0x8, frac);
    func_8004554C(pp + 0x18, pp + 0x1C, pp + 0x14, frac);
    func_8004554C(pp + 0x24, pp + 0x28, pp + 0x20, frac);
    func_8004554C(pp + 0x30, pp + 0x34, pp + 0x2C, frac);
    if (D_801E428C == 2) {
        func_8004554C(pp + 0x3C, pp + 0x40, pp + 0x38, frac);
        func_8004554C(pp + 0x48, pp + 0x4C, pp + 0x44, frac);
    } else {
        func_8004554C(pp + 0x54, pp + 0x58, pp + 0x50, frac);
        func_8004554C(pp + 0x60, pp + 0x64, pp + 0x5C, frac);
    }

    func_80069A38(D_801E3FB6, D_801E3FB7, D_801E3FB8);

    if (D_801E4028 != 0) {
        D_801E3FB9 = (D_801E402A * diff + D_801E402C * D_801E4022) / D_801E4024;
    }

    if (D_801E4022 == D_801E4024) {
        if ((*(u32 *)&D_801E3FB4 & 0xFFFF0000) == 0x80800000 && D_801E3FB8 == 0x80) {
            D_801E3FB4 = 0;
        }
    }

    if (D_801E4026 == 2) {
        D_8009B24C += 0xFA;
        if (D_8009B24C > 0x7FFF) {
            D_8009B24C = 0x7FFF;
        }
    } else {
        D_8009B24C -= 0xFA;
        if (D_8009B24C < 0x1770) {
            D_8009B24C = 0x1770;
        }
    }

    func_800686D4(D_8009B24C, 0x140);
}
