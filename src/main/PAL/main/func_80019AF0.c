#include "common.h"

extern s32 D_8007C6AC;
extern s32 *D_801E8A90;
extern s32 D_8019CA1C;
extern s32 D_8019C708;

void func_8001E6B4(void);

void func_80019AF0(void) {
    s32 *ptr;
    s32 value;

    func_8001E6B4();

    ptr = &D_8007C6AC;
    value = *ptr;
    D_801E8A90 = ptr - 1;
    D_8019CA1C = value;
    D_8019C708 = value * 2;
}
