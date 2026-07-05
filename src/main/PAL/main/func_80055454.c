#include "common.h"

extern s32 D_801E40D4;
extern s32 D_8019C7C8;
extern s16 D_8019CA18;
extern s16 D_801E41A4;

void func_80055454(void) {
    s32 index;
    u8 *ptr;

    D_8019CA18 = -1;
    index = D_801E40D4 - 1;
    if (index >= 0) {
        register s32 one asm("a1") = 1;
        register s32 offset asm("v0") = index * 8;
        ptr = (u8 *)(offset + D_8019C7C8);
        while (index >= 0) {
            if (ptr[5] == one) {
                D_8019CA18 = index;
                break;
            }
            index--;
            ptr -= 8;
        }
    }

    D_801E41A4 = -1;
    index = D_801E40D4 + 1;
    if (index < 13) {
        register s32 one asm("a1") = 1;
        register s32 offset asm("v0") = index * 8;
        ptr = (u8 *)(offset + D_8019C7C8);
        while (index < 13) {
            if (ptr[5] == one) {
                D_801E41A4 = index;
                break;
            }
            index++;
            ptr += 8;
        }
    }
}
