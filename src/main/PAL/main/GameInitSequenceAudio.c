#include "common.h"
extern s32 D_801E6D8C;
void func_8007865C(s32 arg0);
void func_80072B04(s32 arg0);
void func_8005B190(s32 arg0, s32 arg1);
void func_8005E7DC(void);
void func_8005DBD8(void) {
    func_8007865C(0);
    func_80072B04(0x12);
    func_8005B190(0x28, 0x28);
    D_801E6D8C = 0;
    func_8005E7DC();
}
