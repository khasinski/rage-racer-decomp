#include "common.h"
#include "game/car.h"


extern s16 g_PeakOutputRpm asm("D_8019C798");
extern s32 g_EngineRpm asm("D_8019CAB4");
extern s32 g_StandingStartSpin asm("D_8019CA04");
extern s16 g_PeakOutputValue asm("D_801E4B90");
extern s16 g_GripLossTimer asm("D_801E4BA0");
/* Deliberately raw: written here, read nowhere in the image (names.md 15g). */
extern s16 D_801E8A4C;

void GameBeginCarStandingStart(u8 *arg0) asm("func_8002BE18");
void GameBeginCarStandingStart(u8 *arg0) {
    s32 value;
    s16 index;

    value = ((g_EngineRpm - g_PeakOutputRpm) * 10000) / g_CarSpec->revLimit;
    D_801E8A4C = 0;

    if (value < 0) {
        value = g_EngineRpm - 1000;
        if (g_EngineRpm >= 2000) {
            goto done;
        }
        value = 0;
        goto done;
    } else {
        index = *(s16 *)(arg0 + 0x132);
        value *= g_PeakOutputValue / ((index * 200) + 300);
        *(s32 *)(arg0 + 0x150) = *(s32 *)(arg0 + 0x150) / index;
        if (*(s16 *)(arg0 + 0x132) >= 2) {
            g_GripLossTimer = 200;
        }
    }

done:
    g_StandingStartSpin = value;
}
