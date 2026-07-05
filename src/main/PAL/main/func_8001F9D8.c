#include "common.h"

extern s16 D_801E4DAC;
extern s32 D_801E4BB0;
extern s32 D_8019CB6C;
extern s32 D_8009EC8C;
extern u8 D_8009E6D4;
extern u8 D_801F1854;

void func_8001F134(s32 arg0, u8 *arg1, u8 *arg2);
void func_8001F274(s32 arg0, u8 *arg1);

void func_8001F9D8(void) {
    if (D_801E4DAC != 0) {
        func_8001F134(D_801E4BB0, &D_8009E6D4, &D_801F1854);
    } else {
        func_8001F274(D_801E4BB0, &D_8009E6D4);
    }

    D_801E4BB0++;
    if (D_801E4BB0 == D_8019CB6C) {
        D_801E4BB0 = 0;
        D_8009EC8C = 1;
    }
}
