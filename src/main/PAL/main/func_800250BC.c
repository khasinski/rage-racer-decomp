#include "common.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/asset.h"
#include "game/render.h"
#include "game/track.h"
#include "psyq/gpu.h"

extern s32 D_8019C868;
extern s32 g_BgmVolumeSetting asm("D_8019C704");
extern s32 g_SfxVolumeSetting asm("D_801E8A50");
extern s32 g_MonoOutput asm("D_801E6C70");
extern s32 D_801E4D68;
void func_80024B6C(void);
void func_80021224(void);
void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");
s32 func_80017138(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_80023750(s32 arg0);
extern s32 D_801E4D6C;
extern s32 D_801E4B8C;
extern s32 D_801E4B9C;
extern u16 D_8019CE9C;
extern u16 D_8019CE9E;
extern u16 D_801C0684;
extern u16 D_801C0686;
void func_800253A4(void);
extern s32 D_8009F0A0;
void func_8002390C(void);
s32 GameAddTilePrim(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) asm("func_80032F34");
s32 func_800172D4(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void func_800256B8(void);
extern s32 g_CameraViewMode asm("D_8009E870");
extern s32 D_801E40CC;
extern s32 g_CameraCarIndex asm("D_8009E66C");
extern s32 D_8019C768;
extern u8 D_801F18CC[];
extern s32 *D_8019C9A8;
void GameInitRenderState(s32 arg0) asm("func_80017884");
void func_8001D30C(void);
void func_8001D210(void);
void func_80038844(void);
void func_80019E84(s32 arg0);
void func_800458CC(s32 arg0);
void GameInitShuttleScenery(void) asm("func_8003F0F8");
extern s32 D_8019C768, D_801E4B84, D_801E412C;
extern s32 D_8019CE00;
extern s32 D_8019C99C;
extern s32 D_801E4404;
extern s32 D_8019CAF4;
extern s32 D_8019C7BC;
extern s32 g_CameraCarIndex asm("D_8009E66C");
extern s32 D_8019C99C;
extern u8 D_80011010[];
void func_80033AA0(s32 arg0, s32 arg1);
void func_80025940(void);
void func_80016EA0(s32 a0, s32 a1, void *a2, s32 a3);
void func_80019730(void);
void func_80019844(void);
void func_80025AC8(void);

void func_800250BC(void) {
    s32 old;
    u16 pad;

    func_80024B6C();

    switch (D_8019C868) {
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
            GamePlaySoundCue(1);
        }
        pad = g_PadEdge2;
        if (pad & 0x860) {
            g_GameMode = 4;
        } else if (pad & 0x90) {
            g_GameMode = 4;
            g_BgmVolumeSetting = D_801E4D68;
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
            GamePlaySoundCue(1);
        }
        pad = g_PadEdge2;
        if (pad & 0x860) {
            g_GameMode = 4;
        } else if (pad & 0x90) {
            g_GameMode = 4;
            g_SfxVolumeSetting = D_801E4D68;
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
            GamePlaySoundCue(1);
        }
        pad = g_PadEdge2;
        if (pad & 0x860) {
            g_GameMode = 4;
        } else if (pad & 0x90) {
            g_GameMode = 4;
            g_MonoOutput = D_801E4D68;
        }
        break;
    }

    func_80021224();
    pad = g_PadEdge2;
    if (pad & 0x860) {
        GamePlaySoundCue(2);
    } else if (pad & 0x90) {
        GamePlaySoundCue(3);
    }
}

void func_800253A4(void) {
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
    func_80023750(3);
}

void func_800254C8(void) {
    s32 oldX;
    s32 oldY;
    u16 input;
    u16 confirm;
    u32 confirmMask;
    s32 value;

    func_800253A4();

    input = g_PadEdge;
    oldX = D_801E4D68;
    oldY = D_801E4D6C;

    if ((input & 0x1000) && (oldY >= -31)) {
        D_801E4D6C = oldY - 1;
    }

    if (g_PadEdge & 0x4000) {
        value = D_801E4D6C;
        if (value < 23) {
            D_801E4D6C = value + 1;
        }
    }

    if (g_PadEdge & 0x8000) {
        value = D_801E4D68;
        if (value >= -10) {
            D_801E4D68 = value - 1;
        }
    }

    if (g_PadEdge & 0x2000) {
        value = D_801E4D68;
        if (value < 32) {
            D_801E4D68 = value + 1;
        }
    }

    if ((oldX != D_801E4D68) || (oldY != D_801E4D6C)) {
        GamePlaySoundCue(1);
    }

    confirm = g_PadEdge2;
    if (confirm & 0x860) {
        GamePlaySoundCue(2);
        g_GameMode = 1;
        D_801E4B8C = D_801E4D68;
        D_801E4B9C = D_801E4D6C;
    } else {
        confirmMask = confirm & 0x90;
        if (confirmMask != 0) {
            GamePlaySoundCue(3);
            g_GameMode = 1;
            D_801E4D68 = D_801E4B8C;
            D_801E4D6C = D_801E4B9C;
        }
    }

    D_8019CE9C = D_801E4D68;
    D_8019CE9E = D_801E4D6C + 29;
    D_801C0684 = D_801E4D68;
    D_801C0686 = D_801E4D6C + 29;
}

void func_800256B8(void) {
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
        func_8002390C();
    }

    target = 0xF0;
    if (g_GameMode == 6) {
        target = 0x1E0;
    }

    value = D_8009F0A0;
    if (value < target) {
        D_8009F0A0 = value + 4;
    } else if (target < value) {
        D_8009F0A0 = value - 4;
    }

    scratch = (s32 *)0x1F800000;
    rawBase = g_DrawBuffer;
    base = rawBase + 0xBC8;
    pkt = *scratch;

    if (g_GameMode == 6) {
        w120 = 0x120;
        two = 2;
        white = 0xFF;
        pkt = GameAddTilePrim(base, pkt, 0x10, 0x20, w120, two, white, white, white);
        pkt = GameAddTilePrim(base, pkt, 0x10, 0x1C0, w120, two, white, white, white);
        h1c0 = 0x1C0;
        pkt = func_800172D4(base, pkt, 0x10, 0x20, 0x10, h1c0, white, white, white);
        pkt = func_800172D4(base, pkt, 0x130, 0x20, 0x130, h1c0, white, white, white);
    }

    *scratch = GameAddTilePrim(base, pkt, 0, 0, 0x140, D_8009F0A0, 0x85, 0x15, 0xE);
}

void func_80025870(void) {
    *(s32 *)0x1F800000 = GameAddTilePrim(g_DrawBuffer + 204, *(s32 *)0x1F800000, 0, 0, 0x140, 2, 0, 0, 0);
    g_AnimTimer = g_AnimTimer + 1;
    g_SceneTimer = g_SceneTimer + 1;
    if (g_SceneTimer == 2) {
        SetDispMask(1);
    }
    g_GameModeHandlers[g_GameMode]();
    func_800256B8();
}

void func_80025940(void) {
    GameInitRenderState(5);
    func_8001D30C();
    func_8001D210();
    D_801E40CC = *(s32 *)g_TrackEventData;
    func_80038844();
    func_80019E84(*(s16 *)&D_801F18CC[g_CameraCarIndex * 412]);
    func_800458CC(D_8019C9A8[2]);
    g_CameraViewMode = 2;
    g_AnimTimer = 0;
    g_SceneTimer = 0;
    D_8019C768 = 0x180;
    GameInitShuttleScenery();
}

void func_80025A14(void) {
    SetDispMask(0);
    GameSetupDisplay240(0, 0, 0);
    D_8019C768 = 0x80;
    g_FadeLevel = 0x13C;
    g_FadeStep = -4;
    g_SceneId = 0x1C;
    D_801E4B84 = 1;
    D_801E412C = 1;
    D_8019CE00 = 3;
    D_8019C99C = 0;
    g_SceneTimer = 0;
    D_801E4404 = 0;
    D_8019CAF4 = 0x1E;
    D_8019C7BC = 0;
    g_CameraCarIndex = 0;
}

void func_80025AC8(void) {
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
        func_80033AA0(g_FadeLevel, 0x49);
    } else if (d > 0) {
        s32 e = g_FadeLevel;
        e += d;
        v = e;
        g_FadeLevel = v;
        func_80033AA0(v, 0x49);
        if (g_FadeLevel >= 257) {
            SetDispMask(0);
            func_80025940();
            g_FadeStep = 0;
            g_FadeLevel = 0;
            D_8019C99C = 2;
        }
    }
    func_80016EA0(0x5E, 0x72, D_80011010, 0x7812);
}

void func_80025BD8(void) {
    if (g_AssetLoadState == 0) {
        func_80019730();
        func_80019844();
        D_8019C99C = 1;
    }

    func_80025AC8();
}

void func_80025C20(void) {
    if (g_AssetLoadState == 0) {
        g_FadeStep = 4;
    }
    func_80025AC8();
}
