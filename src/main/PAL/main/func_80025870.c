#include "common.h"
extern u8 *D_8019C900;
extern s32 D_8009E694, D_801E40B8, D_8019CB14;
extern void (*D_8007D67C[])(void);
s32 func_80032F34(void *a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8);
void func_80065860(s32 arg0);
void func_800256B8(void);
void func_80025870(void) {
    *(s32 *)0x1F800000 = func_80032F34(D_8019C900 + 204, *(s32 *)0x1F800000, 0, 0, 0x140, 2, 0, 0, 0);
    D_8009E694 = D_8009E694 + 1;
    D_801E40B8 = D_801E40B8 + 1;
    if (D_801E40B8 == 2) {
        func_80065860(1);
    }
    D_8007D67C[D_8019CB14]();
    func_800256B8();
}
