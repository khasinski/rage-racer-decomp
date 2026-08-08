#include <sys/types.h>

#include "common.h"

u_long BiosFileOpen[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090032,
    0,
};

u_long BiosFileSeek[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090033,
    0,
};

u_long BiosFileRead[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090034,
    0,
};

u_long BiosFileWrite[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090035,
    0,
};

u_long BiosFileClose[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090036,
    0,
};

u_long BiosFormatDevice[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090041,
    0,
};

u_long BiosFirstFile[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090042,
    0,
};

u_long BiosNextFile[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090043,
    0,
};
