#include "common.h"

/* NTSC-J advances the race clock at 30 frames per second. */
s32 FramesToMilliseconds(s32 frames, s32 millis) {
    s32 seconds;
    s32 remainder;

    seconds = frames / 30;
    remainder = frames - (seconds * 30);
    remainder = ((remainder * 100) / 3) + millis;
    return (seconds * 1000) + remainder;
}
