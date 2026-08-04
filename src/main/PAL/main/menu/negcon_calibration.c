#include "common.h"
#include "game/state.h"
#include "game/audio.h"
#include "game/menu.h"

void DrawNegconMaxTwistScreen(void);

/*
 * Four {u, v} texel pairs copied into a local that is never read back; see
 * DrawNegconMaxTwistScreen for why this is spelled as a struct copy.
 */
typedef struct NegconUvTemplate {
    u8 uv[8];
} NegconUvTemplate;

extern NegconUvTemplate D_80010070;

/* "Steer play." */
extern char D_80010078[];

extern u8 *g_DrawBuffer asm("D_8019C900");
/* The 0..3 steering-play setting this screen edits. */
extern s16 g_NegconSteerPlay asm("D_8019CAD0");
/* Play in hundredths of a degree per setting; the gauge marks scale from it. */
extern s16 g_NegconPlayPercent[] asm("D_8007C260");

void func_80027874(s32 x, s32 y, char *str, s32 clutIndex);

/* Local wide-parameter views; see GameQueueSprite.c / SetGteLightMatrix.c. */
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
u8 *QueueLineWide(
    void *ot,
    u8 *prim,
    s32 x0,
    s32 y0,
    s32 x1,
    s32 y1,
    s32 r,
    s32 g,
    s32 b) asm("func_800172D4");
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

/*
 * Game mode 10's overlay: the caption, the two nudge arrows, the three digit
 * cells of the setting, the framed panel, and the play gauge - two green rules
 * either side of the white centre line, spaced by the selected play scaled
 * into pixels.
 */
void DrawNegconSteerPlayScreen(void) {
    NegconUvTemplate unused;
    u8 *ot;
    u8 *prim;
    s32 span;
    s32 y;

    unused = D_80010070;
    func_80027874(0x18, 0x30, D_80010078, 0x7F81);
    ot = g_DrawBuffer + 0xCC;
    prim = *(u8 **)0x1F800000;
    prim = DrawLeftArrowWide(ot, prim, 0x28, 0xE0, g_NegconSteerPlay != 0);
    prim = DrawRightArrowWide(ot, prim, 0x108, 0xE0, g_NegconSteerPlay != 3);
    prim = QueueSpriteTransWide(
        ot, prim, 0x70, 0x30, 0xC, 0x18, 0x8C, 0x18, 0x7F81);
    prim = QueueSpriteTransWide(
        ot, prim, 0x7C, 0x30, 0xC, 0x18, g_NegconSteerPlay * 12 + 152, 0x18, 0x7F81);
    prim = QueueSpriteTransWide(
        ot, prim, 0x88, 0x30, 0xC, 0x18, 0x6C, 0x30, 0x7F81);
    prim = QueueDrawModePrimWide(ot, prim, 0x3F);
    prim = (u8 *)AddTilePrim(
        (s32)ot, (s32)prim, 0, 0x28, 0x124, 0x40, 0, 0, 0);
    prim = (u8 *)AddTilePrim(
        (s32)ot, (s32)prim, 0, 0x26, 0x125, 0x44, 0xFF, 0xFF, 0xFF);
    span = ((g_NegconPlayPercent[g_NegconSteerPlay] << 7) / 100) * 2;
    y = 230 - span;
    prim = QueueLineWide(ot, prim, 0x94, y, 0xA8, y, 0x20, 0x40, 0xFF);
    prim = QueueLineWide(ot, prim, 0x94, y + 1, 0xA8, y + 1, 0x20, 0x40, 0xFF);
    prim = QueueLineWide(
        ot, prim, 0x94, span + 230, 0xA8, span + 230, 0x20, 0x40, 0xFF);
    span = span + 231;
    prim = QueueLineWide(ot, prim, 0x94, span, 0xA8, span, 0x20, 0x40, 0xFF);
    prim = QueueLineWide(ot, prim, 0x94, 0xE6, 0xA8, 0xE6, 0, 0, 0);
    *(u8 **)0x1F800000 =
        QueueLineWide(ot, prim, 0x94, 0xE7, 0xA8, 0xE7, 0, 0, 0);
}

extern u8 g_PadType asm("D_801E4369");
/* Its backup, taken by BeginNegconCalibration. */
extern u16 g_NegconSteerPlaySaved asm("D_8019C75C");
/* The arrow pulse angle the setup screens advance every frame. */
extern s32 g_SetupArrowPulse asm("D_8007C13C");
/* One of the four controller-screen animation counters. */
extern s32 g_ControllerSceneAngleX asm("D_801E8A9C");

void DrawOptionHintBar(s32 arg0) asm("func_80023750");

/*
 * Game mode 10: pick the steering play with left/right, confirm with
 * start/cross (on to mode 11, the max-twist screen) or cancel with
 * circle/square. Cancelling - and unplugging the NeGcon - restores the
 * backed-up setting on the way back to mode 1.
 */
void UpdateNegconSteerPlayScreen(void) {
    g_AnimTimer++;
    g_SetupArrowPulse += 96;
    if (g_PadEdge2 & 0x90) {
        PlaySoundCue(3);
        g_GameMode = 1;
        g_NegconSteerPlay = g_NegconSteerPlaySaved;
    } else if (g_PadEdge2 & 0x860) {
        PlaySoundCue(2);
        g_GameMode = 11;
    }
    if (g_PadEdge2 & 0x8000) {
        if (g_NegconSteerPlay > 0) {
            PlaySoundCue(8);
            g_NegconSteerPlay = g_NegconSteerPlay - 1;
        }
    }
    if (g_PadEdge2 & 0x2000) {
        if (g_NegconSteerPlay < 3) {
            PlaySoundCue(8);
            g_NegconSteerPlay = g_NegconSteerPlay + 1;
        }
    }
    if (g_PadType != 0x23) {
        g_GameMode = 1;
        g_NegconSteerPlay = g_NegconSteerPlaySaved;
    }
    g_ControllerSceneAngleX = -896;
    DrawNegconSteerPlayScreen();
    DrawOptionHintBar(4);
    DrawControllerSetupScene(1);
}

extern NegconUvTemplate D_80010084;

/* "Maximum twist." */
extern char D_8001008C[];
/* The 0..3 maximum-twist setting this screen edits. */
extern s16 g_NegconMaxTwist asm("D_801E418C");

/*
 * Game mode 11's overlay: the caption, the two nudge arrows (lit only while
 * the setting can still move that way), the gauge sprite whose width and texel
 * column follow the 0..3 setting, its end cap, and the framed panel.
 */
void DrawNegconMaxTwistScreen(void) {
    NegconUvTemplate unused;
    u8 *ot;
    u8 *prim;
    s32 xoff;
    s32 w;

    unused = D_80010084;
    func_80027874(0x18, 0x30, D_8001008C, 0x7F81);
    ot = g_DrawBuffer + 0xCC;
    prim = *(u8 **)0x1F800000;
    prim = DrawLeftArrowWide(ot, prim, 0x28, 0xE0, g_NegconMaxTwist != 0);
    prim = DrawRightArrowWide(ot, prim, 0x108, 0xE0, g_NegconMaxTwist != 3);
    if (g_NegconMaxTwist == 3) {
        xoff = 0;
        w = 0x24;
    } else {
        xoff = 0xC;
        w = 0x18;
    }
    prim = QueueSpriteTransWide(
        ot, prim, xoff + 0x88, 0x30, w, 0x18, g_NegconMaxTwist * 24, 0x30, 0x7F81);
    prim = QueueSpriteTransWide(ot, prim, 0xAC, 0x30, 4, 0x18, 0x78, 0x30, 0x7F81);
    prim = QueueDrawModePrimWide(ot, prim, 0x3F);
    prim = (u8 *)AddTilePrim(
        (s32)ot, (s32)prim, 0, 0x28, 0x124, 0x40, 0, 0, 0);
    *(s32 *)0x1F800000 = AddTilePrim(
        (s32)ot, (s32)prim, 0, 0x26, 0x125, 0x44, 0xFF, 0xFF, 0xFF);
}

/* Its backup, taken by BeginNegconCalibration. */
extern u16 g_NegconMaxTwistSaved asm("D_8019CB04");

/*
 * Game mode 11: pick the maximum twist range with left/right, confirm with
 * start/cross or cancel with circle/square. Cancelling - and unplugging the
 * NeGcon - restores the backed-up setting on the way back to mode 1.
 */
void UpdateNegconMaxTwistScreen(void) {
    g_AnimTimer++;
    if (g_PadEdge2 & 0x90) {
        PlaySoundCue(3);
        g_GameMode = 1;
        g_NegconMaxTwist = g_NegconMaxTwistSaved;
    } else if (g_PadEdge2 & 0x860) {
        PlaySoundCue(2);
        g_GameMode = 1;
    }
    if (g_PadEdge2 & 0x8000) {
        if (g_NegconMaxTwist > 0) {
            PlaySoundCue(8);
            g_NegconMaxTwist = g_NegconMaxTwist - 1;
        }
    }
    if (g_PadEdge2 & 0x2000) {
        if (g_NegconMaxTwist < 3) {
            PlaySoundCue(8);
            g_NegconMaxTwist = g_NegconMaxTwist + 1;
        }
    }
    if (g_PadType != 0x23) {
        g_GameMode = 1;
        g_NegconMaxTwist = g_NegconMaxTwistSaved;
    }
    g_ControllerSceneAngleX = -896;
    DrawNegconMaxTwistScreen();
    DrawOptionHintBar(4);
    DrawControllerSetupScene(1);
}
