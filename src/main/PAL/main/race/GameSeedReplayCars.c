#include "common.h"
#include "game/race.h"
#include "game/state.h"
#include "game/track.h"
#include "game/car.h"

extern s32 g_PlayerCar asm("D_8009E6D4");

extern s32 g_PlayerTrackPoint asm("D_8009E704");


extern s32 g_ReplayReadCursor asm("D_801F179C");


extern s32 g_Car0TrackPoint asm("D_801F1884");

void GameApplyReplayFrameAndTilt(s32 arg0, void *arg1, void *arg2) asm("func_8001F8D0");

void GameSeedCarLapProgress(void *arg0, s32 arg1) asm("func_8002BF68");

void func_8002C168(void *arg0);

s32 GameFindTrackSegment(void *arg0, s32 arg1) asm("func_80030EB4");

void func_80032280(void *arg0);

void GameInitShuttleScenery(void) asm("func_8003F0F8");

extern s16 g_PlayerTrackSection asm("D_8009E74C");

void GameRequestTrackTexturePage(s32 arg0) asm("func_80019EFC");

typedef struct TrackZone {
    s32 start;
    s32 end;
    s16 code;
    s16 value;
} TrackZone;




extern s16 g_TrackZoneCode asm("D_8019CE34");

extern s16 g_ReverbZoneDepth asm("D_8019C78C");

extern s16 g_TrackZoneDark asm("D_8019CAB0");


extern u8 g_MsgGameExit asm("D_8001147C");

void func_8005E4A4(s32 arg0);

void func_8005B190(s32 arg0, s32 arg1);

void GameRequestSelectBgmAssets(void) asm("func_80018410");





extern s16 g_PlayerLap asm("D_8009E83C");









extern s32 g_LapCount asm("D_801E4364");

extern s32 g_BestLapThisRace asm("D_801E4BCC");



extern s32 g_SectorEndDistance[] asm("D_801E4D98");

extern s32 g_RefSectorTimes[] asm("D_8009AF90");

extern s32 g_BestSectorTimes[][4][3] asm("D_801E41E8");

extern s32 g_BestTotalTimes[][4][2] asm("D_8019C70C");

void GameDrawSplitDelta(s32 arg0, s32 arg1) asm("func_80033308");

void GameDrawTimeValue(s32 x, s32 y, s32 value, s32 color, s32 divisor) asm("func_80033D50");

void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");

void GameSeedReplayCars(void) asm("func_80034F74");
void GameSeedReplayCars(void) {
    void *primary;
    void *secondary;

    GameInitShuttleScenery();

    primary = &g_PlayerCar;
    secondary = g_Cars;
    GameApplyReplayFrameAndTilt(g_ReplayReadCursor, primary, secondary);

    g_PlayerTrackPoint = GameFindTrackSegment(primary, g_PlayerTrackPoint);
    GameSeedCarLapProgress(primary, 1);
    func_8002C168(primary);
    func_80032280(primary);

    if (g_GrandPrixMode == 1) {
        g_Car0TrackPoint = GameFindTrackSegment(secondary, g_Car0TrackPoint);
        GameSeedCarLapProgress(secondary, 1);
        func_8002C168(secondary);
        func_80032280(secondary);
    }
}

void GameUpdateReplayCars(void) asm("func_80035040");
void GameUpdateReplayCars(void) {
    void *ptr = &g_PlayerCar;

    func_8002C168(ptr);
    func_80032280(ptr);

    if (g_GrandPrixMode == 1) {
        ptr = g_Cars;
        func_8002C168(ptr);
        func_80032280(ptr);
    }

    GameRequestTrackTexturePage(g_PlayerTrackSection);
}

s32 GameGetTrackZoneBlend(s32 position) asm("func_800350B4");
s32 GameGetTrackZoneBlend(s32 position) {
    u8 *base;
    s32 scene;
    TrackZone *first;
    /* This pin is load-bearing: removing it changes .text. */
    register TrackZone *zone asm("$6");
    s32 status;
    s32 two;
    s32 start;
    s32 finish;
    s32 code;
    u16 rawCode;

    base = g_TrackEventData;
    scene = g_RaceSeries;
    first = (TrackZone *)(base + 0xA74);
    if (scene != 0) {
        position = g_TrackLength - position;
    }

    status = 0;
    two = 2;
    zone = first;
    g_TrackZoneCode = 0;
    g_ReverbZoneDepth = 0;
    g_TrackZoneDark = 0;

    do {
    start = zone->start;
    finish = zone->end;
    if (start == -1) {
        goto done;
    }

    if ((start < position) && (position < finish)) {
        if (position < start + 0x100) {
            status = 1;
        } else if (finish - 0x100 < position) {
            status = 2;
        } else {
            status = 3;
        }

        rawCode = zone->code;
        g_TrackZoneCode = rawCode;
        /* This barrier is load-bearing: removing it changes .text. */
        asm("" : : : "memory");
        code = (s16)rawCode;
        if (!(code == 0)) {
        if (code > 0) {
        } else {
        if (code == -3) {
            goto code_minus_three;
        }
        goto normalize_code;

        }
        if (code == two) {
            goto code_two;
        }

        } else {
        g_TrackZoneDark = 3;
        goto store_value;

code_two:
        status = 4;
code_minus_three:
        if (status == two) {
            status = 3;
        }
        g_TrackZoneCode = 1;
        goto store_value;

        }
normalize_code:
        if (g_TrackZoneCode < 0) {
            g_TrackZoneCode = -g_TrackZoneCode;
            status = 3;
        }
store_value:
        g_ReverbZoneDepth = zone->value;
    }

    if (status > 0) {
        break;
    }
    zone++;
    } while ((s32)zone < (s32)first + 0xF0);

done:
    switch (status) {
    case 1:
        return position - start;
    case 2:
        return finish - position;
    case 3:
        return 0x100;
    default:
        return 0;
    }
}

void GameExitRaceScene(s32 arg0) asm("func_80035258");
void GameExitRaceScene(s32 arg0) {
    g_SceneId = arg0;
    func_8005E4A4(0);
    func_8005B190(0, 0);
    if (g_SceneId == 6) {
        GameRequestSelectBgmAssets();
    }
    GameDebugPrintf(&g_MsgGameExit);
}

void GameUpdateSplitTimes(void *arg0, s32 arg1, s32 arg2) asm("func_800352B8");
void GameUpdateSplitTimes(void *arg0, s32 arg1, s32 arg2) {
    s32 slot;
    s32 nextSlot;
    s32 delta;
    s32 value;
    s32 tile;
    s32 timeout;
    s32 threshold;
    u8 *route;

    route = (u8 *)arg0 + 0xBC;

    if (arg2 == 2 || arg1 != 0) {
        return;
    }

    slot = g_SectorIndex;
    if (slot >= 0) {
        if ((*(s16 *)((u8 *)arg0 + 0x168) - 1) * g_TrackLength + g_SectorEndDistance[slot] <=
                (*(s32 *)((u8 *)arg0 + 0x6C) + *(s32 *)((u8 *)arg0 + 0x68)) ||
            arg2 != 0) {
            g_SectorTimes[slot] = g_LapTimeMs;
            if (g_LapTimeMs <= 0x927BE) {
                if (arg2 != 0) {
                    delta = g_RefLapTime - g_LapTimeMs;
                } else {
                    delta = g_RefSectorTimes[slot] - g_LapTimeMs;
                }

                g_SplitSign = 1;
                if (delta < 0) {
                    g_SplitSign = -1;
                    delta = -delta;
                    if (arg2 == 0) {
                        GamePlaySoundCue(0x3F);
                    }
                } else if (delta > 0 && arg2 == 0) {
                    GamePlaySoundCue(0x3E);
                }
                g_SplitDelta = delta;
            } else {
                g_SplitSign = 0;
            }

            g_SplitTimer = 0;
            nextSlot = g_SectorIndex;
            nextSlot++;
            nextSlot %= 3;
            g_SectorIndex = nextSlot;

            if (arg2 != 0) {
                g_SplitSector = 2;
                g_SplitTargetTime = g_RefLapTime;
                g_RefLapTime = g_BestLapThisRace;
            } else {
                nextSlot += 2;
                nextSlot %= 3;
                g_SplitSector = nextSlot;
                g_SplitTargetTime = g_RefSectorTimes[nextSlot];
            }

            nextSlot = g_SectorIndex;
            nextSlot += 2;
            nextSlot %= 3;
            nextSlot <<= 2;
            g_LastSectorTime = *(s32 *)((u8 *)g_SectorTimes + nextSlot);
            goto draw;
        }
    }

    if (g_SectorIndex == -2 && arg2 != 0) {
        g_SectorIndex = 0;
        g_SplitSign = 0;
        g_SplitTargetTime = g_BestSectorTimes[g_RaceSeries][g_CourseIndex][0];
        g_SplitTimer = 0x3C;
        g_SplitSector = (u16)g_SectorIndex;
    } else {
    nextSlot = g_SectorIndex;
    if (nextSlot >= 0 && g_LapCount >= *(s16 *)(route + 0xAC)) {
        if (g_SplitTimer < 0x3C) {
            g_SplitTimer++;
            if (g_SplitTimer == 0x3C) {
                g_SplitTargetTime = g_RefSectorTimes[nextSlot];
                g_SplitSign = 0;
                g_SplitSector = (u16)g_SectorIndex;
            }
        }
    } else {
        g_SplitSector = 0;
        g_SplitTimer = 0;
        g_SplitSign = 0;
        g_SplitTargetTime = g_RefSectorTimes[0];
    }
    }

draw:
    if (g_SplitTimer >= 0x3C) {
        threshold = 0x927BE;
        value = g_LapTimeMs;

    } else if (g_SectorIndex >= 0) {
        if (g_SplitSign != 0) {
            if (g_LapCount >= g_PlayerLap) {
                value = g_SplitDelta;
                if (g_SplitSign > 0) {
                    tile = 0x7810;
                } else {
                    tile = 0x780F;
                }
                GameDrawTimeValue(0x80, 0x50, value, tile, 0x3E8);
            }
        }
        threshold = 0x927BE;
        value = g_LastSectorTime;
    } else {
        goto skip_first;
    }

compare_first:
    if (value <= threshold) {
        tile = 0x78CC;
    } else {
        tile = 0x7890;
    }
    GameDrawTimeValue(0x12, 0x2A, value, tile, 0x3E8);

skip_first:
    timeout = 0x3E8;
    GameDrawTimeValue(0x12, 0x20, g_SplitTargetTime, 0x78CC, timeout);
    GameDrawSplitDelta(g_SplitSector, g_SplitSign);

    GameDrawTimeValue(
        0xFA,
        0x7C,
        g_BestTotalTimes[g_RaceSeries][g_CourseIndex][arg1],
        0x78CC,
        timeout);
}
