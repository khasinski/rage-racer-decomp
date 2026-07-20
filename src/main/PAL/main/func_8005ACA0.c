#include "common.h"

extern s32 D_8009E694;
extern s32 D_801E40B8;
extern s32 D_8019C9F8;
extern s32 D_8009B340;
extern s32 D_8009B344;
extern s32 D_8009B348;
extern s32 D_8009B324;
extern s32 D_801E4B88;
extern s32 D_801E40D4;
extern s32 D_8019C7C8;

struct Entry_5ACA0 {
    u8 f0;
    u8 f1;
    u8 pad[6];
};
extern s32 D_8009B30C;
extern u8 D_80082A90;
extern s32 D_8009B2FC;
extern s32 D_8009B314;
extern u8 D_801E4369;
extern s32 D_8009B318;

extern s32 (*D_80082EF0[])(s32);
extern void (*D_80082EB8[])(void);

void func_80065860(s32);
void func_80047024(void *, s32, s32, s32, s32, s32, s32, s32, s32);
s32 func_800487D8(void *, void *);
void func_800496F0(s32, s32);
void func_80046A2C(void *, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
void func_80047E60(s32);

void func_8005ACA0(void) {
    void *scratch;
    register s32 c0 asm("$2");
    register s32 c1 asm("$3");

    c0 = D_8009E694;
    c1 = D_801E40B8;
    scratch = *(void **)0x1F800004;
    c0 += 1;
    c1 += 1;
    D_8009E694 = c0;
    D_801E40B8 = c1;
    if (c1 == 2) {
        func_80065860(1);
    }
    func_80047024(scratch, 0, 0, 0x140, 2, 0, 0, 0, 0xFF);

    if ((u32)(D_8019C9F8 - 1) < 2) {
        *(s32 *)0x1F800064 = 1;
    } else {
        *(s32 *)0x1F800064 = 5;
    }

    if (D_8009B340 > 0) {
        D_80082EF0[D_8009B340](0x14);
    }
    if (D_8009B344 > 0) {
        D_8009B348 = D_80082EF0[D_8009B344](-10);
    }
    D_80082EB8[D_8019C9F8]();

    func_800496F0(D_8009B324, ((struct Entry_5ACA0 *)D_8019C7C8)[(D_8019C9F8 == 0xB) ? D_801E4B88 : D_801E40D4].f1);

    {
        register s32 flag asm("$6");
        flag = D_8009B30C;
        if (flag == 0) {
            return;
        }
    }
    if (func_800487D8(&D_80082A90, &D_8009B2FC) == 0) {
        return;
    }

    if (D_8009B314 != 0) {
        if (D_801E4369 == 0x23) {
            scratch = (void *)((s32)scratch + 4);
            func_80046A2C(scratch, 0xC0, 0x1A1, 0x20, 0xC, 0x94, 0xF4, 0, 0, 0, 0x244, 1, 1, 0x3B);
            func_80046A2C(scratch, 0xF0, 0x1A1, 0x2C, 0xC, 0xB4, 0xF4, 0, 0, 0, 0x244, 1, 1, 0x3B);
        } else {
            scratch = (void *)((s32)scratch + 4);
            func_80046A2C(scratch, 0xC0, 0x1A1, 0x20, 0xC, 0x94, 0xE8, 0, 0, 0, 0x244, 1, 1, 0x3B);
            func_80046A2C(scratch, 0xF0, 0x1A1, 0x2C, 0xC, 0xB4, 0xE8, 0, 0, 0, 0x244, 1, 1, 0x3B);
        }
    }
    func_80047E60(D_8009B318);
}
