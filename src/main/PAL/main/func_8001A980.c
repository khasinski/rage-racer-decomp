#include "common.h"
#include "game/render.h"
#include "game/race.h"
#include "game/car.h"

extern s16 D_8019CA10;
extern s32 D_801E4D18;
extern s32 D_801E8A98;

void func_8001A980(void) {
    D_8019CA10 = 1;
    D_801E4D18 = -0x2C;
    D_801E8A98 = 0;
}

extern s32 D_801E8A98;
extern s16 D_8019CA10;
extern s32 g_CameraViewMode asm("D_8009E870");
extern Matrix D_8009AF00;
extern Matrix D_8019CB18;
extern s16 D_8019CEAA;
extern s16 D_801C0692;
extern s16 D_8019CEAE;
extern s16 D_801C0696;
extern s32 D_801E6828;
extern s32 D_8019C7E4;
extern s32 D_801E4BC8;
extern s32 D_8009E888;
extern s32 D_801E4D18;

void func_80069A58(s32 arg0, s32 arg1);
void func_80069A78(s32 arg0);

/*
 * Sets up the scratchpad render state (0x1F800000) for the rear-view mirror
 * pass: only when all five conditions hold (mirror flag, enabled, etc.) does it
 * save the current matrix into D_8009AF00, install the mirror matrix D_8019CB18,
 * set mode 9 + a narrow clip rect + prim base, flip the ordering flag, and push
 * the pass behind the main scene (depth += 0x800). Returns 1 if the mirror pass
 * is active, else 0.
 */
s32 func_8001A9A8(void) {
    GameScratchpadRenderState *scratch;
    s32 mirrorEnabled;
    register s32 v0reg asm("$2");
    register s32 v1reg asm("$3");
    register s32 y0 asm("$4");

    mirrorEnabled = 0;
    scratch = (GameScratchpadRenderState *)0x1F800000;

    if ((D_801E8A98 != 0) &&
        (D_8019CA10 != 0) &&
        (g_CameraViewMode == 0) &&
        (g_GrandPrixMode != 0) &&
        (g_RacePhase == 2)) {
        mirrorEnabled = 1;
    }

    if (mirrorEnabled != 0) {
        D_8009AF00 = scratch->matrix;
        scratch->matrix = D_8019CB18;

        func_80069A58(0xA0, 0x24);
        func_80069A78(0xC0);

        scratch->mode = 9;
        scratch->x0 = 0x56;
        __asm__("" ::: "memory");
        v0reg = (s32)g_DrawBuffer;
        y0 = *(u16 *)&D_801E4D18;
        scratch->x1 = 0xEA;
        __asm__("" ::: "memory");
        v1reg = D_801E4D18;
        v0reg += 0xBCC;
        scratch->primData = (void *)v0reg;
        v0reg = scratch->orderingFlag;
        scratch->y0 = y0;
        v0reg ^= 1;
        scratch->orderingFlag = v0reg;
        v0reg = y0 + 0x24;
        scratch->y1 = v0reg;

        if (v1reg > 0) {
            D_8019CEAA = y0;
            v0reg = y0 + 0xF0;
        } else {
            v0reg = 0xF0;
            D_8019CEAA = 0;
        }
        D_801C0692 = v0reg;
        __asm__("" ::: "memory");

        v0reg = D_801E4D18;
        v1reg = v0reg + 0x24;
        if (v1reg > 0) {
            v0reg = v1reg - D_8019CEAA;
            D_8019CEAE = v0reg;
            D_801C0696 = v0reg;
        } else {
            D_8019CEAE = 0;
            D_801C0696 = 0;
        }

        D_801E6828 = (s32)&D_8019C7E4;
        D_801E4BC8 = (s32)&D_8009E888;
        scratch->depth += 0x800;
    }

    return mirrorEnabled;
}

extern Matrix D_8009AF00;
extern s32 D_801E6828;
extern s32 D_8019C86C;
extern s32 D_801E4BC8;
extern s32 D_8009EC94;

void func_80069A58(s32 arg0, s32 arg1);
void func_80069A78(s32 arg0);

/*
 * Sibling of func_8001A9A8: closes the mirror pass and restores the full-screen
 * main viewport render state (mode 0xA, full 0x140x0xF0 clip rect, prim base),
 * flips the ordering flag back, pulls the depth back (-= 0x800) and restores the
 * saved main-view matrix from D_8009AF00.
 */
void func_8001ABD8(void) {
    GameScratchpadRenderState *scratch;
    register s32 v0reg asm("$2");
    register s32 v1reg asm("$3");

    scratch = (GameScratchpadRenderState *)0x1F800000;

    func_80069A58(0xA0, 0x78);
    func_80069A78(0x140);

    scratch->mode = 0xA;
    scratch->x1 = 0x140;
    scratch->y1 = 0xF0;
    D_801E6828 = (s32)&D_8019C86C;
    __asm__("" : : : "memory");
    v0reg = (s32)g_DrawBuffer;
    v1reg = (s32)&D_8009EC94;
    D_801E4BC8 = v1reg;
    v1reg = scratch->depth;
    scratch->x0 = 0;
    scratch->y0 = 0;
    v0reg += 0xCC;
    scratch->primData = (void *)v0reg;
    __asm__("" : "=r"(v0reg) : "0"(v0reg) : "memory");
    v0reg = scratch->orderingFlag;
    scratch->depth = v1reg - 0x800;
    scratch->orderingFlag = v0reg ^ 1;
    scratch->matrix = D_8009AF00;
}

extern s32 D_801E4D18;
extern u8 D_8007C728[];
extern u8 D_8007C738[];
extern u8 D_8007C739[];
extern u8 D_8007C73A[];

void SetTile(u8 *arg0) asm("func_80064FF8");
void AddPrim(u32 *arg0, u32 *arg1) asm("func_80064DDC");
s32 func_80016EC4(u8 *arg0, u8 *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 func_80017390(u8 *arg0, s32 arg1, s32 arg2);

u8 *func_8001ACE4(u8 *packet) {
    register u8 *otArg asm("$4");
    register u8 *prim asm("$5");
    u8 *ot;
    register u8 *base asm("$2");
    u8 *base2;
    s32 colorIndex;
    register s32 paletteIndex asm("$3");
    s32 color;
    s32 next;

    base = g_DrawBuffer;
    ot = base + 0xD0;

    SetTile(packet);
    otArg = ot;
    prim = packet;

    *(s16 *)(packet + 8) = 0x54;
    color = 0x98;
    packet[4] = 0;
    packet[5] = 0;
    packet[6] = 0;
    *(s16 *)(packet + 0xC) = color;
    *(s16 *)(packet + 0xA) = *(u16 *)&D_801E4D18 - 2;
    *(s16 *)(packet + 0xE) = 0x28;
    __asm__("" ::: "memory");
    packet += 0x10;
    AddPrim((u32 *)otArg, (u32 *)prim);

    colorIndex = D_8007C728[g_PlayerCarIndex];
    paletteIndex = (colorIndex * 2) + colorIndex;
    base2 = g_DrawBuffer;
    ot = base2 + 0xBD0;
    next = func_80016EC4(ot, packet, 0x56, D_801E4D18, D_8007C73A[paletteIndex], 8, D_8007C738[paletteIndex], D_8007C739[paletteIndex], 0x7800);
    return (u8 *)func_80017390(ot, next, 9);
}

extern s32 D_801E8A98;
extern s16 D_8019CA10;
extern s32 D_801E4D18;
extern s32 D_801E4BC8;

u8 *func_8001ACE4(u8 *packet);
void func_800418D4(void);
void func_80066604(u8 *packet, u8 *drawEnv);
void func_80064DDC(u32 *ot, u32 *prim);
void func_800414F0(s32 arg0, s32 arg1);
void func_80069858(void *arg0);
void func_80027FF4(void *arg0, s32 arg1, s32 arg2);
void func_8004123C(void);
void func_800389F0(void);
void func_8001ABD8(void);

void func_8001ADF4(s32 arg0) {
    u8 **scratch;
    u8 *packet;
    u8 *prim;

    if (arg0 >= 0x169) {
        D_801E8A98 = 1;
    }

    if (D_801E8A98 != 0) {
        if (D_8019CA10 != 0) {
            if (D_801E4D18 < 0x12) {
                D_801E4D18++;
            }
        } else if (D_801E4D18 >= -0x2B) {
            D_801E4D18--;
        }

        if (func_8001A9A8() != 0) {
            scratch = (u8 **)0x1F800000;

            func_800418D4();
            packet = func_8001ACE4(*scratch);
            func_80066604(packet, g_DrawBuffer + 0x70);
            prim = packet;
            packet += 0xC;
            func_80064DDC((u32 *)(g_DrawBuffer + 0x16C8), (u32 *)prim);
            *scratch = packet;
            func_800414F0(-0x3000, 0x6000);
            func_80069858((void *)0x1F800028);
            *(s32 *)0x1F800084 = g_IsEnvironmentMode4;
            func_80027FF4((void *)0x1F800000, D_801E4BC8, 0x40);

            packet = *scratch;
            func_80066604(packet, g_DrawBuffer);
            prim = packet;
            packet += 0xC;
            func_80064DDC((u32 *)(g_DrawBuffer + 0xBD0), (u32 *)prim);
            *scratch = packet;
            func_8004123C();
            func_800389F0();
            func_8001ABD8();
        }
    }
}
