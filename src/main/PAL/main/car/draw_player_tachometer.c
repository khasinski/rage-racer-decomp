#include "common.h"
#include "game/car.h"
#include "game/render.h"
#include "game/state.h"
#include "game/track_internal.h"

s32 DrawPlayerTachometer(void) {
    s32 value;
    s32 type;
    u32 amount;

    if (g_TrackZoneDark != 3) {
        value = g_EnvScriptClock;
        amount = value - 0x1154;
        if (amount < 0x434C) {
            if (amount < 0x80) {
                type = 3;
            } else {
                amount = value - 0x5420;
                if (amount < 0x80) {
                    type = 1;
                } else {
                    type = 0;
                    amount = 0;
                }
            }
        } else {
            type = 2;
            amount = 0;
        }
    } else {
        type = 2;
        amount = 0;
    }

    return DrawTachometer(g_EngineRpm + g_EngineRpmJitter,
                          g_TachoNeedleFlash, type, amount);
}
