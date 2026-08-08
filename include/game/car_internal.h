#ifndef GAME_CAR_INTERNAL_H
#define GAME_CAR_INTERNAL_H

#include "common.h"

typedef struct RaceIntroCameraKey {
    u16 x;
    u16 pad2;
    u16 y;
    u16 pad6;
    u16 z;
    u16 padA;
    s32 mode;
    s16 startFrame;
    s16 duration;
} RaceIntroCameraKey;

typedef struct LaunchSpeedThreshold {
    s16 initial;
    s16 sustain;
} LaunchSpeedThreshold;

extern u32 g_CarModelSlot;
extern RaceIntroCameraKey *g_RaceIntroCameraCursor;
extern LaunchSpeedThreshold g_LaunchSpeedThresholds[];
extern s16 g_TorqueBandEnd[];
extern s16 g_TorqueLossBandEnd[];

#endif
