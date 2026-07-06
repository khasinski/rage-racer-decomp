#include "common.h"

s32 func_800657E4(void);

extern char D_80013374[];
extern void (*D_800941E4)(char *, ...);

void func_80064C94(s32 arg0) {
    s32 mode;
    u32 value;

    mode = func_800657E4();
    if (mode == 1) {
        goto high_mode;
    }

    mode = func_800657E4();
    if (mode != 2) {
        goto low_mode;
    }

high_mode:
    D_800941E4(D_80013374, (((u32)arg0 & 0xFFFF) >> 9) & 3, (((u32)arg0 & 0xFFFF) >> 7) & 3, (((u32)arg0 & 0xFFFF) << 6) & 0x7C0, (((u32)arg0 & 0xFFFF) << 3) & 0x300);
    return;

low_mode:
    value = arg0 & 0xFFFF;
    D_800941E4(D_80013374, (value >> 7) & 3, (value >> 5) & 3, (value << 6) & 0x7C0, ((value << 4) & 0x100) + ((value >> 2) & 0x200));
}
