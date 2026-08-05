#include "common.h"
#include "psyq/gte.h"
#include "game/asset.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"
#include "game/menu.h"
#include "psyq/gpu.h"

extern s16 g_TrackZoneCode;
extern Matrix g_SceneColorMatrix;
extern Matrix D_8007C778;
void func_800698B8(Matrix *arg0);

/* Darkens the scene colour matrix by GetTrackZoneBlend's 0..0x100 track-zone ramp; RestoreColorMatrix puts it back. */
void ApplyZoneLighting(s32 a0, Matrix *a1);
void ApplyZoneLighting(s32 a0, Matrix *a1) {
    Matrix out;
    s32 s1;

    if (g_TrackZoneCode != 0) {
        s1 = 0x100 - (a0 * 3) / 4;
        out.m[0][0] = g_SceneColorMatrix.m[0][0] * s1 / 256;
        out.m[0][1] = g_SceneColorMatrix.m[0][1] * s1 / 256;
        out.m[0][2] = g_SceneColorMatrix.m[0][2] * s1 / 256;
        out.m[1][0] = g_SceneColorMatrix.m[1][0] * s1 / 256;
        out.m[1][1] = g_SceneColorMatrix.m[1][1] * s1 / 256;
        out.m[1][2] = g_SceneColorMatrix.m[1][2] * s1 / 256;
        out.m[2][0] = g_SceneColorMatrix.m[2][0] * s1 / 256;
        out.m[2][1] = g_SceneColorMatrix.m[2][1] * s1 / 256;
        out.m[2][2] = g_SceneColorMatrix.m[2][2] * s1 / 256;
        func_800698B8(&out);
    } else {
        s32 k;
        s32 h;
        s32 kb;
        out.m[0][0] = g_SceneColorMatrix.m[0][0];
        out.m[0][1] = g_SceneColorMatrix.m[0][1];
        out.m[0][2] = g_SceneColorMatrix.m[0][2];
        h = a0 / 2;
        k = 0x100;
        s1 = k - h;
        out.m[1][0] = g_SceneColorMatrix.m[1][0] * s1 / 256;
        out.m[1][1] = g_SceneColorMatrix.m[1][1] * s1 / 256;
        out.m[1][2] = g_SceneColorMatrix.m[1][2] * s1 / 256;
        s1 = k - (a0 * 3) / 4;
        out.m[2][0] = g_SceneColorMatrix.m[2][0] * s1 / 256;
        out.m[2][1] = g_SceneColorMatrix.m[2][1] * s1 / 256;
        out.m[2][2] = g_SceneColorMatrix.m[2][2] * s1 / 256;
        func_800698B8(&out);

        kb = k - a0;
        a1->m[0][0] = a1->m[0][0] * kb / 256 + D_8007C778.m[0][0] * a0 / 256;
        a1->m[0][1] = a1->m[0][1] * kb / 256 + D_8007C778.m[0][1] * a0 / 256;
        a1->m[0][2] = a1->m[0][2] * kb / 256 + D_8007C778.m[0][2] * a0 / 256;
        a1->m[1][0] = a1->m[1][0] * kb / 256 + D_8007C778.m[1][0] * a0 / 256;
        a1->m[1][1] = a1->m[1][1] * kb / 256 + D_8007C778.m[1][1] * a0 / 256;
        a1->m[1][2] = a1->m[1][2] * kb / 256 + D_8007C778.m[1][2] * a0 / 256;
        a1->m[2][0] = a1->m[2][0] * kb / 256 + D_8007C778.m[2][0] * a0 / 256;
        a1->m[2][1] = a1->m[2][1] * kb / 256 + D_8007C778.m[2][1] * a0 / 256;
        a1->m[2][2] = a1->m[2][2] * kb / 256 + D_8007C778.m[2][2] * a0 / 256;
    }
}

extern Matrix g_SceneColorMatrix;
void func_800698B8(Matrix *arg0);
/* Puts the unmodified colour matrix back after ApplyZoneLighting. */
void RestoreColorMatrix(void);
void RestoreColorMatrix(void) { func_800698B8(&g_SceneColorMatrix); }

extern s32 g_FrameSyncThreshold;
extern s32 g_ImageBlockBuffer;
extern u8 *g_CourseProgress;

void CloseLoadedAudioSlots(void);
void UploadImageAsset(s32 arg0);
void RelocateCarModel(void);

/* Scene 9: finishes the asset load, relocates the car model and derives g_GrandPrixRound. */
void EnterRoundScreen(void);
void EnterRoundScreen(void) {
    s32 count;
    u8 *ptr;
    u8 *end;

    SetDispMask(0);
    g_FrameSyncThreshold = 0x80;

    if (g_AssetLoadState != 1) {
        CloseLoadedAudioSlots();
        UploadImageAsset(g_ImageBlockBuffer);
        RelocateCarModel();

        g_FrameSyncThreshold = 0x180;
        g_SceneTimer = 0;
        g_SceneId = 10;
        g_FadeLevel = 0;
        count = (g_GrandPrixClass < 2) ? 3 : 4;
        g_GrandPrixRound = 0;

        if (count != 0) {
            ptr = g_CourseProgress;
            end = (u8 *)(count + (s32)ptr);
            do {
                if (*ptr != 0) {
                    g_GrandPrixRound++;
                }
                ptr++;
            } while ((s32)ptr < (s32)end);
        }

        if (g_CourseProgress[g_CourseIndex] == 0) {
            g_GrandPrixRound++;
        }
    }
}

extern s16 g_RoundScreenFadeDelays[];

s32 UpdateRoundScreenFade(s32 arg0);
s32 UpdateRoundScreenFade(s32 arg0) {
    s32 value;
    s32 ret;

    if (g_SceneId == 10) {
        value = (g_SceneTimer * 4) - g_RoundScreenFadeDelays[arg0];
    } else {
        value = g_FadeLevel;
        if (value > 0) {
            value--;
            g_FadeLevel = value;
        }
        value = g_FadeLevel;
    }

    if (value >= 0) {
        ret = value;
        if (ret < 0x80) {
            return ret;
        }
        ret = 0x7F;
    } else {
        ret = 0;
    }
    return ret;
}

extern s32 g_BgmSelection;
extern char *g_BgmTrackNames[];
extern char D_80010D2C[];

extern char D_80010C30[];
extern char D_80010C40[];
extern char D_80010C44[];
extern char D_80010C50[];
extern char D_80010C5C[];
extern char D_80010C68[];
extern char D_80010C70[];

extern s32 g_BestTotalTimes[][4][2];
extern s32 g_BestLapTimes[][4][2];

s32 UpdateRoundScreenFade(s32 arg0);
void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0, s32 r, s32 g,
                   s32 b, s32 clutX, s32 shadeTex, s32 semiTrans, s32 flags);
void LibcSprintf(void *dst, void *fmt, s32 v) asm("func_800632F0");
void func_80016B7C(u32 a0, u32 a1, void *a2, u32 a3, u32 a4);
void FormatLapTime(void *dst, s32 v);

/* The ROUND screen: course name, round number and either the prize lines or the best times. */
void DrawRoundScreen(void);
void DrawRoundScreen(void) {
    char buf[88];
    s32 col;
    s32 y0;
    void *ot = g_DrawBuffer + 204;

    col = UpdateRoundScreenFade(0);
    func_80046A2C(ot, 0x74, 0x14, 0x58, 0x38, 0xa8, 0xa8, col, col, col, 0x1f, 0, 1, 0x29);
    func_80046A2C(ot, 0x44, 0x50, 0xb8, 0x14, 0x48, 0xe8, col, col, col, 0x80, 0, 1, 0x29);

    col = UpdateRoundScreenFade(1);
    if (g_GrandPrixMode != 0) {
        LibcSprintf(buf, D_80010C30, g_GrandPrixRound);
        func_80016B7C(0x5e, 0x68, buf, 0x7812, col);
        y0 = 0x78;
    } else {
        y0 = 0x68;
    }
    func_80046A2C(ot, 0x5e, y0, 0x84, 0xc, 0, g_CourseIndex * 12 + 156, col, col, col, 0x12, 0, 1, 0x29);

    col = UpdateRoundScreenFade(2);
    if (g_GrandPrixMode != 0) {
        func_80016B7C(0x80, 0x88, D_80010C40, 0x7812, col);
        LibcSprintf(buf, D_80010C44, g_PrizeMoney[g_CourseIndex][g_GrandPrixClass][0]);
        func_80016B7C(0x56, 0x98, buf, 0x7812, col);
        LibcSprintf(buf, D_80010C50, g_PrizeMoney[g_CourseIndex][g_GrandPrixClass][1]);
        func_80016B7C(0x56, 0xa4, buf, 0x7812, col);
        LibcSprintf(buf, D_80010C5C, g_PrizeMoney[g_CourseIndex][g_GrandPrixClass][2]);
        func_80016B7C(0x56, 0xb0, buf, 0x7812, col);
    } else {
        func_80016B7C(0x62, 0x7c, D_80010C68, 0x7812, col);
        FormatLapTime(buf, g_BestTotalTimes[g_GrandPrixSeries][g_CourseIndex][g_GrandPrixMode]);
        func_80016B7C(0x6a, 0x8c, buf, 0x7812, col);
        func_80016B7C(0x6a, 0x9c, D_80010C70, 0x7812, col);
        FormatLapTime(buf, g_BestLapTimes[g_GrandPrixSeries][g_CourseIndex][g_GrandPrixMode]);
        func_80016B7C(0x6a, 0xac, buf, 0x7812, col);
    }
}

s32 func_80016EC4(void *ot, s32 p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
s32 func_80017390(void *ot, s32 p, s32 a);
s32 AddTilePrim(void *ot, s32 p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void func_80016754(s32 x, s32 y, void *str, s32 col);

/* The BGM row: the selection number and the track title from g_BgmTrackNames. */
void DrawBgmSelector(void);
void DrawBgmSelector(void) {
    s32 x;
    char buf[88];
    s32 *scr = (s32 *)0x1F800000;
    s32 p;
    void *ot = g_DrawBuffer + 208;

    p = *scr;
    p = func_80016EC4(ot, p, 0x14, 0xce, 0x58, 8, 0xa8, 0xe0, 0x7812);
    x = (g_BgmSelection == 0xa) ? 0x6c : 0x70;
    p = func_80016EC4(ot, p, x, 0xce, 8, 8, 0x84, 0xc4, 0x7812);
    p = func_80016EC4(ot, p, (g_BgmSelection == 0xa) ? 0x84 : 0x80, 0xce, 8, 8, 0x8c, 0xc4, 0x7812);
    p = func_80017390(ot, p, 0x29);
    p = AddTilePrim(ot, p, 0x10, 0xcc, 0x5b, 0xc, 0x85, 0x15, 0xe);
    p = AddTilePrim(ot, p, 0x6c, 0xcc, 0x1f, 0xc, 0x40, 0x40, 0x40);
    p = AddTilePrim(ot, p, 0x8c, 0xcc, 0xa4, 0xc, 0, 0, 0);
    p = AddTilePrim(ot, p, 0xf, 0xcb, 0x122, 0xe, 0xff, 0xff, 0xff);
    *scr = p;

    LibcSprintf(buf, D_80010D2C, g_BgmSelection);
    x = (g_BgmSelection == 0xa) ? 0x74 : 0x78;
    func_80016754(x, 0xce, buf, 0x78cc);
    func_80016754(0x90, 0xce, g_BgmTrackNames[g_BgmSelection], 0x78cc);
}

extern s32 g_BgmShuffleIndex;
extern s32 g_BgmTrackCount;
extern u8 g_BgmShuffleOrder[];
extern s32 g_BgmTrack;

void PlaySoundCue(s32 cue);
s32 RequestRaceAssets(void);

/* Scene 10: draws the ROUND screen, takes the BGM choice and starts the race at frame 121. */
void UpdateRoundScreen(void);
void UpdateRoundScreen(void) {
    if ((u32)g_SceneTimer < 10000) {
        g_SceneTimer = g_SceneTimer + 1;
    }
    if (g_SceneTimer == 0xf) {
        SetDispMask(1);
    }
    if (g_SceneTimer == 1) {
        SetupDisplay240(0, 0, 0);
    }
    DrawRoundScreen();
    if (g_SceneTimer == 0x20) {
        PlaySoundCue(0x19);
    }
    if (g_FadeLevel == 0) {
        if (RequestRaceAssets() == 0) {
            g_FadeLevel = 0x80;
        }
    } else if ((u32)g_SceneTimer >= 121) {
        g_SceneId = 0xb;
        if ((g_PadHeld & 0x80c) == 0x80c) {
            g_MirrorMode = 1;
        } else {
            g_MirrorMode = 0;
        }
        if (g_BgmSelection == 0) {
            s32 idx = g_BgmShuffleIndex;
            u8 val = g_BgmShuffleOrder[idx];
            g_BgmShuffleIndex = idx + 1;
            g_BgmTrack = val;
            if (g_BgmShuffleIndex == g_BgmTrackCount) {
                g_BgmShuffleIndex = 0;
            }
        } else {
            g_BgmTrack = g_BgmSelection - 1;
        }
        if (g_BgmTrack == 9) {
            g_BgmTrack = 0xe;
        }
    }
    if (g_SceneId == 0xa) {
        u16 flags = g_PadEdge2;
        if (flags & 0x8000) {
            g_BgmSelection = g_BgmSelection - 1;
        } else if (flags & 0x2000) {
            g_BgmSelection = g_BgmSelection + 1;
        }
        g_BgmSelection = (g_BgmSelection + g_BgmTrackCount + 1) % (g_BgmTrackCount + 1);
        DrawBgmSelector();
    }
}

extern Matrix g_SceneColorMatrix;
extern Matrix D_8007C758;
extern Matrix g_SceneLightMatrix;
extern Matrix D_8007C778;
void func_800698B8(Matrix *arg0);
/* Installs the track colour/light matrices, back and far colours and the fog near distance. */
void InitTrackLighting(void);
void InitTrackLighting(void) {
    g_SceneColorMatrix = D_8007C758;
    g_SceneLightMatrix = D_8007C778;
    func_800698B8(&g_SceneColorMatrix);
    SetLightMatrix(&g_SceneLightMatrix);
    SetBackColor(0x20, 0x20, 0x20);
    SetFogNear(0x1770, 0x140);
    SetFarColor(0x80, 0x80, 0x80);
}
