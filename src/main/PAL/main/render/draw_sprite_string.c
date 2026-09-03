#include <sys/types.h>

#include "common.h"
#include "game/prim.h"
#include "game/render.h"
#include "game/render_internal.h"
#include "game/render_types.h"
#include "game/scratchpad.h"
#include "psyq/gpu.h"

void DrawSpriteString(long x, long y, u_char *str, long clutIndex) {
    volatile SPRT *packet;
    long idx;
    u_char *next;
    register u_char *sr __asm("$21");
    register u_char *tableA __asm("$23");
    long ga;
    long gb;
    long w;
    volatile SPRT *oldPacket;
    u_char *otv;
    u_char *tableB;
    RenderBufferAddress packetAddress;

    sr = str;
    next = SCRATCH_PRIM_CURSOR_AS(u_char);
    if (*sr != 0) {
        tableA = g_SpriteFontU;
        tableB = g_SpriteFontV;
        packetAddress.bytes = next;
        packet = packetAddress.volatileSprite;
        do {
            idx = *sr++ - 0x20;
            if (idx != 0) {
                ga = tableA[idx * 2];
                gb = tableB[idx * 2];
                packetAddress.bytes = next;
                SetSprt(packetAddress.sprite);
                SetShadeTex(next, 1);
                next += 0x14;
                oldPacket = packet;
                packet->x0 = x;
                packet->y0 = y;
                packet->u0 = ga;
                packet->v0 = gb;
                w = g_SpriteFontWidth[idx];
                packet->h = 0x18;
                otv = g_DrawBuffer;
                packet->clut = clutIndex;
                packet->w = w;
                packet++;
                packetAddress.volatileSprite = oldPacket;
                AddPrim(otv + 0xCC, packetAddress.sprite);
            }
            x += g_SpriteFontWidth[idx];
        } while (*sr != 0);
    }
    packetAddress.bytes = next;
    SetDrawMode(packetAddress.drawPacket, 0, 1, 0x1D, g_DrawModeEnv);
    AddPrim(g_DrawBuffer + 0xCC, next);
    SCRATCH_PRIM_CURSOR_AS(u_char) = next + 0xC;
}
