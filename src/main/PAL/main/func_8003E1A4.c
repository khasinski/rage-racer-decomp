#include "common.h"
#include "game/race.h"


void GameDrawAnimatedScenery(s32 arg0, s32 arg1) asm("func_8003D6F0");
void GameDrawSpinningScenery(s32 arg0, s32 arg1) asm("func_8003DDAC");
void GameDrawHighClassScenery(void) asm("func_8003E0D0");
void GameDrawStaticScenery(s32 arg0) asm("func_8003DF68");
void GameUpdateShuttleScenery(s32 arg0) asm("func_8003F2A4");
void GameDrawShuttleScenery(s32 arg0) asm("func_8003F4BC");

void GameDrawCourseScenery(s32 arg0, s32 arg1, s32 arg2) asm("func_8003E1A4");

void GameDrawCourseScenery(s32 arg0, s32 arg1, s32 arg2) {
    register s32 mode asm("$16") = arg0;
    register s32 value asm("$18") = arg1;
    register s32 flag asm("$17");

    asm("" : "=r"(mode) : "0"(mode));
    asm("" : "=r"(value) : "0"(value));

    flag = arg2;

    GameDrawAnimatedScenery(value, 0);

    if (g_GrandPrixClass == 5) {
        flag = 0;
    }

    switch (mode) {
    case 0:
        GameDrawSpinningScenery(value, flag);
        if (g_GrandPrixClass >= 4) {
            GameDrawHighClassScenery();
        }
        goto call0;
    case 1:
        if (g_GrandPrixClass >= 2) {
            GameDrawSpinningScenery(value, flag);
        }
        if (flag != 0) {
            GameUpdateShuttleScenery(0);
        }
        GameDrawShuttleScenery(0);
        goto call0;
    case 2:
        if (flag != 0) {
            GameUpdateShuttleScenery(0);
            GameUpdateShuttleScenery(1);
        }
        GameDrawShuttleScenery(0);
        GameDrawShuttleScenery(1);
        goto call0;
    case 3:
        GameDrawAnimatedScenery(value, 1);
        goto call1;
    default:
        return;
    }

call0:
    GameDrawStaticScenery(0);
    return;
call1:
    GameDrawStaticScenery(1);
}
