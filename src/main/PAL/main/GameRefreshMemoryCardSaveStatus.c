#include "common.h"
#include "game/memcard.h"

extern s32 D_8009B738;
extern s32 D_8009B73C;
extern s32 D_8009B740;

s32 GameRefreshMemoryCardSaveStatus(s32 arg0, GameSaveHeaderRow *arg1) {
    s32 ret;

    D_8009B740 = 0x100;
    GameClearSaveHeaderRows(arg1);
    D_8009B738 = GameCountMemoryCardFiles(0, 0);
    D_8009B73C = GameCalculateMemoryCardFreeBlocks(D_8009B738);
    ret = GameScanMemoryCardSaveHeaders(arg1);
    D_8009B740 = 0x200;

    return ret;
}
