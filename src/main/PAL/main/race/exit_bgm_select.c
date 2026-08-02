#include "common.h"
#include "game/asset.h"
#include "game/render.h"
#include "game/state.h"

extern char g_TextNowLoading[] asm("D_80011010");

void DrawFullscreenFadeTile(s32 arg0, s32 arg1) asm("func_80033AA0");
void func_80016EA0(s32 arg0, s32 arg1, char *arg2, s32 arg3);

void ExitBgmSelect(void) asm("func_80026484");
void ExitBgmSelect(void) {
    if (g_AssetLoadState == 0) {
        g_FadeStep = 4;
    }

    if (g_FadeStep < 0) {
        g_FadeLevel += g_FadeStep;
        if (g_FadeLevel < 0) {
            g_FadeLevel = 0;
            g_FadeStep = 0;
        }
        DrawFullscreenFadeTile(g_FadeLevel, 0x49);
    } else if (g_FadeStep > 0) {
        g_FadeLevel += g_FadeStep;
        DrawFullscreenFadeTile(g_FadeLevel, 0x49);
        if (g_FadeLevel >= 0x101) {
            g_SceneId = 0x16;
        }
    }

    func_80016EA0(0x5E, 0x72, g_TextNowLoading, 0x7812);
}
