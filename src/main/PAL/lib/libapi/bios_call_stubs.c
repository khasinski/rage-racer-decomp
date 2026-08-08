#include <sys/types.h>

u_long g_BiosCallStubs[20] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090038,
    0,
    0x240A00A0,
    0x01400008,
    0x240900AB,
    0,
    0x240A00A0,
    0x01400008,
    0x240900AC,
    0,
    0x240A00B0,
    0x01400008,
    0x2409004A,
    0,
    0x240A00B0,
    0x01400008,
    0x2409004B,
    0,
};
