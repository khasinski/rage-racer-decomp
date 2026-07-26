#include "common.h"
#include "game/render.h"
#include "psyq/gpu.h"

extern u8 D_8007BED0[];

/* DR_MODE, 12 bytes: sets the texture page (and the blend mode packed into it)
 * for the primitives that follow, links it into the ordering table and returns
 * the advanced packet cursor. */
u8 *GameQueueDrawModePrim(void *ot, u8 *prim, u16 tpage) {
    u8 *pkt;

    SetDrawMode((DrawPacket *)prim, 0, 1, tpage, D_8007BED0);
    pkt = prim;
    prim += 12;
    AddPrim(ot, pkt);
    return prim;
}
