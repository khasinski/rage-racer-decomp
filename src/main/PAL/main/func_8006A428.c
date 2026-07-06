#include "common.h"

s32 func_8006BCC4_entry(void);
s32 func_8006BD14_entry(void);
s32 func_8006BBD0_entry(void);

s32 func_8006A428(s32 arg0) {
    if (arg0 == 2) {
        func_8006BCC4_entry();
        return 1;
    }

    if (func_8006BD14_entry() != 0) {
        return 0;
    }
    if (arg0 != 1) {
        return 1;
    }

    if (func_8006BBD0_entry() != 0) {
        return 0;
    }

    asm("");
    return 1;
}
