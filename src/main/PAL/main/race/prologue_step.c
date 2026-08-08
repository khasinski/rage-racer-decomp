#include "common.h"
#include "game/state.h"

extern long g_PrologueStep;

void TickPrologueStep(void) {
    void (*func)(void);

    func = D_8007D778[g_PrologueStep];
    g_SceneTimer++;
    func();
}
