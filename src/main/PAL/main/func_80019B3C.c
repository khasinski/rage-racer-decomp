#include "common.h"

extern s16 D_801E4034;
extern s32 D_8009E6A4;
extern s32 D_8007C6A8;
extern s32 D_8007C6AC;
extern s32 *D_801E8A90;
extern s32 D_8019CA1C;
extern s32 D_8019C708;

void func_8001E6B4(void);

void func_80019B3C(void) {
    register s32 index asm("$3");
    register s32 offset asm("$2");
    register s32 *base asm("$4");
    s32 value;

    func_8001E6B4();

    if (D_801E4034 != 0) {
        index = 5;
    } else {
        index = 1;
    }

    offset = D_8009E6A4;
    base = &D_8007C6A8;
    index += offset;
    offset = index << 3;
    value = *(s32 *)((s32)&D_8007C6AC + offset);
    D_801E8A90 = (s32 *)((s32)base + offset);
    D_8019CA1C = value;
    D_8019C708 = value * 2;
}
