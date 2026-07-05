#include "common.h"
extern s32 D_801E4D0C;
void func_8005D6EC(s32 arg0);
void func_80020D90(void) {
    if (D_801E4D0C > 0) {
        D_801E4D0C--;
    }
    if (D_801E4D0C == 0xB4) {
        func_8005D6EC(0x42);
    }
}
