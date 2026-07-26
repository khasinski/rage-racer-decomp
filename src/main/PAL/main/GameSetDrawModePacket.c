#include "common.h"
#include "psyq/gpu.h"

extern u8 D_8007BED0[];

/* Local alias: retail passes the tpage through as a full word, so the narrow
 * parameter in the shared prototype would insert a truncation. */
void SetDrawModeWide(DrawPacket *pkt, s32 a, s32 b, s32 tpage, void *tw) asm("func_800666F4");

/* Fills a DR_MODE packet in place; unlike GameQueueDrawModePrim it neither
 * links the packet nor advances the scratchpad cursor. No callers in retail. */
void GameSetDrawModePacket(u8 *prim, s32 tpage) {
    SetDrawModeWide((DrawPacket *)prim, 0, 1, tpage, D_8007BED0);
}
