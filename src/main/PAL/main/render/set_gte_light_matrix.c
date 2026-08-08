#include "common.h"
#include "game/prim.h"
#include "game/render.h"
#include "game/scratchpad.h"
#include "psyq/gte.h"

extern Matrix g_SceneLightMatrix;

/* Loads the GTE light matrix with g_SceneLightMatrix * `view`, working on a
 * local copy so the caller's view matrix is left alone. */
void SetGteLightMatrix(Matrix *view) {
    Matrix m;

    m = *view;
    MulMatrix2(&g_SceneLightMatrix, &m);
    SetLightMatrix(&m);
}

extern s32 g_GameMode;
extern s32 g_AnimTimer;
extern u8 g_PadType;
extern s16 g_NegconMaxTwist;
extern s16 g_NegconSteerPlay;
extern s32 g_ControllerSceneAngleX;
extern s32 g_ControllerSceneAngleY;
extern s32 g_ScratchViewX asm("0x1F800008");
extern s32 g_ScratchViewY asm("0x1F80000C");
extern void *g_ScratchViewYPointer asm("0x1F80000C");
extern volatile s32 g_ScratchViewZVolatile asm("0x1F800010");
extern void *volatile g_ScratchViewZPointer asm("0x1F800010");
extern s32 g_ScratchViewAngleX asm("0x1F800018");
extern s32 g_ScratchViewAngleY asm("0x1F80001C");
extern s32 g_ScratchViewAngleZ asm("0x1F800020");
extern s32 g_ScratchEnvMode4 asm("0x1F800084");


/* Builds and submits the controller models shown by the pad and NeGcon setup
 * screens. The read-only scratchpad base is retained for the three camera
 * matrix multiplies; write-only scratch locations stay absolute so each store
 * is independently rematerialized. */
void DrawControllerSetupScene(s32 variant) {
    s32 scale[3];
    Matrix xRot;
    Matrix yRot;
    s32 position[3];
    s32 steer;
    s32 model;
    u8 *scratchBase = (u8 *)SCRATCHPAD_ADDR;

    g_ScratchViewZVolatile = 0;
    g_ScratchViewZPointer = (void *)-0x1080;
    position[2] = 0;
    position[1] = 0;
    position[0] = 0;
    g_ScratchViewY = 0;
    g_ScratchViewX = 0;
    g_ScratchViewAngleZ = 0;
    g_ScratchViewAngleY = 0;
    g_ScratchViewAngleX = 0;
    g_ScratchViewYPointer = 0;
    if ((u32)(g_GameMode - 10) < 2) {
        g_ScratchViewZPointer = (void *)-0xC80;
    } else {
        g_ScratchViewYPointer = (void *)-0x40;
    }
    SetCameraRotMatrix();

    if (g_PadType == 0x41) {
        BuildRotMatrixX(&xRot, -0xD0);
        BuildRotMatrixY(&yRot, g_ControllerSceneAngleY + 0x400);
        MulMatrix2(&yRot, &xRot);
        MulMatrix2(scratchBase + 0x28, &xRot);
        scale[2] = 0x1000;
        scale[0] = 0x1000;
        scale[1] = 0x2000;
        ScaleMatrix(&yRot, scale);
        MulMatrix2(&yRot, &xRot);
        SetGteLightMatrix(&xRot);
        SetGteObjectMatrix((void *)0x1F80011C, position, &xRot);
        g_ScratchEnvMode4 = 0;
        model = g_ModelBankCount < 1;
        SubmitModel((void *)SCRATCHPAD_ADDR, model);
        return;
    }

    if (g_PadType != 0x23) {
        return;
    }
    if (g_GameMode == 11) {
        steer = (rsin(g_AnimTimer * 16) *
                 g_NegconSteerRange[g_NegconMaxTwist]) / 512;
    } else if (g_GameMode == 10) {
        steer = ((rsin(g_AnimTimer * 16) << 4) *
                 g_NegconPlayScale[g_NegconSteerPlay]) / 4096;
    } else {
        steer = g_NegconSteer * 8;
    }

    {
        s32 angle = g_ControllerSceneAngleX - 0x40;

        BuildRotMatrixX(&xRot, steer + angle);
    }
    BuildRotMatrixY(&yRot, g_ControllerSceneAngleY + 0x400);
    MulMatrix2(&yRot, &xRot);
    MulMatrix2(scratchBase + 0x28, &xRot);
    scale[2] = 0x1000;
    scale[0] = 0x1000;
    scale[1] = 0x2000;
    ScaleMatrix(&yRot, scale);
    MulMatrix2(&yRot, &xRot);
    SetGteLightMatrix(&xRot);
    SetGteObjectMatrix((void *)0x1F80011C, position, &xRot);
    g_ScratchEnvMode4 = 0;
    SubmitModel((void *)SCRATCHPAD_ADDR, 1);
    if (variant != 0) {
        SetGteObjectMatrix((void *)0x1F80011C, position, &xRot);
        g_ScratchEnvMode4 = 0;
        model = 1;
        if (g_ModelBankCount >= 4) {
            model = 3;
        }
        SubmitModel((void *)SCRATCHPAD_ADDR, model);
    }

    {
        s32 angle = g_ControllerSceneAngleX - 0x40;

        BuildRotMatrixX(&xRot, angle - steer);
    }
    BuildRotMatrixY(&yRot, g_ControllerSceneAngleY + 0x400);
    MulMatrix2(&yRot, &xRot);
    MulMatrix2(scratchBase + 0x28, &xRot);
    scale[2] = 0x1000;
    scale[0] = 0x1000;
    scale[1] = 0x2000;
    ScaleMatrix(&yRot, scale);
    MulMatrix2(&yRot, &xRot);
    SetGteLightMatrix(&xRot);
    SetGteObjectMatrix((void *)0x1F80011C, position, &xRot);
    g_ScratchEnvMode4 = 0;
    model = 1;
    if (g_ModelBankCount >= 3) {
        model = 2;
    }
    SubmitModel((void *)SCRATCHPAD_ADDR, model);
    if (variant != 0) {
        SetGteObjectMatrix((void *)0x1F80011C, position, &xRot);
        g_ScratchEnvMode4 = 0;
        model = 1;
        if (g_ModelBankCount >= 5) {
            model = 4;
        }
        SubmitModel((void *)SCRATCHPAD_ADDR, model);
    }
}

/* Wide-parameter view of the packet builders; see GameQueueSprite.c. */
/* The 16x32 left arrow, plus - while `pulse` is set - a tile over it whose
 * green channel breathes with rsin of the shared arrow angle. */
u8 *DrawLeftArrow(void *ot, u8 *prim, s32 x, s32 y, s32 pulse) {
    prim = GameQueueSprite(ot, prim, x, y, 0x10, 0x20, 0x48, 0xB8, 0x7F82);
    prim = QueueDrawModePrim(ot, prim, 0x39);
    if (pulse != 0) {
        u8 glow = rsin(g_SetupArrowPulse % 0x1000) / 64 - 65;

        prim = (u8 *)AddTilePrim(
            ot, prim, x, y, 0x10, 0x20, 0, glow, 0);
    }
    return prim;
}

/* The 16x32 right arrow, plus - while `pulse` is set - a tile over it whose
 * green channel breathes with rsin of the shared arrow angle. */
u8 *DrawRightArrow(void *ot, u8 *prim, s32 x, s32 y, s32 pulse) {
    prim = GameQueueSprite(ot, prim, x, y, 0x10, 0x20, 0x58, 0xB8, 0x7F82);
    prim = QueueDrawModePrim(ot, prim, 0x39);
    if (pulse != 0) {
        u8 glow = rsin(g_SetupArrowPulse % 0x1000) / 64 - 65;

        prim = (u8 *)AddTilePrim(
            ot, prim, x, y, 0x10, 0x20, 0, glow, 0);
    }
    return prim;
}

/*
 * The framed "CONFIG n" panel at (x, y): the caption strip, then the three
 * digit cells (the middle one steps 8 texels per configuration), then the
 * white frame drawn as four nested tiles.
 */
u8 *DrawPadConfigSelector(ot, prim, x, y, selection)
    void *ot;
    u8 *prim;
    s16 x;
    s16 y;
    s16 selection;
{
    prim = GameQueueShadedSprite(
        ot, prim, x + 6, y + 8, 0x30, 0xC, 0x78, 0xC0, 0x7F40);
    prim = QueueDrawModePrim(ot, prim, 0x3A);
    prim = GameQueueSpriteTrans(
        ot, prim, x + 18, y + 32, 8, 0x10, 0x68, 0x28, 0x7F40);
    prim = GameQueueSpriteTrans(
        ot, prim, x + 26, y + 32, 8, 0x10, selection * 8 + 80, 0x18, 0x7F40);
    prim = GameQueueSpriteTrans(
        ot, prim, x + 34, y + 32, 8, 0x10, 0x68, 0x28, 0x7F40);
    prim = QueueDrawModePrim(ot, prim, 0x5B);
    prim = (u8 *)AddTilePrim(
        ot, prim, x + 1, y + 2, 0x3A, 0x14, 0, 0, 0);
    prim = (u8 *)AddTilePrim(
        ot, prim, x + 2, y + 26, 0x38, 0x1A, 0xFF, 0xFF, 0xFF);
    prim = (u8 *)AddTilePrim(
        ot, prim, x + 1, y + 24, 0x3A, 0x1E, 0, 0, 0);
    return (u8 *)AddTilePrim(
        ot, prim, x, y, 0x3C, 0x38, 0xFF, 0xFF, 0xFF);
}
