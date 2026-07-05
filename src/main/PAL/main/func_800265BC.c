#include "common.h"

extern s32 D_801E4B30;
extern s32 D_8019C768;
extern s32 D_801E682C;
extern s32 D_801E42E0;
extern s32 D_801E40B8;
extern s32 D_801E42E4;
extern s32 D_8009E66C;

void func_80065860(s32 arg0);
void func_8001BE9C(s32 arg0, s32 arg1, s32 arg2);
void func_8001A3C0(void *arg0);
void func_80019730(void);
s32 func_80019844(void);

void func_800265BC(void) {
    register s32 initialValue asm("$16");

    func_80065860(0);
    func_8001BE9C(0, 0, 0);

    initialValue = 0x80;
    D_8019C768 = initialValue;
    func_8001A3C0((void *)D_801E4B30);
    func_80019730();
    func_80019844();

    D_801E682C = 0;
    D_801E42E0 = initialValue;
    D_801E40B8 = 0;
    D_801E42E4 = 0x1E;
    D_8009E66C = 0;
}
