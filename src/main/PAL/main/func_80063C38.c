#include "common.h"

extern u8 D_800132C0[];

void func_80063C88(s32 arg0);

void func_80063C38(u8 *arg0) {
    u8 *ptr;
    s32 value;

    ptr = arg0;
    if (ptr == 0) {
        ptr = D_800132C0;
    }

    while (value = *ptr++, value != 0) {
        func_80063C88(value);
    }
}
