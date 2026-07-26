#include "common.h"
#include "game/race.h"
#include "game/state.h"

extern s32 D_801E429C;
extern s32 D_801E4184;
extern s16 D_801E4DAC;
extern GameRaceProgress *D_801E4FAC;

void func_80033AA0(s32 arg0, s32 arg1);
void func_8001B2D4(void);

void func_8001B884(void) {
    s32 value;
    GameRaceProgress *ptr;

    value = D_801E429C + 1;
    D_801E429C = value;
    func_80033AA0(value * 2, 0x59);

    if (D_801E429C >= 0x81) {
        switch (D_801E4184) {
        case 0:
        case 1:
            ptr = D_801E4FAC;
            D_801E4DAC = 1;
            if (ptr->progression == -1) {
                ptr->progression = 0;
                g_SceneId = 0x1F;
                g_GrandPrixSeries = 0;
            } else {
                g_SceneId = 6;
            }
            break;
        case 2:
            D_801E4DAC = 0;
            g_SceneId = 6;
            break;
        case 3:
            g_SceneId = 0x19;
            break;
        case 4:
            g_SceneId = 0x16;
            break;
        }
    }

    func_8001B2D4();
}
