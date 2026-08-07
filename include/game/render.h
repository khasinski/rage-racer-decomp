#ifndef GAME_RENDER_H
#define GAME_RENDER_H

#include "common.h"
#include "game/scratchpad.h"
#include "psyq/gte.h"

typedef struct TimedDrawCommand {
    s16 time;
    s16 type;
    s32 arg0;
    s32 arg1;
} TimedDrawCommand;

/* A ready-made SPRT description; BuildSpriteFromDesc expands it into a scratchpad
 * SPRT. g_TachoNeedleSprite is the one instance. */
typedef struct GameSpriteDesc {
    u16 x;
    u16 y;
    u16 w;
    u16 h;
    u8 u0;
    u8 pad9;
    u8 v0;
    u8 padB;
    u16 clut;
    u8 padE[2];
    s32 semiTrans;
} GameSpriteDesc;

typedef struct GameRenderAxisMatrix {
    s16 m[3][3];
} GameRenderAxisMatrix;

/*
 * Camera / horizon row. Base g_CamRow, indexed +8*screen. `horizon` is added
 * to / subtracted from a GameRenderObject's y during the view transform.
 */
typedef struct CamRow {
    u8 pad0[0xC];
    s16 axis0;   /* 0x0C */
    u16 axis1;   /* 0x0E */
    u16 axis2;   /* 0x10 */
    s16 horizon; /* 0x12 */
} CamRow;


/* One display buffer, which is what InitRenderState sets that rectangle to.
 * See SetupDisplay240 below: the 240 mode is "two 320x240 buffers stacked at
 * y=0 / y=0xF0" and sets the GTE projection with SetGeomScreen(0x140). The
 * 480 mode is the pair treated as one, which is the SCRATCH_CLIP_Y1 = 0x1E0 that
 * menu/frontend.c writes. */
#define SCREEN_WIDTH   0x140
#define SCREEN_HEIGHT  0xF0

typedef struct GameRenderView {
    s16 angle_0;    /* 0x00 */
    s16 angle_2;    /* 0x02 */
    s16 angle_4;    /* 0x04 */
    s16 horizon_6;  /* 0x06 */
} GameRenderView;

/*
 * The camera car object at g_CameraCar. Eleven of its words are also declared
 * as standalone globals (g_CameraCarY, g_CameraCarAngleY, g_CameraCarSpeed and
 * so on), which is where the names below come from.
 *
 * This is the same block as GameCarRuntime in game/car.h: 45 offsets agree on
 * type, and both arrive at headingAngle for 0xA0 independently. They are still
 * two declarations because the tails disagree - car.h reads 0x7C, 0x80, 0x84,
 * 0x88, 0xAC and 0xBC as halfwords, which shifts every later offset by four.
 */
typedef struct GameRenderObject {
    s32 x;          /* 0x00 */
    s32 y;          /* 0x04 */
    s32 z;          /* 0x08 */
    s32 field_0C;
    s32 motionX;
    s32 motionY;
    s32 motionZ;
    s32 field_1C;
    s32 angleX;   /* 0x20 */
    s32 angleY;   /* 0x24 */
    s32 angleZ;   /* 0x28 */
    s32 field_2C;
    s32 trackPointIndex;
    s32 field_34;
    s32 field_38;
    s32 field_3C;
    s32 field_40;
    s32 angle_44;   /* 0x44 */
    s32 flags_48;   /* 0x48 (bit 0x1000) */
    s32 field_4C;
    s32 angle_50;   /* 0x50 */
    s32 angle_54;   /* 0x54 */
    s32 angle_58;   /* 0x58 */
    s32 field_5C;
    s32 field_60;   /* 0x60 */
    s32 field_64;   /* 0x64 */
    s32 progressA;
    s32 progressB;
    s32 trackProgress;   /* 0x70 */
    s32 previousTrackProgress;
    s32 trackSection;
    s32 field_7C;
    s32 field_80;
    s32 field_84;
    s32 field_88;
    s32 field_8C;
    s32 field_90;
    s32 field_94;
    s32 field_98;
    s32 field_9C;
    s32 headingAngle;
    s32 speed;
    s32 speedRamp;
    s32 field_AC;
    s32 field_B0;
    s32 field_B4;
    s32 field_B8;
    s32 field_BC;
    s32 field_C0;
    s32 stepX;
    s32 field_C8;
    s32 stepZ;
    s32 field_D0;
    s32 field_D4;
    s32 field_D8;
    s32 field_DC;
    s32 field_E0;
    s32 field_E4;   /* 0xE4 */
} GameRenderObject;

typedef struct GameRenderSourcePoint {
    u16 field_0;
    u8 pad2[2];
    u16 field_4;
    u8 pad6[2];
    u16 field_8;
    u8 padA[0x16];
    u16 field_20;
    u8 pad22[2];
    u16 field_24;
    u8 pad26[2];
    u16 field_28;
    u8 pad2A[6];
    u32 field_30;
    u8 pad34[0x10];
    u16 field_44;
    u8 pad46[2];
    u16 field_48;
    u8 pad4A[0x16];
    u16 field_60;
    u8 pad62[0x2A];
    s16 field_8C;
    u8 pad8E[0x20];
    s16 field_AE;
} GameRenderSourcePoint;

typedef struct GameRenderSinglePoint {
    u16 field_0;
    u16 field_2;
    u16 field_4;
    u16 field_6;
    u16 field_8;
    u16 field_A;
    u16 field_C;
    u16 field_E;
    s32 field_10;
    u32 field_14;
    u16 field_18;
    u8 pad1A[2];
} GameRenderSinglePoint;

typedef struct GameRenderPairPoint {
    u16 first_0;
    u16 first_2;
    u16 first_4;
    u16 first_6;
    u16 first_8;
    u16 first_A;
    u16 first_C;
    u16 first_E;
    u16 second_10;
    u16 second_12;
    u16 second_14;
    u16 second_16;
    u16 second_18;
    u16 second_1A;
    u16 second_1C;
    u16 second_1E;
    s32 field_20;
    u32 first_24;
    u32 second_28;
    u16 first_2C;
    u16 second_2E;
} GameRenderPairPoint;

/*
 * Rotation-matrix builders. Each fills only the 3x3 part of `mtx` with a
 * rotation about one axis by a 12-bit angle (0x1000 = one turn), leaving the
 * translation alone; sin/cos come from rsin/rcos
 * and 1.0 is 0x1000.
 */
void BuildRotMatrixZ(void *mtx, s32 angle);
void BuildRotMatrixY(void *mtx, s32 angle);
void BuildRotMatrixX(void *mtx, s32 angle);
/*
 * Composes Y*X*Z from the scratchpad camera angles (0x1F800018 / 0x1C / 0x20)
 * into the scratchpad matrix at 0x1F800028 and installs it with SetRotMatrix;
 * D_8019CB18 gets the same matrix pre-multiplied by a 180-degree Y turn.
 */
void SetCameraRotMatrix(void);
/*
 * atan2 over the arctangent table g_AtanTable, in 12-bit angle units
 * (0x400 = 90 degrees). Argument order is (x, y), the reverse of C's atan2:
 * Atan2(0, +y) is 0x400.
 */
s32 Atan2(s32 x, s32 y);
/* Shortest way round the 0x1000 circle between two 12-bit angles: the signed
 * delta from `from` to `to` in (-0x800, 0x800], and its magnitude. */
s32 GetAngleDelta(s32 from, s32 to);
/* (s32 a, s32 b); left unprototyped because UpdateCarDrivetrain calls it with two
 * extra arguments that the original left live in a2/a3. */
s32 GetAngleDistance();

/*
 * Sets up both environments for the frame and clears to (r, g, b):
 * 240 = two 320x240 buffers stacked at y=0 / y=0xF0, 480 = one 320x480 pair.
 * Both also set the GTE projection (SetGeomOffset / SetGeomScreen 0x140).
 */
void SetupDisplay240(s32 r, s32 g, s32 b);
void SetupDisplay480(s32 r, s32 g, s32 b);

/*
 * Model banks. SelectModelBank points the scratchpad bank cursor
 * (0x1F800050/54/58) and g_ModelBankCount at entry `index` of the registered
 * bank table g_ModelBanks; SubmitModel then walks model `index` of that bank
 * into the scratchpad ordering table. The Course variants use the separate
 * course object bank at 0x1F800048 (size g_CourseModelCount); ...2 is the same
 * routine running the second opcode table (jtbl_8007DA64, not jtbl_8007DA54).
 * All four are entry points of the hand-written GTE engine, so `ctx` is always
 * the scratchpad base 0x1F800000.
 */
void SelectModelBank(s32 index);
void SubmitModel(void *ctx, s32 index);
void SubmitCourseModel(void *ctx, s32 index);
void SubmitCourseModel2(void *ctx, s32 index);

/* Per-frame draw loop over the world object array g_CourseObjects: culls each entry
 * against the visibility bitmask, transforms it and submits its model. */
void DrawCourseObjects(void);

/*
 * Per-frame environment step: advances the course's environment command script
 * (g_EnvScriptCursor), cross-fades the 16-entry sky/fog CLUT between
 * g_EnvironmentModePrev and g_EnvironmentMode into VRAM at (0xE0, 0x1E6), and
 * updates the GTE far colour and fog distance. Does not draw anything.
 */
void UpdateEnvironment(void);

/*
 * 2D/HUD primitive emitters. All of them pack the primitive at the scratchpad
 * cursor `*(u8 **)0x1F800000`, bump that cursor past the primitive and link it
 * into the ordering table `ot` with AddPrim. Sprites and textured quads turn a
 * linear CLUT index into VRAM clut coordinates (20 cluts per row, first row at
 * y = 0x1E0). Where an `alpha` argument exists, 0xFF means opaque; anything
 * else enables semi-transparency and appends a blend packet via func_80017390.
 */
void DrawSprite(
    void *ot,
    s16 x0,
    s16 y0,
    s16 w,
    u16 h,
    u16 u0,
    u16 v0,
    u8 r,
    u8 g,
    u8 b,
    u16 clutIndex,
    s32 shadeTex,
    s32 semiTrans,
    u32 flags);
void DrawFlatTriangle(
    void *ot,
    s16 x0,
    s16 y0,
    s16 x1,
    u16 y1,
    u16 x2,
    u16 y2,
    u8 r,
    u8 g,
    u8 b,
    s32 semiTrans,
    u32 flags);

/* The same routine seen through signed coordinates. Retail's two callers
 * disagree about these three parameters and the disagreement is load-bearing:
 * the definition in SetDrawClipRect.c matches only as u16, while
 * DrawScriptedTriangle sign-extends them, which a u16 prototype cannot
 * produce. Both views are therefore real. See names.md 30 on width and
 * signedness being per-use rather than per-field. */
void DrawFlatTriangleSigned(
    void *ot,
    s16 x0,
    s16 y0,
    s16 x1,
    s16 y1,
    s16 x2,
    s16 y2,
    u8 r,
    u8 g,
    u8 b,
    s32 semiTrans,
    u32 flags) asm("DrawFlatTriangle");
void DrawFlatQuad(
    void *ot,
    s16 x0,
    s16 y0,
    s16 x1,
    u16 y1,
    u16 x2,
    u16 y2,
    u16 x3,
    u16 y3,
    u8 r,
    u8 g,
    u8 b,
    s32 semiTrans,
    u32 flags);
/* POLY_FT4: four xy/uv pairs, flat rgb, tpage and a CLUT index as depth key. */
void GameDrawTexturedQuad(
    s32 ot,
    s16 x0,
    s16 y0,
    s16 x1,
    u16 y1,
    u16 x2,
    u16 y2,
    u16 x3,
    u16 y3,
    u8 u0,
    u8 v0,
    u8 u1,
    u8 v1,
    u8 u2,
    u8 v2,
    u8 u3,
    u8 v3,
    u8 r,
    u8 g,
    u8 b,
    u16 clutIndex,
    s32 shadeTex,
    s32 semiTrans,
    u16 tpage);
/* TILE: solid rectangle at (x, y) sized (w, h). */
void DrawSolidRect(
    void *ot,
    s32 x,
    s32 y,
    s32 w,
    u16 h,
    u8 r,
    u8 g,
    u8 b,
    u8 alpha);
void DrawLine(
    void *ot,
    s32 x0,
    s32 y0,
    s32 x1,
    u16 y1,
    u8 r,
    u8 g,
    u8 b,
    u8 alpha);
/* LINE_F3: flat-shaded 3-point polyline. */
void DrawPolyLine3(
    void *ot,
    s16 x0,
    s16 y0,
    s16 x1,
    u16 y1,
    u16 x2,
    u16 y2,
    u8 r,
    u8 g,
    u8 b,
    u8 alpha);
/* LINE_G2: line interpolating rgb0 -> rgb1. */
void DrawGradientLine(
    void *ot,
    s32 x0,
    s32 y0,
    s32 x1,
    u16 y1,
    u8 r0,
    u8 g0,
    u8 b0,
    u8 r1,
    u8 g1,
    u8 b1,
    u8 alpha);
/* Two-pixel-thick rectangle border, built from six DrawLine calls. */
void DrawRectOutline(
    void *ot,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    u8 r,
    u8 g,
    u8 b,
    u8 alpha);
/* Clips (x, y, w, h) to the 320x480 frame and queues a SetDrawArea packet. */
void SetDrawClipRect(
    void *ot,
    s32 x,
    s32 y,
    s32 w,
    s32 h);

/*
 * Text and number output, both built on DrawSprite. The two fonts differ
 * only in cell size and glyph table: small = 6x12 (D_8007F984), large = 8x16
 * (D_8007FA3C). Bit 0x80 of `flags` selects fixed-width cells instead of the
 * per-glyph widths in the table.
 */
void DrawSmallText(
    s32 x,
    s16 y,
    u8 *str,
    u8 r,
    u8 g,
    u8 b,
    u16 clutIndex,
    s32 flags);
void DrawLargeText(
    s32 x,
    s16 y,
    u8 *str,
    u8 r,
    u8 g,
    u8 b,
    u16 clutIndex,
    s32 flags);
/* Right-aligned decimal, up to 10 digits, leading zeros blanked. Returns the
 * number of digit sprites emitted. flags: 1 = large font, 4 = fixed digits at
 * v=0xDC, 8 = draw into the overlay OT layer. */
s32 GameDrawNumber(
    s32 x,
    s16 y,
    s32 flags,
    u32 value,
    u8 r,
    u8 g,
    u8 b,
    u16 clutIndex,
    u8 primitiveCount);
/* Blits an 8x6 bit pattern from D_8007F6E8 as 4x8 blocks; negative argument
 * animates through the table. */
void DrawBitPatternOverlay(s32 pattern);

/*
 * Base of the draw work area for the frame being built. Almost every drawing
 * routine takes its ordering table from g_DrawBuffer + 0xCC and links
 * primitives into it (AddPrim / QueueDrawModePrim); further sub-buffers live
 * at +0x70, +0xBD0 and +0x16C8. Set by the display swap, not from C yet.
 *
 * Six translation units need a different declared type (four `volatile`, one
 * plain s32) and carry their own aliased declaration so that the name is the
 * same everywhere without changing the generated load.
 */
/*
 * Measured under emulation (2026-08-03, in-race, ~/Projects/rage-trace):
 * Gpu_ClearOTagDma is the only writer of two ordering tables, each exactly
 * 0x1600 bytes = 1408 entries, at 0x8019CF04 and 0x801C06EC -- one per frame
 * context, so the contexts are 0x237E8 apart. Those two runs are also the
 * busiest memory in a race frame.
 */
extern u8 *g_DrawBuffer;

/*
 * Full-screen fade level, 0..0x100, passed straight to
 * DrawFullscreenFadeTile and DrawRaceEndBanner. Each frame the
 * owning scene adds g_FadeStep and clamps back into range, so a scene fades in
 * or out just by setting the step.
 */
extern s32 g_FadeLevel;

/* Per-frame delta added to g_FadeLevel; negative fades out, positive fades in. */
extern s32 g_FadeStep;

/*
 * Timed draw script: a table of {time, type, arg0, arg1} entries replayed
 * against a progress counter, terminated by time < 0. Element types 0/1/9 draw
 * a sprite, 10/19 a line, 20/29 a triangle and 30/39 a textured quad; the +9
 * variants are skipped while g_MenuAltLayout is set. Each element interpolates its
 * position from a packed s16 velocity pair by (elapsed * velocity) >> 5.
 * Returns 1 once the progress counter has reached the terminator's limit.
 */
s32 RunTimedDrawScript(
    void *commands,
    s32 *progress,
    s32 step);

/*
 * The progress counter the menu/UI screens hand to RunTimedDrawScript. Each
 * screen resets it to 0 on entry, then passes &g_UiScriptProgress with step +1
 * while opening and -1 while closing, and treats `<= 0` as "the close animation
 * has finished". It is also fed to DrawFadingMenuSprites as the elapsed time of the
 * panel it is animating.
 */
extern s32 g_UiScriptProgress;

/*
 * A second, independent RunTimedDrawScript progress counter. The menu
 * screens animate two script layers at once and step them separately - see
 * UpdateCourseSelectScreen, which drives &g_UiScriptProgress against one command table and
 * &g_UiScriptProgress2 against the screen's own tables in the same frame. Which
 * layer is "background" and which is "foreground" is not settled, hence the
 * neutral name (cf. g_PadPressedRepeat / g_PadPressed).
 */
extern s32 g_UiScriptProgress2;
void DrawScriptedSprite(
    s32 elapsed,
    u8 *style,
    u8 *record,
    s32 useAlpha);
void DrawScriptedLine(s32 elapsed, u8 *style, u8 *record);
void DrawScriptedTriangle(s32 elapsed, u8 *style, u8 *record);
void DrawScriptedQuad(s32 elapsed, u8 *style, s32 *record);

/*
 * Low-level packet builders from the first 0x3900 bytes of .text (the boot /
 * controller-setup / top-level block). Unlike the GameDraw* emitters above,
 * these take the ordering table AND the packet cursor explicitly and return
 * the advanced cursor, so a caller can build a run of packets and write the
 * scratchpad cursor back once at the end.
 *   "Shaded" = takes an extra intensity written to r = g = b (SetShadeTex is
 *              NOT applied, so the texel is modulated).
 *   "Trans"  = calls SetSemiTrans(prim, 1).
 * Plain GameQueueSprite/GameQueueTexturedRect apply SetShadeTex(prim, 1), i.e.
 * the texture is drawn raw.
 */
/* SPRT, 20 bytes. */
u8 *GameQueueShadedSprite(
    void *ot,
    u8 *prim,
    s16 x,
    s16 y,
    s16 w,
    s16 h,
    u8 u,
    u8 v,
    u16 clutIndex,
    u8 intensity);
u8 *GameQueueShadedSpriteTrans(
    void *ot,
    u8 *prim,
    s16 x,
    s16 y,
    s16 w,
    s16 h,
    u8 u,
    u8 v,
    u16 clutIndex,
    u8 intensity);
u8 *GameQueueSpriteTrans(
    void *ot,
    u8 *prim,
    s16 x,
    s16 y,
    s16 w,
    s16 h,
    u8 u,
    u8 v,
    u16 clutIndex);
/* TILE, 16 bytes. */
u8 *GameQueueTileTrans(
    void *ot,
    u8 *prim,
    s16 x,
    s16 y,
    s16 w,
    s16 h,
    u8 r,
    u8 g,
    u8 b);
/* LINE_F2, 16 bytes. */
u8 *GameQueueLine(
    void *ot,
    u8 *prim,
    s16 x0,
    s16 y0,
    s16 x1,
    s16 y1,
    u8 r,
    u8 g,
    u8 b);
/*
 * POLY_FT4, 40 bytes, built as an axis-aligned rectangle at (x, y) sized
 * (w, h). A negative w or h flips the u or v axis instead of the geometry.
 * GameQueueShadedTexturedRect derives the uv extent from w/h;
 * GameQueueTexturedRect takes uSpan/vSpan separately.
 */
u8 *GameQueueShadedTexturedRect(
    void *ot,
    u8 *prim,
    s16 x,
    s16 y,
    s16 w,
    s16 h,
    u8 u,
    u8 v,
    u16 clutIndex,
    u16 tpage,
    u8 intensity);

/*
 * Wide forms used by callers whose packet fields are still represented as
 * words at the call boundary.  The dispatcher selects the compact SPRT packet
 * when no four-corner geometry is needed, or POLY_FT4 when texture span and
 * geometry must remain independent.
 *
 * This must remain inline.  GCC 2.6.3 records the widest outgoing argument
 * area before it folds the packet-kind branch; that accounts for the 48-byte
 * argument area in DrawStartCountdown's retail stack frame.
 */
/* Body: (void *ot, u8 *prim, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v,
 * s32 clutIndex). The list stays empty because most callers hold the ordering
 * table and the packet cursor in s32 locals, and cc1 2.6.3 dies on the
 * int-to-pointer argument conversion rather than diagnosing it. */
u8 *GameQueueSprite();
u8 *GameQueueTexturedRect(
    void *ot,
    u8 *prim,
    s16 x,
    s16 y,
    s16 w,
    s16 h,
    u8 u,
    u8 v,
    u8 uSpan,
    u8 vSpan,
    u16 clutIndex,
    u16 tpage);

typedef enum GameTexturePacketKind {
    GAME_TEXTURE_PACKET_SPRT,
    GAME_TEXTURE_PACKET_FT4
} GameTexturePacketKind;

static __inline__ u8 *GameQueueTexturePacketWide(
    void *ot,
    u8 *prim,
    s32 x,
    s32 y,
    s32 w,
    s32 h,
    s32 u,
    s32 v,
    s32 uSpan,
    s32 vSpan,
    s32 clutIndex,
    s32 tpage,
    GameTexturePacketKind kind)
{
    if (kind == GAME_TEXTURE_PACKET_SPRT) {
        return GameQueueSprite(
            ot, prim, x, y, w, h, u, v, clutIndex);
    }
    return GameQueueTexturedRect(
        ot, prim, x, y, w, h, u, v, uSpan, vSpan, clutIndex, tpage);
}
/*
 * DR_MODE, 12 bytes: SetDrawMode(prim, 0, 1, tpage, &g_DrawModeEnv) + AddPrim.
 * This is the "blend packet" the GameDraw* emitters above append when their
 * alpha argument is not 0xFF. SetDrawModePacket only fills the packet in
 * place (no AddPrim, no cursor advance) and has no callers in the retail EXE.
 */
u8 *QueueDrawModePrim(void *ot, u8 *prim, u16 tpage);
void SetDrawModePacket(u8 *prim, s32 tpage);

/*
 * A third font, separate from the small (6x12) / large (8x16) pair above:
 * fixed 8x8 SPRT_8 cells on tpage 9, uv from the two-byte-per-glyph table
 * g_Font8x8Cells indexed by (ch - 0x20), 8 pixels of advance per character. Each
 * variant closes the run with its own DR_MODE packet (tpage 9 / 0x29 / 0x49)
 * and stores the scratchpad cursor back itself.
 */
/* Body: (s32 x, s32 y, u8 *str, s32 clutIndex). The list stays empty because
 * record_entry.c hands a packet pointer to x, several callers hand a char or
 * void pointer to str, and swap_car_model_slot.c passes only three arguments;
 * cc1 2.6.3 segfaults on those conversions rather than diagnosing them. */
void DrawText8x8();
/* The body reads (s32, s32, u8 *, s32, u8); its callers were compiled
 * against a full word for every argument, so the list stays empty. */
void GameDrawText8x8Shaded();
/* The body reads (s32, s32, u8 *, s32); see GameDrawText8x8Shaded above. */
void DrawText8x8Trans();
/*
 * The proportional font: 12-pixel-tall SPRT cells. Characters below 'a' use
 * the {u, v} pairs at g_PropFontU with fixed 12x12 cells; 'a'..'u' and 'v'+ use
 * the {u, v, width, advance} rows at g_WordFontU / g_HighFontU, so lowercase is
 * proportionally spaced. Space advances 12 and emits nothing.
 * `intensity` == 0x100 selects the opaque raw-texture path (SetShadeTex);
 * anything else is written to r = g = b with SetSemiTrans.
 * DrawProportionalText is the wrapper that passes 0x100.
 */
void GameDrawProportionalTextShaded(
    s32 x,
    s32 y,
    u8 *str,
    s32 clutIndex,
    s32 intensity);
/* Body: (s32 x, s32 y, u8 *str, s32 clutIndex). The list stays empty because
 * callers spell x as a packet pointer and str as char * / void *, and cc1
 * 2.6.3 segfaults on both conversions. */
void DrawProportionalText();

/* Loads the GTE light matrix with g_SceneLightMatrix * `view`. */
void SetGteLightMatrix(Matrix *view);
/*
 * Per-object GTE setup: writes (object position - camera position) as an
 * SVECTOR into `work`, runs it through the scratchpad view matrix at
 * 0x1F800028, scales the result by 4 into work's Matrix translation, then
 * SetRotMatrix(rot) + SetTransMatrix(&work->mtx).
 * `work` layout: SVECTOR @0, VECTOR @8, Matrix @24 (m @24, t @44).
 */
/* Callers spell the third argument Matrix *, void * or Vec4 *; an empty
 * parameter list lets each keep its own.  The body reads
 * (void *work, void *objectPos, Matrix *rot). */
void SetGteObjectMatrix();

/*
 * The environment colour timeline and the sky it feeds. The state is nine
 * 12-byte { cur, from, to } RGB slots at g_EnvColors + 0x0C * k - slot 0 the GTE
 * far/fog colour, 1..8 the sky gradient - lerped every frame by UpdateEnvironment.
 */
/* One packed RGB triple of that timeline. The block starts at 0x801E3FB6, i.e.
 * 2 mod 4, so every word in it is half-aligned and must be declared packed --
 * the compiler emits lwl/lwr, not lw. */
typedef struct GameEnvColor {
    u32 rgb __attribute__((packed));
} GameEnvColor;

/* One timeline slot: the live colour and the pair it is being lerped between.
 * LoadEnvironmentCue rolls `cur` into `from` and the cue's value into `to`;
 * UpdateEnvironment walks `cur` across over g_EnvLerpDuration frames. */
typedef struct GameEnvColorSlot {
    GameEnvColor cur;
    GameEnvColor from;
    GameEnvColor to;
} GameEnvColorSlot;

/* The nine slots. [0] is the GTE far/fog colour (SetFarColor takes its three
 * bytes; g_EnvSpare is its unused fourth byte), [1..8] the sky-gradient bands.
 * Only six are lerped on a given course: [5]/[6] and [7]/[8] are alternates
 * picked by g_CourseIndex == 2. */
extern GameEnvColorSlot g_EnvColors[9];

/* Jumps that timeline to `time` and applies one frame, then programs
 * SetFarColor + SetFogNear. */
/* replay.c hands it a pointer difference, environment_script.c an s32;
 * an empty parameter list lets both units keep their own spelling. */
void SeekEnvironmentScript();
/* The backdrop: half a 16-segment panorama cylinder over gradient bands shaded
 * between successive colour slots. */
void DrawSkyBackground(void);

/* Environment mode of the loaded course variant, from variant data +0x2C. Also
 * the index of the target 48-byte (16 x RGB) sky palette in g_EnvPaletteTable;
 * mode 2 alone gets clear fog (SetFogNear ramps to 0x7FFF, else to 0x1770). */
extern s16 g_EnvironmentMode;
/* The mode the previous variant had; the sky-CLUT lerp's source palette. */
extern s32 g_EnvironmentModePrev;
/* Sky palette records, 48 bytes each, indexed by environment mode. Installed
 * from the loaded environment block by SetEnvPaletteTable. */
extern u8 *g_EnvPaletteTable;
/* g_EnvironmentMode == 4. Picks DrawStaticScenery's model 0x3B over 0x3A
 * and the `flags & 2` prop set over `flags & 1`; also forwarded to scratchpad
 * 0x1F800084 by every car/track renderer. */
extern s32 g_IsEnvironmentMode4;
/* That forwarding slot is SCRATCH_ENV_MODE4 in game/scratchpad.h. */

/*
 * Per-view cell culling, rebuilt every frame by BuildVisibleCells and swapped in
 * lockstep for the mirror pass. Per-file types; see docs/names.md 12c.
 *   g_VisibleCellMask  g_VisibleCellMask  32 words, mask[sy] |= 1 << sx over the grid
 *   g_VisibleCellList  g_VisibleCellList  the matching visible-cell record list
 *   g_SceneLightMatrix g_SceneLightMatrix  assigned from a per-scene constant, then
 *                                  combined per object and set with gte_SetLightMatrix
 */

/* Declared identically by 82 translation units before this
 * header carried them. */

extern char D_80013374[];
extern s16 D_8007D380[][2];
extern s16 D_80092B08[];
extern s16 D_80093308[];
extern s16 D_80093B08[];
extern s16 D_8009E782;
extern Matrix D_8019CB18;
extern u8 D_801E8AFC;
extern u8 g_CarModelByCourse[][11];
extern u8 g_CarTrackSection[];
extern s32 g_FmvFrameHeight;
extern s32 g_FmvFrameWidth;
extern s32 g_FmvState;
extern s32 g_FmvStreamEnded;
extern volatile u32 *g_FmvStripBuffers[];
extern s16 g_FmvStripHeight;
extern s16 g_FmvStripWidth;
extern s16 g_MirrorViewEnabled;
extern s32 g_ModelBankCount;
extern s16 g_NegconSteer;
extern s16 g_NegconSteerRange[];
extern u8 *g_ReplayFramesTimeAttack;
extern s32 g_SetupArrowPulse;
extern s16 g_SinTable[];
extern s32 g_StreamSectorCount;
extern s32 g_TrackTextureSectionHi;
extern s32 g_TrackTextureSectionLo;

void ApplyZoneLighting(s32 arg0, Matrix *arg1);
s32 BezierEase(s32 t, s32 control);
void BuildVisibleCells(s32 arg0, s32 arg1);
void EndMirrorPass(void);
void RestoreColorMatrix(void);
s32 rsinCore(s32 arg0);

/* Declared identically by 95 translation units before this
 * header carried them. */

extern char D_8001339C[];
extern char D_800133B4[];
extern char D_800133C4[];
extern char D_800133DC[];
extern char D_800133E8[];
extern char D_800133F4[];
extern char D_80013410[];
extern char D_8001342C[];
extern char D_80013438[];
extern char D_80013478[];
extern char D_80013498[];
extern char D_800134AC[];
extern char D_800134C4[];
extern char D_800135CC[];
extern char D_800135E0[];
extern char D_800135F8[];
extern s16 D_8007C70A;
extern u8 D_8007C728[];
extern u8 D_8007C738[];
extern u8 D_8007C739[];
extern u8 D_8007C73A[];
extern u8 D_8007DAF4[];
extern u8 D_8007DBE4[];
extern Matrix D_8009AF00;
extern s32 D_8009B250[];
extern s32 D_8009B264;
extern s16 D_8009E836;
extern s32 D_8009E888;
extern s32 D_8019C7E4;
extern s16 D_8019CEAA;
extern s16 D_8019CEAE;
extern u8 D_801C0504[];
extern u8 D_801C0618[];
extern s16 D_801C0692;
extern s16 D_801C0696;
extern u8 D_801E4BF8[];
extern u8 D_801E4CF7;
extern s16 g_AtanTable[];
extern volatile s32 g_CameraPathKey;
extern s32 g_CameraPathNextKey;
extern s32 g_CameraPathTick;
extern volatile u16 g_DispEnv0H;
extern u8 g_DispEnv0Rgb24;
extern volatile u16 g_DispEnv0W;
extern volatile u16 g_DispEnv0X;
extern volatile u16 g_DispEnv1H;
extern u8 g_DispEnv1Rgb24;
extern volatile u16 g_DispEnv1W;
extern u8 g_DrawEnv0Dither;
extern u8 g_DrawEnv1Dither;
extern u8 g_DrawEnvCache;
extern volatile s32 g_FmvStripDone;
extern volatile s32 g_FmvStripRectIndex;
extern volatile s16 g_FmvUploadRectX;
extern volatile s16 g_FmvUploadRectY;
extern s32 g_FmvVlcIndex;
extern u8 g_Font8x8Cells[];
extern u8 g_GpuJumpTable[];
extern u8 g_HighFontU[];
extern u8 g_HighFontV[];
extern u8 g_HighFontWidth[];
extern u8 g_HighFontYOffset[];
extern s32 g_MirrorPanelY;
extern s32 g_MirrorUnlocked;
extern char g_MsgFmvDecodeTimeout[];
extern char g_MsgFmvSector[];
extern s32 g_NegconPlayScale[];
extern u8 g_PropFontU[];
extern u8 g_PropFontV[];
extern s32 g_TrackTextureCursorRow;
extern s32 g_TrackTexturePageWanted;
extern s32 g_TrackTextureTargetRow;
extern u8 g_VramHeightTable[];
extern u8 g_VramWidthTable[];
extern u8 g_WordFontAdvance[];
extern u8 g_WordFontU[];
extern u8 g_WordFontV[];
extern u8 g_WordFontWidth[];

void BuildAxisRotMatrix(GameRenderAxisMatrix *out, s32 sinTerm, s32 cosTerm, s32 axis);
s32 CdRead2(s32 arg0);
void DecDCTReset(s32 arg0);
void DecDCTin(volatile u32 *arg0, s32 arg1);
void DrawMinuteSecondTime(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
u8 *DrawMirrorFrame(u8 *packet);
void *DrawOTagEnv(void *ot, void *env);
void GPU_cw(void *arg0);
void Gpu_BuildDrawEnvCmds(void *arg0, void *arg1);
s32 Gpu_Reset(s32 arg0);
void MatrixApplyVectorComponents(s16 *mtx, s32 x, s32 y, s32 z, s32 *outX, s32 *outY, s32 *outZ);
void MatrixApplyZRotation(Matrix *arg0, s32 arg1);
void MdecUnpackStatus(void *arg0, s32 arg1);
s32 SetGraphReverse(s32 arg0);
void SetupFmvBuffers(s32 arg0);
void SwapTrackTexturePageNow(void);
void SwapTrackTextureRow(void);

#endif
