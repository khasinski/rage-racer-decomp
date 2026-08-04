#include "common.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/asset.h"
#include "game/render.h"
#include "game/track.h"
#include "psyq/gpu.h"

extern s32 g_SoundOptionCursor asm("D_8019C868");
extern s32 g_BgmVolumeSetting asm("D_8019C704");
extern s32 g_SfxVolumeSetting asm("D_801E8A50");
extern s32 g_MonoOutput asm("D_801E6C70");
extern s32 g_ScreenOffsetEditX asm("D_801E4D68");
void DrawSoundOptionScreen(void) asm("func_80024B6C");
void ApplyAudioSettings(void) asm("func_80021224");
void PlaySoundCue(s32 cue) asm("func_8005D6EC");
s32 func_80017138(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void DrawOptionHintBar(s32 arg0) asm("func_80023750");
extern s32 g_ScreenOffsetEditY asm("D_801E4D6C");
extern s32 g_ScreenOffsetX asm("D_801E4B8C");
extern s32 g_ScreenOffsetY asm("D_801E4B9C");
extern u16 D_8019CE9C;
extern u16 D_8019CE9E;
extern u16 D_801C0684;
extern u16 D_801C0686;
/* The four-arrow reticle of the screen-position panel. */
void DrawScreenAdjustScreen(void) asm("func_800253A4");
void DrawScreenAdjustScreen(void);
extern s32 g_OptionLetterboxHeight asm("D_8009F0A0");
void DrawPadTypeHint(void) asm("func_8002390C");
s32 AddTilePrim(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) asm("func_80032F34");
s32 func_800172D4(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
/* The setup scene's per-frame overlay: pad hint, letterbox, and the alignment frame in mode 6. */
void DrawOptionSceneOverlay(void) asm("func_800256B8");
void DrawOptionSceneOverlay(void);
extern s32 g_CameraViewMode asm("D_8009E870");
extern s32 g_TrackWalkStart asm("D_801E40CC");
extern s32 g_CameraCarIndex asm("D_8009E66C");
extern s32 g_FrameSyncThreshold asm("D_8019C768");
extern u8 g_CarTrackSection[] asm("D_801F18CC");
extern s32 *g_CamRow asm("D_8019C9A8");
void InitRenderState(s32 arg0) asm("func_80017884");
void LoadTrackTexturePageRange(void) asm("func_8001D30C");
void InitTrackLighting(void) asm("func_8001D210");
void BuildStartingGrid(void) asm("func_80038844");
void SetTrackTexturePageNow(s32 arg0) asm("func_80019E84");
void func_800458CC(s32 arg0);
void InitShuttleScenery(void) asm("func_8003F0F8");
extern s32 g_FrameSyncThreshold;
extern s32 g_BgmSelectCursor asm("D_801E4B84");
extern s32 g_BgmSelectShowUi asm("D_801E412C");
extern s32 g_BgmSelectCdTrack asm("D_8019CE00");
extern s32 g_BgmSelectStep asm("D_8019C99C");
extern s32 g_BgmSelectTrack asm("D_801E4404");
extern s32 g_BgmChangeDelay asm("D_8019CAF4");
extern s32 g_CdTrackEnded asm("D_8019C7BC");
extern s32 g_CameraCarIndex asm("D_8009E66C");
extern s32 g_BgmSelectStep;
extern u8 g_TextNowLoading[] asm("D_80011010");
void DrawFullscreenFadeTile(s32 arg0, s32 arg1) asm("func_80033AA0");
void InitTrackScene(void) asm("func_80025940");
void func_80016EA0(s32 a0, s32 a1, void *a2, s32 a3);
void InstallCourseAssets(void) asm("func_80019730");
void RequestTrackDataAssets(void) asm("func_80019844");
void UpdateOptionSceneFade(void) asm("func_80025AC8");

/* g_GameModeHandlers[5]: left/right edits the selected audio setting, cancel restores it. */
void UpdateSoundSettingAdjust(void) asm("func_800250BC");
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
    next = func_80017138(base, next, 0x9A, 0x88, w0c, h18, 0xC8, y48, color);
    next = func_80017138(base, next, 0x9A, 0xB8, w0c, h18, 0xD4, y48, color);
    next = func_80017138(base, next, 0xA6, 0xA0, w0c, h18, 0xE0, y48, color);
    *scratch = func_80017138(base, next, 0x8E, 0xA0, w0c, h18, 0xEC, y48, color);
    DrawOptionHintBar(3);
}

/* g_GameModeHandlers[6]: moves the screen offset and commits it to g_ScreenOffsetX/Y. */
void UpdateScreenAdjustScreen(void) asm("func_800254C8");
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
        pkt = func_800172D4(base, pkt, 0x10, 0x20, 0x10, h1c0, white, white, white);
        pkt = func_800172D4(base, pkt, 0x130, 0x20, 0x130, h1c0, white, white, white);
    }

    *scratch = AddTilePrim(base, pkt, 0, 0, 0x140, g_OptionLetterboxHeight, 0x85, 0x15, 0xE);
}

/* Scene 23: the setup / OPTION scene, dispatching g_GameModeHandlers[g_GameMode]. */
void UpdateOptionScene(void) asm("func_80025870");
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

void InitTrackScene(void) asm("func_80025940");
void InitTrackScene(void) {
    InitRenderState(5);
    LoadTrackTexturePageRange();
    InitTrackLighting();
    g_TrackWalkStart = *(s32 *)g_TrackEventData;
    BuildStartingGrid();
    SetTrackTexturePageNow(*(s16 *)&g_CarTrackSection[g_CameraCarIndex * 412]);
    func_800458CC(g_CamRow[2]);
    g_CameraViewMode = 2;
    g_AnimTimer = 0;
    g_SceneTimer = 0;
    g_FrameSyncThreshold = 0x180;
    InitShuttleScenery();
}

void EnterBgmSelectScreen(void) asm("func_80025A14");
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

void UpdateOptionSceneFade(void) asm("func_80025AC8");
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
    func_80016EA0(0x5E, 0x72, g_TextNowLoading, 0x7812);
}

void UpdateBgmSelectLoad(void) asm("func_80025BD8");
void UpdateBgmSelectLoad(void) {
    if (g_AssetLoadState == 0) {
        InstallCourseAssets();
        RequestTrackDataAssets();
        g_BgmSelectStep = 1;
    }

    UpdateOptionSceneFade();
}

void UpdateBgmSelectFadeIn(void) asm("func_80025C20");
void UpdateBgmSelectFadeIn(void) {
    if (g_AssetLoadState == 0) {
        g_FadeStep = 4;
    }
    UpdateOptionSceneFade();
}
