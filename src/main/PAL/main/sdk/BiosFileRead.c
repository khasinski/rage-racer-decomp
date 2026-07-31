#include <sys/types.h>

#include "common.h"

u_long BiosFileRead[4] asm("func_80063250") __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090034,
    0,
};
