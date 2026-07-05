#include "common.h"
extern s32 D_8009903C;
extern s32 D_80099040;
extern s32 D_8009904C;
extern s32 D_80099050;
void func_8006DF34(void);
void func_8006C17C(void);
void func_8006DF64(s32 arg0, void *arg1);
void func_8006BCC4(void) {
    D_80099040 = 0;
    D_8009903C = 0;
    D_80099050 = 0;
    D_8009904C = 0;
    func_8006DF34();
    func_8006DF64(2, (void *)func_8006C17C);
}
