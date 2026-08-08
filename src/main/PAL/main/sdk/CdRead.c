#include <sys/types.h>

#include "common.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "psyq/cd.h"
#include "psyq/kernel.h"
void SetShadeTex(u_char *prim, long enabled);

extern long g_CdReadCallback;
extern u_char D_8007D7BC[];
extern u_char D_8007D7BD[];
extern u_char D_8007D87C[];
extern u_char g_DrawModeEnv[];
void SetSprt(u_char *prim);
void AddPrim(void *ot, void *prim);
void *SetDrawMode(void *prim, long a, long b, long c, void *d);
long AddTilePrim(long ot, long prim, long x, long y, long w, long h, long r, long g, long b);

long CdRead(long sectors, void *buf, long readMode) {
    long savedArg0;
    long mode;
    long value;

    value = (long)&g_CdReadMode;
    *(volatile long *)value = readMode;
    value = *(volatile long *)value;
    mode = value & 0x30;

    switch (mode) {
    case 0:
        value = 0x200;
        g_CdReadSectorWords = value;
        break;
    case 0x20:
        value = 0x249;
        g_CdReadSectorWords = value;
        break;
    default:
        value = 0x246;
        mode = (long)&g_CdReadSectorWords;
        *(volatile long *)mode = value;
        break;
    }

    mode = (long)&g_CdReadMode;
    savedArg0 = sectors;
    value = *(volatile long *)mode;
    value |= 0x20;
    *(volatile long *)mode = value;
    g_CdReadBuffer = (long)buf;
    g_CdReadSectorCount = savedArg0;
    g_CdReadSavedSyncCallback = CdSyncCallback(0);
    g_CdReadSavedReadyCallback = CdReadyCallback(0);
    g_CdReadStartVSync = VSync(-1);

    if ((CdStatus() & 0xE0) != 0) {
        CdControlB(9, 0, 0);
    }

    return CdReadRetry(0) > 0;
}

long CdReadSync(long mode, long result) {
    long savedMode;
    long savedResult;
    volatile long *state;
    long status;

    savedMode = mode;
    savedResult = result;
    state = &g_CdReadStartVSync;

    do {
        long now;

        now = VSync(-1);
        if (state[0] + 0x4B0 < now) {
            status = -1;
        } else {

        if (state[-2] < 0) {
            CdReadRetry(1);
            status = state[-7];
        } else {

        now = VSync(-1);
        if (state[-1] + 0x3C < now) {
            CdReadRetry(1);
            status = state[-7];
        } else {

        status = state[-2];

        }
        }
        }
        if (savedMode != 0) {
            break;
        }
    } while (status > 0);

    CdReady(1, savedResult);
    return status;
}

long CdReadCallback(long callback) {
    long old = g_CdReadCallback;

    g_CdReadCallback = callback;
    return old;
}

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
        tableA = D_8007D7BC;
        tableB = D_8007D7BD;
        packet = (CdReadSprite *)next;
        do {
            idx = *sr++ - 0x20;
            if (idx != 0) {
                ga = tableA[idx * 2];
                gb = tableB[idx * 2];
                SetSprt(next);
                SetShadeTex(next, 1);
                next += 0x14;
                oldPacket = packet;
                packet->x = x;
                packet->y = y;
                packet->u = ga;
                packet->v = gb;
                w = D_8007D87C[idx];
                packet->h = 0x18;
                otv = g_DrawBuffer;
                packet->clut = clutIndex;
                packet->w = w;
                packet++;
                AddPrim(otv + 0xCC, oldPacket);
            }
            x += D_8007D87C[idx];
        } while (*sr != 0);
    }
    SetDrawMode(next, 0, 1, 0x1D, g_DrawModeEnv);
    AddPrim(g_DrawBuffer + 0xCC, next);
    SCRATCH_PRIM_CURSOR_AS(u_char) = next + 0xC;
}

void DrawShadowedTile(long x, long y, long w, long h) {
    long temp;

    temp = AddTilePrim(x, y, w + 1, h + 2, 0xC2, 0x1C, 0, 0, 0);
    AddTilePrim(x, temp, w, h, 0xC4, 0x20, 0xFF, 0xFF, 0xFF);
}
