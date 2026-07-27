#include "common.h"
#include "game/cd.h"

s32 _card_info(s32 arg0) asm("func_80063DAC");
s32 _card_load(s32 arg0) asm("func_80063DBC");

s32 GameCdReadStatusPair(s32 high, s32 low) asm("func_8005EBB0");
s32 GameCdReadStatusPair(s32 high, s32 low) {
    s32 cmd;
    s32 ret;
    s32 event;

    cmd = (high << 4) + low;

    _card_info(cmd);
    event = GameWaitMemoryCardHwEvent();

    switch (event) {
    case 1:
        ret = 1;
        break;
    case 2:
        ret = -3;
        break;
    case 3:
        ret = -1;
        break;
    case 4:
        ret = 2;
        GameClearMemoryCardSwEvents();
        _card_clear(cmd);
        GameWaitMemoryCardSwEvent();
        goto return_minus_one;
    default:
        ret = -3;
        break;
    }

return_minus_one:
    if (ret == -1 || ret == -3) {
        return ret;
    }

    GameClearMemoryCardHwEvents();
    _card_load(cmd);
    event = GameWaitMemoryCardHwEvent();

    switch (event) {
    case 1:
        return ret;
    case 2:
        ret = -3;
        break;
    case 3:
        ret = -1;
        break;
    case 4:
        ret = -2;
        break;
    default:
        ret = -3;
        break;
    }

    return ret;
}
