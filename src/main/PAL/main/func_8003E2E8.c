#include "common.h"
#include "game/race.h"
#include "game/state.h"


void func_8003DA90(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_8003DDAC(s32 arg0, s32 arg1);
void func_8003E0D0(void);
void func_8003DF68(s32 arg0);
void func_8003F2A4(s32 arg0);
void func_8003F4BC(s32 arg0);

void func_8003E2E8(s32 arg0, s32 arg1) {
    s32 value = arg0;
    s32 flag = arg1;
    s32 mode;

    if (g_GrandPrixClass == 5) {
        flag = 0;
    }

    func_8003DA90(value, 0, g_SceneId == 0x11, flag);

    mode = g_CourseIndex & 3;
    switch (mode) {
    case 0:
        func_8003DDAC(value, flag);
        if (g_GrandPrixClass >= 4) {
            func_8003E0D0();
        }
        goto call0;
    case 1:
        if (g_GrandPrixClass >= 2) {
            func_8003DDAC(value, flag);
        }
        if (flag != 0) {
            func_8003F2A4(0);
        }
        func_8003F4BC(0);
        goto call0;
    case 2:
        if (flag != 0) {
            func_8003F2A4(0);
            func_8003F2A4(1);
        }
        func_8003F4BC(0);
        func_8003F4BC(1);
        goto call0;
    case 3:
        func_8003DA90(value, 1, g_SceneId == 0x11, flag);
        goto call1;
    default:
        return;
    }

call0:
    func_8003DF68(0);
    return;
call1:
    func_8003DF68(1);
}
