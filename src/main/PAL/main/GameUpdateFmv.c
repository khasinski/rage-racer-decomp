#include "common.h"

extern s32 D_8009F094;
extern u32 g_AssetBase asm("D_8019C904");

void GameStartFmvPlayback(u32 arg0) asm("func_8001E79C");
void GameDecodeFmvFrame(void) asm("func_8001E8A4");
void GameEndFmv(void) asm("func_8001EA34");

void GameUpdateFmv(void) asm("func_8001E71C");
void GameUpdateFmv(void) {
    s32 state = D_8009F094;

    if (state == 1) {
        goto state_1;
    }
    if (state < 2) {
        if (state == 0) {
            goto state_0;
        }
        goto done;
    }
    if (state == 2) {
        goto state_2;
    }
    goto done;

state_0:
    GameStartFmvPlayback(g_AssetBase);
state_1:
    GameDecodeFmvFrame();
    goto done;

state_2:
    GameEndFmv();

done:
}
