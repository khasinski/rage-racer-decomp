#include "common.h"

extern u32 D_801E40B8;
extern s32 D_8007BED8;
extern u32 D_8009E6CC;
extern s32 D_801E682C;
extern u8 D_801E7734[];

void func_80065860(s32 arg0);
void func_80025940(void);
void func_80025E54(s32 arg0);
void func_80042BC0(s32 arg0);
void func_80042BF0(void);
void func_800266D0(void);

void func_80026824(void) {
    register s32 mode asm("$16");
    u32 timer;

    timer = D_801E40B8;
    if (timer < 0x2710) {
        D_801E40B8 = timer + 1;
    }

    if (D_801E40B8 == 2) {
        func_80065860(1);
    }

    if (D_8007BED8 == 0) {
        func_80025940();

        D_801E682C = 1;
        mode = D_801E7734[D_8009E6CC];
        func_80025E54(mode);

        mode += 3;
        if (mode == 0xC) {
            mode = 0x11;
        }

        func_80042BC0(mode);
        func_80042BF0();
    }

    func_800266D0();
}
