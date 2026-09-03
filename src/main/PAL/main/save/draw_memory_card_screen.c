#include "common.h"
#include "asm_macros.h"
#include "game/prim.h"
#include "game/render_internal.h"
#include "game/memcard.h"
#include "game/menu.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/state.h"

#ifndef DRAW_MEMORY_CARD_ONLY_MESSAGE

void DrawMemoryCardScreen(s32 showBar, s32 variant, s32 cursor, s32 barRow)
{
    u8 *base = g_DrawBuffer + 0xCC;
    u8 **scratch = &SCRATCH_PRIM_CURSOR_AS(u8);
    u8 *next;
    s32 i;
    s32 y;

    next = GameQueueSpriteTrans(base, *scratch, 0x24, 0x38, 0x20, 0x18, 0xA0, 0x90, 0x7F40);
    if (variant != 0) {
        next = GameQueueSpriteTrans(base, next, 0x24, 0x58, 0x24, 0x18, 0xCC, 0x90, 0x7F40);
    }
    if (variant != 0) {
        y = 0x78;
    } else {
        y = 0x58;
    }
    next = GameQueueSpriteTrans(base, next, 0x24, y, 0x1C, 0x18, 0xD0, 0x60, 0x7F40);
    next = GameQueueSpriteTrans(base, next, 0x48, 0xB8, 0x10, 0x10, 0, 0xC8, 0x7F40);
    next = GameQueueSpriteTrans(base, next, 0x68, 0xB8, 0x34, 0x10, 0x10, 0xC8, 0x7F40);
    next = GameQueueSpriteTrans(base, next, 0xB0, 0xB8, 0x14, 0x10, 0x44, 0xC8, 0x7F40);
    *scratch = next;
    DrawMenuCursorArrow(0x14, (cursor * 32) + 0x38);
    DrawOptionHintBar(variant + 5);
    DrawPadTypeHint();

    base = g_DrawBuffer + 0xD8;
    next = AddTilePrim(base, *scratch, 0x5D, 0x3C, 0xE4, 0x40, 0, 0, 0);
    next = AddTilePrim(base, next, 0x5C, 0x3A, 0xE5, 0x44, 0xFF, 0xFF, 0xFF);
    for (i = 0; i < 3; i++) {
        next = DrawShadowedTile(base, next, 0x3E, 0xD0 + i * 0x30);
    }

    if (showBar != 0) {
        next = AddTilePrim(base, next, 0x3C, ((barRow * 3) << 4) + 0xCC, 0xC8, 0x28, 0x89, 0xFF, 0x76);
    }
    next = AddTilePrim(base, next, 0, 0, 0x140, 0xF0, 0x85, 0x15, 0xE);
    SCRATCH_PRIM_CURSOR_AS(u8) = next;
}

#else

void DrawMemoryCardMessage(s32 message) {
#ifndef MEMORY_CARD_MESSAGES_JAPAN
    s32 index;
    MemoryCardMessageRow *entry;
    s32 x;
    s32 y;
    s16 *table;
    s32 one;
    u8 code;
    u8 *next;
    u8 *base;
    u32 messageRange;
    u32 delta;

    index = message;
    entry = g_McMessageRows[index];
    x = 0x60;
    y = 0x40;
    messageRange = index - 0x10;
    if (messageRange >= 2 && index != 0x12) {
        one = 1;
        table = g_McMessageColumnX;
        code = 1;
        do {
            if (code != one) {
                x = table[code];
                y = 0x60;
            }
            DrawSpriteString(x, y, entry->text, 0x7F81);
            code = entry->column;
            entry++;
        } while (code != 0);
    }

    base = g_DrawBuffer + 0xCC;
    next = SCRATCH_PRIM_CURSOR_AS(u8);
    if (index == 6 || index == 8 || index == 0xA || index == 0xC) {
        next = GameQueueSprite(base, next, 0xDE, 0x60, 0xC, 0x18, 0x84, 0x48, 0x7F81);
    }
    if (index == 7 || index == 9 || index == 0xB || index == 0xD) {
        next = GameQueueSprite(base, next, 0xAC, 0x60, 0xC, 0x18, 0x84, 0x48, 0x7F81);
    }
    if (index == 5 && (g_SceneTimer & 0x10) != 0) {
        next = GameQueueSprite(base, next, 0x108, 0x60, 0xC, 0x18, 0x90, 0x48, 0x7F81);
    }
    delta = index - 0x10;
    if (delta < 2 || index == 0x12) {
        next = GameQueueSprite(base, next, x, y, 0x6C, 0x18, 0, delta * 0x18, 0x7F81);
        next = QueueDrawModePrim(base, next, 0x3F);
    } else {
        next = QueueDrawModePrim(base, next, 0x3D);
    }
    SCRATCH_PRIM_CURSOR_AS(u8) = next;
#else
    typedef struct JapaneseMemoryCardGlyph {
        u8 u;
        u8 v;
        u8 width;
        u8 column;
    } JapaneseMemoryCardGlyph;
    typedef union MemoryCardMessageAddress {
        MemoryCardMessageRow *row;
        JapaneseMemoryCardGlyph *glyph;
    } MemoryCardMessageAddress;
    s32 index;
    MATCH_REGISTER(JapaneseMemoryCardGlyph *, entry, "$17");
    MATCH_REGISTER(u8 *, column, "$16");
    s32 x;
    s32 y;
    s16 *table;
    u8 code;
    u8 *next;
    u8 *base;
    u32 delta;
    MemoryCardMessageAddress address;

    index = message;
    x = 0x60;
    y = 0x40;
    code = 1;
    table = g_McMessageColumnX;
    next = SCRATCH_PRIM_CURSOR_AS(u8);
    base = g_DrawBuffer + 0xCC;
    address.row = g_McMessageRows[index];
    entry = address.glyph;
    column = &entry->column;
    KEEP_REGISTER(column);
    do {
        if (code != 1) {
            x = table[code];
            y = 0x60;
        }
        next = GameQueueSprite(
            base, next, x, y, *(column - 1), 0x18,
            (entry++)->u, *(column - 2), 0x7F81);
        code = *column;
        x += *(column - 1);
        column += sizeof(*entry);
    } while (code != 0);

    if (index == 6 || index == 8 || index == 0xA || index == 0xC) {
        next = GameQueueSprite(base, next, 0xA8, 0x60, 0xC, 0x18, 0xF4, 0x30, 0x7F81);
    }
    if (index == 7 || index == 9 || index == 0xB || index == 0xD) {
        next = GameQueueSprite(base, next, 0x6C, 0x60, 0xC, 0x18, 0xF4, 0x30, 0x7F81);
    }
    if (index == 5 && (g_SceneTimer & 0x10) != 0) {
        next = GameQueueSprite(base, next, 0x108, 0x60, 0xC, 0x18, 0xF4, 0x18, 0x7F81);
    }
    delta = index - 0x10;
    if (delta < 2 || index == 0x12) {
        next = QueueDrawModePrim(base, next, 0x3F);
    } else {
        next = QueueDrawModePrim(base, next, 0x3D);
    }
    SCRATCH_PRIM_CURSOR_AS(u8) = next;
#endif
}

#endif
