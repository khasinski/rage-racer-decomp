#include "common.h"

extern s16 D_801E4DAC;
extern u32 D_801E4BB0;
extern u32 D_8019CB6C;
extern u32 D_8009EC8C;

void func_8001F100(void) {
    u32 value;

    value = D_801E4DAC;
    D_801E4BB0 = 0;
    if (value != 0) {
        value = 0x5DC;
    } else {
        value = 0xA0A;
    }
    D_8019CB6C = value;
    D_8009EC8C = 0;
}
