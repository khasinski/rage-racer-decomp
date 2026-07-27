#include "common.h"

/*
 * PAL executable entry and PsyQ BIOS jump table.
 *
 * This block sets up scratch memory, $gp, $sp, and $fp before jumping into the
 * game init routine. The following labels are callable BIOS A0/B0 stubs whose
 * exact register protocol cannot be expressed as normal C.
 */
u32 D_800630B4[47] __attribute__((section(".text"))) = {
    0x3C02800A,
    0x2442AED8,
    0x3C03801F,
    0x24632A10,
    0xAC400000,
    0x24420004,
    0x0043082B,
    0x1420FFFC,
    0x00000000,
    0x24020004,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x3C048006,
    0x24843160,
    0x00822021,
    0x8C820000,
    0x3C088000,
    0x0048E825,
    0x3C04801F,
    0x24842A10,
    0x000420C0,
    0x000420C2,
    0x3C03800A,
    0x8C63A520,
    0x00000000,
    0x00432823,
    0x00A42823,
    0x00882025,
    0x3C01800A,
    0xAC3FAED8,
    0x3C1C800A,
    0x279CAED8,
    0x03A0F021,
    0x0C018C5C,
    0x20840004,
    0x3C1F800A,
    0x8FFFAED8,
    0x00000000,
    0x0C005944,
    0x00000000,
    0x0000004D,
    0x00200000,
    0x00200000,
    0x00200000,
    0x00200000,
};

u32 func_80063170[4] __attribute__((section(".text"))) = {
    0x240A00A0,
    0x01400008,
    0x24090039,
    0x00000000,
};

u32 func_80063180[8] __attribute__((section(".text"))) = {
    0x240A00A0,
    0x01400008,
    0x24090070,
    0x00000000,
    0x240A00A0,
    0x01400008,
    0x2409009F,
    0x00000000,
};

u32 func_800631A0[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090008,
    0x00000000,
};

u32 func_800631B0[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090009,
    0x00000000,
};

u32 func_800631C0[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x2409000B,
    0x00000000,
};

u32 func_800631D0[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x2409000C,
    0x00000000,
};

u32 func_800631E0[12] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x2409000D,
    0x00000000,
    0x240A00B0,
    0x01400008,
    0x24090012,
    0x00000000,
    0x240A00B0,
    0x01400008,
    0x24090013,
    0x00000000,
};

u32 func_80063210[4] __attribute__((section(".text"))) = {
    0x24040001,
    0x0000000C,
    0x03E00008,
    0x00000000,
};
