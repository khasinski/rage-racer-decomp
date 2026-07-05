#include "common.h"

extern s32 D_8007F5FC;
extern u8 D_8009B194;

void func_80042FA0(u8 arg0);

void func_8004318C(s32 arg0) {
    D_8007F5FC = arg0;
    func_80042FA0(D_8009B194);
}
