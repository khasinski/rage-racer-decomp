#include "common.h"
#include "game/car.h"

/* Deliberately raw: written here, read nowhere in the image (names.md 15g). */
extern s16 D_801E8A4C;

void BeginCarStandingStart(u8 *arg0) {
    s32 value;
    s16 index;

    value = ((g_EngineRpm - g_PeakOutputRpm) * 10000) / g_CarSpec->revLimit;
    D_801E8A4C = 0;

    if (value < 0) {
        value = g_EngineRpm - 1000;
        if (g_EngineRpm < 2000) {
            value = 0;
        }
    } else {
        index = *(s16 *)(arg0 + 0x132);
        value *= g_PeakOutputValue / ((index * 200) + 300);
        *(s32 *)(arg0 + 0x150) = *(s32 *)(arg0 + 0x150) / index;
        if (*(s16 *)(arg0 + 0x132) >= 2) {
            g_GripLossTimer = 200;
        }
    }

    g_StandingStartSpin = value;
}
