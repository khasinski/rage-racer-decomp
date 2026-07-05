#include "common.h"
extern s16 D_801E6D90;
extern s32 D_801E6D94;
void func_80072698(s32 arg0, s32 arg1, s32 arg2);
void func_8005E7A0(s32 arg0) {
    D_801E6D94 = arg0;
    func_80072698(D_801E6D90, (s16)arg0, (s16)arg0);
}
