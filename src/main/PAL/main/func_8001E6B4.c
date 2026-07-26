#include "common.h"
#include "game/state.h"
#include "game/cd.h"
extern s32 D_8009F094;
extern s32 g_StreamReturnScene asm("D_8019C760");
s32 func_8005B9CC(void);
void func_8006A534(s32 arg0, s32 arg1);
s32 CdControl(s32 com, void *param, s32 result) asm("func_8006A5A4");
void func_8001E6B4(s32 arg0) {
    func_8005B9CC();
    GameResetCdAudioState();
    D_8009F094 = 0;
    g_StreamReturnScene = arg0;
    g_SceneId = 5;
    func_8006A534(0, 0);
    CdControl(9, 0, 0);
}
