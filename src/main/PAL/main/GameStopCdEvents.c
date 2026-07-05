#include "common.h"
#include "game/audio.h"
#include "game/cd.h"

void GameStopCdEvents(void) {
    GameDisableCdEventGroup();
    GameCloseCdEventGroup();
}
