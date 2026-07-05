#include "common.h"

extern u8 *D_8019C900;

void func_80064DDC(void *arg0, void *arg1);
void *func_80017390(void *arg0, void *arg1, s32 arg2);

void func_80032E9C(s32 arg0) {
    register s32 count asm("$18");
    register s32 i asm("$16");
    register s32 offset asm("$17");
    register void **scratch asm("$16");

    count = 9;
    if (arg0 != 0) {
        count = 0xC;
    }

    i = 6;
    if (i < count) {
        offset = 0x23770;
        do {
            u8 *base = D_8019C900;

            i++;
            func_80064DDC(base + 0xCC, (void *)(offset + (s32)base));
            offset += 0x14;
        } while (i < count);
    }

    scratch = (void **)0x1F800000;
    *scratch = func_80017390(D_8019C900 + 0xCC, *scratch, 9);
}
