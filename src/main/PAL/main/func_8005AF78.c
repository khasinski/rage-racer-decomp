#include "common.h"
extern s32 D_801E42E4;
extern s32 D_801E6D9C;
void func_800731CC(void);
void func_80071018(void);
void func_8005E900(void);
void func_8005AF78(void) {
    if (D_801E42E4 == 0xC) {
        func_800731CC();
    } else {
        func_80071018();
        if (D_801E6D9C != 0) {
            func_8005E900();
        }
    }
}
