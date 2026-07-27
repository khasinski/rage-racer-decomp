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
void GameUpdateRacePosition(void) asm("func_8003AE2C");
void GameUpdateRacePosition(void) {
    register s32 active asm("$6");
    register s32 i asm("$5");
    register s32 sentinel asm("$8");
    register GameCarRuntimeProgressWindow *entry asm("$4");
    s32 total;

    active = 1;
    if (g_LapCount >= g_PlayerLap.count) {
        i = 0;
        sentinel = -1;
        total = g_PlayerProgressA + g_PlayerProgressB;
        entry = g_CarProgressB;

        do {
            if (entry->activeFlag != sentinel) {
                if ((((s32 *)entry)[-1] + entry->field_6C) - total > 0) {
                    active++;
                }
            }
            i++;
            entry++;
        } while (i < 0xB);

        g_RacePosition = active;
    }
}
