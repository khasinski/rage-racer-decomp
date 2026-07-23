#include "common.h"
#include "game/cd.h"

s32 func_80063DAC(s32 arg0);
s32 func_80063DBC(s32 arg0);

s32 func_8005EBB0(s32 high, s32 low) {
    s32 cmd;
    s32 ret;
    s32 event;

    cmd = (high << 4) + low;

    func_80063DAC(cmd);
    event = GameWaitCdResultEvent();

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
        GameClearCdCompleteEvents();
        GameIssueCdCommand3F(cmd);
        GameWaitCdCompleteEvent();
        goto return_minus_one;
    default:
        ret = -3;
        break;
    }

return_minus_one:
    if (ret == -1 || ret == -3) {
        return ret;
    }

    GameClearCdResultEvents();
    func_80063DBC(cmd);
    event = GameWaitCdResultEvent();

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
