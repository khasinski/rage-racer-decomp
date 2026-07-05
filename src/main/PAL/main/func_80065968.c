#include "common.h"

typedef struct Rect {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} Rect;

extern void (*GPU_printf)(char *, ...) asm("D_800941E4");
extern u8 g_GraphDebug asm("D_800941EA");
extern s16 D_800941EC;
extern s16 D_800941EE;
extern char D_80013548[];
extern char D_80013554[];
extern char D_80013568[];

void CheckPrim(char *arg0, Rect *rect) asm("func_80065968");

void CheckPrim(char *arg0, Rect *rect) {
    switch (g_GraphDebug) {
    case 1: {
        register s32 w asm("$5") = rect->w;
        s32 maxX = D_800941EC;
        register s32 x asm("$7");
        register s32 y asm("$3");
        register s32 h asm("$6");
        s32 maxY;

        if (maxX < w) {
            goto bad;
        }
        x = rect->x;
        if (maxX < w + x) {
            goto bad;
        }
        y = rect->y;
        maxY = D_800941EE;
        if (maxY < y) {
            goto bad;
        }
        h = rect->h;
        if (maxY < y + h) {
            goto bad;
        }
        if (w <= 0) {
            goto bad;
        }
        if (x < 0) {
            goto bad;
        }
        if (y < 0) {
            goto bad;
        }
        if (h > 0) {
            return;
        }
bad:
        GPU_printf(D_80013548, arg0);
        GPU_printf(D_80013554, rect->x, rect->y, rect->w, rect->h);
        return;
    }
    case 2:
        GPU_printf(D_80013568, arg0);
        GPU_printf(D_80013554, rect->x, rect->y, rect->w, rect->h);
        return;
    }
}
