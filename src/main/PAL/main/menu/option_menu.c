#include "common.h"
#include "game/audio.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/random.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/state.h"
#include "game/vector.h"

s32 QueueSpriteTransWide(s32 ot, s32 prim, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 clutIndex) asm("GameQueueSpriteTrans");
s32 GameQueueDrawModePrimWide(s32 ot, s32 prim, s32 tpage) asm("QueueDrawModePrim");
void DrawMenuCursorArrow(s32 x, s32 y);
extern s32 g_ScreenOffsetX;
extern s32 g_ScreenOffsetY;
extern ScoreRecord g_ClassRecords[];
extern ClassRecordSprite D_8007D5D4[];
void StartOptionMenuExit(s32 scene);
s32 AddTilePrim(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
void DrawOptionHintBar(s32 variant);
s32 GameQueueShadedSpriteTransWide(s32 ot, s32 prim, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 clutIndex, s32 intensity) asm("GameQueueShadedSpriteTrans");

void DrawOptionRootMenu(void) {
    u8 *base = g_DrawBuffer;
    s32 h18 = 0x18;
    s32 h48 = 0x48;
    s32 color = 0x7F40;
    register s32 *scratch asm("$21") = &SCRATCH_PRIM_CURSOR_WORD;
    s32 tmp;
    s32 state;

    base += 0xCC;
    tmp = QueueSpriteTransWide((s32)base, *scratch, 0x24, 0x94, 0x3C, h18, 0, h48, color);
    tmp = QueueSpriteTransWide((s32)base, tmp, 0x24, 0xB4, 0x88, h18, 0x40, h48, color);
    tmp = QueueSpriteTransWide((s32)base, tmp, 0x24, 0xD4, 0x74, h18, 0, 0x60, color);
    tmp = QueueSpriteTransWide((s32)base, tmp, 0x24, 0xF4, 0x5C, h18, 0x74, 0x60, color);
    tmp = QueueSpriteTransWide((s32)base, tmp, 0x24, 0x114, 0x64, h18, 0, 0x78, color);
    tmp = QueueSpriteTransWide((s32)base, tmp, 0x24, 0x134, 0x1C, h18, 0xD0, 0x60, color);
    tmp = GameQueueDrawModePrimWide((s32)base, tmp, 0x3F);

    state = g_GameMode;
    *scratch = tmp;
    if (state == 1) {
        DrawMenuCursorArrow(0x14, (g_OptionMenuCursor * 32) + 0x94);
    }
}

/* g_GameModeHandlers[1]: the six-row root menu and where each row goes. */
void UpdateOptionRootMenu(void) {
    s32 old;
    s32 value;
    s32 buttons;

    DrawOptionRootMenu();

    old = g_OptionMenuCursor;
    if (g_PadPressed & PAD_UP) {
        g_OptionMenuCursor = old - 1;
    } else if (g_PadPressed & PAD_DOWN) {
        g_OptionMenuCursor = old + 1;
    }

    g_OptionMenuCursor = (g_OptionMenuCursor + 6) % 6;
    if (old != g_OptionMenuCursor) {
        PlaySoundCue(1);
    }

    buttons = g_PadPressed;
    if (buttons & 0x860) {
        PlaySoundCue(2);
        switch (g_OptionMenuCursor) {
        case 0:
            g_GameMode = 2;
            g_ClassRecordMenuCursor = 0;
            g_ScreenOffsetEditY = 0;
            g_ScreenOffsetEditX = 0;
            break;
        case 1:
            BeginControllerConfig();
            g_GameMode = 7;
            break;
        case 2:
            g_GameMode = 4;
            g_SoundOptionCursor = 0;
            break;
        case 3:
            g_GrandPrixMode = 0;
            g_GrandPrixSeries = 0;
            g_GrandPrixClass = (Random15() & 0xFFF) % 5;
            value = Random15() & 0xFFF;
            g_CourseIndex = value % 4;
            if ((g_GrandPrixClass < 2) && (g_CourseIndex == 3)) {
                g_CourseIndex = (Random15() & 0xFFF) % 3;
            }
            RequestTrackLoad();
            StartOptionMenuExit(0x1B);
            break;
        case 4:
            g_GameMode = 6;
            g_ScreenOffsetEditX = g_ScreenOffsetX;
            g_ScreenOffsetEditY = g_ScreenOffsetY;
            break;
        case 5:
            StartOptionMenuExit(2);
            break;
        }
    } else {
        s32 masked;

        masked = buttons & 0x90;
        if (masked) {
            PlaySoundCue(3);
            StartOptionMenuExit(2);
        }
    }
}

void DrawClassRecordDetail(void) {
    s32 raw = (s32)g_DrawBuffer;
    s32 base = raw + 0xCC;
    s32 next = SCRATCH_PRIM_CURSOR_WORD;
    s32 idx = g_ScreenOffsetEditY * 6 + g_ScreenOffsetEditX;
    s32 x;
    s32 y = 0x38;
    s32 i;

    if (g_GameMode == 3) {
        next = AddTilePrim(raw + 0xD4, next,
                             D_8007D5A8[idx].vx - 2, D_8007D5A8[idx].vy - 4,
                             0x24, 0x58, 0x89, 0xFF, 0x76);
    }
    next = QueueSpriteTransWide(base, next, 0xBC, 0x40, 0x18, 0x10, 0, 0x6C, 0x7F40);
    next = QueueSpriteTransWide(base, next, 0xD8, 0x40, 8, 0x10, g_ScreenOffsetEditX * 8 + 8, 0x18, 0x7F40);

    x = 0xB4;
    if (g_ClassRecords[idx].place == -1) {
        for (i = 0; i < 8; i++) {
            next = QueueSpriteTransWide(base, next, x + 0x30 + i * 8, y + 8, 8, 0x10, 0x38, 0x28, 0x7F40);
        }
    } else {
        next = QueueSpriteTransWide(base, next, 0xE4, 0x40,
                             D_8007D658[idx].b, 0x10,
                             D_8007D658[idx].r, D_8007D658[idx].g, 0x7F40);
    }

    next = QueueSpriteTransWide(base, next, x | 8, y + 0x28, 0x44, 0x10, 0x1C, 0x6C, 0x7F40);
    next = QueueSpriteTransWide(base, next, x + 100, y + 0x28, 8, 0x10,
                         (s16)((s16)g_ClassRecords[idx].clears / 10) << 3, 0x18, 0x7F40);
    next = QueueSpriteTransWide(base, next, x + 108, y + 0x28, 8, 0x10,
                         (s16)((s16)g_ClassRecords[idx].clears % 10) << 3, 0x18, 0x7F40);
    next = GameQueueDrawModePrimWide(base, next, 0x3B);
    next = AddTilePrim(base, next, x + 78, y + 47, 0x14, 2, 0xFF, 0xFF, 0xFF);
    next = AddTilePrim(base, next, x, y, 0x7C, 0x1E, 0, 0, 0);
    next = AddTilePrim(base, next, x, y + 32, 0x7C, 0x1E, 0, 0, 0);
    SCRATCH_PRIM_CURSOR_WORD = AddTilePrim(base, next, x - 1, y - 2, 0x7E, 0x42, 0xFF, 0xFF, 0xFF);
}

void DrawClassRecordGrid(void) {
    s32 base;
    s32 next;
    s32 i;
    s32 x, y;
    s32 flag;

    base = (s32)g_DrawBuffer + 0xCC;
    next = SCRATCH_PRIM_CURSOR_WORD;
    next = QueueSpriteTransWide(base, next, 0x24, 0x38, 0x24, 0x18, 0x38, 0x90, 0x7F40);
    next = QueueSpriteTransWide(base, next, 0x24, 0x58, 0x1C, 0x18, 0xD0, 0x60, 0x7F40);
    SCRATCH_PRIM_CURSOR_WORD = next;
    DrawMenuCursorArrow(0x14, (g_ClassRecordMenuCursor * 32) + 56);
    next = SCRATCH_PRIM_CURSOR_WORD;

    for (i = 0; i < 11; i++) {
        x = D_8007D5A8[i].vx;
        y = D_8007D5A8[i].vy;
        flag = g_ClassRecords[i].place;
        switch (flag) {
        case 1:
            next = (s32)GameQueueSprite(base, next, x, y, 0x20, 0x50,
                                 D_8007D5D4[i].u1, D_8007D5D4[i].v1, D_8007D5D4[i].clut1);
            break;
        case 2:
            next = (s32)GameQueueSprite(base, next, x, y, 0x20, 0x50,
                                 D_8007D5D4[i].u2, D_8007D5D4[i].v2, D_8007D5D4[i].clut2);
            break;
        case 3:
            next = (s32)GameQueueSprite(base, next, x, y, 0x20, 0x50,
                                 D_8007D5D4[i].u2, D_8007D5D4[i].v2, D_8007D5D4[i].clut3);
            break;
        }
        if (g_ClassRecords[i].place <= 0) {
            next = (s32)GameQueueSprite(base + 4, next, x, y, 0x20, 0x50, 0x60, 0x70, 0x7E80);
        } else {
            next = (s32)GameQueueSprite(base + 4, next, x, y, 0x20, 0x50, 0x80, 0x70, 0x7E81);
        }
    }

    next = GameQueueDrawModePrimWide(base, next, 0x3E);
    next = GameQueueDrawModePrimWide(base + 4, next, 0x3C);
    SCRATCH_PRIM_CURSOR_WORD = next;
    DrawOptionHintBar(0);
}

/* g_GameModeHandlers[2]: two-row menu into the class-record grid. */
void UpdateClassRecordMenu(void) {
    u16 *buttonPtr;
    s32 oldCursor;
    u16 buttons;

    DrawClassRecordGrid();

    buttonPtr = &g_PadPressed;
    oldCursor = g_ClassRecordMenuCursor;
    buttons = *buttonPtr;
    if (buttons & 0x1000) {
        g_ClassRecordMenuCursor = oldCursor - 1;
    }
    if (buttons & 0x4000) {
        g_ClassRecordMenuCursor = g_ClassRecordMenuCursor + 1;
    }

    g_ClassRecordMenuCursor = (g_ClassRecordMenuCursor + 2) % 2;
    if (oldCursor != g_ClassRecordMenuCursor) {
        PlaySoundCue(1);
    }

    buttons = *buttonPtr;
    if (buttons & 0x860) {
        PlaySoundCue(2);
        if (g_ClassRecordMenuCursor != 0) {
            g_GameMode = 1;
        } else {
            g_GameMode = 3;
        }
    } else if (buttons & 0x90) {
        PlaySoundCue(3);
        g_GameMode = 1;
    }

    DrawClassRecordDetail();
}

/* g_GameModeHandlers[3]: moves the cursor over the eleven class cells. */
void UpdateClassRecordBrowse(void) {
    s32 oldCursor;
    s32 oldFlag;
    u16 b;
    DrawClassRecordGrid();
    oldCursor = g_ScreenOffsetEditX;
    oldFlag = g_ScreenOffsetEditY;
    if ((g_PadPressed & PAD_UP) && oldFlag == 1) {
        g_ScreenOffsetEditY = 0;
    }
    if ((g_PadPressed & PAD_DOWN) && g_ScreenOffsetEditY == 0) {
        g_ScreenOffsetEditY = 1;
    }
    b = g_PadPressed;
    if (b & 0x8000) {
        g_ScreenOffsetEditX = g_ScreenOffsetEditX - 1;
    }
    if (b & 0x2000) {
        g_ScreenOffsetEditX = g_ScreenOffsetEditX + 1;
    }
    {
        s32 c;
        c = g_ScreenOffsetEditX;
        c += 6;
        c = c % 6;
        g_ScreenOffsetEditX = c;
        if (c == 5) {
            g_ScreenOffsetEditY = 0;
        }
        if (oldCursor != c || oldFlag != g_ScreenOffsetEditY) {
            PlaySoundCue(1);
        }
    }
    if (g_PadPressed & (PAD_START | PAD_SQUARE | PAD_CROSS | PAD_CIRCLE | PAD_TRIANGLE)) {
        PlaySoundCue(2);
        g_GameMode = 2;
    }
    DrawClassRecordDetail();
}

void DrawVolumeBar(s32 arg0, s32 arg1) {
    s32 b = arg1;
    s32 base = (s32)g_DrawBuffer + 0xCC;
    s32 next;
    s32 i;
    s32 y;
    s32 c;

    next = QueueSpriteTransWide(base, SCRATCH_PRIM_CURSOR_WORD, 0x4E, b + 0xA, 0x10, 0xC, 0xB4, 0xC4, 0x7F40);
    next = QueueSpriteTransWide(base, next, 0xE4, b + 0xA, 0x10, 0xC, 0xC4, 0xC4, 0x7F40);
    next = GameQueueDrawModePrimWide(base, next, 0x3A);
    c = 0x46;
    i = 0;
    if (i <= arg0) {
        do {
            y = 0x1C + (i * 8);
            next = (s32)GameQueueSprite(base, next, c + y, b + 4, 4, 0x18, 0xFC, 0x40, 0x7E82);
            i++;
        } while (i <= arg0);
    }
    next = GameQueueDrawModePrimWide(base, next, 0x39);
    next = AddTilePrim(base, next, c | 1, b + 2, 0xB2, 0x1C, 0, 0, 0);
    SCRATCH_PRIM_CURSOR_WORD = AddTilePrim(base, next, c, b, 0xB4, 0x20, 0xFF, 0xFF, 0xFF);
}

void DrawSoundOptionScreen(void) {
    s32 base;
    s32 *scratch;
    s32 color;
    s32 s0;
    s32 s3;
    s32 n;

    DrawMenuCursorArrow(0x14, (g_SoundOptionCursor * 32) + 56);
    color = 0x7F;

    scratch = &SCRATCH_PRIM_CURSOR_WORD;
    s3 = 0x18;
    s0 = 0x78;
    base = (s32)g_DrawBuffer + 0xCC;

    n = *scratch;
    n = QueueSpriteTransWide(base, n, 0x24, 0x38, 0x2C, s3, 0x9C, s0, 0x7F40);
    n = QueueSpriteTransWide(base, n, 0x24, 0x58, s3, s3, 0xC8, s0, 0x7F40);
    n = QueueSpriteTransWide(base, n, 0x24, 0x78, 0x38, s3, 0, 0x90, 0x7F40);
    n = QueueSpriteTransWide(base, n, 0x24, 0x98, 0x1C, s3, 0xD0, 0x60, 0x7F40);
    *scratch = n;

    DrawOptionHintBar(2);

    n = *scratch;
    if (g_MonoOutput != 0) {
        color = 0x20;
    }

    n = GameQueueShadedSpriteTransWide(base, n, 0x66, 0x12A, s3, 0xC, 0xD4, 0xC4, 0x7F40, color);
    n = AddTilePrim(base, n, 0x47, 0x122, 0x56, 0x1C, 0x85, 0x15, 0xE);
    n = AddTilePrim(base, n, 0x46, 0x120, 0x58, 0x20, color * 2, color * 2, color * 2);

    color = 0x20;
    if (g_MonoOutput != 0) {
        color = 0x7F;
    }

    n = GameQueueShadedSpriteTransWide(base, n, 0xBA, 0x12A, 0x28, 0xC, 0xB4, 0xD0, 0x7F40, color);
    n = AddTilePrim(base, n, 0xA3, 0x122, 0x56, 0x1C, 0x85, 0x15, 0xE);
    n = AddTilePrim(base, n, 0xA2, 0x120, 0x58, 0x20, color * 2, color * 2, color * 2);
    {
        s32 a0v = g_BgmVolumeSetting;
        *scratch = n;
        DrawVolumeBar(a0v, 0xD0);
    }
    DrawVolumeBar(g_SfxVolumeSetting, 0xF8);

    if (g_GameMode != 5) {
        return;
    }

    n = *scratch;
    switch (g_SoundOptionCursor) {
    case 0:
        n = AddTilePrim(base, n, 0x44, 0xCC, 0xB8, 0x28, 0x89, 0xFF, 0x76);
        break;
    case 1:
        n = AddTilePrim(base, n, 0x44, 0xF4, 0xB8, 0x28, 0x89, 0xFF, 0x76);
        break;
    case 2:
        n = AddTilePrim(base, n, (g_MonoOutput != 0) ? 0xA0 : 0x44, 0x11C, 0x5C, 0x28, 0x89, 0xFF, 0x76);
        break;
    }
    SCRATCH_PRIM_CURSOR_WORD = n;
}

/* g_GameModeHandlers[4]: the four-row sound menu; confirm backs the setting up and enters mode 5. */
void UpdateSoundOptionMenu(void) {
    u16 *buttonsPtr;
    u16 buttons;
    s32 old;
    s32 index;

    DrawSoundOptionScreen();
    buttonsPtr = &g_PadPressed;
    buttons = *buttonsPtr;
    old = g_SoundOptionCursor;
    if (buttons & 0x1000) {
        g_SoundOptionCursor = old - 1;
    }
    if (buttons & 0x4000) {
        g_SoundOptionCursor++;
    }

    index = (g_SoundOptionCursor + 4) % 4;
    g_SoundOptionCursor = index;
    if (old != index) {
        PlaySoundCue(1);
    }

    buttons = *buttonsPtr;
    if (buttons & 0x860) {
        PlaySoundCue(2);
        g_GameMode = 5;
        switch (g_SoundOptionCursor) {
        case 0:
            g_ScreenOffsetEditX = g_BgmVolumeSetting;
            break;
        case 1:
            g_ScreenOffsetEditX = g_SfxVolumeSetting;
            break;
        case 2:
            g_ScreenOffsetEditX = g_MonoOutput;
            break;
        case 3:
            g_GameMode = 1;
            break;
        }
    } else if (buttons & 0x90) {
        PlaySoundCue(3);
        g_GameMode = 1;
    }
}
