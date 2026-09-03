#include "common.h"
#include "asm_macros.h"
#include "game/prim.h"
#include "game/render_internal.h"
#include "game/memcard.h"
#include "game/menu.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "game/state.h"


void DrawMemoryCardMessage(s32 message) {
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
}
