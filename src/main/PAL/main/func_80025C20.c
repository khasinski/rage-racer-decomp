#include "common.h"

extern s32 D_8007BED8;
extern s32 D_801E42A0;

void func_80025AC8(void);

void func_80025C20(void) {
    if (D_8007BED8 == 0) {
        D_801E42A0 = 4;
    }
    func_80025AC8();
}
