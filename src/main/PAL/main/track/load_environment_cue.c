#include "common.h"
#include "game/render.h"

extern s16 g_EnvFogEnabled;
extern u8 g_EnvSpare;
extern s16 g_EnvLerpDuration;
extern s16 g_EnvironmentMode;
/* Cue halfword +0x2E, bit 15 inverted. Its only effect anywhere is to enable
 * the g_EnvSpareFrom -> g_EnvSpareTo lerp of g_EnvSpare; the name states that
 * and nothing more, because no reader of g_EnvSpare survives in the image. */
extern s16 g_EnvSpareLerp;
extern s16 g_EnvSpareFrom;
extern s16 g_EnvSpareTo;
extern s32 g_IsEnvironmentMode4;
extern s32 g_EnvironmentModePrev;

void LoadEnvironmentCue(GameEnvColor *arg0) {
    GameEnvColor *src;
    s32 mode;
    s32 newMode;
    s32 compareMode;
    s32 signedMode;
    s32 field28;
    u32 flag;

    src = arg0;
    g_EnvFogEnabled = 1;

    g_EnvColors[0].to = src[1];
    g_EnvColors[0].from = g_EnvColors[0].cur;
    g_EnvColors[1].to = src[2];
    g_EnvColors[1].from = g_EnvColors[1].cur;
    g_EnvColors[2].to = src[3];
    g_EnvColors[2].from = g_EnvColors[2].cur;
    g_EnvColors[3].to = src[4];
    g_EnvColors[3].from = g_EnvColors[3].cur;
    g_EnvColors[4].to = src[5];
    g_EnvColors[4].from = g_EnvColors[4].cur;
    g_EnvColors[5].to = src[6];
    g_EnvColors[5].from = g_EnvColors[5].cur;
    g_EnvColors[6].to = src[7];
    g_EnvColors[6].from = g_EnvColors[6].cur;
    g_EnvColors[7].to = src[8];
    g_EnvColors[7].from = g_EnvColors[7].cur;
    g_EnvColors[8].to = src[9];
    g_EnvColors[8].from = g_EnvColors[8].cur;

    field28 = *(u16 *)((u8 *)src + 0x28);
    mode = g_EnvironmentMode;
    g_EnvLerpDuration = field28;
    newMode = *(u16 *)((u8 *)src + 0x2C);
    g_EnvironmentMode = newMode;
    flag = *(u16 *)((u8 *)src + 0x2E);
    g_EnvironmentModePrev = mode;
    g_EnvSpareLerp = ((flag >> 15) ^ 1);
    compareMode = 4;

    if (g_EnvSpareLerp != 0) {
        g_EnvSpareFrom = g_EnvSpare;
        g_EnvSpareTo = *(u16 *)((u8 *)src + 0x2E);
    }

    signedMode = (s16)newMode;
    if (signedMode == compareMode) {
        g_IsEnvironmentMode4 = 1;
    } else {
        g_IsEnvironmentMode4 = 0;
    }
}
