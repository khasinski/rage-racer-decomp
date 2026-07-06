#include "common.h"

extern s32 D_8009E6D4;
extern s32 D_8009E704;
extern s16 D_801E4DAC;
extern s32 D_801F179C;
extern s32 D_801F1854;
extern s32 D_801F1884;

void func_8001F8D0(s32 arg0, void *arg1, void *arg2);
void func_8002BF68(void *arg0, s32 arg1);
void func_8002C168(void *arg0);
s32 func_80030EB4(void *arg0, s32 arg1);
void func_80032280(void *arg0);
void func_8003F0F8(void);

void func_80034F74(void) {
    void *primary;
    void *secondary;

    func_8003F0F8();

    primary = &D_8009E6D4;
    secondary = &D_801F1854;
    func_8001F8D0(D_801F179C, primary, secondary);

    D_8009E704 = func_80030EB4(primary, D_8009E704);
    func_8002BF68(primary, 1);
    func_8002C168(primary);
    func_80032280(primary);

    if (D_801E4DAC == 1) {
        D_801F1884 = func_80030EB4(secondary, D_801F1884);
        func_8002BF68(secondary, 1);
        func_8002C168(secondary);
        func_80032280(secondary);
    }
}
