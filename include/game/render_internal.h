#ifndef GAME_RENDER_INTERNAL_H
#define GAME_RENDER_INTERNAL_H

#include "common.h"
#include "game/camera_types.h"
#include "game/vector.h"
#include "psyq/gpu.h"
#include "psyq/gte.h"

typedef struct FontGlyph {
    u8 u;
    u8 v;
    u16 width;
} FontGlyph;

typedef struct CameraKey {
    s32 eyeX;
    s32 eyeY;
    s32 eyeZ;
    s32 atX;
    s32 atY;
    s32 atZ;
    s32 duration;
    s32 control;
} CameraKey;

/* The environment block at the head of each 0x237E8-byte frame context. */
typedef struct GameFrameEnvironmentHeader {
    DrawEnv draw;
    DispEnv display;
    DrawEnv mirrorDraw;
} GameFrameEnvironmentHeader;

typedef char GameFrameEnvironmentHeaderSizeCheck[
    sizeof(GameFrameEnvironmentHeader) == 0xCC ? 1 : -1];

#define GAME_FRAME_CONTEXT_SIZE 0x237E8
#define GAME_FRAME_OT_LENGTH 0x2C0

typedef struct RaceHudPackets {
    DrawPacket tachometerDrawModes[2];
    SPRT tachometerFace;
    SPRT lapTimes[6];
    SPRT labels[6];
} RaceHudPackets;

typedef char RaceHudPacketsSizeCheck[
    sizeof(RaceHudPackets) == 0x11C ? 1 : -1];

typedef struct GameFrameLayout {
    GameFrameEnvironmentHeader environment;
    u_long orderingTables[2][GAME_FRAME_OT_LENGTH];
    u8 primitiveBuffer[0x22000];
    RaceHudPackets raceHud;
} GameFrameLayout;

typedef char GameFrameLayoutSizeCheck[
    sizeof(GameFrameLayout) == GAME_FRAME_CONTEXT_SIZE ? 1 : -1];

typedef union GameFrameContext {
    GameFrameEnvironmentHeader environment;
    GameFrameLayout layout;
    u8 bytes[GAME_FRAME_CONTEXT_SIZE];
    volatile u8 volatileBytes[GAME_FRAME_CONTEXT_SIZE];
} GameFrameContext;

typedef union GameFrameContextAddress {
    u8 *bytes;
    GameFrameContext *context;
} GameFrameContextAddress;

static __inline__ GameFrameContext *GetGameFrameContext(u8 *bytes) {
    GameFrameContextAddress address;

    address.bytes = bytes;
    return address.context;
}

typedef union ScreenOffset {
    s32 value;
    u16 displayValue;
} ScreenOffset;

extern Matrix g_MirrorViewMatrix;
extern Matrix g_SceneLightMatrix;
extern u32 g_ScratchRenderMode;
extern TrackRenderTable *g_TrackRenderTable;
extern FontGlyph g_SmallFontGlyphs[];
extern FontGlyph g_LargeFontGlyphs[];
extern CameraKey g_CameraPath[];
extern u32 g_MainVisibleCellMask[];
extern Vec4 g_MainVisibleCellList[];
extern u32 *g_VisibleCellMask;
extern Vec4 *g_VisibleCellList;
extern CameraViewMode g_CameraViewMode;
extern s16 g_AtanTable[];
extern u8 *g_DrawBuffer;
extern GameFrameContext g_FrameContexts[2];
extern ScreenOffset g_ScreenOffsetX;
extern ScreenOffset g_ScreenOffsetY;
extern s32 g_FrameParity;
extern DrawEnv g_DrawEnv1;
extern DrawEnv g_MirrorDrawEnv1;
extern u8 g_Font8x8Cells[];
extern u8 g_DrawModeEnv[];
extern u8 g_PropFontU[];
extern u8 g_PropFontV[];
extern u8 g_WordFontU[];
extern u8 g_WordFontV[];
extern u8 g_WordFontWidth[];
extern u8 g_WordFontAdvance[];
extern u8 g_HighFontU[];
extern u8 g_HighFontV[];
extern u8 g_HighFontWidth[];
extern u8 g_HighFontYOffset[];
extern s32 g_MenuOverlayPatternAnimOffset;
extern u8 g_MenuOverlayPatternTable[];
extern u8 g_SpriteFontU[];
extern u8 g_SpriteFontV[];
extern u8 g_SpriteFontWidth[];

#endif
