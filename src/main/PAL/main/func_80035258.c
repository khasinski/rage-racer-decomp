#include "common.h"
extern s32 D_801E42E4;
extern u8 D_8001147C;
void func_8005E4A4(s32 arg0);
void func_8005B190(s32 arg0, s32 arg1);
void func_80018410(void);
void func_8001674C(void *arg0);
void func_80035258(s32 arg0) {
    D_801E42E4 = arg0;
    func_8005E4A4(0);
    func_8005B190(0, 0);
    if (D_801E42E4 == 6) {
        func_80018410();
    }
    func_8001674C(&D_8001147C);
}
