#include "common.h"
#include "game/prim.h"
#include "game/asset.h"
#include "game/asset_internal.h"
#include "game/audio.h"
#include "game/car.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/render.h"
#include "game/render_internal.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "game/track.h"
#include "psyq/gpu.h"

/* g_GameModeHandlers[6]: moves the screen offset and commits it to g_ScreenOffsetX/Y. */
void UpdateScreenAdjustScreen(void) {
    s32 oldX;
    s32 oldY;
    u16 input;
    u16 confirm;
    u32 confirmMask;
    s32 value;

    DrawScreenAdjustScreen();

    input = g_PadPressedRepeat;
    oldX = g_ScreenOffsetEditX;
    oldY = g_ScreenOffsetEditY;

    if ((input & 0x1000) && (oldY >= 1)) {
        g_ScreenOffsetEditY = oldY - 1;
    }

    if (g_PadPressedRepeat & PAD_DOWN) {
        value = g_ScreenOffsetEditY;
        if (value < 32) {
            g_ScreenOffsetEditY = value + 1;
        }
    }

    if (g_PadPressedRepeat & PAD_LEFT) {
        value = g_ScreenOffsetEditX;
        if (value >= (-11)) {
            g_ScreenOffsetEditX = value - 1;
        }
    }

    if (g_PadPressedRepeat & PAD_RIGHT) {
        value = g_ScreenOffsetEditX;
        if (value < 32) {
            g_ScreenOffsetEditX = value + 1;
        }
    }

    if ((oldX != g_ScreenOffsetEditX) || (oldY != g_ScreenOffsetEditY)) {
        PlaySoundCue(1);
    }

    confirm = g_PadPressed;
    if (confirm & 0x860) {
        PlaySoundCue(2);
        g_GameMode = 1;
        g_ScreenOffsetX.value = g_ScreenOffsetEditX;
        g_ScreenOffsetY.value = g_ScreenOffsetEditY;
    } else {
        confirmMask = confirm & 0x90;
        if (confirmMask != 0) {
            PlaySoundCue(3);
            g_GameMode = 1;
            g_ScreenOffsetEditX = g_ScreenOffsetX.value;
            g_ScreenOffsetEditY = g_ScreenOffsetY.value;
        }
    }

    g_DispEnv0ScreenX = g_ScreenOffsetEditX;
    g_DispEnv0ScreenY = g_ScreenOffsetEditY + 0;
    g_DispEnv1ScreenX = g_ScreenOffsetEditX;
    g_DispEnv1ScreenY = g_ScreenOffsetEditY + 0;
}
