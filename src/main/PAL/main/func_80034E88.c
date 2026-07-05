#include "common.h"
extern u8 D_8001144C;
extern u8 D_8001145C;
s32 func_8005B5C4(void);
void func_8001674C(void *arg0);
void func_80034E88(void) {
    if (func_8005B5C4() != 0) {
        func_8001674C(&D_8001144C);
    }
    func_8001674C(&D_8001145C);
}
