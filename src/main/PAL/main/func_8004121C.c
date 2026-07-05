#include "common.h"

extern u32 *D_801E4B2C;
extern u32 D_801E4BBC;

void func_8004121C(u32 *arg0) {
    u32 value;

    value = *arg0;
    D_801E4B2C = arg0 + 1;
    D_801E4BBC = value;
}
