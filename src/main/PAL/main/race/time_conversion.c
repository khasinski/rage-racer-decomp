#include "common.h"

s32 FramesToMilliseconds(s32 frames, s32 millis) {
    s32 seconds;
    s32 secondsMs;

    seconds = frames / 25;
    secondsMs = seconds * 1000;
    frames -= seconds * 25;
    frames = (frames * 40) + millis;
    return secondsMs + frames;
}
