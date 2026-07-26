#include "common.h"
#include "game/audio.h"
#include "game/waypoint.h"
#include "game/race.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/track.h"

extern s16 D_8019C750;

extern s16 D_8019C78C;


extern u8 D_8009E6D4[];

extern s32 D_8009E744;

extern s16 D_8009E74C;

extern s32 D_801E3E7C;

extern s32 D_801E4030;


extern s16 D_801E414C;



extern s16 D_801E43FC;

extern s32 D_801E4BAC;




void func_8001C974(void);

void func_80019EFC(s32 arg0);

void func_80033AA0(s32 arg0, s32 arg1);

void func_8003479C(s32 arg0);

void func_800350B4(s32 arg0);

void func_80035258(s32 arg0);

void func_8003CF14(u8 *arg0, s32 arg1);

void GameDrawCourseScenery(s32 arg0, s32 arg1, s32 arg2) asm("func_8003E1A4");

void func_8003E590(void);

void func_8003EAF4(void);

void func_8003EC98(void);

void func_8003F02C(void);

void func_8003F9C4(void);

void func_80040730(void);

void func_8004087C(s32 arg0);

void func_80040ADC(s32 arg0);

void func_80040DB4(s32 arg0);

void func_8004123C(void);

void func_80041840(void);

void func_800418D4(void);

void func_80042BF0(void);

void func_80042C0C(void);

void func_80042C28(void);

void func_80042CCC(s32 arg0);

void func_80045CD4(void);

extern s16 D_8009E83C;

extern s32 D_8019C700;

extern TrackWaypointSeed D_8007DFD4[];

extern TrackWaypointRuntime D_801E4DF4[];

/*
 * Initializes the 6 TrackWaypointRuntime slots for the current course. The
 * `magic`/mult/mfhi block is the compiler's divide-by-10 idiom computing the
 * track index (D_8009E83C-1)/10, clamped to 0..9, which selects a
 * TrackWaypointSeed row in D_8007DFD4. Each slot i is seeded at
 * origin + step*i (x,y), with the fixed constants 0x1766 and 0x174, and marked
 * inactive. Register pins and the raw tail-offset writes are match-load-bearing.
 */

void func_80037200(void) {
    s32 value;
    s32 option;

    option = 0;
    g_SceneTimer++;
    if ((u32)g_SceneTimer < 0x3D) {
        func_8001C974();
        value = g_SceneTimer - 6;
        func_80033AA0(0xFF - (((value * 3) * 4) - value), 0x49);
    }

    if (D_8019C750 > 0) {
        D_8019C750--;
    }

    if ((u32)((u16)g_RacePhase - 1) < 2 && (g_PadEdge2 & 0x800) && D_8019C750 <= 0) {
        D_8019C750 = 0;
        D_801E4BAC = (u32)D_801E4BAC < 1;

        if (D_801E4BAC != 0) {
            func_80042C0C();
            GameForceAllEffectVoicesEnabled(0);
            D_801E414C = 0;
            GamePlaySoundCue(2);
        } else if (D_801E414C == 2) {
            D_801E43FC = 0;
            g_RacePhase = 7;
            func_80042CCC(0x1E);
        } else if (D_801E414C == 1) {
            func_80035258(0xB);
        } else {
            GameForceAllEffectVoicesEnabled(1);
            if (g_RacePhase >= 2) {
                func_80042C28();
            }
        }
    }

    if (g_RacePhase == 7) {
        value = D_801E43FC;
        if (value > 0) {
            func_80033AA0(value * 3, 0x49);
            option = 6;
            value = D_801E43FC;
        }
        if (value >= 0x3D) {
            func_80035258(option);
        }
        D_801E43FC++;
    }

    if (D_801E4BAC != 0) {
        if ((g_PadEdge2 & 0x1000) && D_801E414C > 0) {
            D_801E414C--;
            GamePlaySoundCue(1);
        }

        if ((g_PadEdge2 & 0x4000) && D_801E414C < (2 - g_GrandPrixMode)) {
            D_801E414C++;
            GamePlaySoundCue(1);
        }

        g_SceneTimer--;
        func_8003479C(D_801E414C);
        func_8003CF14(D_8009E6D4, 0);
        func_800350B4(D_8009E744);
        func_80019EFC(D_8009E74C);
        func_800418D4();
        *(s32 *)0x1F800084 = D_801E4030;
        func_80041840();
        func_8004123C();
        func_8003EAF4();
        func_8003F02C();
        func_80040730();
        GameDrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 0);
        return;
    }

    g_AnimTimer++;

    if ((u32)g_SceneTimer >= 0x1F && g_RacePhase == 0) {
        func_80042BF0();
        g_RacePhase = 2;
        D_8019C750 = 0xA;
    }

    if (D_801E3E7C >= g_TrackLength && g_RacePhase < 3) {
        D_801E43FC = 0;
        g_RacePhase = 7;
        func_80042CCC(0x1E);
        GameForceAllEffectVoicesEnabled(0);
    }

    GameUpdateLoadedAudioVoices(0, 1);
    func_8003CF14(D_8009E6D4, 1);
    func_80019EFC(D_8009E74C);
    func_80045CD4();
    func_800418D4();
    *(s32 *)0x1F800084 = D_801E4030;
    func_80041840();
    func_8004123C();
    func_8003E590();
    func_8003EAF4();
    func_8003EC98();
    func_8003F02C();
    func_8003F9C4();
    func_80040730();
    GameDrawCourseScenery(g_CourseIndex & 3, g_SceneTimer, 1);
    func_800350B4(D_8009E744);
    GameSetReverbDepth(D_8019C78C, D_8019C78C);
    func_80040DB4(D_8009E744);
    func_80040ADC(D_8009E744);
    func_8004087C(D_8009E74C);
}

void func_80037714(void) {
    register TrackWaypointRuntime *waypoint asm("$7");
    register s32 i asm("$5");
    register TrackWaypointSeed *seed asm("$6");
    register s32 code asm("$9");
    register s32 field18 asm("$8");
    register char *tail asm("$4");
    register s32 track asm("$4");
    register s32 temp asm("$2");
    register s32 magic asm("$3");

    magic = 0x66666667;
    temp = D_8009E83C;
    track = temp - 1;
    asm volatile("mult %0,%1" : : "r"(track), "r"(magic));
    temp = track >> 31;
    asm volatile("mfhi %0" : "=r"(magic));
    magic >>= 2;
    magic -= temp;
    temp = (magic << 2) + magic;
    temp <<= 1;
    track -= temp;
    if (track < 0) {
        track = 0;
    } else if (track >= 9) {
        track = 9;
    }

    waypoint = D_801E4DF4;
    temp = (track << 1) + track;
    temp <<= 2;
    magic = (s32)D_8007DFD4;
    seed = (TrackWaypointSeed *)(temp + magic);
    i = 0;
    code = 0x1766;
    field18 = 0x174;
    tail = (char *)waypoint + 0x1C;

    do {
        waypoint->active = 0;
        *(s32 *)(tail - 0x18) = seed->x + (seed->stepX * i);
        *(s32 *)(tail - 0x10) = seed->y + (seed->stepY * i);
        *(s32 *)(tail - 0x14) = code;
        *(s32 *)(tail - 0x4) = field18;
        *(s32 *)(tail - 0x8) = 0;
        *(s32 *)tail = 0;

        i++;
        waypoint++;
        tail += sizeof(*waypoint);
    } while (i < 6);

    D_8019C700 = 0;
}
