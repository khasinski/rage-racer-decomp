#include "common.h"
#include "game/render.h"
#include "game/state.h"

/* Scene 34: the still shown after the ending FMV. Fades in, waits 300
 * frames or a confirm press, fades out and returns to scene 2. */
void UpdateEndingStill(void) {
    s32 v0, v1;
    if ((g_SceneTimer = g_SceneTimer + 1) == 2) {
        SetDispMask(1);
    }
    v0 = g_FadeLevel + g_FadeStep;
    g_FadeLevel = v0;
    v1 = g_FadeStep;
    if (v1 > 0) {
        if (v0 >= 257) {
            g_FadeLevel = 0x100;
            g_FadeStep = 0;
        }
    } else if (v1 == 0) {
        if (g_SceneTimer == 0x12C || (g_PadPressed & PAD_CONFIRM)) {
            g_FadeLevel = 0x100;
            g_FadeStep = -4;
        }
    } else if (v0 == 0) {
        g_SceneId = 2;
    }
    DrawRaceEndBanner(g_FadeLevel);
}
