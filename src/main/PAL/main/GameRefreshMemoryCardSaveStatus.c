#include "common.h"
#include "game/memcard.h"
#include "game/menu.h"

extern s32 D_8009B738;
extern s32 D_8009B73C;

s32 GameRefreshMemoryCardSaveStatus(s32 arg0, GameSaveHeaderRow *arg1) {
    s32 ret;

    GameMenuLoadPhase = 0x100;
    GameClearSaveHeaderRows(arg1);
    D_8009B738 = GameCountMemoryCardFiles(0, 0);
    D_8009B73C = GameCalculateMemoryCardFreeBlocks(D_8009B738);
    ret = GameScanMemoryCardSaveHeaders(arg1);
    GameMenuLoadPhase = 0x200;

    return ret;
}
