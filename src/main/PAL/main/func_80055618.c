#include "common.h"

extern s32 D_8009B338;
extern s32 D_8019CB0C;
extern s32 D_8019C9F8;
extern s32 D_8019C9F0;
extern s32 D_8009B31C;
extern s32 D_8009B320;

void func_80051238(void);
void func_80055454(void);
void func_8004FCE8(s32 arg0, s32 arg1, s32 arg2);
void func_8005131C(void);
void func_8004CF30(s32 arg0);

void func_80055618(void) {
    D_8019CB0C = D_8009B338;
    func_80051238();
    D_8019C9F8 = 4;
    D_8019C9F0 = 0;
    func_80055454();
    func_8004FCE8(D_8009B31C, D_8009B320, 0);
    func_8005131C();
    func_8004CF30(-9);
}
