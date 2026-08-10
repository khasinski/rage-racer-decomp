#ifndef GAME_SCRATCHPAD_H
#define GAME_SCRATCHPAD_H

#include "common.h"
#include "game/vector.h"
#include "psyq/gte.h"

/*
 * The PS1 scratchpad (fast RAM) at 0x1F800000, as the renderer lays it out.
 *
 * Two views of the same 0x80 bytes exist because the code uses both: this
 * struct, for the routines that walk the block through a pointer, and the
 * named slot macros below, for the routines that touch one word at a time.
 * The hand-written GTE engine always runs with $a0 = 0x1F800000 and reads
 * these words by offset, so each macro name is what the engine does with the
 * word, cited from the disassembly.
 *
 * The slots are macros, not the `extern T x asm("0x1F8000NN")` spelling used
 * in render/set_gte_light_matrix.c: that spelling is a const-CSE lever and
 * moves the emitted code, a macro cannot.
 */
typedef struct GameScratchpadRenderState {
    s32 pad0;
    void *primData;
    u8 pad8[0x14];
    s32 depth;
    u8 pad20[8];
    Matrix matrix;
    u8 pad48[0x20];
    s32 orderingFlag;
    s32 mode;
    u8 pad70[8];
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
} GameScratchpadRenderState;

typedef struct ObjectMatrixWork {
    s16 relative[3];
    s16 pad06;
    LVec view;
    s32 pad14;
    Matrix mtx;
} ObjectMatrixWork;

#define SCRATCH_OBJECT_MATRIX_WORK ((ObjectMatrixWork *)0x1F80011C)

typedef union ScratchViewCoordinate {
    s32 value;
    struct {
        u16 low;
        u16 high;
    } half;
} ScratchViewCoordinate;

typedef struct ScratchViewCoordinates {
    ScratchViewCoordinate x;
    ScratchViewCoordinate y;
    ScratchViewCoordinate z;
} ScratchViewCoordinates;

typedef union ScratchViewPosition {
    ScratchViewCoordinates components;
    LVec vector;
} ScratchViewPosition;

typedef struct ScratchViewState {
    ScratchViewPosition position;
    s32 reserved14;
    s32 angleX;
    s32 angleY;
    s32 angleZ;
} ScratchViewState;

typedef union ScratchBlockAddress {
    s32 *words;
    Block16 *blocks;
} ScratchBlockAddress;

typedef union CarTrackRadius {
    s32 value;
    struct {
        u16 low;
        u16 high;
    } half;
} CarTrackRadius;

typedef struct CarTrackScratch {
    s32 arcCenterX;
    s32 arcCenterZ;
    s32 carToCenterX;
    s32 carToCenterZ;
    CarTrackRadius carRadius;
    CarTrackRadius pointRadius;
    CarTrackRadius nextPointRadius;
    u8 pad1C[8];
    s32 pointToCenterX;
    s32 nextPointToCenterX;
    s32 pointToCenterZ;
    s32 nextPointToCenterZ;
    s32 headingSin;
    s32 headingCos;
    s32 knockbackMode;
    u8 pad40[0x20];
    u16 offsetX;
    s16 offsetY;
    s16 offsetZ;
    s16 offsetPad;
    s32 correctionX;
    s32 correctionY;
    s32 correctionZ;
    s32 reserved74;
    s16 curveMode;
    s16 arcIndex;
    s16 arcSpan;
    s16 sweptAngle;
    s16 pointAngle;
    s16 nextPointAngle;
    s16 arcLateral;
    s16 trackWidth;
    s16 rightHalfWidth;
    s16 leftHalfWidth;
    s16 relativeHeading;
    s16 crossSlope;
    s16 heading;
    s16 surfacePitch;
    s16 camberAngle;
    u16 segmentLength;
} CarTrackScratch;

#define CAR_TRACK_SCRATCH ((CarTrackScratch *)0x1F80011C)
#define CAR_TRACK_POINT_RADIUS (*(s32 *)0x1F800130)

#define SCRATCHPAD_ADDR 0x1F800000
#define SCRATCHPAD_AS(type) ((type *)SCRATCHPAD_ADDR)
#define SCRATCHPAD SCRATCHPAD_AS(GameScratchpadRenderState)
#define SCRATCHPAD_BYTES SCRATCHPAD_AS(u8)

/*
 * The primitive-packing cursor. Every emitter packs a GPU packet at it, bumps
 * it past the packet and stores it back, so each one spells the slot with the
 * packet type it is building. SCRATCH_PRIM_CURSOR_AS gives that type without
 * repeating the address; _WORD is the same slot where the retail code carried
 * the cursor in an integer instead.
 */
#define SCRATCH_PRIM_CURSOR_AS(type) (*(type **)0x1F800000)
#define SCRATCH_PRIM_CURSOR          SCRATCH_PRIM_CURSOR_AS(void)
#define SCRATCH_PRIM_CURSOR_WORD     (*(s32 *)0x1F800000)
#define SCRATCH_PRIM_CURSOR_VOLATILE (*(u8 *volatile *)SCRATCHPAD_ADDR)
#define SCRATCH_PRIM_CURSOR_SLOT     (&SCRATCH_PRIM_CURSOR_VOLATILE)

/* Ordering table the emitters link finished packets into. */
#define SCRATCH_OT_BASE_AS(type)     (*(type **)0x1F800004)
#define SCRATCH_OT_BASE              SCRATCH_OT_BASE_AS(void)
#define SCRATCH_OT_BASE_WORD         (*(s32 *)0x1F800004)

/* The srav amount InitRenderState installs; see SCRATCH_FACE_OT_SHIFT below. */
#define SCRATCH_OT_SHIFT             (*(s32 *)0x1F800064)

/* View transform consumed by the model render path. SetCameraRotMatrix builds
 * the matrix at 0x28 from the three angles; the position words are the camera
 * translation passed to SetGteObjectMatrix. */
#define SCRATCH_VIEW_X       (*(s32 *)0x1F800008)
#define SCRATCH_VIEW_Y       (*(s32 *)0x1F80000C)
#define SCRATCH_VIEW_Z       (*(s32 *)0x1F800010)
#define SCRATCH_VIEW_ANGLE_X (*(s32 *)0x1F800018)
#define SCRATCH_VIEW_ANGLE_Y (*(s32 *)0x1F80001C)
#define SCRATCH_VIEW_ANGLE_Z (*(s32 *)0x1F800020)
#define SCRATCH_VIEW_POSITION_BLOCK ((Vec4 *)0x1F800008)
#define SCRATCH_VIEW_STATE   ((ScratchViewState *)0x1F800008)
#define SCRATCH_VIEW_MATRIX_GTE ((Matrix *)0x1F800028)

extern s32 g_ScratchViewX asm("0x1F800008");
extern s32 g_ScratchViewY asm("0x1F80000C");
extern volatile s32 g_ScratchViewZVolatile asm("0x1F800010");
extern s32 g_ScratchViewAngleX asm("0x1F800018");
extern s32 g_ScratchViewAngleY asm("0x1F80001C");
extern s32 g_ScratchViewAngleZ asm("0x1F800020");
extern s32 g_ScratchEnvMode4 asm("0x1F800084");

/* Course object bank. SubmitCourseModel / SubmitCourseModel2 (0x800296BC,
 * 0x80029E58) load it and index by model id; size is g_CourseModelCount. */
#define SCRATCH_COURSE_BANK    (*(void **)0x1F800048)

/* Model bank cursor, pointed at one g_ModelBanks entry by SelectModelBank.
 * MODELS is the model pointer array (bank + 0xC) that SubmitModel indexes by
 * id << 2 (0x80028DEC); NORMALS is bank[2] rebased, the 8-byte SVECTORs the
 * Emit*G4 / Emit*GT4 quad builders index by id << 3 and feed to ncct/nccs
 * (0x80029168). TABLE1 is bank[1] rebased; nothing in the disassembled engine
 * reads it, so it is named for where it comes from, not what it holds. */
#define SCRATCH_MODEL_MODELS   (*(void **)0x1F800050)
#define SCRATCH_MODEL_TABLE1   (*(void **)0x1F800054)
#define SCRATCH_MODEL_NORMALS  (*(void **)0x1F800058)

/* Terrain: the per-cell record array SubmitTerrainCells indexes by cell id
 * (0x80028078) and the face array SubmitTerrainCellFaces walks (0x80028168). */
#define SCRATCH_CELL_TABLE     (*(void **)0x1F80005C)
#define SCRATCH_CELL_FACES     (*(void **)0x1F800060)

/* The srav amount that turns a transformed Z into an ordering-table index:
 * OT_SHIFT on the cell-face path (0x800283C0), FACE_OT_SHIFT on the mode-1
 * path, where it is read as a halfword (0x80028474). InitRenderState sets
 * OT_SHIFT from its parameter, 5 for the race scene and 1 for two menus. */
#define SCRATCH_FACE_OT_SHIFT  (*(s32 *)0x1F80006C)

/* Mirror flag. Non-zero makes the engine negate the GTE rotation matrix
 * (0x80028000, 0x80028E00); track/draw_terrain_cells.c compares it against
 * g_MirrorMode. Same word as the struct's `orderingFlag`. */
#define SCRATCH_MIRROR         (*(s32 *)0x1F800068)

/* Two packed GTE RGBC words, read whole with lwc2 into cop2 register 6:
 * EmitPolyFT4Fog takes 0x70 (0x80029468), EmitPolyGT4Fog takes 0x74
 * (0x80029620). The fourth byte is the GPU primitive code the emitter stamps
 * into the packet, 0x2C for a 40-byte POLY_FT4 and 0x3C for a 52-byte
 * POLY_GT4. */
#define SCRATCH_FT4_R          (*(u8 *)0x1F800070)
#define SCRATCH_FT4_G          (*(u8 *)0x1F800071)
#define SCRATCH_FT4_B          (*(u8 *)0x1F800072)
#define SCRATCH_FT4_CODE       (*(u8 *)0x1F800073)
#define SCRATCH_GT4_R          (*(u8 *)0x1F800074)
#define SCRATCH_GT4_G          (*(u8 *)0x1F800075)
#define SCRATCH_GT4_B          (*(u8 *)0x1F800076)
#define SCRATCH_GT4_CODE       (*(u8 *)0x1F800077)

/* Screen clip rectangle every emitter rejects primitives against; the same
 * four halfwords as the struct's x0/y0/x1/y1. menu/frontend.c raises Y1 to 0x1E0 for
 * the 480-line modes. */
#define SCRATCH_CLIP_X0        (*(u16 *)0x1F800078)
#define SCRATCH_CLIP_Y0        (*(u16 *)0x1F80007A)
#define SCRATCH_CLIP_X1        (*(u16 *)0x1F80007C)
#define SCRATCH_CLIP_Y1        (*(u16 *)0x1F80007E)

/* g_IsEnvironmentMode4, forwarded here by every car and track renderer for the
 * GTE engine to read. Spelled as a macro rather than an `extern ... asm()`
 * symbol on purpose: the extern form lets gcc 2.6.3 hold the address in a
 * register across calls, which changes the output. */
#define SCRATCH_ENV_MODE4   (*(s32 *)0x1F800084)

#endif
