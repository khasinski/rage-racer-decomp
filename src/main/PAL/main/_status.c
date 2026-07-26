#include "common.h"

extern u32 *D_800942BC;

u32 _status(void) asm("func_80066D6C");
u32 _status(void) {
    return *D_800942BC;
}
