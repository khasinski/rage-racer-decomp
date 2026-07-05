#include "common.h"
#include "game/audio.h"
#include "game/cd.h"

void GameStartCdEvents(void) {
    GameOpenCdEventGroup();
    GameEnableCdEventGroup();
}
