#include "common.h"
#include "game/asset.h"
#include "game/render.h"
#include "game/state.h"

extern char D_80011010[];

void func_80033AA0(s32 arg0, s32 arg1);
void func_80016EA0(s32 arg0, s32 arg1, char *arg2, s32 arg3);

void func_80026484(void) {
    s32 delta;

    if (g_AssetLoadState == 0) {
        g_FadeStep = 4;
    }

    delta = g_FadeStep;
    if (delta < 0) {
        g_FadeLevel += delta;
        if (g_FadeLevel < 0) {
            g_FadeLevel = 0;
            g_FadeStep = 0;
        }
        func_80033AA0(g_FadeLevel, 0x49);
    } else if (delta > 0) {
        g_FadeLevel += delta;
        func_80033AA0(g_FadeLevel, 0x49);
        if (g_FadeLevel >= 0x101) {
            g_SceneId = 0x16;
        }
    }

    func_80016EA0(0x5E, 0x72, D_80011010, 0x7812);
}
