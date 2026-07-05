#include "common.h"
extern s32 D_8009F094;
extern s32 D_8019C760;
extern s32 D_801E42E4;
s32 func_8005B9CC(void);
void func_80042C94(void);
void func_8006A534(s32 arg0, s32 arg1);
void func_8006A5A4(s32 arg0, s32 arg1, s32 arg2);
void func_8001E6B4(s32 arg0) {
    func_8005B9CC();
    func_80042C94();
    D_8009F094 = 0;
    D_8019C760 = arg0;
    D_801E42E4 = 5;
    func_8006A534(0, 0);
    func_8006A5A4(9, 0, 0);
}
