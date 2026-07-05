#include "common.h"

typedef struct {
    u8 pad0[0x30];
    s32 index;
    u8 pad34[0x6C];
    s32 angle;
} UnkVehicle8002CD08;

extern u8 *D_8009E688;

s32 func_8002CD08(UnkVehicle8002CD08 *arg0) {
    register s32 index asm("$3") = arg0->index;
    register s32 scaled asm("$2");
    register u8 *table asm("$3");
    register s32 complement asm("$3");
    register s32 target asm("$5");
    s32 diff;

    asm volatile("" : "=r"(index) : "0"(index));
    scaled = index * 3;
    table = D_8009E688;
    target = *(s16 *)(((scaled << 3) + (s32)table) + 0xA);
    complement = 0xC00;
    complement -= target;
    diff = arg0->angle - complement;
    diff &= 0xFFF;
    return (u32)(diff - 0x401) < 0x7FFU;
}
