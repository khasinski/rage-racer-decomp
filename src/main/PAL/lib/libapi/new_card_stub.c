#include <sys/types.h>

#include "common.h"

u_long _new_card[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090050,
    0,
};
