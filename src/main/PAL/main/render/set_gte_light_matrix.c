#include "common.h"
#include "game/render.h"
#include "psyq/gte.h"

extern Matrix g_SceneLightMatrix asm("D_8009E6AC");

/* Loads the GTE light matrix with g_SceneLightMatrix * `view`, working on a
 * local copy so the caller's view matrix is left alone. */
void SetGteLightMatrix(Matrix *view) {
    Matrix m;

    m = *view;
    MulMatrix2(&g_SceneLightMatrix, &m);
    SetLightMatrix(&m);
}

extern s32 g_GameMode asm("D_8019CB14");
extern s32 g_AnimTimer asm("D_8009E694");
extern u8 g_PadType asm("D_801E4369");
extern s16 g_NegconSteer asm("D_801E437E");
extern s16 g_NegconMaxTwist asm("D_801E418C");
extern s16 g_NegconSteerPlay asm("D_8019CAD0");
extern s16 g_NegconSteerRange[] asm("D_8007C020");
extern s32 g_NegconPlayScale[] asm("D_8007C140");
extern s32 g_ControllerSceneAngleX asm("D_801E8A9C");
extern s32 g_ControllerSceneAngleY asm("D_801E8AA4");
extern s32 g_ModelBankCount asm("D_801E4168");
extern s32 g_Scratch08 asm("0x1F800008");
extern s32 g_Scratch0C asm("0x1F80000C");
extern void *g_Scratch0CPointer asm("0x1F80000C");
extern volatile s32 g_Scratch10Volatile asm("0x1F800010");
extern void *volatile g_Scratch10Pointer asm("0x1F800010");
extern s32 g_Scratch18 asm("0x1F800018");
extern s32 g_Scratch1C asm("0x1F80001C");
extern s32 g_Scratch20 asm("0x1F800020");
extern s32 g_Scratch84 asm("0x1F800084");

s32 rsin(s32 angle) asm("func_80068568");

/* Builds and submits the controller models shown by the pad and NeGcon setup
 * screens. The read-only scratchpad base is retained for the three camera
 * matrix multiplies; write-only scratch locations stay absolute so each store
 * is independently rematerialized. */
void func_80014618(s32 variant) {
    s32 scale[3];
    Matrix xRot;
    Matrix yRot;
    s32 position[3];
    s32 steer;
    register s32 product asm("$2");
    register s32 angle asm("$5");
    s32 model;
    u8 *scratchBase = (u8 *)0x1F800000;

    g_Scratch10Volatile = 0;
    g_Scratch10Pointer = (void *)-0x1080;
    position[2] = 0;
    position[1] = 0;
    position[0] = 0;
    g_Scratch0C = 0;
    g_Scratch08 = 0;
    g_Scratch20 = 0;
    g_Scratch1C = 0;
    g_Scratch18 = 0;
    g_Scratch0CPointer = 0;
    if ((u32)(g_GameMode - 10) < 2) {
        g_Scratch10Pointer = (void *)-0xC80;
    } else {
        g_Scratch0CPointer = (void *)-0x40;
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
        g_Scratch84 = 0;
        model = g_ModelBankCount < 1;
        SubmitModel((void *)0x1F800000, model);
        return;
    }

    if (g_PadType != 0x23) {
        return;
    }
    if (g_GameMode == 11) {
        product = rsin(g_AnimTimer * 16) * g_NegconSteerRange[g_NegconMaxTwist];
        if (product < 0) {
            product += 0x1FF;
        }
        steer = product >> 9;
    } else if (g_GameMode == 10) {
        product =
            (rsin(g_AnimTimer * 16) << 4) * g_NegconPlayScale[g_NegconSteerPlay];
        if (product < 0) {
            product += 0xFFF;
        }
        steer = product >> 12;
    } else {
        product = g_NegconSteer;
        steer = product * 8;
    }

    angle = g_ControllerSceneAngleX - 0x40;
    BuildRotMatrixX(&xRot, steer + angle);
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
    g_Scratch84 = 0;
    SubmitModel((void *)0x1F800000, 1);
    if (variant != 0) {
        SetGteObjectMatrix((void *)0x1F80011C, position, &xRot);
        g_Scratch84 = 0;
        model = 1;
        if (g_ModelBankCount >= 4) {
            model = 3;
        }
        SubmitModel((void *)0x1F800000, model);
    }

    angle = g_ControllerSceneAngleX - 0x40;
    BuildRotMatrixX(&xRot, angle - steer);
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
    g_Scratch84 = 0;
    model = 1;
    if (g_ModelBankCount >= 3) {
        model = 2;
    }
    SubmitModel((void *)0x1F800000, model);
    if (variant != 0) {
        SetGteObjectMatrix((void *)0x1F80011C, position, &xRot);
        g_Scratch84 = 0;
        model = 1;
        if (g_ModelBankCount >= 5) {
            model = 4;
        }
        SubmitModel((void *)0x1F800000, model);
    }
}

/* Free-running angle the controller-setup screens pulse their arrows with. */
extern s32 g_SetupArrowPulse asm("D_8007C13C");

/* Wide-parameter view of the packet builders; see GameQueueSprite.c. */
u8 *QueueSpriteWide(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex) asm("func_80016EC4");
s32 AddTilePrim(
    s32 ot,
    s32 prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 r,
    s32 g,
    s32 b) asm("func_80032F34");
u8 *DrawLeftArrow(void *ot, u8 *prim, s32 x, s32 y, s32 pulse) asm("func_80014A60");

/* The 16x32 left arrow, plus - while `pulse` is set - a tile over it whose
 * green channel breathes with rsin of the shared arrow angle. */
u8 *DrawLeftArrow(void *ot, u8 *prim, s32 x, s32 y, s32 pulse) {
    prim = QueueSpriteWide(ot, prim, x, y, 0x10, 0x20, 0x48, 0xB8, 0x7F82);
    prim = QueueDrawModePrim(ot, prim, 0x39);
    if (pulse != 0) {
        u8 glow = rsin(g_SetupArrowPulse % 0x1000) / 64 - 65;

        prim = (u8 *)AddTilePrim(
            (s32)ot, (s32)prim, x, y, 0x10, 0x20, 0, glow, 0);
    }
    return prim;
}

u8 *DrawRightArrow(void *ot, u8 *prim, s32 x, s32 y, s32 pulse) asm("func_80014B70");

/* The 16x32 right arrow, plus - while `pulse` is set - a tile over it whose
 * green channel breathes with rsin of the shared arrow angle. */
u8 *DrawRightArrow(void *ot, u8 *prim, s32 x, s32 y, s32 pulse) {
    prim = QueueSpriteWide(ot, prim, x, y, 0x10, 0x20, 0x58, 0xB8, 0x7F82);
    prim = QueueDrawModePrim(ot, prim, 0x39);
    if (pulse != 0) {
        u8 glow = rsin(g_SetupArrowPulse % 0x1000) / 64 - 65;

        prim = (u8 *)AddTilePrim(
            (s32)ot, (s32)prim, x, y, 0x10, 0x20, 0, glow, 0);
    }
    return prim;
}

/* game/state.h is deliberately not included: it types `selection` as s32,
 * which would drop the sign-extension retail performs. */

/*
 * Retail calls GameQueueShadedSprite with only nine arguments, leaving its
 * tenth (intensity) whatever happened to be in the argument slot. Declared
 * here with the nine it actually passes so the call site matches.
 */
u8 *QueueShadedSpriteNine(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex) asm("func_80016F8C");

/* Local wide-parameter views; see GameQueueSprite.c. */
u8 *QueueSpriteTransWide(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 clutIndex) asm("func_80017138");
u8 *QueueDrawModePrimWide(void *ot, u8 *prim, s32 tpage) asm("func_80017390");

/* Local declaration: `selection` is a short here - retail loads the argument
 * slot as a word and sign-extends it into the texel offset. */
u8 *DrawPadConfigSelector(
    void *ot,
    u8 *prim,
    s16 x,
    s16 y,
    s16 selection) asm("func_80014C80");

/*
 * The framed "CONFIG n" panel at (x, y): the caption strip, then the three
 * digit cells (the middle one steps 8 texels per configuration), then the
 * white frame drawn as four nested tiles.
 */
u8 *DrawPadConfigSelector(
    void *ot,
    u8 *prim,
    s16 x,
    s16 y,
    s16 selection) {
    prim = QueueShadedSpriteNine(
        ot, prim, x + 6, y + 8, 0x30, 0xC, 0x78, 0xC0, 0x7F40);
    prim = QueueDrawModePrimWide(ot, prim, 0x3A);
    prim = QueueSpriteTransWide(
        ot, prim, x + 18, y + 32, 8, 0x10, 0x68, 0x28, 0x7F40);
    prim = QueueSpriteTransWide(
        ot, prim, x + 26, y + 32, 8, 0x10, selection * 8 + 80, 0x18, 0x7F40);
    prim = QueueSpriteTransWide(
        ot, prim, x + 34, y + 32, 8, 0x10, 0x68, 0x28, 0x7F40);
    prim = QueueDrawModePrimWide(ot, prim, 0x5B);
    prim = (u8 *)AddTilePrim(
        (s32)ot, (s32)prim, x + 1, y + 2, 0x3A, 0x14, 0, 0, 0);
    prim = (u8 *)AddTilePrim(
        (s32)ot, (s32)prim, x + 2, y + 26, 0x38, 0x1A, 0xFF, 0xFF, 0xFF);
    prim = (u8 *)AddTilePrim(
        (s32)ot, (s32)prim, x + 1, y + 24, 0x3A, 0x1E, 0, 0, 0);
    return (u8 *)AddTilePrim(
        (s32)ot, (s32)prim, x, y, 0x3C, 0x38, 0xFF, 0xFF, 0xFF);
}
