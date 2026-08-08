#include <sys/types.h>

#include "common.h"

u_long DeliverEvent[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090007,
    0,
};
