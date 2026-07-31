#include "common.h"
#include "psyq/gpu.h"
#include "game/state.h"

extern s32 g_TrackTextureSectionLo asm("D_801E4284");
extern s32 g_TrackTextureSectionHi asm("D_801E4288");
extern s32 g_TrackTexturePageWanted asm("D_801E6F10");
/*
 * Old-style definition on purpose: GameSetTrackTexturePageNow and GameRequestTrackTexturePage call this
 * with no argument at all (the original relied on whatever was left in $4), so
 * the unit must not expose a prototype for it.
 */
s32 GameSelectTrackTexturePage(arg0)
s32 arg0;
{
    s32 ret;
    s32 one;

    ret = 0;
    if (arg0 < g_TrackTextureSectionLo) {
        goto low;
    }

    ret = 0x100;
    if (arg0 >= g_TrackTextureSectionHi) {
        goto high;
    }

    one = 1;
    g_TrackTexturePageWanted = one;
    goto done;

high:
    ret = 0;
low:
    g_TrackTexturePageWanted = 0;

done:
    return ret;
}

extern s16 D_8007C70A;
extern u8 D_801E4BF8[];
extern u8 *g_TrackTextureShadow asm("D_801E42D0");
extern s32 g_TrackTexturePageWanted asm("D_801E6F10");
void StoreImage(Rect *rect, void *data) asm("func_80065B88");
void LoadImage(Rect *rect, void *data) asm("func_80065B24");
void DrawSync(long mode) asm("func_800658FC");

void GameSwapTrackTexturePageNow(void) asm("func_80019D7C");
void GameSwapTrackTexturePageNow(void) {
    s32 buffer[0xE0];
    s32 page = 0;
    s16 *rectY = &D_8007C70A;
    Rect *rect = (Rect *)((s32)&D_8007C70A - 2);
    u8 **basePtr = &g_TrackTextureShadow;
    s32 value;
    s32 *src;
    s32 *dst;
    s32 count;

    do {
        *rectY = page + 0x100;
        value = 1 - D_801E4BF8[page];
        if (D_801E4BF8[page] == g_TrackTexturePageWanted) {
            StoreImage(rect, buffer);
            DrawSync(0);
            LoadImage(rect, *basePtr + ((((page * 8) - page) << 7)));
            DrawSync(0);

            src = buffer;
            dst = (s32 *)(*basePtr + ((((page * 8) - page) << 7)));
            count = 0;
            do {
                *dst++ = *src++;
                count++;
            } while (count < 0xE0);

            D_801E4BF8[page] = value;
        }
        page++;
    } while (page < 0x100);
}

extern s32 g_TrackTextureTargetRow asm("D_801E8AF8");
extern s32 g_TrackTextureCursorRow asm("D_8019CA6C");
void GameSwapTrackTexturePageNow(void) asm("func_80019D7C");

void GameSetTrackTexturePageNow(void) asm("func_80019E84");
void GameSetTrackTexturePageNow(void) {
    s32 temp;

    temp = GameSelectTrackTexturePage();
    g_TrackTextureTargetRow = temp;
    g_TrackTextureCursorRow = temp;
    GameSwapTrackTexturePageNow();
}

extern u8 D_801E4CF7;
extern s32 g_TrackTexturePageWanted asm("D_801E6F10");
extern s32 g_TrackTextureTargetRow asm("D_801E8AF8");
extern s32 g_TrackTextureCursorRow asm("D_8019CA6C");
void GameResetTrackTextureSwap(void) asm("func_80019EBC");
void GameResetTrackTextureSwap(void) {
    s32 value = 1;
    s32 i = 0xFF;
    u8 *ptr = &D_801E4CF7;

    do {
        *ptr = value;
        i--;
        ptr--;
    } while (i >= 0);

    g_TrackTexturePageWanted = 0;
    g_TrackTextureTargetRow = 0;
    g_TrackTextureCursorRow = 0;
}

extern s32 g_TrackTextureTargetRow asm("D_801E8AF8");
void GameRequestTrackTexturePage(void) asm("func_80019EFC");
void GameRequestTrackTexturePage(void) {
    g_TrackTextureTargetRow = GameSelectTrackTexturePage();
}

extern s16 D_8007C70A;
extern s32 g_TrackTextureCursorRow asm("D_8019CA6C");
extern u8 D_801E4BF8[];
extern s32 g_TrackTexturePageWanted asm("D_801E6F10");
extern u8 *g_TrackTextureShadow asm("D_801E42D0");
void StoreImage(Rect *rect, void *data) asm("func_80065B88");
void LoadImage(Rect *rect, void *data) asm("func_80065B24");
void DrawSync(long mode) asm("func_800658FC");

void GameSwapTrackTextureRow(void) asm("func_80019F24");
void GameSwapTrackTextureRow(void) {
    s32 buffer[0xE0];
    s16 *rectY;
    s32 value;
    s32 one;
    s32 *dst;
    s32 *src;
    s32 count;
    u8 **basePtr;
    Rect *rect;
    s32 copyIndex;
    s32 copyOffset;
    s32 index;

    rectY = &D_8007C70A;
    *rectY = (u16)g_TrackTextureCursorRow + 0x100;
    one = 1;
    value = one - D_801E4BF8[g_TrackTextureCursorRow];
    if (D_801E4BF8[g_TrackTextureCursorRow] == g_TrackTexturePageWanted) {
        rectY = (s16 *)((u8 *)rectY - 2);
        rect = (Rect *)rectY;
        StoreImage(rect, buffer);
        DrawSync(0);

        index = g_TrackTextureCursorRow;
        rect = (Rect *)rectY;
        basePtr = &g_TrackTextureShadow;
        LoadImage(rect, *basePtr + ((((index * 8) - index) << 7)));
        DrawSync(0);

        src = buffer;
        copyIndex = g_TrackTextureCursorRow;
        count = 0;
        copyOffset = (copyIndex * 8) - copyIndex;
        dst = (s32 *)(*basePtr + (copyOffset << 7));
        do {
            *dst++ = *src++;
            count++;
        } while (count < 0xE0);

        D_801E4BF8[g_TrackTextureCursorRow] = value;
    }
}

extern s32 g_TrackTextureCursorRow asm("D_8019CA6C");
extern s32 g_TrackTextureTargetRow asm("D_801E8AF8");
void GameSwapTrackTextureRow(void) asm("func_80019F24");
s32 VSync(s32 mode) asm("func_8006DD30");

void GameStepTrackTextureSwap(void) asm("func_8001A030");
void GameStepTrackTextureSwap(void) {
    while (g_TrackTextureCursorRow != g_TrackTextureTargetRow) {
        if (VSync(1) >= 471) {
            break;
        }

        if (g_TrackTextureCursorRow < g_TrackTextureTargetRow) {
            GameSwapTrackTextureRow();
            g_TrackTextureCursorRow++;
        } else {
            g_TrackTextureCursorRow--;
            GameSwapTrackTextureRow();
        }
    }
}

extern s32 g_TrackTextureCursorRow asm("D_8019CA6C");
extern u8 g_CarTrackSection[] asm("D_801F18CC");

s32 GameRandom15(void) asm("func_800632B0");

s32 GameCycleBgmSelectCameraCar(s32 mask, s32 current) asm("func_8001A0E4");
s32 GameCycleBgmSelectCameraCar(s32 mask, s32 current) {
    s32 random;
    s32 candidate;
    s32 offset;
    s32 first;

    if (mask & g_SceneTimer) {
        return current;
    }
    if ((g_TrackTextureCursorRow == 0) || (g_TrackTextureCursorRow == 0x100)) {
        random = GameRandom15() & 0x7FFF;
        candidate = random % 11;

        offset = (((((current * 3) * 4) + current) * 8) - current) * 4;
        first = GameSelectTrackTexturePage(*(s16 *)&g_CarTrackSection[offset]);

        offset = (((((candidate * 3) * 4) + candidate) * 8) - candidate) * 4;
        if (first == GameSelectTrackTexturePage(*(s16 *)&g_CarTrackSection[offset])) {
            return candidate;
        }
    }
    return current;
}

extern s32 g_TrackTextureCursorRow asm("D_8019CA6C");
extern u8 g_CarTrackSection[] asm("D_801F18CC");

s32 GameRandom15(void) asm("func_800632B0");

s32 GameCycleAttractCameraCar(s32 mask, s32 current) asm("func_8001A1F0");
s32 GameCycleAttractCameraCar(s32 mask, s32 current) {
    s32 random;
    s32 candidate;
    s32 offset;
    s32 first;

    if (mask & g_SceneTimer) {
        return current;
    }
    if ((g_TrackTextureCursorRow == 0) || (g_TrackTextureCursorRow == 0x100)) {
        random = GameRandom15() & 0x7FFF;
        candidate = random % 4;

        offset = (((((current * 3) * 4) + current) * 8) - current) * 4;
        first = GameSelectTrackTexturePage(*(s16 *)&g_CarTrackSection[offset]);

        offset = (((((candidate * 3) * 4) + candidate) * 8) - candidate) * 4;
        if (first == GameSelectTrackTexturePage(*(s16 *)&g_CarTrackSection[offset])) {
            return candidate;
        }
    }
    return current;
}
