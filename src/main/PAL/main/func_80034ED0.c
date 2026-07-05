#include "common.h"
extern u8 D_8001146C;
void func_8005BB1C(void);
void func_8005B468(s32 arg0);
void func_8005B204(s32 arg0, s32 arg1, s32 arg2);
void func_8001674C(void *arg0);
void func_80034ED0(void) {
    func_8005BB1C();
    func_8005B468(1);
    func_8005B204(2, 0, 0);
    func_8001674C(&D_8001146C);
}
