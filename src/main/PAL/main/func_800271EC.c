#include "common.h"
#include "game/state.h"

extern s32 D_801E4178;
extern void (*D_8007D778[])(void);

void func_800271EC(void) {
    void (*func)(void);

    func = D_8007D778[D_801E4178];
    g_SceneTimer++;
    func();
}
