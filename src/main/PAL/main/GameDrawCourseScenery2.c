#include "common.h"
#include "game/race.h"
#include "game/state.h"


void GameDrawAnimatedScenery2(s32 arg0, s32 arg1, s32 arg2, s32 arg3) asm("func_8003DA90");
void GameDrawSpinningScenery(s32 arg0, s32 arg1) asm("func_8003DDAC");
void GameDrawHighClassScenery(void) asm("func_8003E0D0");
void GameDrawStaticScenery(s32 arg0) asm("func_8003DF68");
void GameUpdateShuttleScenery(s32 arg0) asm("func_8003F2A4");
void GameDrawShuttleScenery(s32 arg0) asm("func_8003F4BC");

void GameDrawCourseScenery2(s32 arg0, s32 arg1) asm("func_8003E2E8");

void GameDrawCourseScenery2(s32 arg0, s32 arg1) {
    s32 value = arg0;
    s32 flag = arg1;
    s32 mode;

    if (g_GrandPrixClass == 5) {
        flag = 0;
    }

    GameDrawAnimatedScenery2(value, 0, g_SceneId == 0x11, flag);

    mode = g_CourseIndex & 3;
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
        GameDrawAnimatedScenery2(value, 1, g_SceneId == 0x11, flag);
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
