#ifndef GAME_FRONTEND_INTERNAL_H
#define GAME_FRONTEND_INTERNAL_H

#include "common.h"

extern u32 g_FrontendIdleTimer;

#ifndef GAME_DISP_ENV1_X_DECL
#define GAME_DISP_ENV1_X_DECL extern u16 g_DispEnv1X
#endif
GAME_DISP_ENV1_X_DECL;
#undef GAME_DISP_ENV1_X_DECL

#endif
