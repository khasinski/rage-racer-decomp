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
}
