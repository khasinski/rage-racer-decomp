#include <sys/types.h>

#include "common.h"
#include "game/prim.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "psyq/gpu.h"

extern u_char g_SpriteFontU[];
extern u_char g_SpriteFontV[];
extern u_char g_SpriteFontWidth[];
extern u_char g_DrawModeEnv[];

typedef struct CdReadSprite {
    u_char tag[8];
    volatile short x;
    volatile short y;
    volatile u_char u;
    volatile u_char v;
    volatile u_short clut;
    volatile short w;
    volatile short h;
} CdReadSprite;

void DrawSpriteString(long x, long y, u_char *str, long clutIndex) {
    CdReadSprite *packet;
    long idx;
    u_char *next;
    register u_char *sr __asm("$21");
    register u_char *tableA __asm("$23");
    long ga;
    long gb;
    long w;
    CdReadSprite *oldPacket;
    u_char *otv;
    u_char *tableB;

    sr = str;
    next = SCRATCH_PRIM_CURSOR_AS(u_char);
    if (*sr != 0) {
        tableA = g_SpriteFontU;
        tableB = g_SpriteFontV;
        packet = (CdReadSprite *)next;
        do {
            idx = *sr++ - 0x20;
            if (idx != 0) {
                ga = tableA[idx * 2];
                gb = tableB[idx * 2];
                SetSprt((SPRT *)next);
                SetShadeTex(next, 1);
                next += 0x14;
                oldPacket = packet;
                packet->x = x;
                packet->y = y;
                packet->u = ga;
                packet->v = gb;
                w = g_SpriteFontWidth[idx];
                packet->h = 0x18;
                otv = g_DrawBuffer;
                packet->clut = clutIndex;
                packet->w = w;
                packet++;
                AddPrim(otv + 0xCC, oldPacket);
            }
            x += g_SpriteFontWidth[idx];
        } while (*sr != 0);
    }
    SetDrawMode((DrawPacket *)next, 0, 1, 0x1D, g_DrawModeEnv);
    AddPrim(g_DrawBuffer + 0xCC, next);
    SCRATCH_PRIM_CURSOR_AS(u_char) = next + 0xC;
}

void DrawShadowedTile(long x, long y, long w, long h) {
    long temp;

    temp = (long)AddTilePrim((void *)x, (void *)y, w + 1, h + 2, 0xC2, 0x1C, 0, 0, 0);
    AddTilePrim((void *)x, (void *)temp, w, h, 0xC4, 0x20, 0xFF, 0xFF, 0xFF);
}
