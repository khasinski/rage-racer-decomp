#include "common.h"
#include "game/state.h"
#include "game/render.h"

s32 GameQueueTileTransWide(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) asm("GameQueueTileTrans");
s32 GameQueueDrawModePrimWide(u8 *arg0, s32 arg1, s32 arg2) asm("QueueDrawModePrim");
extern u32 g_OptionMenuExitScene;
/* The 0x140x0x1E0 twin of DrawFullscreenFadeTile, for the 480-line setup scene. */
void DrawFullscreenFadeTile480(s32 color, s32 arg1);
void DrawFullscreenFadeTile480(s32 arg0, s32 arg1);
void DrawOptionRootMenu(void);

void DrawFullscreenFadeTile480(s32 color, s32 arg1) {
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
    next = GameQueueTileTransWide(base, scratchValue, 0, 0, width, height, color, color, color);
    *scratch = GameQueueDrawModePrimWide(base, next, arg1);
}

/* Arms the fade-out that leaves the setup menu for scene `scene`. */
void StartOptionMenuExit(u32 arg0) {
    g_OptionMenuExitScene = arg0;
    g_GameMode = 0;
    g_FadeStep = 8;
}

/* g_GameModeHandlers[0]: integrates the fade, then enters mode 1 or leaves for g_OptionMenuExitScene. */
void UpdateOptionMenuFade(void) {
    g_FadeLevel += g_FadeStep;

    if (g_FadeLevel < 0) {
        g_FadeStep = 0;
        g_GameMode = 1;
    } else if (g_FadeLevel >= 0x101) {
        g_SceneId = g_OptionMenuExitScene;
    }

    DrawFullscreenFadeTile480(g_FadeLevel, 0x49);
    DrawOptionRootMenu();
}
