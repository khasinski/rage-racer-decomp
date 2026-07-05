#include "common.h"

s32 func_800731CC(void);
s32 func_8005E600(s32 arg0);
s32 func_8005B948(s32 arg0);
void func_800736E8(void);
void func_80073614(s32 arg0);
void func_80073748(s32 arg0, s32 arg1);
void func_80078018(s32 arg0);
void func_8006DD30(s32 arg0);
void func_80072B3C(s32 arg0);
void func_80072260(void);
void func_80071C24(void);

extern s32 D_801E6C9C;
extern s16 D_801E6CB0;
extern s16 D_801E6CB2;

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B768", func_8005B768);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B768", func_8005B89C);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B768", func_8005B948);

s32 func_8005B9CC(void) {
    func_800731CC();
    if (func_8005E600(1) == 0) {
        return 0;
    }
    if (func_8005B948(2) == 0) {
        return 0;
    }
    if (func_8005B948(3) == 0) {
        return 0;
    }
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B768", func_8005BA20);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B768", func_8005BB1C);
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8005B768", func_8005BC14);

void func_8005BC80(void) {
    register s32 i asm("$16");
    register s32 *flag asm("$3") = &D_801E6C9C;

    asm volatile("" : "=r"(flag) : "0"(flag));
    if (*flag != 0) {
        *flag = 0;
        func_800736E8();
        func_80073614(0);
        func_80073748(0, 0);
        i = 0;
        while (i < 24) {
            func_80078018((s16)i);
            i++;
        }
        func_8006DD30(2);
        func_80072B3C(D_801E6CB0);
        func_80072B3C(D_801E6CB2);
        func_80072260();
        func_80071C24();
    }
}
