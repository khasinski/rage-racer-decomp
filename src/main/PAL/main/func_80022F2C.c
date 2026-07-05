#include "common.h"
extern s32 D_8019C768;
extern s32 D_801E42A0;
extern s32 D_801E42E0;
extern s32 D_801E42E4;
extern s32 D_801E40B8;
void func_8006A534(s32 arg0, s32 arg1);
void func_8006A5A4(s32 arg0, s32 arg1, s32 arg2);
void func_80065860(s32 arg0);
void func_8001BE9C(s32 arg0, s32 arg1, s32 arg2);
void func_80022F2C(void) {
    func_8006A534(0, 0);
    func_8006A5A4(9, 0, 0);
    func_80065860(0);
    func_8001BE9C(0, 0, 0);
    D_8019C768 = 0x80;
    D_801E42A0 = 4;
    D_801E42E0 = 0;
    D_801E42E4 = 0x22;
    D_801E40B8 = 0;
}
