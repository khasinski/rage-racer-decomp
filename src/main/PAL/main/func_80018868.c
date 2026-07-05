#include "common.h"

extern s32 D_8007BED8;

s32 func_8001882C(s32 arg0);
void func_80019C04(void);

void func_80018868(s32 arg0) {
    func_8001882C(arg0);

    while (D_8007BED8 != 0) {
        func_80019C04();
    }
}
