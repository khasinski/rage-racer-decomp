#include "common.h"
#include "game/race.h"
#include "game/state.h"
#include "game/track.h"
#include "game/car.h"

extern s32 g_PlayerCar;

extern s32 g_PlayerTrackPoint;


extern s32 g_ReplayReadCursor;


extern s32 g_Car0TrackPoint;

void ApplyReplayFrameAndTilt(s32 arg0, void *arg1, void *arg2);

void SeedCarLapProgress(void *arg0, s32 arg1);

void AccumulateLapProgress(void *arg0);

s32 FindTrackSegment(void *arg0, s32 arg1);


void InitShuttleScenery(void);

extern s16 g_PlayerTrackSection;

void RequestTrackTexturePage(s32 arg0);

typedef struct TrackZone {
    s32 start;
    s32 end;
    s16 code;
    s16 value;
} TrackZone;




extern s16 g_TrackZoneCode;

extern s16 g_ReverbZoneDepth;

extern s16 g_TrackZoneDark;


extern u8 g_MsgGameExit;

void ForceAllEffectVoicesEnabled(s32 arg0);

void SetReverbDepth(s32 arg0, s32 arg1);

void RequestSelectBgmAssets(void);





extern s16 g_PlayerLap;









extern s32 g_LapCount;

extern s32 g_BestLapThisRace;



extern s32 g_SectorEndDistance[];

extern s32 g_RefSectorTimes[];

extern s32 g_BestSectorTimes[][4][3];

extern s32 g_BestTotalTimes[][4][2];

void DrawSplitDelta(s32 arg0, s32 arg1);

void DrawTimeValue(s32 x, s32 y, s32 value, s32 color, s32 divisor);

void PlaySoundCue(s32 cue);

void SeedReplayCars(void);
void SeedReplayCars(void) {
    void *primary;
    void *secondary;

    InitShuttleScenery();

    primary = &g_PlayerCar;
    secondary = g_Cars;
    ApplyReplayFrameAndTilt(g_ReplayReadCursor, primary, secondary);

    g_PlayerTrackPoint = FindTrackSegment(primary, g_PlayerTrackPoint);
    SeedCarLapProgress(primary, 1);
    AccumulateLapProgress(primary);
    ResetCarTrackState(primary);

    if (g_GrandPrixMode == 1) {
        g_Car0TrackPoint = FindTrackSegment(secondary, g_Car0TrackPoint);
        SeedCarLapProgress(secondary, 1);
        AccumulateLapProgress(secondary);
        ResetCarTrackState(secondary);
    }
}

void UpdateReplayCars(void);
void UpdateReplayCars(void) {
    void *ptr = &g_PlayerCar;

    AccumulateLapProgress(ptr);
    ResetCarTrackState(ptr);

    if (g_GrandPrixMode == 1) {
        ptr = g_Cars;
        AccumulateLapProgress(ptr);
        ResetCarTrackState(ptr);
    }

    RequestTrackTexturePage(g_PlayerTrackSection);
}

s32 GetTrackZoneBlend(s32 position);
s32 GetTrackZoneBlend(s32 position) {
    u8 *base;
    s32 scene;
    TrackZone *first;
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
        switch (0) { default:
        if (position < start + 0x100) {
            status = 1;
        } else if (finish - 0x100 < position) {
            status = 2;
        } else {
            status = 3;
        }

        rawCode = zone->code;
        RAW(g_TrackZoneCode) = rawCode;
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
        break;

code_two:
        status = 4;
code_minus_three:
        if (status == two) {
            status = 3;
        }
        g_TrackZoneCode = 1;
        break;

        }
normalize_code:
        if (g_TrackZoneCode < 0) {
            g_TrackZoneCode = -g_TrackZoneCode;
            status = 3;
        }
        }
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

void ExitRaceScene(s32 arg0);
void ExitRaceScene(s32 arg0) {
    g_SceneId = arg0;
    ForceAllEffectVoicesEnabled(0);
    SetReverbDepth(0, 0);
    if (g_SceneId == 6) {
        RequestSelectBgmAssets();
    }
    DebugPrintf(&g_MsgGameExit);
}

void UpdateSplitTimes(void *arg0, s32 arg1, s32 arg2);
void UpdateSplitTimes(void *arg0, s32 arg1, s32 arg2) {
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
    switch (0) { default:
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
                        PlaySoundCue(0x3F);
                    }
                } else if (delta > 0 && arg2 == 0) {
                    PlaySoundCue(0x3E);
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
            break;
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

}
    switch (0) { default:
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
                DrawTimeValue(0x80, 0x50, value, tile, 0x3E8);
            }
        }
        threshold = 0x927BE;
        value = g_LastSectorTime;
    } else {
        break;
    }

    if (value <= threshold) {
        tile = 0x78CC;
    } else {
        tile = 0x7890;
    }
    DrawTimeValue(0x12, 0x2A, value, tile, 0x3E8);

    }
    timeout = 0x3E8;
    DrawTimeValue(0x12, 0x20, g_SplitTargetTime, 0x78CC, timeout);
    DrawSplitDelta(g_SplitSector, g_SplitSign);

    DrawTimeValue(
        0xFA,
        0x7C,
        g_BestTotalTimes[g_RaceSeries][g_CourseIndex][arg1],
        0x78CC,
        timeout);
}
