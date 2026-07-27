#include "common.h"
#include "game/state.h"
#include "game/render.h"
#include "game/audio.h"
#include "game/menu.h"

extern u8 g_PadType asm("D_801E4369");

/* The two 0..7 selections; g_PadType picks which one the screen edits. */
extern s16 g_PadMappingIndex asm("D_8019CE08");
extern s16 g_NegconMappingIndex asm("D_8019CB08");
/* 0 while a controller is present, else the error code the banner reports. */
extern s32 g_PadErrorState asm("D_801E79C8");

/* "INSERT CONTROLLER" / "CONTROLLER ERROR" */
extern char D_80010000[];
extern char D_80010014[];

extern u8 *g_DrawBuffer asm("D_8019C900");

void DrawProportionalTextWide(
    s32 x,
    s32 y,
    char *str,
    s32 clutIndex) asm("func_80016EA0");
u8 *DrawLeftArrowWide(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 pulse) asm("func_80014A60");
u8 *DrawRightArrowWide(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 pulse) asm("func_80014B70");
u8 *DrawPadConfigSelectorWide(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 selection) asm("func_80014C80");
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

/*
 * Game mode 7's screen: either the "no controller" banner, or the two nudge
 * arrows (lit only while the selection can still move that way), the selection
 * panel and the diagram for whichever controller is plugged in. The NeGcon
 * variant adds a translucent caption strip under the diagram.
 */
void GameDrawControllerConfigScreen(void) {
    s32 selection;
    s32 leftLit;
    s32 rightLit;
    u8 *ot;
    u8 *prim;

    if (g_PadType == 0x23) {
        selection = g_NegconMappingIndex;
    } else {
        selection = g_PadMappingIndex;
    }
    /* The arrows light while the selection can still move that way; the xor
     * is folded into `selection` because retail lets it clobber the loaded
     * value rather than allocating a second register. */
    leftLit = selection != 0;
    selection ^= 7;
    rightLit = selection != 0;
    if (g_PadErrorState != 0) {
        if (g_PadErrorState == 1) {
            DrawProportionalTextWide(0x3A, 0xEA, D_80010000, 0x7812);
        } else {
            DrawProportionalTextWide(0x40, 0xEA, D_80010014, 0x7812);
        }
    } else {
        ot = g_DrawBuffer + 0xCC;
        prim = *(u8 **)0x1F800000;
        prim = DrawLeftArrowWide(ot, prim, 0x28, 0xE0, leftLit);
        prim = DrawRightArrowWide(ot, prim, 0x108, 0xE0, rightLit);
        if (g_PadType == 0x23) {
            prim = DrawPadConfigSelectorWide(ot, prim, 0xF0, 0x28, g_NegconMappingIndex);
            prim = GameDrawNegconConfigDiagram(ot, prim);
            prim = QueueSpriteTransWide(
                ot, prim, 0x10, 0x40, 0xD8, 0x10, 0, 0xA8, 0x7F40);
            prim = QueueDrawModePrimWide(ot, prim, 0x3F);
        } else {
            prim = DrawPadConfigSelectorWide(ot, prim, 0xF0, 0x28, g_PadMappingIndex);
            prim = GameDrawPadConfigDiagram(ot, prim);
        }
        *(u8 **)0x1F800000 = prim;
    }
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/menu/GameDrawControllerConfigScreen", func_800155EC);

/* "Hold the "NeGcon" in an untwisted" / "position and press start button." */
extern char D_80010028[];
extern char D_8001004C[];

void func_80027874(s32 x, s32 y, char *str, s32 clutIndex);
s32 GameAddTilePrim(
    s32 ot,
    s32 prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 r,
    s32 g,
    s32 b) asm("func_80032F34");

/* Game mode 9's own overlay: the two lines of instructions over a black panel
 * inside a white border, both drawn into the 0xD0 sub-buffer of the current
 * draw buffer from the shared scratchpad packet cursor. */
void GameDrawNegconNeutralScreen(void) {
    s32 *cursor = (s32 *)0x1F800000;
    s32 ot;
    s32 prim;

    func_80027874(0x18, 0x30, D_80010028, 0x7F81);
    func_80027874(0x18, 0x48, D_8001004C, 0x7F81);
    ot = (s32)(g_DrawBuffer + 0xD0);
    prim = *cursor;
    prim = GameAddTilePrim(ot, prim, 0, 0x28, 0x124, 0x40, 0, 0, 0);
    *cursor = GameAddTilePrim(ot, prim, 0, 0x26, 0x125, 0x44, 0xFF, 0xFF, 0xFF);
}

/*
 * The six live NeGcon settings. All of them are persisted to the memory card,
 * so they also appear as fields of GameSaveBlock (game/memcard.h).
 */
extern u16 g_NegconSteerNeutral asm("D_801E4BF0");
extern u16 g_NegconNeutralI asm("D_8019CA08");
extern u16 g_NegconNeutralII asm("D_8019CA0A");
extern u16 g_NegconNeutralL asm("D_8019CA0C");
extern u16 g_NegconSteerPlay asm("D_8019CAD0");
extern u16 g_NegconMaxTwist asm("D_801E418C");

/* ...and where each of them is parked while the calibration screens run, so a
 * cancel can put every one of them back. */
extern u16 g_NegconSteerNeutralSaved asm("D_801E41A0");
extern u16 g_NegconNeutralISaved asm("D_8009F0A8");
extern u16 g_NegconNeutralIISaved asm("D_8009F0AA");
extern u16 g_NegconNeutralLSaved asm("D_8009F0AC");
extern u16 g_NegconSteerPlaySaved asm("D_8019C75C");
extern u16 g_NegconMaxTwistSaved asm("D_8019CB04");

/* The same four screen counters GameBeginControllerConfig clears. */
extern s32 g_ControllerSceneAngleY asm("D_801E8AA4");
extern s32 g_ControllerSceneAngleX asm("D_801E8A9C");
extern s32 g_PadConfigFlipTimer asm("D_801E7A4C");
extern s32 g_PadConfigFlipPhase asm("D_801E6C7C");

/*
 * Entry hook for the NeGcon calibration sequence: snapshots the six live
 * settings, clears the three the neutral/twist screens are about to measure
 * along with the screen counters, and enters game mode 9 (the "hold the
 * controller still" screen).
 *
 * The pins and the empty barrier only hold the reads ahead of the clears and
 * keep the last two snapshots in the registers retail used; the code itself is
 * ordinary C (see docs/ASM_AND_GTE_POLICY.md).
 */
void GameBeginNegconCalibration(void) {
    register u16 twist asm("$3");
    register u16 mode asm("$4");
    u16 *neutral = &g_NegconNeutralI;
    u16 steer = g_NegconSteerNeutral;
    u16 neutral0 = *neutral;
    u16 neutral1 = g_NegconNeutralII;
    u16 neutral2 = g_NegconNeutralL;

    twist = g_NegconSteerPlay;
    mode = g_NegconMaxTwist;
    asm("");

    *neutral = 0;
    g_ControllerSceneAngleY = 0;
    g_ControllerSceneAngleX = 0;
    g_PadConfigFlipTimer = 0;
    g_PadConfigFlipPhase = 0;
    g_NegconSteerNeutral = 0;
    g_NegconNeutralII = 0;
    g_NegconNeutralL = 0;
    g_GameMode = 9;
    g_NegconSteerNeutralSaved = steer;
    g_NegconNeutralISaved = neutral0;
    g_NegconNeutralIISaved = neutral1;
    g_NegconNeutralLSaved = neutral2;
    g_NegconSteerPlaySaved = twist;
    g_NegconMaxTwistSaved = mode;
}

/* The raw NeGcon axes in the BIOS pad buffer (g_PadBuffers + 4): steering,
 * then the three analog buttons. Four consecutive unsigned bytes, 0..255 with
 * 0x80 the mechanical centre -- which is what the `- 128` below proves; the
 * three buttons keep their raw 0..255 and are re-centred by subtracting the
 * latched g_NegconNeutral* instead. */
extern u8 g_NegconAxisSteer asm("D_801E4040");
extern u8 g_NegconAxisI asm("D_801E4041");
extern u8 g_NegconAxisII asm("D_801E4042");
extern u8 g_NegconAxisL asm("D_801E4043");

void func_80023750(s32 arg0);

/*
 * Game mode 9: hold the NeGcon still and press start. Start latches the four
 * axes as the neutral point and advances to mode 10 (the steering play
 * screen); unplugging the NeGcon drops straight back to mode 1.
 */
void GameUpdateNegconNeutralScreen(void) {
    g_AnimTimer++;
    if (g_PadEdge2 & 0x800) {
        GamePlaySoundCue(2);
        g_GameMode = 10;
        g_NegconSteerNeutral = g_NegconAxisSteer - 128;
        g_NegconNeutralI = g_NegconAxisI;
        g_NegconNeutralII = g_NegconAxisII;
        g_NegconNeutralL = g_NegconAxisL;
    }
    if (g_PadType != 0x23) {
        g_GameMode = 1;
    }
    GameDrawNegconNeutralScreen();
    func_80023750(4);
    GameDrawControllerSetupScene(0);
}
