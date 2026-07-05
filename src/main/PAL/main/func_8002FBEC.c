#include "common.h"
#include "game/track.h"

extern s32 D_8009E6A8;
extern GameTrackPoint *D_8009E688;
extern s32 D_8009E70C;

s32 func_8002FAE8(s32 arg0, s32 arg1, s32 arg2);

s32 func_8002FBEC(s32 arg0) {
    s32 next = (arg0 + 1) % D_8009E6A8;

    return 0x800 - func_8002FAE8(D_8009E688[arg0].angle, D_8009E688[next].angle, D_8009E70C);
}
