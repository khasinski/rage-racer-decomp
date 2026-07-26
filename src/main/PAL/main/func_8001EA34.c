#include "common.h"
#include "game/state.h"
extern s32 D_8019C760;
extern s32 D_8009AF74;
void func_800640B0(s32 arg0);
void func_8006CE20(void);
void func_8001EA34(void) {
    func_800640B0(0);
    func_8006CE20();
    g_SceneId = D_8019C760;
    D_8019C760 = D_8009AF74;
}
