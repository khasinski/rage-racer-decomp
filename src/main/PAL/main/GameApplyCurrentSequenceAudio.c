#include "common.h"
extern s16 D_801E6D90;
extern s16 D_801E6D94;
void func_80072698(s32 arg0, s32 arg1, s32 arg2);
void func_8005B190(s32 arg0, s32 arg1);
void func_8005EA6C(void) {
    func_80072698(D_801E6D90, D_801E6D94, D_801E6D94);
    func_8005B190(0x28, 0x28);
}
