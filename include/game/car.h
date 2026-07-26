#ifndef GAME_CAR_H
#define GAME_CAR_H

#include "common.h"

typedef struct GameCarEntry {
    u8 modelVariant;
    u8 pad1[2];
    u8 shapeIndex;
    u8 textureIndex;
    u8 enabled;
    u8 pad6[2];
} GameCarEntry;

typedef struct GameCarRuntime {
    s32 x;
    s16 y;
    u8 pad6[2];
    s32 z;
    s32 field_0C;
    s32 field_10;
    s32 field_14;
    s32 field_18;
    s32 field_1C;
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 field_2C;
    s32 trackPointIndex;
    s32 field_34;
    s32 field_38;
    s32 field_3C;
    s32 field_40;
    s32 field_44;
    s32 field_48;
    s32 field_4C;
    s32 field_50;
    s32 field_54;
    s32 field_58;
    s32 field_5C;
    s32 field_60;
    s32 field_64;
    s32 field_68;
    s32 field_6C;
    s32 trackProgress;
    s32 previousTrackProgress;
    s16 field_78;
    s16 field_7A;
    s16 velocityX;
    s16 velocityZ;
    s16 motionActive;
    u16 motionTimer;
    s16 motionMode;
    s16 motionModeTimer;
    s16 motionValue;
    s16 field_8A;
    s16 field_8C;
    s16 field_8E;
    s16 field_90;
    s16 field_92;
    s16 field_94;
    s16 field_96;
    s16 field_98;
    s16 field_9A;
    s16 field_9C;
    s16 field_9E;
    s32 headingAngle;
    s32 field_A4;
    s32 field_A8;
    s16 activeFlag;
    s16 field_AE;
    s32 field_B0;
    s32 field_B4;
    s16 routeRow;
    u8 padBA[2];
    s32 field_BC;
    s32 field_C0;
    s32 field_C4;
    s32 field_C8;
    s32 field_CC;
    s32 field_D0;
    s32 field_D4;
    s32 field_D8;
    s32 field_DC;
    s32 field_E0;
    s32 field_E4;
    s32 field_E8;
    s32 field_EC;
    s16 field_F0;
    s16 field_F2;
    s32 field_F4;
    s32 field_F8;
    s32 field_FC;
    s32 routeIndex;
    s16 field_104;
    u8 pad106[2];
    s32 field_108;
    s16 field_10C;
    s16 field_10E;
    s16 field_110;
    s16 field_112;
    s16 field_114;
    s16 field_116;
    s16 field_118;
    s16 field_11A;
    s16 field_11C;
    s16 field_11E;
    s16 field_120;
    s16 field_122;
    s16 field_124;
    s16 field_126;
    s16 field_128;
    s16 field_12A;
    s16 field_12C;
    s16 field_12E;
    s16 field_130;
    s16 field_132;
    s32 field_134;
    s16 field_138;
    s16 field_13A;
    s16 field_13C;
    s16 field_13E;
    u8 pad140[0xC];
    u8 field_14C;
    u8 field_14D;
    u8 field_14E;
    u8 field_14F;
    u8 pad150[0xC];
    s16 field_15C;
    s16 field_15E;
    u8 pad160[0x3C];
} GameCarRuntime;

typedef struct GameCarRuntimeProgressWindow {
    s32 field_6C;
    u8 pad4[0x3C];
    s16 activeFlag;
    u8 pad42[0x15A];
} GameCarRuntimeProgressWindow;

/*
 * Active car-entry table: the per-car static config (modelVariant, enabled)
 * used by the grid/car-select code. GameUpdateMainMenuInput repoints it at one of three
 * tables (D_801E4F44, D_8019C914, D_801E4388) depending on the mode, so it is
 * a pointer rather than a fixed array.
 */
extern GameCarEntry *g_CarTable asm("D_8019C7C8");

/*
 * Index into g_CarTable of the car the player drives. func_80018530 /
 * func_80018A70 use it to pick which car model+texture pack to install
 * (`func_80017848(g_PlayerCarIndex, entry->modelVariant)`, then
 * func_8001D748 / func_8001D900 with the entry's shapeIndex / textureIndex,
 * guarded by `< 10` so the eleventh slot is skipped); the garage screens
 * (func_80058C14 / func_8005A3A4 / func_800563A0) edit
 * g_CarTable[g_PlayerCarIndex] in place. Distinct from D_801E4B88, the cursor
 * of the car list being browsed - buying a car sets
 * `g_CarTable[D_801E4B88].enabled = 1` and only then
 * `g_PlayerCarIndex = D_801E4B88`.
 */
extern s32 g_PlayerCarIndex asm("D_801E40D4");

/*
 * Cursor of the car list being browsed in the shop / car-select screen.
 * func_80059320 steps it to the next g_CarTable entry with `enabled == 0` (a car
 * the player does not own yet); confirming a purchase does
 * `g_CarTable[g_CarListCursor].enabled = 1` and only then
 * `g_PlayerCarIndex = g_CarListCursor` (func_80059558). While g_MenuScreen is
 * 0xB the garage renderer draws this car instead of the player's.
 */
extern s32 g_CarListCursor asm("D_801E4B88");

/*
 * The loaded player car's spec / configuration block. func_80018FC4 installs the
 * pack for g_PlayerCarIndex and hands the first sub-block to func_80034DF4,
 * whose whole body is `g_CarSpec = arg0;`. The physics, gearbox and HUD read it
 * by raw offset: +0x100 rev limit, +0x104 upshift speed, +0x106 redline,
 * +0x10A a countdown timer, +0x140/+0x142 the tachometer origin, plus the
 * gear-ratio / shift-speed tables func_8002DEFC walks.
 */
extern u8 *g_CarSpec asm("D_801E42D8");

typedef struct GameCarTrackAngleWindow {
    u8 pad0[0x30];
    s32 trackPointIndex;
    u8 pad34[0x6C];
    s32 headingAngle;
} GameCarTrackAngleWindow;

#endif
