#include "common.h"
#include "psyq/cd.h"

extern s32 D_8009E69C;
extern s16 D_8019C790;
extern s32 D_8019C79C;
extern s32 D_8019C994;
extern s32 D_8019C9A0;
extern s32 D_801E3E08;
extern s32 D_801E4190;
extern s32 D_801E8274;

void func_8006D1B0(s32 arg0);

void StSetStream(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_8006D1B0(1);
    D_801E8274 = 0;
    D_8019C994 = arg3;
    D_8019C79C = arg0 & 1;
    D_801E4190 = 0;
    D_801E3E08 = 0;
    D_8019C790 = 0;
    D_8009E69C = 0;
    D_8019C9A0 = arg4;
}
