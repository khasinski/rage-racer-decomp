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


/* g_GameModeHandlers[5]: left/right edits the selected audio setting, cancel restores it. */
void UpdateSoundSettingAdjust(void) {
    s32 old;
    u16 pad;

    DrawSoundOptionScreen();

    switch (g_SoundOptionCursor) {
    case 0:
        old = g_BgmVolumeSetting;
        if ((g_PadPressed & PAD_LEFT) && old > 0) {
            g_BgmVolumeSetting = old - 1;
        }
        if (g_PadPressed & PAD_RIGHT) {
            s32 value = g_BgmVolumeSetting;
            if (value < 15) {
                g_BgmVolumeSetting = value + 1;
            }
        }
        if (old != g_BgmVolumeSetting) {
            PlaySoundCue(1);
        }
        pad = g_PadPressed;
        if (pad & 0x860) {
            g_GameMode = 4;
        } else if (pad & 0x90) {
            g_GameMode = 4;
            g_BgmVolumeSetting = g_ScreenOffsetEditX;
        }
        break;

    case 1:
        old = g_SfxVolumeSetting;
        if ((g_PadPressed & PAD_LEFT) && old > 0) {
            g_SfxVolumeSetting = old - 1;
        }
        if (g_PadPressed & PAD_RIGHT) {
            s32 value = g_SfxVolumeSetting;
            if (value < 15) {
                g_SfxVolumeSetting = value + 1;
            }
        }
        if (old != g_SfxVolumeSetting) {
            PlaySoundCue(1);
        }
        pad = g_PadPressed;
        if (pad & 0x860) {
            g_GameMode = 4;
        } else if (pad & 0x90) {
            g_GameMode = 4;
            g_SfxVolumeSetting = g_ScreenOffsetEditX;
        }
        break;

    case 2:
        old = g_MonoOutput;
        if ((g_PadPressed & PAD_LEFT) && old > 0) {
            g_MonoOutput = old - 1;
        }
        if (g_PadPressed & PAD_RIGHT) {
            s32 value = g_MonoOutput;
            if (value <= 0) {
                g_MonoOutput = value + 1;
            }
        }
        if (old != g_MonoOutput) {
            PlaySoundCue(1);
        }
        pad = g_PadPressed;
        if (pad & 0x860) {
            g_GameMode = 4;
        } else if (pad & 0x90) {
            g_GameMode = 4;
            g_MonoOutput = g_ScreenOffsetEditX;
        }
        break;
    }

    ApplyAudioSettings();
    pad = g_PadPressed;
    if (pad & 0x860) {
        PlaySoundCue(2);
    } else if (pad & 0x90) {
        PlaySoundCue(3);
    }
}

void DrawScreenAdjustScreen(void) {
    u8 *base = g_DrawBuffer;
    s32 color = 0x7F40;
    s32 y48 = 0x48;
    s32 h18 = 0x18;
    s32 w0c = 0xC;
    u8 **scratch = &SCRATCH_PRIM_CURSOR_AS(u8);
    u8 *next;

    base += 0xCC;
    next = *scratch;
    next = GameQueueSpriteTrans(base, next, 0x9A, 0x88, w0c, h18, 0xC8, y48, color);
    next = GameQueueSpriteTrans(base, next, 0x9A, 0xB8, w0c, h18, 0xD4, y48, color);
    next = GameQueueSpriteTrans(base, next, 0xA6, 0xA0, w0c, h18, 0xE0, y48, color);
    *scratch = GameQueueSpriteTrans(base, next, 0x8E, 0xA0, w0c, h18, 0xEC, y48, color);
    DrawOptionHintBar(3);
}
