#ifndef GAME_MENU_INTERNAL_H
#define GAME_MENU_INTERNAL_H

#include "common.h"
#include "game/menu_types.h"
#include "game/team_logo.h"
#include "psyq/gpu.h"

extern PaintColorTable g_PaintColorTable;
extern s32 g_PaintPalettePulsePhase;
extern s32 g_MenuAltLayout;
extern s32 g_OwnedCarCounterSlide;
extern u16 g_TeamLogoClut[16];
extern TeamLogoCanvas g_TeamLogoCanvas;
extern u8 g_TeamLogoExpertMode;
extern s32 g_TeamLogoCursorY;
extern s32 g_TeamLogoViewY;
extern s32 g_TeamLogoGuideMode;
extern s32 g_TeamLogoBrushSize;
extern TeamLogoCoordinate g_TeamLogoCursorX;
extern TeamLogoCoordinate g_TeamLogoViewX;
extern TeamLogoColorIndex g_TeamLogoPenColor;
extern s32 g_TeamLogoPaletteMode;
extern s32 g_TeamLogoColorChannel;
extern s32 g_TeamLogoDpadRepeatTimer;
extern s32 g_TeamLogoDpadRepeatMask;
extern s32 g_TeamLogoGuideModePrev;
extern s32 g_TeamLogoPaintArmed;
extern s32 g_TeamLogoBlankClut;
extern s32 g_MenuLightBurstLevel;
extern const MenuLightBurstBand g_MenuLightBurstBandX;
extern const MenuLightBurstBand g_MenuLightBurstBandY;
extern const char g_MsgOrdinalSt[4];
extern const char g_MsgOrdinalNd[4];
extern const char g_MsgOrdinalRd[4];
extern const char g_MsgOrdinalTh[8];
extern RaceRecord g_RankingRecords[2][4][5];
extern RaceRecord g_TimeRecords[2][4][5];
extern ClassRecordSprite g_ClassRecordCellSprites[];
extern ScoreRecord g_ClassRecords[11];
extern s32 g_ClassWinCount;
extern NegconUvTemplate g_NegconSteerPlayUvQuad;
extern NegconUvTemplate g_NegconMaxTwistUvQuad;
extern DesignModeCellMask g_DesignModeCellMask;

#endif
