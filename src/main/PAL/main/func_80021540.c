#include "common.h"

extern s16 D_8019CB40[];
extern volatile s32 D_801E4DA8;
extern s32 D_801E40A8;

void func_80021540(void) {
    s32 offset;
    s32 current;
    s32 one;
    register s32 value asm("$2");

    D_801E4DA8 = 0;
    one = 1;
    offset = 0;
    do {
        current = *(s16 *)((u8 *)D_8019CB40 + offset);
        offset += 4;
        if (current == one) {
            D_801E4DA8 = D_801E4DA8 + 1;
        }
    } while (offset < 0x2C);

    value = D_801E4DA8;
    value = value < 5;
    if (value) {
        value = 9;
    } else {
        value = 10;
    }
    D_801E40A8 = value;
}
