#include "common.h"
#include "game/state.h"

extern long g_PrologueStep;

void TickPrologueStep(void) {
    void (*func)(void);

    func = g_PrologueSteps[g_PrologueStep];
    g_SceneTimer++;
    func();
}
