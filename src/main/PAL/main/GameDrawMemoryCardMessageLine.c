#include "common.h"
#include "game/memcard.h"

extern u8 D_800128FC[];

void func_80016754(s32 arg0, s32 arg1, void *arg2, s32 arg3);

void GameDrawMemoryCardMessageLine(s32 arg0, s32 arg1) {
    func_80016754(0x28, 0xB8, &D_800128FC[arg1 * 30], 0x78CC);
}
