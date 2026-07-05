#include "common.h"
#include "game/memcard.h"

extern s32 D_8019C768;
extern s32 D_801E7A54;

void GameAdvanceSaveHeaderCounter(void) {
    if (D_8019C768 == 0x80) {
        D_801E7A54++;
    } else {
        D_801E7A54 += 2;
    }
}
