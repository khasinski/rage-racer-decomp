#include "common.h"

extern u8 *D_8019C900;
extern s16 D_801E4DAC;
extern s32 D_801E428C;
extern s32 D_8009EC90;
extern s32 D_8009E6A4;
extern s16 D_8019CABC;

extern char D_80010C30[];
extern char D_80010C40[];
extern char D_80010C44[];
extern char D_80010C50[];
extern char D_80010C5C[];
extern char D_80010C68[];
extern char D_80010C70[];

typedef struct {
    s32 a, b, c;
} Rec;
extern Rec D_8007BEEC[][6];
extern s32 D_8019C70C[][4][2];
extern s32 D_801E4408[][4][2];

s32 func_8001C8F0(s32 arg0);
void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0, s32 r, s32 g,
                   s32 b, s32 clutX, s32 shadeTex, s32 semiTrans, s32 flags);
void func_800632F0(void *dst, void *fmt, s32 v);
void func_80016B7C(u32 a0, u32 a1, void *a2, u32 a3, u32 a4);
void func_80021CD4(void *dst, s32 v);

void func_8001C974(void) {
    char buf[88];
    s32 col;
    s32 y0;
    void *ot = D_8019C900 + 204;

    col = func_8001C8F0(0);
    func_80046A2C(ot, 0x74, 0x14, 0x58, 0x38, 0xa8, 0xa8, col, col, col, 0x1f, 0, 1, 0x29);
    func_80046A2C(ot, 0x44, 0x50, 0xb8, 0x14, 0x48, 0xe8, col, col, col, 0x80, 0, 1, 0x29);

    col = func_8001C8F0(1);
    if (D_801E4DAC != 0) {
        func_800632F0(buf, D_80010C30, D_8009EC90);
        func_80016B7C(0x5e, 0x68, buf, 0x7812, col);
        y0 = 0x78;
    } else {
        y0 = 0x68;
    }
    func_80046A2C(ot, 0x5e, y0, 0x84, 0xc, 0, D_801E428C * 12 + 156, col, col, col, 0x12, 0, 1, 0x29);

    col = func_8001C8F0(2);
    if (D_801E4DAC != 0) {
        func_80016B7C(0x80, 0x88, D_80010C40, 0x7812, col);
        func_800632F0(buf, D_80010C44, D_8007BEEC[D_801E428C][D_8009E6A4].a);
        func_80016B7C(0x56, 0x98, buf, 0x7812, col);
        func_800632F0(buf, D_80010C50, D_8007BEEC[D_801E428C][D_8009E6A4].b);
        func_80016B7C(0x56, 0xa4, buf, 0x7812, col);
        func_800632F0(buf, D_80010C5C, D_8007BEEC[D_801E428C][D_8009E6A4].c);
        func_80016B7C(0x56, 0xb0, buf, 0x7812, col);
    } else {
        func_80016B7C(0x62, 0x7c, D_80010C68, 0x7812, col);
        func_80021CD4(buf, D_8019C70C[D_8019CABC][D_801E428C][D_801E4DAC]);
        func_80016B7C(0x6a, 0x8c, buf, 0x7812, col);
        func_80016B7C(0x6a, 0x9c, D_80010C70, 0x7812, col);
        func_80021CD4(buf, D_801E4408[D_8019CABC][D_801E428C][D_801E4DAC]);
        func_80016B7C(0x6a, 0xac, buf, 0x7812, col);
    }
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8001C974", func_8001CD54);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8001C974", func_8001CFB4);
