#include <sys/types.h>

#include "common.h"
#include "game/render.h"

extern volatile long g_CdReadSectorCount asm("D_8007D790");
extern volatile long g_CdReadBuffer asm("D_8007D794");
extern volatile long g_CdReadMode asm("D_8007D79C");
extern volatile long g_CdReadSectorWords asm("D_8007D7A0");
extern volatile long g_CdReadStartVSync asm("D_8007D7AC");
extern volatile long g_CdReadSavedSyncCallback asm("D_8007D7B4");
extern volatile long g_CdReadSavedReadyCallback asm("D_8007D7B8");
long CdSyncCallback(long arg0) asm("func_8006A574");
long CdReadyCallback(long arg0) asm("func_8006A58C");
long VSync(long mode) asm("func_8006DD30");
long CdStatus(void) asm("func_8006A3E8");
long CdControlB(long arg0, void *arg1, long arg2) asm("func_8006A808");
long CdReadRetry(long arg0) asm("func_8002745C");
void CdReady(long arg0, long arg1) asm("func_8006A554");
extern long g_CdReadCallback asm("D_8007D78C");
extern u_char D_8007D7BC[];
extern u_char D_8007D7BD[];
extern u_char D_8007D87C[];
extern u_char D_8007BED0[];
void SetSprt(u_char *prim) asm("func_80064FA8");
void func_80064EB8(u_char *prim, long enabled);
void AddPrim(void *ot, void *prim) asm("func_80064DDC");
void *func_800666F4(void *prim, long a, long b, long c, void *d);
long AddTilePrim(long arg0, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7, long arg8) asm("func_80032F34");

long CdRead(long arg0, long arg1, long arg2) asm("func_80027688");
long CdRead(long arg0, long arg1, long arg2) {
    long savedArg0;
    long mode;
    long value;

    value = (long)&g_CdReadMode;
    *(volatile long *)value = arg2;
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
    savedArg0 = arg0;
    value = *(volatile long *)mode;
    value |= 0x20;
    *(volatile long *)mode = value;
    g_CdReadBuffer = arg1;
    g_CdReadSectorCount = savedArg0;
    g_CdReadSavedSyncCallback = CdSyncCallback(0);
    g_CdReadSavedReadyCallback = CdReadyCallback(0);
    g_CdReadStartVSync = VSync(-1);

    if ((CdStatus() & 0xE0) != 0) {
        CdControlB(9, 0, 0);
    }

    return CdReadRetry(0) > 0;
}

long CdReadSync(long arg0, long arg1) asm("func_80027790");
long CdReadSync(long arg0, long arg1) {
    long savedArg0;
    long savedArg1;
    volatile long *state;
    long result;

    savedArg0 = arg0;
    savedArg1 = arg1;
    state = &g_CdReadStartVSync;

    do {
        long now;

        now = VSync(-1);
        if (state[0] + 0x4B0 < now) {
            result = -1;
        } else {

        if (state[-2] < 0) {
            goto reset_read;
        }

        now = VSync(-1);
        if (state[-1] + 0x3C < now) {
reset_read:
            CdReadRetry(1);
            result = state[-7];
        } else {

        result = state[-2];

        }
        }
        if (savedArg0 != 0) {
            break;
        }
    } while (result > 0);

    CdReady(1, savedArg1);
    return result;
}

long CdReadCallback(long arg0) asm("func_8002785C");
long CdReadCallback(long arg0) {
    long old = g_CdReadCallback;

    g_CdReadCallback = arg0;
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

void DrawSpriteString(long x, long y, u_char *str, long arg3) asm("func_80027874");
void DrawSpriteString(long x, long y, u_char *str, long arg3) {
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
    next = *(u_char **) 0x1F800000;
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
                func_80064EB8(next, 1);
                next += 0x14;
                oldPacket = packet;
                packet->x = x;
                packet->y = y;
                packet->u = ga;
                packet->v = gb;
                w = D_8007D87C[idx];
                packet->h = 0x18;
                otv = g_DrawBuffer;
                packet->clut = arg3;
                packet->w = w;
                packet++;
                AddPrim(otv + 0xCC, oldPacket);
            }
            x += D_8007D87C[idx];
        } while (*sr != 0);
    }
    func_800666F4(next, 0, 1, 0x1D, D_8007BED0);
    AddPrim(g_DrawBuffer + 0xCC, next);
    *(u_char **) 0x1F800000 = next + 0xC;
}

void func_800279EC(long arg0, long arg1, long arg2, long arg3) {
    long temp;

    temp = AddTilePrim(arg0, arg1, arg2 + 1, arg3 + 2, 0xC2, 0x1C, 0, 0, 0);
    AddTilePrim(arg0, temp, arg2, arg3, 0xC4, 0x20, 0xFF, 0xFF, 0xFF);
}
