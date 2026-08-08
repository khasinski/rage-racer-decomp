#include "common.h"
#include "game/render.h"
#include "game/track.h"

void LoadEnvironmentCue(GameEnvironmentCue *cue) {
    s32 mode;
    s32 newMode;
    s32 compareMode;
    s32 signedMode;
    s32 field28;
    u32 flag;

    g_EnvFogEnabled = 1;

    g_EnvColors[0].to = cue->colors[0];
    g_EnvColors[0].from = g_EnvColors[0].cur;
    g_EnvColors[1].to = cue->colors[1];
    g_EnvColors[1].from = g_EnvColors[1].cur;
    g_EnvColors[2].to = cue->colors[2];
    g_EnvColors[2].from = g_EnvColors[2].cur;
    g_EnvColors[3].to = cue->colors[3];
    g_EnvColors[3].from = g_EnvColors[3].cur;
    g_EnvColors[4].to = cue->colors[4];
    g_EnvColors[4].from = g_EnvColors[4].cur;
    g_EnvColors[5].to = cue->colors[5];
    g_EnvColors[5].from = g_EnvColors[5].cur;
    g_EnvColors[6].to = cue->colors[6];
    g_EnvColors[6].from = g_EnvColors[6].cur;
    g_EnvColors[7].to = cue->colors[7];
    g_EnvColors[7].from = g_EnvColors[7].cur;
    g_EnvColors[8].to = cue->colors[8];
    g_EnvColors[8].from = g_EnvColors[8].cur;

    field28 = *(u16 *)((u8 *)cue + 0x28);
    mode = g_EnvironmentMode;
    g_EnvLerpDuration = field28;
    newMode = *(u16 *)((u8 *)cue + 0x2C);
    g_EnvironmentMode = newMode;
    flag = *(u16 *)((u8 *)cue + 0x2E);
    g_EnvironmentModePrev = mode;
    g_EnvSpareLerp = ((flag >> 15) ^ 1);
    compareMode = 4;

    if (g_EnvSpareLerp != 0) {
        g_EnvSpareFrom = g_EnvSpare;
        g_EnvSpareTo = *(u16 *)((u8 *)cue + 0x2E);
    }

    signedMode = (s16)newMode;
    if (signedMode == compareMode) {
        g_IsEnvironmentMode4 = 1;
    } else {
        g_IsEnvironmentMode4 = 0;
    }
}
