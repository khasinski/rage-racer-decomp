#include "common.h"

extern volatile u8 D_8009B194;

void func_80042FA0(u8 arg0);

void func_80043134(s32 arg0) {
    s32 product = (arg0 << 7) - arg0;

    D_8009B194 = product / 15;
    func_80042FA0(D_8009B194);
}
