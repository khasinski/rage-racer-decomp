#include "common.h"
#include "game/memcard.h"

extern s32 D_8009B740;
extern char D_800128AC[];
extern char D_800127D8[];

s32 GameWriteMemoryCardSaveSlot(s32 arg0, GameSaveHeaderRow *arg1) {
    u8 block0[0x200];
    u8 block1[0x1000];
    s32 i;

    for (i = 0x1FF; i >= 0; i--) {
        block0[i] = 0;
    }

    D_8009B740 = 0x1000;
    return GameWriteMemoryCardSaveFile(
        D_800128AC + arg0 * 0x1A,
        D_800127D8 + arg0 * 0x46,
        block0,
        arg1,
        block1);
}
