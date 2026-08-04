#include "common.h"
#include "game/car.h"
#include "game/race.h"

extern s32 g_PlayerProgressA asm("D_8009E73C");
extern s32 g_PlayerProgressB asm("D_8009E740");
extern GameRaceRanking g_PlayerLap asm("D_8009E83C");
extern s32 g_LapCount asm("D_801E4364");
extern GameCarRuntimeProgressWindow g_CarProgressB[] asm("D_801F18C0");

/* Counts the cars whose lap progress is ahead of the player and publishes the
 * result as g_RacePosition (1 = leader). Only runs on the final lap. */
void UpdateRacePosition(void);
void UpdateRacePosition(void) {
    s32 active;
    s32 i;
    s32 total;

    active = 1;
    if (g_LapCount >= g_PlayerLap.count) {
        total = g_PlayerProgressA + g_PlayerProgressB;

        for (i = 0; i < 0xB; i++) {
            GameCarRuntimeProgressWindow *entry = &g_CarProgressB[i];
            if (entry->activeFlag != -1) {
                if ((((s32 *)entry)[-1] + entry->field_6C) - total > 0) {
                    active++;
                }
            }
        }

        g_RacePosition = active;
    }
}
