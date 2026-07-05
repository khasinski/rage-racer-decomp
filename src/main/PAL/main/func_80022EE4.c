#include "common.h"
extern s32 D_801E42E4;
void func_8006A534(s32 arg0, s32 arg1);
void func_8006A5A4(s32 arg0, s32 arg1, s32 arg2);
void func_80018410(void);
void func_80022EE4(void) {
    func_8006A534(0, 0);
    func_8006A5A4(9, 0, 0);
    D_801E42E4 = 6;
    func_80018410();
}
