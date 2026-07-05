#include "common.h"

extern s32 D_8007BED8;

s32 func_800189E4(s32 arg0);
void func_80019C04(void);

void func_80018A20(s32 arg0) {
    func_800189E4(arg0);

    while (D_8007BED8 != 0) {
        func_80019C04();
    }
}
