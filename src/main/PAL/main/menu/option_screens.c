#include "common.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/asset.h"
#include "game/render.h"
#include "game/track.h"
#include "psyq/gpu.h"
#include "game/audio.h"
#include "game/car.h"
void SeekEnvironmentScript(s32 arg0);

extern s32 g_SoundOptionCursor;
extern s32 g_BgmVolumeSetting;
extern s32 g_SfxVolumeSetting;
extern s32 g_MonoOutput;
extern s32 g_ScreenOffsetEditX;
void DrawSoundOptionScreen(void);
s32 QueueSpriteTransWide(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) asm("GameQueueSpriteTrans");
void DrawOptionHintBar(s32 arg0);
extern s32 g_ScreenOffsetEditY;
extern s32 g_ScreenOffsetX;
extern s32 g_ScreenOffsetY;
extern u16 D_8019CE9C;
extern u16 D_8019CE9E;
extern u16 D_801C0684;
extern u16 D_801C0686;
/* The four-arrow reticle of the screen-position panel. */
void DrawScreenAdjustScreen(void);
void DrawScreenAdjustScreen(void);
extern s32 g_OptionLetterboxHeight;
void DrawPadTypeHint(void);
s32 AddTilePrim(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 QueueLineWide(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) asm("GameQueueLine");
/* The setup scene's per-frame overlay: pad hint, letterbox, and the alignment frame in mode 6. */
void DrawOptionSceneOverlay(void);
void DrawOptionSceneOverlay(void);
extern s32 g_CameraViewMode;
extern s32 g_TrackWalkStart;
extern s32 g_FrameSyncThreshold;
extern u8 g_CarTrackSection[];
extern s32 *g_CamRow;
void InitRenderState(s32 arg0);
void LoadTrackTexturePageRange(void);
void InitTrackLighting(void);
void BuildStartingGrid(void);
void SetTrackTexturePageNow(s32 arg0);
extern s32 g_FrameSyncThreshold;
extern s32 g_BgmSelectCursor;
extern s32 g_BgmSelectShowUi;
extern s32 g_BgmSelectCdTrack;
extern s32 g_BgmSelectStep;
extern s32 g_BgmSelectTrack;
extern s32 g_BgmChangeDelay;
extern s32 g_CdTrackEnded;
extern s32 g_BgmSelectStep;
extern u8 g_TextNowLoading[];
void DrawFullscreenFadeTile(s32 arg0, s32 arg1);
void InitTrackScene(void);
void RequestTrackDataAssets(void);
void UpdateOptionSceneFade(void);

/* g_GameModeHandlers[5]: left/right edits the selected audio setting, cancel restores it. */
void UpdateSoundSettingAdjust(void) {
    s32 old;
    u16 pad;

    DrawSoundOptionScreen();

    switch (g_SoundOptionCursor) {
    case 0:
        old = g_BgmVolumeSetting;
        if ((g_PadEdge2 & 0x8000) && old > 0) {
            g_BgmVolumeSetting = old - 1;
        }
        if (g_PadEdge2 & 0x2000) {
            s32 value = g_BgmVolumeSetting;
            if (value < 15) {
                g_BgmVolumeSetting = value + 1;
            }
        }
        if (old != g_BgmVolumeSetting) {
            PlaySoundCue(1);
        }
        pad = g_PadEdge2;
        if (pad & 0x860) {
            g_GameMode = 4;
        } else if (pad & 0x90) {
            g_GameMode = 4;
            g_BgmVolumeSetting = g_ScreenOffsetEditX;
        }
        break;

    case 1:
        old = g_SfxVolumeSetting;
        if ((g_PadEdge2 & 0x8000) && old > 0) {
            g_SfxVolumeSetting = old - 1;
        }
        if (g_PadEdge2 & 0x2000) {
            s32 value = g_SfxVolumeSetting;
            if (value < 15) {
                g_SfxVolumeSetting = value + 1;
            }
        }
        if (old != g_SfxVolumeSetting) {
            PlaySoundCue(1);
        }
        pad = g_PadEdge2;
        if (pad & 0x860) {
            g_GameMode = 4;
        } else if (pad & 0x90) {
            g_GameMode = 4;
            g_SfxVolumeSetting = g_ScreenOffsetEditX;
        }
        break;

    case 2:
        old = g_MonoOutput;
        if ((g_PadEdge2 & 0x8000) && old > 0) {
            g_MonoOutput = old - 1;
        }
        if (g_PadEdge2 & 0x2000) {
            s32 value = g_MonoOutput;
            if (value <= 0) {
                g_MonoOutput = value + 1;
            }
        }
        if (old != g_MonoOutput) {
            PlaySoundCue(1);
        }
        pad = g_PadEdge2;
        if (pad & 0x860) {
            g_GameMode = 4;
        } else if (pad & 0x90) {
            g_GameMode = 4;
            g_MonoOutput = g_ScreenOffsetEditX;
        }
        break;
    }

    ApplyAudioSettings();
    pad = g_PadEdge2;
    if (pad & 0x860) {
        PlaySoundCue(2);
    } else if (pad & 0x90) {
        PlaySoundCue(3);
    }
}

void DrawScreenAdjustScreen(void) {
    s32 base = (s32)g_DrawBuffer;
    s32 color = 0x7F40;
    s32 y48 = 0x48;
    s32 h18 = 0x18;
    s32 w0c = 0xC;
    s32 *scratch = (s32 *)0x1F800000;
    s32 next;

    base += 0xCC;
    next = *scratch;
    next = QueueSpriteTransWide(base, next, 0x9A, 0x88, w0c, h18, 0xC8, y48, color);
    next = QueueSpriteTransWide(base, next, 0x9A, 0xB8, w0c, h18, 0xD4, y48, color);
    next = QueueSpriteTransWide(base, next, 0xA6, 0xA0, w0c, h18, 0xE0, y48, color);
    *scratch = QueueSpriteTransWide(base, next, 0x8E, 0xA0, w0c, h18, 0xEC, y48, color);
    DrawOptionHintBar(3);
}

/* g_GameModeHandlers[6]: moves the screen offset and commits it to g_ScreenOffsetX/Y. */
void UpdateScreenAdjustScreen(void) {
    s32 oldX;
    s32 oldY;
    u16 input;
    u16 confirm;
    u32 confirmMask;
    s32 value;

    DrawScreenAdjustScreen();

    input = g_PadEdge;
    oldX = g_ScreenOffsetEditX;
    oldY = g_ScreenOffsetEditY;

    if ((input & 0x1000) && (oldY >= -31)) {
        g_ScreenOffsetEditY = oldY - 1;
    }

    if (g_PadEdge & 0x4000) {
        value = g_ScreenOffsetEditY;
        if (value < 23) {
            g_ScreenOffsetEditY = value + 1;
        }
    }

    if (g_PadEdge & 0x8000) {
        value = g_ScreenOffsetEditX;
        if (value >= -10) {
            g_ScreenOffsetEditX = value - 1;
        }
    }

    if (g_PadEdge & 0x2000) {
        value = g_ScreenOffsetEditX;
        if (value < 32) {
            g_ScreenOffsetEditX = value + 1;
        }
    }

    if ((oldX != g_ScreenOffsetEditX) || (oldY != g_ScreenOffsetEditY)) {
        PlaySoundCue(1);
    }

    confirm = g_PadEdge2;
    if (confirm & 0x860) {
        PlaySoundCue(2);
        g_GameMode = 1;
        g_ScreenOffsetX = g_ScreenOffsetEditX;
        g_ScreenOffsetY = g_ScreenOffsetEditY;
    } else {
        confirmMask = confirm & 0x90;
        if (confirmMask != 0) {
            PlaySoundCue(3);
            g_GameMode = 1;
            g_ScreenOffsetEditX = g_ScreenOffsetX;
            g_ScreenOffsetEditY = g_ScreenOffsetY;
        }
    }

    D_8019CE9C = g_ScreenOffsetEditX;
    D_8019CE9E = g_ScreenOffsetEditY + 29;
    D_801C0684 = g_ScreenOffsetEditX;
    D_801C0686 = g_ScreenOffsetEditY + 29;
}

void DrawOptionSceneOverlay(void) {
    s32 *scratch;
    void *base;
    s32 pkt;
    s32 target;
    s32 value;
    s32 w120;
    s32 two;
    s32 white;
    s32 h1c0;
    u8 *rawBase;

    if (g_GameMode != 9) {
        DrawPadTypeHint();
    }

    target = 0xF0;
    if (g_GameMode == 6) {
        target = 0x1E0;
    }

    value = g_OptionLetterboxHeight;
    if (value < target) {
        g_OptionLetterboxHeight = value + 4;
    } else if (target < value) {
        g_OptionLetterboxHeight = value - 4;
    }

    scratch = (s32 *)0x1F800000;
    rawBase = g_DrawBuffer;
    base = rawBase + 0xBC8;
    pkt = *scratch;

    if (g_GameMode == 6) {
        w120 = 0x120;
        two = 2;
        white = 0xFF;
        pkt = AddTilePrim(base, pkt, 0x10, 0x20, w120, two, white, white, white);
        pkt = AddTilePrim(base, pkt, 0x10, 0x1C0, w120, two, white, white, white);
        h1c0 = 0x1C0;
        pkt = QueueLineWide(base, pkt, 0x10, 0x20, 0x10, h1c0, white, white, white);
        pkt = QueueLineWide(base, pkt, 0x130, 0x20, 0x130, h1c0, white, white, white);
    }

    *scratch = AddTilePrim(base, pkt, 0, 0, 0x140, g_OptionLetterboxHeight, 0x85, 0x15, 0xE);
}

/* Scene 23: the setup / OPTION scene, dispatching g_GameModeHandlers[g_GameMode]. */
void UpdateOptionScene(void) {
    *(s32 *)0x1F800000 = AddTilePrim(g_DrawBuffer + 204, *(s32 *)0x1F800000, 0, 0, 0x140, 2, 0, 0, 0);
    g_AnimTimer = g_AnimTimer + 1;
    g_SceneTimer = g_SceneTimer + 1;
    if (g_SceneTimer == 2) {
        SetDispMask(1);
    }
    g_GameModeHandlers[g_GameMode]();
    DrawOptionSceneOverlay();
}

void InitTrackScene(void) {
    InitRenderState(5);
    LoadTrackTexturePageRange();
    InitTrackLighting();
    g_TrackWalkStart = *(s32 *)g_TrackEventData;
    BuildStartingGrid();
    SetTrackTexturePageNow(*(s16 *)&g_CarTrackSection[g_CameraCarIndex * 412]);
    SeekEnvironmentScript(g_CamRow[2]);
    g_CameraViewMode = 2;
    g_AnimTimer = 0;
    g_SceneTimer = 0;
    g_FrameSyncThreshold = 0x180;
    InitShuttleScenery();
}

void EnterBgmSelectScreen(void) {
    SetDispMask(0);
    SetupDisplay240(0, 0, 0);
    g_FrameSyncThreshold = 0x80;
    g_FadeLevel = 0x13C;
    g_FadeStep = -4;
    g_SceneId = 0x1C;
    g_BgmSelectCursor = 1;
    g_BgmSelectShowUi = 1;
    g_BgmSelectCdTrack = 3;
    g_BgmSelectStep = 0;
    g_SceneTimer = 0;
    g_BgmSelectTrack = 0;
    g_BgmChangeDelay = 0x1E;
    g_CdTrackEnded = 0;
    g_CameraCarIndex = 0;
}

void UpdateOptionSceneFade(void) {
    s32 d;
    s32 v;
    if (g_SceneTimer == 0xF) {
        SetDispMask(1);
    }
    d = g_FadeStep;
    if (d < 0) {
        s32 e = g_FadeLevel;
        e += d;
        g_FadeLevel = e;
        if (g_FadeLevel < 0) {
            g_FadeLevel = 0;
            g_FadeStep = 0;
        }
        DrawFullscreenFadeTile(g_FadeLevel, 0x49);
    } else if (d > 0) {
        s32 e = g_FadeLevel;
        e += d;
        v = e;
        g_FadeLevel = v;
        DrawFullscreenFadeTile(v, 0x49);
        if (g_FadeLevel >= 257) {
            SetDispMask(0);
            InitTrackScene();
            g_FadeStep = 0;
            g_FadeLevel = 0;
            g_BgmSelectStep = 2;
        }
    }
    DrawProportionalText(0x5E, 0x72, g_TextNowLoading, 0x7812);
}

void UpdateBgmSelectLoad(void) {
    if (g_AssetLoadState == 0) {
        InstallCourseAssets();
        RequestTrackDataAssets();
        g_BgmSelectStep = 1;
    }

    UpdateOptionSceneFade();
}

void UpdateBgmSelectFadeIn(void) {
    if (g_AssetLoadState == 0) {
        g_FadeStep = 4;
    }
    UpdateOptionSceneFade();
}
