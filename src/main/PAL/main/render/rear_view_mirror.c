#include "common.h"
#include "game/render.h"
#include "game/race.h"
#include "game/car.h"
void SetRotMatrix(void *arg0);
void SetDrawArea(u8 *packet, u8 *drawEnv);

extern s16 g_MirrorViewEnabled;
extern s32 g_MirrorPanelY;
extern s32 g_MirrorUnlocked;
void ResetMirrorState(void);
void ResetMirrorState(void) {
    g_MirrorViewEnabled = 1;
    g_MirrorPanelY = -0x2C;
    g_MirrorUnlocked = 0;
}

extern s32 g_MirrorUnlocked;
extern s16 g_MirrorViewEnabled;
extern s32 g_CameraViewMode;
extern Matrix D_8009AF00;
extern Matrix D_8019CB18;
extern s16 D_8019CEAA;
extern s16 D_801C0692;
extern s16 D_8019CEAE;
extern s16 D_801C0696;
extern s32 g_VisibleCellMask;
extern s32 D_8019C7E4;
extern s32 g_VisibleCellList;
extern s32 D_8009E888;
extern s32 g_MirrorPanelY;
void SetGeomOffset(long ofx, long ofy);
void SetGeomScreen(long h);

/*
 * Sets up the scratchpad render state (0x1F800000) for the rear-view mirror
 * pass: only when all five conditions hold (mirror flag, enabled, etc.) does it
 * save the current matrix into D_8009AF00, install the mirror matrix D_8019CB18,
 * set mode 9 + a narrow clip rect + prim base, flip the ordering flag, and push
 * the pass behind the main scene (depth += 0x800). Returns 1 if the mirror pass
 * is active, else 0.
 */
s32 BeginMirrorPass(void);
s32 BeginMirrorPass(void) {
    GameScratchpadRenderState *scratch;
    s32 mirrorEnabled;
    register s32 v0reg asm("$2");
    s32 v1reg;
    s32 y0;

    mirrorEnabled = 0;
    scratch = (GameScratchpadRenderState *)0x1F800000;

    if ((g_MirrorUnlocked != 0) &&
        (g_MirrorViewEnabled != 0) &&
        (g_CameraViewMode == 0) &&
        (g_GrandPrixMode != 0) &&
        (g_RacePhase == 2)) {
        mirrorEnabled = 1;
    }

    if (mirrorEnabled != 0) {
        D_8009AF00 = scratch->matrix;
        scratch->matrix = D_8019CB18;

        SetGeomOffset(0xA0, 0x24);
        SetGeomScreen(0xC0);

        v0reg = 9;
        scratch->mode = v0reg;
        v0reg = 0x56;
        scratch->x0 = v0reg;
        v0reg = (s32)g_DrawBuffer;
        y0 = *(u16 *)&g_MirrorPanelY;
        scratch->x1 = 0xEA;
        __asm__("");
        v1reg = g_MirrorPanelY;
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

        v0reg = g_MirrorPanelY;
        v1reg = v0reg + 0x24;
        if (v1reg > 0) {
            v0reg = v1reg - D_8019CEAA;
            D_8019CEAE = v0reg;
            D_801C0696 = v0reg;
        } else {
            D_8019CEAE = 0;
            D_801C0696 = 0;
        }

        g_VisibleCellMask = (s32)&D_8019C7E4;
        g_VisibleCellList = (s32)&D_8009E888;
        scratch->depth += 0x800;
    }

    return mirrorEnabled;
}

extern Matrix D_8009AF00;
extern s32 g_VisibleCellMask;
extern s32 D_8019C86C;
extern s32 g_VisibleCellList;
extern s32 D_8009EC94;

void SetGeomOffset(long ofx, long ofy);
void SetGeomScreen(long h);

/*
 * Sibling of BeginMirrorPass: closes the mirror pass and restores the full-screen
 * main viewport render state (mode 0xA, full 0x140x0xF0 clip rect, prim base),
 * flips the ordering flag back, pulls the depth back (-= 0x800) and restores the
 * saved main-view matrix from D_8009AF00.
 */
void EndMirrorPass(void);
void EndMirrorPass(void) {
    GameScratchpadRenderState *scratch;
    register s32 v0reg asm("$2");
    s32 v1reg;

    scratch = (GameScratchpadRenderState *)0x1F800000;

    SetGeomOffset(0xA0, 0x78);
    SetGeomScreen(0x140);

    v0reg = 0xA;
    scratch->mode = v0reg;
    v0reg = 0x140;
    scratch->x1 = v0reg;
    v0reg = 0xF0;
    scratch->y1 = v0reg;
    v0reg = (s32)&D_8019C86C;
    g_VisibleCellMask = v0reg;
    v0reg = (s32)g_DrawBuffer;
    g_VisibleCellList = (s32)&D_8009EC94;
    v1reg = scratch->depth;
    scratch->x0 = 0;
    scratch->y0 = 0;
    v0reg += 0xCC;
    scratch->primData = (void *)v0reg;
    v0reg = scratch->orderingFlag;
    scratch->depth = v1reg - 0x800;
    scratch->orderingFlag = v0reg ^ 1;
    scratch->matrix = D_8009AF00;
}

extern s32 g_MirrorPanelY;
extern u8 D_8007C728[];
extern u8 D_8007C738[];
extern u8 D_8007C739[];
extern u8 D_8007C73A[];

void SetTile(u8 *arg0);
void AddPrim(void *ot, void *prim);
s32 GameQueueDrawModePrimWide(u8 *arg0, s32 arg1, s32 arg2) asm("QueueDrawModePrim");

u8 *DrawMirrorFrame(u8 *packet);
u8 *DrawMirrorFrame(u8 *packet) {
    u8 *otArg;
    u8 *prim;
    u8 *ot;
    u8 *base;
    u8 *base2;
    s32 colorIndex;
    s32 paletteIndex;
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
    *(s16 *)(packet + 0xA) = *(u16 *)&g_MirrorPanelY - 2;
    *(s16 *)(packet + 0xE) = 0x28;
    packet += 0x10;
    AddPrim((u32 *)otArg, (u32 *)prim);

    colorIndex = D_8007C728[g_PlayerCarIndex];
    paletteIndex = colorIndex * 3;
    base2 = g_DrawBuffer;
    ot = base2 + 0xBD0;
    next = (s32)GameQueueSprite(ot, packet, 0x56, g_MirrorPanelY, D_8007C73A[paletteIndex], 8, D_8007C738[paletteIndex], D_8007C739[paletteIndex], 0x7800);
    return (u8 *)GameQueueDrawModePrimWide(ot, next, 9);
}

extern s32 g_MirrorUnlocked;
extern s16 g_MirrorViewEnabled;
extern s32 g_MirrorPanelY;
extern s32 g_VisibleCellList;

u8 *DrawMirrorFrame(u8 *packet);
void DrawSkyBackground(void);
void AddPrim(void *ot, void *prim);
void BuildVisibleCells(s32 arg0, s32 arg1);
void SubmitTerrainCells(void *arg0, s32 arg1, s32 arg2);
void EndMirrorPass(void);

void DrawRearViewMirror(s32 arg0);
void DrawRearViewMirror(s32 arg0) {
    u8 **scratch;
    u8 *packet;
    u8 *prim;

    if (arg0 >= 0x169) {
        g_MirrorUnlocked = 1;
    }

    if (g_MirrorUnlocked != 0) {
        if (g_MirrorViewEnabled != 0) {
            if (g_MirrorPanelY < 0x12) {
                g_MirrorPanelY++;
            }
        } else if (g_MirrorPanelY >= -0x2B) {
            g_MirrorPanelY--;
        }

        if (BeginMirrorPass() != 0) {
            scratch = (u8 **)0x1F800000;

            DrawSkyBackground();
            packet = DrawMirrorFrame(*scratch);
            SetDrawArea(packet, g_DrawBuffer + 0x70);
            prim = packet;
            packet += 0xC;
            AddPrim((u32 *)(g_DrawBuffer + 0x16C8), (u32 *)prim);
            *scratch = packet;
            BuildVisibleCells(-0x3000, 0x6000);
            SetRotMatrix((void *)0x1F800028);
            *(s32 *)0x1F800084 = g_IsEnvironmentMode4;
            SubmitTerrainCells((void *)0x1F800000, g_VisibleCellList, 0x40);

            packet = *scratch;
            SetDrawArea(packet, g_DrawBuffer);
            prim = packet;
            packet += 0xC;
            AddPrim((u32 *)(g_DrawBuffer + 0xBD0), (u32 *)prim);
            *scratch = packet;
            DrawCourseObjects();
            DrawCars();
            EndMirrorPass();
        }
    }
}
