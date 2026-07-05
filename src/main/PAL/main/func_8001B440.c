#include "common.h"

extern s32 D_8009F098;
extern s32 D_801E6F1C;

void func_8001B2D4(void);

void func_8001B440(void) {
    if (++D_801E6F1C == 0x30) {
        D_8009F098 = 2;
    }

    func_8001B2D4();
}
