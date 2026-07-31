#include <sys/types.h>

#include "common.h"

u_long BiosFormatDevice[4] asm("func_80063280") __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090041,
    0,
};
