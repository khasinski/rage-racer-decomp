#include "common.h"
extern s32 D_8009AF58, D_801E8A90;
void func_80063E44(s32 arg0);
void DecDCToutCallback(s32 arg0) asm("func_800640B0");
void func_8006A058(s32 arg0, s32 arg1);
void func_8006CF68(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 GameStartStreamRead(s32 arg0) asm("func_8001F018");
void GameOpenFmvStream(s32 arg0) asm("func_8001EB5C");
void GameOpenFmvStream(s32 arg0) {
    func_80063E44(0);
    DecDCToutCallback(arg0);
    func_8006A058(D_8009AF58, 0x20);
    func_8006CF68(1, 1, -1, 0, 0);
    GameStartStreamRead(D_801E8A90);
}
