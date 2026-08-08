#include "common.h"
#include "game/state.h"
#include "game/race_internal.h"

void TickPrologueStep(void) {
    void (*func)(void);

    func = g_PrologueSteps[g_PrologueStep];
    g_SceneTimer++;
    func();
}
