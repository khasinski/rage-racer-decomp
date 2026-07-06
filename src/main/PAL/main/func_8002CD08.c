#include "common.h"
#include "game/track.h"
#include "game/car.h"

extern GameTrackPoint *D_8009E688;

s32 func_8002CD08(GameCarTrackAngleWindow *arg0) {
    register s32 index asm("$3") = arg0->trackPointIndex;
    register s32 scaled asm("$2");
    register GameTrackPoint *table asm("$3");
    register s32 complement asm("$3");
    register s32 target asm("$5");
    s32 diff;

    asm volatile("" : "=r"(index) : "0"(index));
    scaled = index * 3;
    table = D_8009E688;
    target = ((GameTrackPoint *)((scaled << 3) + (s32)table))->angle;
    complement = 0xC00;
    complement -= target;
    diff = arg0->headingAngle - complement;
    diff &= 0xFFF;
    return (u32)(diff - 0x401) < 0x7FFU;
}
