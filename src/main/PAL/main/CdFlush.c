#include "common.h"
#include "psyq/cd.h"

extern s32 D_80099048;
extern s32 D_8009903C;
extern s32 D_80099040;
extern char *D_80099060[];
extern char *D_800990E0[];
extern u8 D_800136D0[];

void func_8006BAF0(void);
s32 func_8006B0D4(s32 arg0, s32 arg1);
s32 func_8006B354(s32 arg0, s32 arg1);

void func_8006A494(void) {
    func_8006BAF0();
}

s32 func_8006A4B4(s32 arg0) {
    s32 old = D_80099048;
    D_80099048 = arg0;
    return old;
}

char *func_8006A4CC(s32 arg0) {
    arg0 &= 0xFF;
    if ((u32)arg0 >= 0x1C) {
        return (char *)D_800136D0;
    }
    return D_80099060[arg0];
}

char *func_8006A500(s32 arg0) {
    arg0 &= 0xFF;
    if ((u32)arg0 >= 7) {
        return (char *)D_800136D0;
    }
    return D_800990E0[arg0];
}

s32 func_8006A534(s32 arg0, s32 arg1) {
    return func_8006B0D4(arg0, arg1);
}

s32 func_8006A554(s32 arg0, s32 arg1) {
    return func_8006B354(arg0, arg1);
}

s32 func_8006A574(s32 arg0) {
    s32 old = D_8009903C;

    D_8009903C = arg0;
    return old;
}

s32 func_8006A58C(s32 arg0) {
    s32 old = D_80099040;

    D_80099040 = arg0;
    return old;
}
