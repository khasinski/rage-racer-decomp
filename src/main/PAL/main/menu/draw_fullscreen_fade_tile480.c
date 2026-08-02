#include "common.h"
#include "game/state.h"
#include "game/render.h"

s32 func_8001720C(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(u8 *arg0, s32 arg1, s32 arg2);
extern u32 g_OptionMenuExitScene asm("D_8019C7B0");
/* The 0x140x0x1E0 twin of GameDrawFullscreenFadeTile (func_80033AA0), for the 480-line setup scene. */
void GameDrawFullscreenFadeTile480(s32 color, s32 arg1) asm("func_80023A60");
void GameDrawFullscreenFadeTile480(s32 arg0, s32 arg1);
void func_80023BB4(void);

void GameDrawFullscreenFadeTile480(s32 color, s32 arg1) {
    u8 *base;
    s32 *scratch;
    s32 next;
    s32 width;
    s32 height;
    s32 scratchValue;

    base = g_DrawBuffer + 0xCC;
    if (color < 0) {
        color = 0;
    } else if (color >= 0x100) {
        color = 0xFF;
    }

    width = 0x140;
    height = 0x1E0;
    scratch = (s32 *)0x1F800000;
    scratchValue = *scratch;
    next = func_8001720C(base, scratchValue, 0, 0, width, height, color, color, color);
    *scratch = func_80017390(base, next, arg1);
}

/* Arms the fade-out that leaves the setup menu for scene `scene`. */
void GameStartOptionMenuExit(u32 arg0) asm("func_80023B08");
void GameStartOptionMenuExit(u32 arg0) {
    g_OptionMenuExitScene = arg0;
    g_GameMode = 0;
    g_FadeStep = 8;
}

/* g_GameModeHandlers[0]: integrates the fade, then enters mode 1 or leaves for g_OptionMenuExitScene. */
void GameUpdateOptionMenuFade(void) asm("func_80023B2C");
void GameUpdateOptionMenuFade(void) {
    g_FadeLevel += g_FadeStep;

    if (g_FadeLevel < 0) {
        g_FadeStep = 0;
        g_GameMode = 1;
    } else if (g_FadeLevel >= 0x101) {
        g_SceneId = g_OptionMenuExitScene;
    }

    GameDrawFullscreenFadeTile480(g_FadeLevel, 0x49);
    func_80023BB4();
}
