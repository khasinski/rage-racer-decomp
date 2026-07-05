#include "common.h"

extern u8 D_801E8AFC;
extern u8 *D_8009F0A4;
extern u8 *D_8019C7A4;

void func_8001F0E0(void) {
    D_8009F0A4 = &D_801E8AFC;
    D_8019C7A4 = &D_801E8AFC;
}
