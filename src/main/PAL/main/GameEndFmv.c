#include "common.h"
#include "game/state.h"
extern s32 g_StreamReturnScene asm("D_8019C760");
extern s32 D_8009AF74;
void DecDCToutCallback(s32 arg0) asm("func_800640B0");
void StUnSetRing(void) asm("func_8006CE20");
void GameEndFmv(void) asm("func_8001EA34");
void GameEndFmv(void) {
    DecDCToutCallback(0);
    StUnSetRing();
    g_SceneId = g_StreamReturnScene;
    g_StreamReturnScene = D_8009AF74;
}
