#include "common.h"
#include "game/audio.h"
#include "game/menu.h"

extern s32 D_8009B9A0;

void GameStartMenuExitFade(void) {
    GameStopCdEvents();
    D_8009B9A0 = 8;
}
