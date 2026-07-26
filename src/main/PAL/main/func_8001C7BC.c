#include "common.h"
#include "game/asset.h"
#include "game/race.h"
#include "game/state.h"
#include "game/render.h"

extern s32 D_8019C768;
extern s32 D_801E4B30;
extern u8 *D_8009E67C;

void func_80065860(s32 arg0);
void func_8005B9CC(void);
void func_8001A3C0(s32 arg0);
void func_80018F08(void);

void func_8001C7BC(void) {
    s32 count;
    u8 *ptr;
    u8 *end;

    func_80065860(0);
    D_8019C768 = 0x80;

    if (g_AssetLoadState != 1) {
        func_8005B9CC();
        func_8001A3C0(D_801E4B30);
        func_80018F08();

        D_8019C768 = 0x180;
        g_SceneTimer = 0;
        g_SceneId = 10;
        g_FadeLevel = 0;
        count = (g_GrandPrixClass < 2) ? 3 : 4;
        g_GrandPrixRound = 0;

        if (count != 0) {
            ptr = D_8009E67C;
            end = (u8 *)(count + (s32)ptr);
            do {
                if (*ptr != 0) {
                    g_GrandPrixRound++;
                }
                ptr++;
            } while ((s32)ptr < (s32)end);
        }

        if (D_8009E67C[g_CourseIndex] == 0) {
            g_GrandPrixRound++;
        }
    }
}
