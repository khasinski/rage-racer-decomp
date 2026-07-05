#include "common.h"
#include "psyq/cd.h"

extern char *D_80099060[];
extern char *D_800990E0[];
extern u8 D_800136D0[];

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
