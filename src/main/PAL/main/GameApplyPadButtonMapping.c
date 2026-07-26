#include "common.h"
#include "game/state.h"

extern s16 D_8019CE08;
extern s16 D_8019CB08;

/* Re-applies the button mapping from the two saved selections. */
void GameApplyPadButtonMapping(void) {
    GameLoadPadButtonMapping(D_8019CE08, D_8019CB08);
}
