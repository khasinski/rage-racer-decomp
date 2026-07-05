#include "common.h"

extern u32 D_801E40B8;
extern u32 D_8009E694;
extern s32 D_8009E66C;
extern u32 D_8009E870;
extern u32 D_801E4030;
extern u8 D_801F18CC[];
extern u8 D_801F1854[];

void func_800266D0(void);
void func_80033AA0(s32 arg0, u32 arg1);
void func_80042CCC(u32 arg0);
void func_800268EC(void);
s32 func_8001A1F0(u32 arg0, s32 arg1);
void func_8003BB50(void);
void func_80019EFC(s32 arg0);
void func_80043BCC(u32 arg0, u8 *arg1);
void func_800389F0(void);
void func_80045CD4(void);
void func_800418D4(void);
void func_80041888(void);
void func_8004123C(void);
void func_8003E2E8(u32 arg0, u32 arg1);

void func_80026920(void) {
    u32 value;
    u32 timer;
    s32 index;
    s32 offset;

    D_801E40B8++;
    if (D_801E40B8 < 0x3D) {
        func_800266D0();
        value = D_801E40B8 - 6;
        func_80033AA0(0xFF - (((value * 3) * 4) - value), 0x49);
    }

    timer = D_801E40B8;
    if (timer == 0x6CC) {
        func_80042CCC(0x38);
        timer = D_801E40B8;
    }
    if (timer >= 0x6CD) {
        register u32 adjusted asm("v0");

        adjusted = timer - 0x6CC;
        func_80033AA0((adjusted * 4) + adjusted, 0x49);
    }

    if (D_801E40B8 == 0x708) {
        func_800268EC();
    }

    D_8009E694++;
    D_8009E66C = func_8001A1F0(0xFF, D_8009E66C);
    func_8003BB50();

    index = D_8009E66C;
    offset = (((((index * 3) * 4) + index) * 8) - index) * 4;
    func_80019EFC(*(s16 *)&D_801F18CC[offset]);

    func_80043BCC(
        D_8009E870,
        &D_801F1854[(((((D_8009E66C * 3) * 4) + D_8009E66C) * 8) - D_8009E66C) * 4]);
    func_800389F0();
    func_80045CD4();
    func_800418D4();
    *(u32 *)0x1F800084 = D_801E4030;
    func_80041888();
    func_8004123C();
    func_8003E2E8(D_8009E694, 1);
}
