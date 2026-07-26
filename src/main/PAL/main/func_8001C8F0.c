#include "common.h"
#include "game/state.h"
#include "game/render.h"

extern s16 D_8007C798[];

s32 func_8001C8F0(s32 arg0) {
    s32 value;
    s32 ret;

    if (g_SceneId == 10) {
        s32 offset;
        s32 counter;

        offset = arg0 << 1;
        counter = g_SceneTimer;
        value = (counter << 2) - *(s16 *)((u8 *)D_8007C798 + offset);
    } else {
        value = g_FadeLevel;
        if (value > 0) {
            value--;
            g_FadeLevel = value;
        }
        value = g_FadeLevel;
    }

    if (value >= 0) {
        ret = value;
        if (ret < 0x80) {
            return ret;
        }
        ret = 0x7F;
    } else {
        ret = 0;
    }
    return ret;
}
