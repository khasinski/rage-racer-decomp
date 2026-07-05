#include "common.h"
#include "psyq/cd.h"

extern s32 D_80099048;
extern char *D_80099060[];
extern char *D_800990E0[];
extern u8 D_800136D0[];

void func_8006BAF0(void);

void func_8006A494(void) {
    func_8006BAF0();
}

s32 func_8006A4B4(s32 arg0) {
    s32 old = D_80099048;
    D_80099048 = arg0;
    return old;
}

char *CdComstr(s32 arg0) {
    arg0 &= 0xFF;
    if ((u32)arg0 >= 0x1C) {
        return (char *)D_800136D0;
    }
    return D_80099060[arg0];
}

char *CdIntstr(s32 arg0) {
    arg0 &= 0xFF;
    if ((u32)arg0 >= 7) {
        return (char *)D_800136D0;
    }
    return D_800990E0[arg0];
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/func_8006A494", func_8006A534);
