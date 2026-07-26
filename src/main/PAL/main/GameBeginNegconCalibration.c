#include "common.h"
#include "game/state.h"

/*
 * The six live NeGcon settings. All of them are persisted to the memory card,
 * so they also appear as fields of GameSaveBlock (game/memcard.h).
 */
extern u16 D_801E4BF0;
extern u16 D_8019CA08;
extern u16 D_8019CA0A;
extern u16 D_8019CA0C;
extern u16 D_8019CAD0;
extern u16 D_801E418C;

/* ...and where each of them is parked while the calibration screens run, so a
 * cancel can put every one of them back. */
extern u16 D_801E41A0;
extern u16 D_8009F0A8;
extern u16 D_8009F0AA;
extern u16 D_8009F0AC;
extern u16 D_8019C75C;
extern u16 D_8019CB04;

/* The same four screen counters GameBeginControllerConfig clears. */
extern s32 D_801E8AA4;
extern s32 D_801E8A9C;
extern s32 D_801E7A4C;
extern s32 D_801E6C7C;

/*
 * Entry hook for the NeGcon calibration sequence: snapshots the six live
 * settings, clears the three the neutral/twist screens are about to measure
 * along with the screen counters, and enters game mode 9 (the "hold the
 * controller still" screen).
 *
 * The pins and the empty barrier only hold the reads ahead of the clears and
 * keep the last two snapshots in the registers retail used; the code itself is
 * ordinary C (see docs/ASM_AND_GTE_POLICY.md).
 */
void GameBeginNegconCalibration(void) {
    register u16 twist asm("$3");
    register u16 mode asm("$4");
    u16 *neutral = &D_8019CA08;
    u16 steer = D_801E4BF0;
    u16 neutral0 = *neutral;
    u16 neutral1 = D_8019CA0A;
    u16 neutral2 = D_8019CA0C;

    twist = D_8019CAD0;
    mode = D_801E418C;
    asm("");

    *neutral = 0;
    D_801E8AA4 = 0;
    D_801E8A9C = 0;
    D_801E7A4C = 0;
    D_801E6C7C = 0;
    D_801E4BF0 = 0;
    D_8019CA0A = 0;
    D_8019CA0C = 0;
    g_GameMode = 9;
    D_801E41A0 = steer;
    D_8009F0A8 = neutral0;
    D_8009F0AA = neutral1;
    D_8009F0AC = neutral2;
    D_8019C75C = twist;
    D_8019CB04 = mode;
}
