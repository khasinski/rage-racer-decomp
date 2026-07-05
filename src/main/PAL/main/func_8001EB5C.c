#include "common.h"
extern s32 D_8009AF58, D_801E8A90;
void DecDCTReset(s32 arg0) asm("func_80063E44");
void func_800640B0(s32 arg0);
void StSetRing(s32 arg0, s32 arg1) asm("func_8006A058");
void func_8006CF68(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_8001F018(s32 arg0);
void func_8001EB5C(s32 arg0) {
    DecDCTReset(0);
    func_800640B0(arg0);
    StSetRing(D_8009AF58, 0x20);
    func_8006CF68(1, 1, -1, 0, 0);
    func_8001F018(D_801E8A90);
}
