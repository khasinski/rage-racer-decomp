#ifndef GAME_TEAM_LOGO_H
#define GAME_TEAM_LOGO_H

#include "psyq/gpu.h"

typedef union TeamLogoRectCoordinate {
    s16 value;
    struct {
        u8 low;
        s8 high;
    } byte;
} TeamLogoRectCoordinate;

typedef union TeamLogoRect {
    Rect rect;
    struct {
        TeamLogoRectCoordinate x;
        TeamLogoRectCoordinate y;
        TeamLogoRectCoordinate w;
        TeamLogoRectCoordinate h;
    } coordinate;
} TeamLogoRect;

extern TeamLogoRect g_TeamLogoRect;
extern Rect g_TeamLogoClutRect;

#endif
