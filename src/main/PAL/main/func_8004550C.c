#include "common.h"

extern u32 D_801E4D84;
extern u32 *D_801E42F4;
extern u32 D_8019C774;

void func_8004550C(u32 *arg0) {
    u32 value0;
    u32 value1;

    value0 = *arg0;
    arg0++;
    D_801E4D84 = value0;
    value1 = *arg0;
    arg0++;
    D_801E42F4 = arg0;
    D_8019C774 = value1;
}
