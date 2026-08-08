#ifndef GAME_SAVE_INTERNAL_H
#define GAME_SAVE_INTERNAL_H

#include "common.h"
#include "game/car.h"
#include "game/menu_types.h"
#include "psyq/gpu.h"

#ifndef GAME_SAVE_BGM_TYPE
#define GAME_SAVE_BGM_TYPE s32
#endif

extern CarEntry g_SaveDefaults[13];
extern ScoreRecord g_ClassRecords[11];
typedef struct ClassClearView {
    u16 clears;
    s16 nextPlace;
} ClassClearView;
typedef struct CourseProgressState {
    u8 bestPlace[4];
    s16 unlockPending;
    s16 retriesRemaining;
} CourseProgressState;
extern ClassClearView g_ClassClears[11];
#ifndef GAME_CLASS_WIN_COUNT_QUALIFIER
#define GAME_CLASS_WIN_COUNT_QUALIFIER
#endif
extern GAME_CLASS_WIN_COUNT_QUALIFIER s32 g_ClassWinCount;
#ifndef GAME_GRAND_PRIX_PROGRESS_DECL
#define GAME_GRAND_PRIX_PROGRESS_DECL extern u8 g_GrandPrixCourseProgress[8]
#endif
#ifndef GAME_EXTRA_GRAND_PRIX_PROGRESS_DECL
#define GAME_EXTRA_GRAND_PRIX_PROGRESS_DECL extern u8 g_ExtraGrandPrixCourseProgress[8]
#endif
GAME_GRAND_PRIX_PROGRESS_DECL;
GAME_EXTRA_GRAND_PRIX_PROGRESS_DECL;
#ifndef GAME_COURSE_PROGRESS_TYPE
#define GAME_COURSE_PROGRESS_TYPE u8
#endif
extern GAME_COURSE_PROGRESS_TYPE *g_CourseProgress;
extern GAME_SAVE_BGM_TYPE g_BgmSelection;
#ifndef GAME_TEAM_LOGO_CANVAS_DECL
#define GAME_TEAM_LOGO_CANVAS_DECL extern u16 g_TeamLogoCanvas[0x400]
#endif
#ifndef GAME_TEAM_LOGO_RECT_DECL
#define GAME_TEAM_LOGO_RECT_DECL extern Rect g_TeamLogoRect
#endif
#ifndef GAME_TEAM_LOGO_CLUT_RECT_DECL
#define GAME_TEAM_LOGO_CLUT_RECT_DECL extern Rect g_TeamLogoClutRect
#endif
GAME_TEAM_LOGO_CANVAS_DECL;
GAME_TEAM_LOGO_RECT_DECL;
GAME_TEAM_LOGO_CLUT_RECT_DECL;
#ifndef GAME_BEST_LAP_DECL
#define GAME_BEST_LAP_DECL extern s32 g_BestLapTimes[2][4][2]
#endif
GAME_BEST_LAP_DECL;
#ifndef GAME_BEST_TOTAL_DECL
#define GAME_BEST_TOTAL_DECL extern s32 g_BestTotalTimes[2][4][2]
#endif
GAME_BEST_TOTAL_DECL;
#ifndef GAME_BEST_SECTOR_DECL
#define GAME_BEST_SECTOR_DECL extern s32 g_BestSectorTimes[2][4][3]
#endif
GAME_BEST_SECTOR_DECL;

#undef GAME_SAVE_BGM_TYPE
#undef GAME_CLASS_WIN_COUNT_QUALIFIER
#undef GAME_BEST_TOTAL_DECL
#undef GAME_BEST_LAP_DECL
#undef GAME_BEST_SECTOR_DECL
#undef GAME_GRAND_PRIX_PROGRESS_DECL
#undef GAME_EXTRA_GRAND_PRIX_PROGRESS_DECL
#undef GAME_COURSE_PROGRESS_TYPE
#undef GAME_TEAM_LOGO_CANVAS_DECL
#undef GAME_TEAM_LOGO_RECT_DECL
#undef GAME_TEAM_LOGO_CLUT_RECT_DECL

#endif
