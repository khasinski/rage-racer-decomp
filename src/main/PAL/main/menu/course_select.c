#include "common.h"
#include "game/state.h"
#include "game/race.h"
#include "game/render.h"
#include "game/menu.h"
#include "game/car.h"

s32 DrawRankingScreen(s32 arg0);

const u8 g_NowLoadingText[] asm("D_80011B60") = "NOW LOADING";
void func_80016754(s32 arg0, s32 arg1, void *arg2, s32 arg3);
/* Blinks the "NOW LOADING" string at D_80011B60. */
void DrawNowLoadingText(void) asm("func_80052738");
void DrawNowLoadingText(void) {
    if (g_SceneTimer & 8) {
        func_80016754(0x74, 0xEC, (void *)g_NowLoadingText, 0x78CC);
    }
}

extern s32 g_TimeAttackPlateStep asm("D_8009B334");
extern s32 D_8009B360;
extern s32 D_8009B364;
extern s32 D_8009B368;
extern s32 g_MenuViewSpin asm("D_801E8268");
extern u8 *g_CourseProgress asm("D_8009E67C");
extern s32 g_PlayerCar asm("D_8009E6D4");
extern s32 D_8009E6D8;
extern s32 g_PlayerCarZ asm("D_8009E6DC");
extern s32 D_8009E6F4;
extern s32 D_8009E6F8;
extern s32 D_8009E6FC;
extern s32 D_8009E718;
extern s32 D_8009E71C;
extern s32 g_PlayerTrackProgress asm("D_8009E744");
extern u8 g_TeamLogoClutRect[] asm("D_8007BEDC");
extern u8 g_TeamLogoRect[] asm("D_8007BEE4");
extern u8 g_TeamLogoClut[] asm("D_801E444C");
extern u8 g_TeamLogoCanvas[] asm("D_801E6F2C");
s32 RequestCarSelectAssets(void) asm("func_80018530");
void func_8005E88C(void);
void func_80049418(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void LoadImage(void *rect, void *data) asm("func_80065B24");
void UploadTeamNameTexture(void *arg0, s32 arg1) asm("func_8001D530");

/* g_MenuScreenUpdate[0]: waits for the car-select assets, then opens screen 1. */
void EnterCourseSelectScreen(void) asm("func_80052778");
void EnterCourseSelectScreen(void) {
    s32 one;
    register s32 initValue;
    register s32 mode asm("$4");
    s32 largeValue;
    u8 *table;
    s32 eight;

    DrawNowLoadingText();
    if (RequestCarSelectAssets() != 0) {
        return;
    }

    one = 1;
    func_8005E88C();
    g_MenuHandlerIndex = one;
    g_MenuScreen = one;
    func_80049418(0, 0, 0, 0);

    initValue = 0x7A120;
    mode = 0x3D090;
    largeValue = 0x1F0000;
    g_MenuViewOffset = mode;
    mode = g_CourseIndex;
    eight = 8;
    g_MenuViewSpin = eight;
    table = g_CourseProgress;
    largeValue |= 0x4000;
    g_UiScriptProgress = 0;
    g_PlayerCar = 0;
    D_8009E6D8 = 0;
    g_PlayerCarZ = 0;
    D_8009E6F4 = 0;
    D_8009E6F8 = 0;
    D_8009E6FC = 0;
    g_PlayerTrackProgress = 0;
    D_8009E718 = 0;
    D_8009E71C = 0;
    g_MenuViewAngleTarget = 0x7A120;
    g_MenuViewAngle = initValue;
    g_MenuViewOffsetTarget = 0;
    D_8009B360 = largeValue;
    D_8009B364 = 0;
    D_8009B368 = table[mode & 3];

    if (mode >= 4) {
        g_TimeAttackPlateStep = one;
    } else {
        g_TimeAttackPlateStep = -1;
    }

    LoadImage(g_TeamLogoRect, g_TeamLogoCanvas);
    LoadImage(g_TeamLogoClutRect, g_TeamLogoClut);
    UploadTeamNameTexture(g_TeamNameChars, g_TeamNameLength);
}

typedef struct CourseSelectSpriteBounds {
    s32 x;
    s32 y;
    s32 w;
    s32 h;
} CourseSelectSpriteBounds;

typedef struct CourseSelectPrizeTable {
    s32 values[4][6][3];
} CourseSelectPrizeTable;

typedef struct CourseSelectScrollState {
    s32 value;
} CourseSelectScrollState;

extern CourseSelectPrizeTable g_CourseSelectPrizeTable asm("D_8007BEEC");
extern CourseSelectScrollState g_CourseSelectScrollState asm("D_8009B2C0");
extern s32 D_8009B2C0;

void GameDrawSpriteWide(
    void *ot,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 r,
    s32 g,
    s32 b,
    s32 clut,
    s32 shadeTex,
    s32 semiTrans,
    s32 flags) asm("func_80046A2C");
void GameDrawSolidRectWide(
    void *ot,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 r,
    s32 g,
    s32 b,
    s32 alpha) asm("func_80047024");
void GameDrawLineWide(
    void *ot,
    s32 x0,
    s32 y0,
    s32 x1,
    s32 y1,
    s32 r,
    s32 g,
    s32 b,
    s32 alpha) asm("func_8004711C");
s32 GameDrawNumberWide(
    s32 x,
    s32 y,
    s32 flags,
    s32 value,
    s32 r,
    s32 g,
    s32 b,
    s32 clut,
    s32 primitiveCount) asm("func_80047BD4");

/*
 * The menu lays out several two-sprite labels from the first sprite's sliding
 * Y. GCC 2.6.3 integrates this explicit inline at -O2; its optional bounds
 * result also preserves the retail caller frame before null outputs fold away.
 */
static __inline__ s32 GameDrawSlidingSprite(
    void *ot,
    s32 x,
    s32 baseY,
    s32 slide,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 r,
    s32 g,
    s32 b,
    s32 clut,
    s32 shadeTex,
    s32 semiTrans,
    s32 flags,
    CourseSelectSpriteBounds *bounds)
{
    CourseSelectSpriteBounds result;
    s32 y;

    y = baseY - slide;
    if (bounds != 0) {
        result.x = x;
        result.y = y;
        result.w = w;
        result.h = h;
        *bounds = result;
    }
    GameDrawSpriteWide(
        ot, x, y, w, h, u, v, r, g, b, clut,
        shadeTex, semiTrans, flags);
    return y;
}

s32 DrawCourseSelectScreen(s32 step) asm("func_8005290C");
s32 DrawCourseSelectScreen(s32 step)
{
    void *otBase;
    void *ot;
    u8 fade;
    u16 slide;
    s16 headerWidth;
    s32 deltaY;
    /* Load-bearing in the prize loop: without this pin the function is 848 words. */
    register s32 coordinateY asm("$16");
    s32 lineColor;
    s32 row;
    s32 digitCount;
    s32 prizeOffset;
    CourseSelectPrizeTable *prizeTable;
    s32 prizeFade;
    s32 prizeClut;
    s32 gpHeight;
    s32 gpClut;
    s32 gpSemiTrans;
    s32 gpFlags;
    s32 gpSlide;
    s32 gpFade;

    otBase = *(void **)0x1F800004;
    ot = (u8 *)otBase + 4;
    if (step == 0) {
        D_8009B2C0 = 0;
        return (s32)otBase;
    }

    if (step > 0) {
        D_8009B2C0 += step;
        if (D_8009B2C0 >= 0x1FD) {
            D_8009B2C0 = 0x1FC;
        }
        slide = 0;
    } else {
        D_8009B2C0 += step;
        if (D_8009B2C0 < 0) {
            D_8009B2C0 = 0;
        }
        deltaY = 0x1FC - D_8009B2C0;
        slide = (u16)((u32)(deltaY * deltaY) >> 11);
    }

    if (g_MenuAltLayout != 0) {
        return g_CourseSelectScrollState.value;
    }

    slide -= 0x28;
    fade = (u8)((u32)D_8009B2C0 >> 2);

    if (g_GrandPrixMode != 0) {
        if (g_SeriesSelection == 0) {
            switch (g_GrandPrixClass) {
            case 0:
                headerWidth = 0x24;
                GameDrawSpriteWide(
                    ot, 0x50, 0xB0 - (s16)slide, 0x24, 0x10,
                    0, 0x38, fade, fade, fade, 0x244, 0, 1, 0x5B);
                break;
            case 1:
                headerWidth = 0x20;
                GameDrawSpriteWide(
                    ot, 0x50, 0xB0 - (s16)slide, 0x20, 0x10,
                    0x24, 0x38, fade, fade, fade, 0x244, 0, 1, 0x5B);
                break;
            case 2:
                headerWidth = 0x28;
                GameDrawSpriteWide(
                    ot, 0x50, 0xB0 - (s16)slide, 0x28, 0x10,
                    0x44, 0x38, fade, fade, fade, 0x244, 0, 1, 0x5B);
                break;
            case 3:
                headerWidth = 0x30;
                GameDrawSpriteWide(
                    ot, 0x50, 0xB0 - (s16)slide, 0x30, 0x10,
                    0x6C, 0x38, fade, fade, fade, 0x244, 0, 1, 0x5B);
                break;
            case 4:
                headerWidth = 0x30;
                GameDrawSpriteWide(
                    ot, 0x50, 0xB0 - (s16)slide, 0x30, 0x10,
                    0x9C, 0x38, fade, fade, fade, 0x244, 0, 1, 0x5B);
                break;
            }
        } else {
            switch (g_GrandPrixClass) {
            case 0:
                headerWidth = 0x30;
                GameDrawSpriteWide(
                    ot, 0x50, 0xB0 - (s16)slide, 0x30, 0x10,
                    0xCC, 0x38, fade, fade, fade, 0x244, 0, 1, 0x5B);
                break;
            case 1:
                headerWidth = 0x40;
                GameDrawSpriteWide(
                    ot, 0x50, 0xB0 - (s16)slide, 0x40, 0x10,
                    0, 0x48, fade, fade, fade, 0x244, 0, 1, 0x5B);
                break;
            case 2:
                headerWidth = 0x3C;
                GameDrawSpriteWide(
                    ot, 0x50, 0xB0 - (s16)slide, 0x3C, 0x10,
                    0x40, 0x48, fade, fade, fade, 0x244, 0, 1, 0x5B);
                break;
            case 3:
                headerWidth = 0x28;
                GameDrawSpriteWide(
                    ot, 0x50, 0xB0 - (s16)slide, 0x28, 0x10,
                    0x7C, 0x48, fade, fade, fade, 0x244, 0, 1, 0x5B);
                break;
            case 4:
                headerWidth = 0x20;
                GameDrawSpriteWide(
                    ot, 0x50, 0xB0 - (s16)slide, 0x20, 0x10,
                    0xA4, 0x48, fade, fade, fade, 0x244, 0, 1, 0x5B);
                break;
            case 5:
                headerWidth = 0x28;
                GameDrawSpriteWide(
                    ot, 0x50, 0xB0 - (s16)slide, 0x28, 0x10,
                    0xC4, 0x48, fade, fade, fade, 0x244, 0, 1, 0x5B);
                break;
            }
        }

        gpHeight = 0x10;
        gpClut = 0x244;
        gpSemiTrans = 1;
        gpSlide = (s16)slide;
        gpFade = fade;
        gpFlags = 0x5B;
        GameDrawSpriteWide(
            ot, headerWidth + 0x50, 0xB0 - gpSlide, 0x10,
            gpHeight, 0xEC, 0x48, gpFade, gpFade, gpFade,
            gpClut, 0, gpSemiTrans, gpFlags);

        coordinateY = GameDrawSlidingSprite(
            ot, 0x50, 0x97, gpSlide, 0x1A, gpHeight, 0x60, 0xCC,
            gpFade, gpFade, gpFade, gpClut, 0, gpSemiTrans, gpFlags, 0);
        GameDrawSpriteWide(
            ot, 0x6C, coordinateY, 8, 0x10,
            g_GrandPrixClass * 8 + 8, 0x18,
            gpFade, gpFade, gpFade, gpClut, 0, gpSemiTrans, gpFlags);

        lineColor = (u32)gpFade << 1;
        GameDrawLineWide(
            ot, 0x48, 0xAA - gpSlide, 0xAF, 0xAA - gpSlide,
            lineColor, lineColor, lineColor, 0x40);
        GameDrawLineWide(
            ot, 0x48, 0xAB - gpSlide, 0xAF, 0xAB - gpSlide,
            lineColor, lineColor, lineColor, 0x40);
        GameDrawSolidRectWide(
            ot, 0x48, 0x94 - gpSlide, 0x68, 0x30,
            lineColor, lineColor, lineColor,
            (u32)gpFade < 0x7F ? 0x20 : 0xFF);
        GameDrawSpriteWide(
            ot, 0xB0, 0x94 - (s16)slide, 0x20, 0x30,
            0x60, 0x88, fade, fade, fade, 0x25B, 0, 1, 0x39);
    }

    coordinateY = GameDrawSlidingSprite(
        ot, 0x4C, 0xD0, (s16)slide, 0x18, 0xC, 0x18, 0xDC,
        fade, fade, fade, 0x244, 0, 1, 0x3A, 0);
    GameDrawSpriteWide(
        ot, 0x68, coordinateY, 0x12, 0xC,
        0x32, 0xDC, fade, fade, fade, 0x244, 0, 1, 0x3A);

    coordinateY = GameDrawSlidingSprite(
        ot, 0x4C, 0xF8, (s16)slide, 0x18, 0xC, 0x18, 0xDC,
        fade, fade, fade, 0x244, 0, 1, 0x3A, 0);
    GameDrawSpriteWide(
        ot, 0x68, coordinateY, 0x1A, 0xC,
        0x46, 0xDC, fade, fade, fade, 0x244, 0, 1, 0x3A);

    switch (g_CourseIndex & 3) {
    case 0:
        coordinateY = GameDrawSlidingSprite(
            ot, 0x4C, 0xE0, (s16)slide, 8, 0x10, 8, 0x18,
            fade, fade, fade, 0x244, 0, 1, 0x3B, 0);
        GameDrawSpriteWide(
            ot, 0x54, coordinateY, 0x54, 0x10,
            0, 0x9C, fade, fade, fade, 0x244, 0, 1, 0x3B);
        GameDrawSpriteWide(
            ot, 0x4C, 0x108 - (s16)slide, 0x20, 0x10,
            0x44, 0xB4, fade, fade, fade, 0x244, 0, 1, 0x3A);
        break;
    case 1:
        coordinateY = GameDrawSlidingSprite(
            ot, 0x4C, 0xE0, (s16)slide, 8, 0x10, 0x10, 0x18,
            fade, fade, fade, 0x244, 0, 1, 0x3B, 0);
        GameDrawSpriteWide(
            ot, 0x54, coordinateY, 0x4C, 0x10,
            0x54, 0x9C, fade, fade, fade, 0x244, 0, 1, 0x3B);
        GameDrawSpriteWide(
            ot, 0x4C, 0x108 - (s16)slide, 0x20, 0x10,
            0x64, 0xB4, fade, fade, fade, 0x244, 0, 1, 0x3A);
        break;
    case 2:
        coordinateY = GameDrawSlidingSprite(
            ot, 0x4C, 0xE0, (s16)slide, 8, 0x10, 0x18, 0x18,
            fade, fade, fade, 0x244, 0, 1, 0x3B, 0);
        GameDrawSpriteWide(
            ot, 0x54, coordinateY, 0x48, 0x10,
            0, 0xAC, fade, fade, fade, 0x244, 0, 1, 0x3B);
        GameDrawSpriteWide(
            ot, 0x4C, 0x108 - (s16)slide, 0x20, 0x10,
            0x84, 0xB4, fade, fade, fade, 0x244, 0, 1, 0x3A);
        break;
    case 3:
        coordinateY = GameDrawSlidingSprite(
            ot, 0x4C, 0xE0, (s16)slide, 8, 0x10, 0x20, 0x18,
            fade, fade, fade, 0x244, 0, 1, 0x3B, 0);
        GameDrawSpriteWide(
            ot, 0x54, coordinateY, 0x5C, 0x10,
            0xA4, 0x9C, fade, fade, fade, 0x244, 0, 1, 0x3B);
        GameDrawSpriteWide(
            ot, 0x4C, 0x108 - (s16)slide, 0x1E, 0x10,
            0xA4, 0xB4, fade, fade, fade, 0x244, 0, 1, 0x3A);
        break;
    }

    if (g_GrandPrixMode != 0) {
        GameDrawSpriteWide(
            ot, 0x4C, 0x140 - (s16)slide, 0x18, 0x10,
            0xB4, 0xCC, fade, fade, fade, 0x244, 0, 1, 0x3A);
        GameDrawSpriteWide(
            ot, 0x4C, 0x150 - (s16)slide, 0x18, 0x10,
            0xCC, 0xCC, fade, fade, fade, 0x244, 0, 1, 0x3A);
        GameDrawSpriteWide(
            ot, 0x4C, 0x160 - (s16)slide, 0x18, 0x10,
            0xE4, 0xCC, fade, fade, fade, 0x244, 0, 1, 0x3A);

        row = 0;
        prizeOffset = (s16)slide - 0x140;
        prizeTable = &g_CourseSelectPrizeTable;
        prizeFade = fade;
        prizeClut = 0x244;
        do {
            coordinateY = row * 0x10 - prizeOffset;
            digitCount = GameDrawNumberWide(
                0x65, coordinateY, 9,
                prizeTable->values[g_CourseIndex & 3][g_GrandPrixClass][row],
                prizeFade, prizeFade, prizeFade, prizeClut, 0x20);
            row++;
            GameDrawSpriteWide(
                ot, digitCount * 8 + 0x65, coordinateY, 0xC, 0x10,
                0xF4, 0x28, prizeFade, prizeFade, prizeFade,
                prizeClut, 0, 1, 0x3B);
        } while (row < 3);
    }

    return D_8009B2C0;
}

/* The mirror of CanSelectNextCourse. */
s32 CanSelectPrevCourse(void) asm("func_80053650");
s32 CanSelectPrevCourse(void) {
    s32 v1 = 0;
    if (g_GrandPrixMode != 0) {
        v1 = (g_SeriesSelection != 0) << 2;
    }
    return v1 < g_CourseIndex;
}

s32 CanSelectNextCourse(void) asm("func_80053688");

s32 CanSelectNextCourse(void) {
    s32 limit;

    if (g_GrandPrixMode != 0) {
        if (g_SeriesSelection != 0) {
            limit = (g_GrandPrixClass < 2) ? 6 : 7;
        } else {
            limit = (g_GrandPrixClass < 2) ? 2 : 3;
        }
    } else if (g_AdvancedSeriesUnlocked != 0) {
        limit = (g_MaxClassReached[1] < 2) ? 6 : 7;
    } else {
        limit = (g_MaxClassReached[0] < 2) ? 2 : 3;
    }

    return g_CourseIndex < limit;
}

extern s32 g_CarNamePlateStep asm("D_8009B31C");
extern s32 g_MenuPlateCarIndex asm("D_8009B320");
extern s32 g_MenuHintBarStep asm("D_8009B30C");
extern s32 g_MenuConfirmTimer asm("D_8009B300");
extern s32 D_8009B310;
extern s32 D_8009B348;
extern s32 g_CourseSwapDelay asm("D_8009B354");
extern s32 g_MenuCourseModelIndex asm("D_8009B36C");
extern s32 g_MenuPendingCourseIndex asm("D_8009B370");
extern s32 g_CarSwapFromIndex asm("D_8009B374");
extern s32 g_CarSwapToIndex asm("D_8009B378");
extern u8 g_MenuSubCursor asm("D_8009B2F0");
extern u8 *D_8019C764;
extern s32 g_CourseSelectOption asm("D_8019C7AC");
extern s32 g_PlayerMoney asm("D_8019C908");
/* u16 view of race.h's s16 g_GrandPrixSeries; the header is included here, so this
 * translation unit's deliberately different type needs its own spelling. */
extern u16 g_GrandPrixSeriesU16 asm("D_8019CABC");
extern u8 D_80081818;
extern u8 D_800817A0;
extern u8 g_UiChromeScript asm("D_80082460");
extern u8 D_800827FC;
extern u8 g_UiChromeScript2 asm("D_80082790");
extern u8 D_80082604;
extern u8 D_800825A4;
extern u8 g_MenuBlankCaption asm("D_80011BA0");
void ResetCourseProgress(s32 arg0) asm("func_800212F0");
void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0,
                   s32 r, s32 g, s32 b, s32 clut, s32 sh, s32 st, s32 flags);
s32 func_800487D8(u8 *commands, s32 *progress, s32 step);
void func_800489AC(s32 arg0, s32 arg1, s32 arg2);
void func_80048B88(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                   s32 a7, s32 a8, s32 a9, s32 a10);
void func_80048D64(s32 x0, s32 y0, s32 x1, s32 y1, s32 useFlash);
void func_80049418(s32, s32, s32, s32);
void func_8004CF30(s32 arg0);
void func_8004F3EC(s32 arg0, s32 arg1);
void func_8004FCE8(s32 arg0, s32 arg1, s32 arg2);
s32 DrawClassChangeCurtain(s32 arg0) asm("func_8005026C");
void FlipCourseCard(s32 *p0, s32 *p1, s32 *p2) asm("func_800506BC");
void func_800509C4(s32 arg0);
void func_8005194C(void);
void PlaySoundCue(s32 cue) asm("func_8005D6EC");
void func_8005E8E0(void);
void UpdateCourseSelectScreen(void) asm("func_80053730");
void UpdateCourseSelectScreen(void) {
    void *ot;
    u8 *hdr;
    s32 state;
    s32 res;
    s32 sel;
    s32 prev;
    s32 cnt;
    s32 t;
    s32 u;
    s32 lap;
    s32 i;
    GameRaceProgress *p;
    ot = *(void **)0x1F800004;
    g_MenuAltLayout = g_MenuAltLayoutSetting;
    if (g_GrandPrixMode != 0) {
        FlipCourseCard(&D_8009B364, &D_8009B360, &D_8009B368);
    } else {
        func_800509C4(g_TimeAttackPlateStep);
    }
    func_8004FCE8(g_CarNamePlateStep, g_MenuPlateCarIndex, 0);
    func_8005194C();
    hdr = &D_80081818;
    if (g_GrandPrixMode != 0) {
        hdr = &D_800817A0;
    }
    state = GameMenuBusy;
    if (state == 0) {
        g_MenuHintBarStep = 1;
        func_800487D8(D_8019C764, &g_UiScriptProgress2, -1);
        res = CanSelectPrevCourse();
        func_80049418(1, 1, res, CanSelectNextCourse());
        func_800489AC(g_UiScriptProgress, 2, g_CourseSelectOption);
        func_800487D8(hdr, &g_UiScriptProgress, 0);
        func_8004CF30(7);
        if ((func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1) != 0) && (g_UiScriptProgress2 <= 0)) {
            g_MenuOverlayPattern = -1;
            if (g_PadEdge2 & 0x1000) {
                PlaySoundCue(1);
                g_CourseSelectOption = (g_CourseSelectOption > 0) ? g_CourseSelectOption - 1 : 2;
            }
            if (g_PadEdge2 & 0x4000) {
                PlaySoundCue(1);
                g_CourseSelectOption = (g_CourseSelectOption < 2) ? g_CourseSelectOption + 1 : 0;
            }
            if ((g_PadHeld & 0x8000) && (CanSelectPrevCourse() != 0)) {
                t = g_MenuViewAngleTarget;
                u = g_MenuViewAngle;
                if (t < u ? (u - t <= 0x3D08F) : (t - u <= 0x3D08F)) {
                    if (g_MenuPendingCourseIndex < 0) {
{
                        s32 llap;
                        s32 lprev;
                        s32 lu;
                        s32 lt;
                        PlaySoundCue(8);
                        lu = g_MenuViewAngle;
                        llap = g_CourseIndex;
                        lprev = g_MenuViewAngleTarget;
                        lt = D_8009B364;
                        g_MenuViewAngleTarget = 0;
                        g_CourseSwapDelay = 0;
                        g_MenuCourseModelIndex = llap;
                        llap = llap - 1;
                        g_MenuViewAngle = (lu - lprev) + 0x7A120;
                        D_8009B360 = (D_8009B360 - lt) + 0x1F4000;
                        g_CourseIndex = llap;
                        g_MenuPendingCourseIndex = llap;
                        D_8009B368 = g_CourseProgress[llap & 3];
                        g_TimeAttackPlateStep = (llap < 4) ? -1 : 1;
                    }
                    }
                }
            }
            if ((g_PadHeld & 0x2000) && (CanSelectNextCourse() != 0)) {
                t = g_MenuViewAngleTarget;
                u = g_MenuViewAngle;
                if (t < u ? (u - t <= 0x3D08F) : (t - u <= 0x3D08F)) {
                    if (g_MenuPendingCourseIndex < 0) {
{
                            s32 llap;
                            s32 lprev;
                            s32 lu;
                            s32 lt;
                            s32 lbase;
                            PlaySoundCue(8);
                            lu = 0x7A120;
                            llap = g_CourseIndex;
                            lprev = g_MenuViewAngleTarget;
                            lt = g_MenuViewAngle;
                            lbase = D_8009B364;
                            g_MenuViewAngleTarget = 0xF4240;
                            g_CourseSwapDelay = 0;
                            g_MenuCourseModelIndex = llap;
                            llap = llap + 1;
                            lprev = lprev - lt;
                            lu = lu - lprev;
                            D_8009B360 = (D_8009B360 - lbase) + 0x1F4000;
                            g_CourseIndex = llap;
                            g_MenuPendingCourseIndex = llap;
                            g_MenuViewAngle = lu;
                            D_8009B368 = g_CourseProgress[llap & 3];
                            g_TimeAttackPlateStep = (llap < 4) ? -1 : 1;
                        }
                    }
                }
            }
            if (g_PadEdge2 & 0x860) {
                sel = g_CourseSelectOption;
                if (sel == 0) {
                    PlaySoundCue(2);
                    GameMenuBusy = 1;
                    g_MenuOverlayPattern = 1;
                    g_TimeAttackPlateStep = -1;
                    g_MenuViewOffsetTarget = 0x3D090;
                    D_8009B368 = 0;
                    D_8009B360 = (D_8009B360 - D_8009B364) + 0x1F4000;
                } else if (sel == 2) {
                    if (g_GrandPrixMode != 0) {
                        u16 hv;
                        PlaySoundCue(2);
                        hv = 0;
                        if (g_GrandPrixClass < 5) {
                            hv = g_GrandPrixSeriesU16;
                        }
                        D_8019C764 = &D_80082604;
                        GameMenuBusy = -1;
                        g_GrandPrixSeriesU16 = hv;
                        g_UiScriptProgress2 = 0;
                        g_MenuSubCursor = 1;
                    } else {
                        PlaySoundCue(3);
                        func_8005E8E0();
                        g_MenuHintBarStep = -1;
                        g_TimeAttackPlateStep = -1;
                        g_MenuViewOffsetTarget = 0x3D090;
                        GameMenuBusy = sel;
                        D_8009B368 = 0;
                        g_GrandPrixSeriesU16 = g_CourseIndex >> 2;
                        D_8009B360 = (D_8009B360 - D_8009B364) + 0x1F4000;
                    }
                } else {
                    PlaySoundCue(2);
                    if (g_GrandPrixMode != 0) {
                        D_8019C764 = &D_800825A4;
                        GameMenuBusy = -2;
                        g_UiScriptProgress2 = 0;
                        g_MenuSubCursor = g_GrandPrixClass;
                    } else {
                        GameMenuBusy = 3;
                        g_MenuOverlayPattern = 1;
                        g_TimeAttackPlateStep = -1;
                    }
                }
            }
        }
    } else if (state < 0) {
        if (state == -1) {
            u16 *pad;
            func_800487D8(&D_800827FC, &g_UiScriptProgress2, 0);
            func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
            if (func_800487D8(D_8019C764, &g_UiScriptProgress2, 1) != 0) {
                if (g_PadEdge2 & 0x860) {
                    PlaySoundCue((g_MenuSubCursor != 0) ? 2 : 3);
                    GameMenuBusy = -3;
                    g_MenuConfirmTimer = 0x23;
                }
                pad = (u16 *)&g_PadEdge2;
                if (*pad & 0x90) {
                    PlaySoundCue(3);
                    GameMenuBusy = -4;
                }
                if (*pad & 0x8000) {
                    PlaySoundCue(1);
                    g_MenuSubCursor = 1;
                }
                if (*pad & 0x2000) {
                    PlaySoundCue(1);
                    g_MenuSubCursor = 0;
                }
                func_80048D64((g_MenuSubCursor != 0) ? 0xB8 : 0xDA, 0x8C, 0x20, 0x20, 0);
                func_80046A2C(ot, 0xC0, 0x94, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80046A2C(ot, 0xE3, 0x94, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80048B88(0xB8, 0x8C, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32)&g_MenuBlankCaption);
                func_80048B88(0xDA, 0x8C, 0x20, 0x20, 0x1E, 0x4E, 0x95, 0, 0, 0, (s32)&g_MenuBlankCaption);
            }
        } else if (state == -2) {
            u16 *pad;
            if (func_800487D8(D_8019C764, &g_UiScriptProgress2, 1) != 0) {
                if (g_PadEdge2 & 0x860) {
                    PlaySoundCue(2);
                    if (g_MenuSubCursor == g_GrandPrixClass) {
                        GameMenuBusy = 0;
                    } else {
                        GameMenuBusy = -5;
                        D_8009B310 = 0;
                        g_MenuConfirmTimer = 0x23;
                        DrawClassChangeCurtain(0);
                    }
                }
                pad = (u16 *)&g_PadEdge2;
                if (*pad & 0x90) {
                    PlaySoundCue(3);
                    GameMenuBusy = 0;
                }
                if (*pad & 0x1000) {
                    PlaySoundCue(1);
                    g_MenuSubCursor = (g_MenuSubCursor != 0) ? g_MenuSubCursor - 1 : g_RaceProgress->maxClassReached;
                }
                if (g_PadEdge2 & 0x4000) {
                    PlaySoundCue(1);
                    g_MenuSubCursor = (g_MenuSubCursor < g_RaceProgress->maxClassReached) ? g_MenuSubCursor + 1 : 0;
                }
                func_80048D64(0xB8, g_MenuSubCursor * 0x1E + 0x6C, 0x38, 0x20, 0);
                for (i = 0; i < g_RaceProgress->maxClassReached + 1; i++) {
                    func_80046A2C(ot, 0xC0, i * 0x1E + 0x74, 0x1A, 0x10, 0x60, 0xCC, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80046A2C(ot, 0xE0, i * 0x1E + 0x74, 8, 0x10, i * 8 + 8, 0x18, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80048B88(0xB8, i * 0x1E + 0x6C, 0x38, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32)&g_MenuBlankCaption);
                }
            }
        } else if (state == -3) {
            cnt = g_MenuConfirmTimer;
            if (cnt <= 0) {
                func_800487D8(&D_800827FC, &g_UiScriptProgress2, -1);
                func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
                func_800487D8(D_8019C764, &g_UiScriptProgress2, 0);
                if (g_UiScriptProgress2 <= 0) {
                    func_8005E8E0();
                    GameMenuBusy = (g_MenuSubCursor != 0) ? 4 : 2;
                    g_MenuHintBarStep = -1;
                    g_MenuViewOffsetTarget = 0x3D090;
                    D_8009B368 = 0;
                    D_8009B360 = (D_8009B360 - D_8009B364) + 0x1F4000;
                }
            } else {
                g_MenuConfirmTimer = cnt - 1;
                func_800487D8(&D_800827FC, &g_UiScriptProgress2, 0);
                func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
                func_800487D8(D_8019C764, &g_UiScriptProgress2, 1);
                func_80048D64((g_MenuSubCursor != 0) ? 0xB8 : 0xDA, 0x8C, 0x20, 0x20, 1);
                func_80046A2C(ot, 0xC0, 0x94, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80046A2C(ot, 0xE3, 0x94, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
                func_80048B88(0xB8, 0x8C, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32)&g_MenuBlankCaption);
                func_80048B88(0xDA, 0x8C, 0x20, 0x20, 0x1E, 0x4E, 0x95, 0, 0, 0, (s32)&g_MenuBlankCaption);
            }
        } else if (state == -4) {
            func_800487D8(&D_800827FC, &g_UiScriptProgress2, -1);
            func_800487D8(&g_UiChromeScript2, &g_UiScriptProgress2, 0);
            func_800487D8(D_8019C764, &g_UiScriptProgress2, 0);
            if (g_UiScriptProgress2 <= 0) {
                GameMenuBusy = 0;
            }
        } else if (state == -5) {
            cnt = g_MenuConfirmTimer;
            if (cnt <= 0) {
                if (D_8009B310 != 0) {
                    if (DrawClassChangeCurtain(-1) == 0) {
                        GameMenuBusy = 0;
                        g_UiScriptProgress2 = 0;
                    }
                } else {
                    if (DrawClassChangeCurtain(1) >= 0x19) {
                        D_8009B310 = 1;
                        g_GrandPrixClass = g_MenuSubCursor;
                        ResetCourseProgress(g_MenuSubCursor);
                        g_MenuViewAngle = 0x7A120;
                        g_MenuViewAngleTarget = 0x7A120;
                        g_CourseSelectOption = 0;
                        g_MenuPendingCourseIndex = -1;
                        D_8009B360 = 0;
                        g_CourseIndex = g_CourseIndex & ~3;
                        g_MenuCourseModelIndex = g_CourseIndex;
                        D_8009B368 = g_CourseProgress[0];
                    }
                    func_800487D8(D_8019C764, &g_UiScriptProgress2, 1);
                    func_80048D64(0xB8, g_MenuSubCursor * 0x1E + 0x6C, 0x38, 0x20, 1);
                    for (i = 0; i < g_RaceProgress->maxClassReached + 1; i++) {
                        func_80046A2C(ot, 0xC0, i * 0x1E + 0x74, 0x1A, 0x10, 0x60, 0xCC, 0, 0, 0, 0x244, 1, 1, 0x3B);
                        func_80046A2C(ot, 0xE0, i * 0x1E + 0x74, 8, 0x10, i * 8 + 8, 0x18, 0, 0, 0, 0x244, 1, 1, 0x3B);
                        func_80048B88(0xB8, i * 0x1E + 0x6C, 0x38, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32)&g_MenuBlankCaption);
                    }
                }
            } else {
                g_MenuConfirmTimer = cnt - 1;
                func_800487D8(D_8019C764, &g_UiScriptProgress2, 1);
                func_80048D64(0xB8, g_MenuSubCursor * 0x1E + 0x6C, 0x38, 0x20, 1);
                for (i = 0; i < g_RaceProgress->maxClassReached + 1; i++) {
                    func_80046A2C(ot, 0xC0, i * 0x1E + 0x74, 0x1A, 0x10, 0x60, 0xCC, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80046A2C(ot, 0xE0, i * 0x1E + 0x74, 8, 0x10, i * 8 + 8, 0x18, 0, 0, 0, 0x244, 1, 1, 0x3B);
                    func_80048B88(0xB8, i * 0x1E + 0x6C, 0x38, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32)&g_MenuBlankCaption);
                }
            }
        }
        res = CanSelectPrevCourse();
        func_80049418(1, 1, res, CanSelectNextCourse());
        func_800489AC(g_UiScriptProgress, 2, g_CourseSelectOption);
        func_800487D8(hdr, &g_UiScriptProgress, 0);
        func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 1);
        func_8004CF30(7);
    } else {
        g_MenuHandlerIndex = -1;
        g_MenuHandlerIndex2 = 1;
        res = CanSelectPrevCourse();
        func_80049418(-1, 1, res, CanSelectNextCourse());
        func_800487D8(hdr, &g_UiScriptProgress, -1);
        func_800487D8(&g_UiChromeScript, &g_UiScriptProgress, 0);
        func_800489AC(g_UiScriptProgress, 2, g_CourseSelectOption);
        func_8004CF30(-9);
        if (g_UiScriptProgress <= 0) {
            switch (GameMenuBusy) {
            case 1:
                if (g_MenuViewOffset > 0x3D08F) {
                    g_MenuScreen = 3;
                    g_MenuHandlerIndex = 4;
                    func_8004F3EC(0, 0);
                    func_80049418(0, 0, 0, 0);
                    g_CarSwapToIndex = -1;
                    g_MenuViewAngle = 0;
                    g_MenuViewAngleTarget = 0;
                    g_MenuViewOffset = 0x3D090;
                    g_MenuViewOffsetTarget = 0;
                    g_CarSwapFromIndex = g_PlayerCarIndex;
                    goto clear;
                }
                break;
            case 2:
                if ((D_8009B348 <= 0) && (g_MenuViewOffset > 0x3D08F)) {
                    s32 raw;
                    s32 d;
                    s32 lapc;
                    s32 half;
                    raw = g_CourseIndex;
                    p = g_RaceProgress;
                    d = g_PlayerCarIndex;
                    g_SceneId = 2;
                    lapc = g_GrandPrixClass;
                    half = g_GrandPrixMode;
                    raw = raw & 3;
                    g_CourseIndex = raw;
                    p->course = raw;
                    p->carIndex = d;
                    p->classIndex = lapc;
                    if (half != 0) {
                        p->unk10 = g_PlayerMoney;
                        goto clear;
                    }
                    goto setlast;
                }
                break;
            case 3:
                g_MenuScreen = 2;
                g_MenuHandlerIndex = 2;
                goto clear;
            case 4:
                if ((D_8009B348 <= 0) && (g_MenuViewOffset > 0x3D08F)) {
                    s32 raw;
                    s32 d;
                    s32 lapc;
                    s32 half;
                    g_SceneId = 0x18;
                    raw = g_CourseIndex & 3;
                    g_RaceProgress->course = (g_CourseIndex = raw);
                    d = g_PlayerCarIndex;
                    lapc = g_GrandPrixClass;
                    half = g_GrandPrixMode;
                    g_RaceProgress->carIndex = d;
                    g_RaceProgress->classIndex = lapc;
                    if (half != 0) {
                        g_RaceProgress->unk10 = g_PlayerMoney;
                    } else {
                        p = g_RaceProgress;
                    setlast:
                        p->unk10 = (s16)g_GrandPrixSeriesU16;
                    }
                clear:
                    g_UiScriptProgress = 0;
                    GameMenuBusy = 0;
                }
                break;
            default:
                goto clear;
            }
        }
    }
}

extern s32 D_8009B2C4;

s32 DrawRankingScreen(s32 step) asm("func_80054C84");
s32 DrawRankingScreen(s32 arg0) {
    s32 value;

    if (arg0 == 0) {
        D_8009B2C4 = 0;
        return;
    }

    if (arg0 > 0) {
        value = arg0 + D_8009B2C4;
        D_8009B2C4 = value;
        if (value >= 0x1FD) {
            D_8009B2C4 = 0x1FC;
        }
    } else {
        value = arg0 + D_8009B2C4;
        D_8009B2C4 = value;
        if (value < 0) {
            D_8009B2C4 = 0;
        }
    }

    return D_8009B2C4;
}
