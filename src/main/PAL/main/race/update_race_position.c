#include "common.h"
#include "game/car.h"
#include "game/race.h"

/* Counts the cars whose lap progress is ahead of the player and publishes the
 * result as g_RacePosition (1 = leader). Only runs on the final lap. */
void UpdateRacePosition(void) {
    s32 active;
    s32 i;
    s32 total;
    CarProgressWindow *cars;

    active = 1;
    if (g_LapCount >= ((GameRaceRanking *)&g_PlayerLap)->count) {
        total = g_PlayerProgressA + g_PlayerProgressB;
        cars = (CarProgressWindow *)&g_Cars[0].field_6C;

        for (i = 0; i < 0xB; i++) {
            CarProgressWindow *entry = &cars[i];
            if (entry->activeFlag != -1) {
                if ((((s32 *)entry)[-1] + entry->field_6C) - total > 0) {
                    active++;
                }
            }
        }

        g_RacePosition = active;
    }
}
