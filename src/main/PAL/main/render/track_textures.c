#include "common.h"
#include "psyq/gpu.h"
#include "game/state.h"
#include "game/random.h"
#include "game/asset.h"
#include "game/render.h"
#include "psyq/kernel.h"

/*
 * Old-style definition on purpose: SetTrackTexturePageNow and RequestTrackTexturePage call this
 * with no argument at all (the original relied on whatever was left in $4), so
 * the unit must not expose a prototype for it.
 */
s32 SelectTrackTexturePage(section)
s32 section;
{
    s32 ret;
    s32 one;

    ret = 0;
    switch (0) { default:
    if (section >= g_TrackTextureSectionLo) {
        ret = 0x100;
        if (section >= g_TrackTextureSectionHi) {
            ret = 0;
        } else {
            one = 1;
            g_TrackTexturePageWanted = one;
            break;
        }
    }
    g_TrackTexturePageWanted = 0;
    }
    return ret;
}

void SwapTrackTexturePageNow(void) {
    s32 buffer[0xE0];
    s32 page = 0;
    s16 *rectY = &g_TrackTextureRowRectY;
    Rect *rect = (Rect *)((u8 *)&g_TrackTextureRowRectY - 2);
    u8 **basePtr = &g_TrackTextureShadow;
    s32 value;
    s32 *src;
    s32 *dst;
    s32 count;

    do {
        *rectY = page + 0x100;
        value = 1 - g_TrackTextureShadowPage[page];
        if (g_TrackTextureShadowPage[page] == g_TrackTexturePageWanted) {
            StoreImage(rect, buffer);
            DrawSync(0);
            LoadImage(rect, *basePtr + (((page * 7) << 7)));
            DrawSync(0);

            src = buffer;
            dst = (s32 *)(*basePtr + (((page * 7) << 7)));
            count = 0;
            do {
                *dst++ = *src++;
                count++;
            } while (count < 0xE0);

            g_TrackTextureShadowPage[page] = value;
        }
        page++;
    } while (page < 0x100);
}

void SetTrackTexturePageNow(s32 trackSection) {
    s32 temp;

    temp = SelectTrackTexturePage();
    g_TrackTextureTargetRow = temp;
    g_TrackTextureCursorRow = temp;
    SwapTrackTexturePageNow();
}

void ResetTrackTextureSwap(void) {
    s32 value = 1;
    s32 i = 0xFF;
    u8 *ptr = &g_TrackTextureShadowPageLast;

    do {
        *ptr = value;
        i--;
        ptr--;
    } while (i >= 0);

    g_TrackTexturePageWanted = 0;
    g_TrackTextureTargetRow = 0;
    g_TrackTextureCursorRow = 0;
}

void RequestTrackTexturePage(void) {
    g_TrackTextureTargetRow = SelectTrackTexturePage();
}

void SwapTrackTextureRow(void) {
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

    rectY = &g_TrackTextureRowRectY;
    *rectY = (u16)g_TrackTextureCursorRow + 0x100;
    one = 1;
    value = one - g_TrackTextureShadowPage[g_TrackTextureCursorRow];
    if (g_TrackTextureShadowPage[g_TrackTextureCursorRow] == g_TrackTexturePageWanted) {
        rectY = (s16 *)((u8 *)rectY - 2);
        rect = (Rect *)rectY;
        StoreImage(rect, buffer);
        DrawSync(0);

        index = g_TrackTextureCursorRow;
        rect = (Rect *)rectY;
        basePtr = &g_TrackTextureShadow;
        LoadImage(rect, *basePtr + (((index * 7) << 7)));
        DrawSync(0);

        src = buffer;
        copyIndex = g_TrackTextureCursorRow;
        count = 0;
        copyOffset = copyIndex * 7;
        dst = (s32 *)(*basePtr + (copyOffset << 7));
        do {
            *dst++ = *src++;
            count++;
        } while (count < 0xE0);

        g_TrackTextureShadowPage[g_TrackTextureCursorRow] = value;
    }
}

void StepTrackTextureSwap(void) {
    while (g_TrackTextureCursorRow != g_TrackTextureTargetRow) {
        if (VSync(1) >= 471) {
            break;
        }

        if (g_TrackTextureCursorRow < g_TrackTextureTargetRow) {
            SwapTrackTextureRow();
            g_TrackTextureCursorRow++;
        } else {
            g_TrackTextureCursorRow--;
            SwapTrackTextureRow();
        }
    }
}

s32 CycleBgmSelectCameraCar(s32 mask, s32 current) {
    s32 random;
    s32 candidate;
    s32 offset;
    s32 first;

    if (mask & g_SceneTimer) {
        return current;
    }
    if ((g_TrackTextureCursorRow == 0) || (g_TrackTextureCursorRow == 0x100)) {
        random = Random15() & 0x7FFF;
        candidate = random % 11;

        offset = (((((current * 3) * 4) + current) * 8) - current) * 4;
        first = SelectTrackTexturePage(*(s16 *)&g_CarTrackSection[offset]);

        offset = (((((candidate * 3) * 4) + candidate) * 8) - candidate) * 4;
        if (first == SelectTrackTexturePage(*(s16 *)&g_CarTrackSection[offset])) {
            return candidate;
        }
    }
    return current;
}

s32 CycleAttractCameraCar(s32 mask, s32 current) {
    s32 random;
    s32 candidate;
    s32 offset;
    s32 first;

    if (mask & g_SceneTimer) {
        return current;
    }
    if ((g_TrackTextureCursorRow == 0) || (g_TrackTextureCursorRow == 0x100)) {
        random = Random15() & 0x7FFF;
        candidate = random % 4;

        offset = (((((current * 3) * 4) + current) * 8) - current) * 4;
        first = SelectTrackTexturePage(*(s16 *)&g_CarTrackSection[offset]);

        offset = (((((candidate * 3) * 4) + candidate) * 8) - candidate) * 4;
        if (first == SelectTrackTexturePage(*(s16 *)&g_CarTrackSection[offset])) {
            return candidate;
        }
    }
    return current;
}
