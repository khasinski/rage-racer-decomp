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
    u8 reserved1C[0x40];
    DispEnv display;
    DrawEnv mirrorDraw;
} GameFrameEnvironmentHeader;

extern Matrix g_MirrorViewMatrix;
extern struct GameRenderObject g_CameraCar;
extern Matrix g_SceneLightMatrix;
extern u32 g_ScratchRenderMode;
extern u8 *g_CamRow;
extern FontGlyph g_SmallFontGlyphs[];
extern FontGlyph g_LargeFontGlyphs[];
extern CameraKey g_CameraPath[];
extern u32 g_MainVisibleCellMask[];
extern Vec4 g_MainVisibleCellList[];
extern u32 *g_VisibleCellMask;
extern Vec4 *g_VisibleCellList;
extern s32 g_CameraViewMode;
extern s16 g_AtanTable[];
#ifndef GAME_DRAW_BUFFER_QUALIFIER
#define GAME_DRAW_BUFFER_QUALIFIER
#endif
extern u8 *GAME_DRAW_BUFFER_QUALIFIER g_DrawBuffer;
#undef GAME_DRAW_BUFFER_QUALIFIER
#ifndef GAME_FRAME_CONTEXT_QUALIFIER
#define GAME_FRAME_CONTEXT_QUALIFIER
#endif
extern GAME_FRAME_CONTEXT_QUALIFIER u8 g_FrameContexts[];
#undef GAME_FRAME_CONTEXT_QUALIFIER
#ifndef GAME_SCREEN_OFFSET_TYPE
#define GAME_SCREEN_OFFSET_TYPE s32
#endif
extern GAME_SCREEN_OFFSET_TYPE g_ScreenOffsetX;
extern GAME_SCREEN_OFFSET_TYPE g_ScreenOffsetY;
#undef GAME_SCREEN_OFFSET_TYPE
extern s32 g_FrameParity;
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
