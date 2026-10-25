#include "common.h"

extern s32 D_801D0894;
extern s32 D_800A4D08;

s32 func_80074B88(s32 a0, s32 *a1);
void ChangeClearRCntStub(s32 a0);
void func_80074B98(void);

void func_80074AD8(s32 param) {
    D_801D0894 = param;
    D_800A4D08 = -1;
    KernelCallbackSlot3();
    func_80074B88(0x20000001, &D_800A4D08);
    ChangeClearRCntStub(0);
}

s32 func_80074B28(void) {
    func_80074B98();
    return ~D_800A4D08;
}

void func_80074B78(void);

void func_80074B58(void) {
    func_80074B78();
}
