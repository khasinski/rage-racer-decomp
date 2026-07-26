#include "common.h"
#include "game/race.h"
#include "game/render.h"
#include "game/menu.h"

extern s32 D_8009B334;
extern s32 D_8009B360;
extern s32 D_8009B364;
extern s32 D_8009B368;
extern s32 D_801E8268;
extern u8 *D_8009E67C;
extern s32 D_8009E6D4;
extern s32 D_8009E6D8;
extern s32 D_8009E6DC;
extern s32 D_8009E6F4;
extern s32 D_8009E6F8;
extern s32 D_8009E6FC;
extern s32 D_8009E718;
extern s32 D_8009E71C;
extern s32 D_8009E744;
extern u8 D_8007BEDC[];
extern u8 D_8007BEE4[];
extern u8 D_801E444C[];
extern u8 D_801E6F2C[];

void func_80052738(void);
s32 func_80018530(void);
void func_8005E88C(void);
void func_80049418(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void LoadImage(void *rect, void *data) asm("func_80065B24");
void func_8001D530(void *arg0, s32 arg1);

void func_80052778(void) {
    register s32 one asm("$16");
    register s32 initValue asm("$3");
    register s32 mode asm("$4");
    register s32 largeValue asm("$5");
    register u8 *table asm("$2");
    register s32 eight asm("$2");

    func_80052738();
    if (func_80018530() != 0) {
        return;
    }

    one = 1;
    func_8005E88C();
    g_MenuHandlerIndex = one;
    g_MenuScreen = one;
    func_80049418(0, 0, 0, 0);

    initValue = 0x7A120;
    mode = 0x3D090;
    largeValue = 0x1F0000;
    asm volatile("" : "=r"(largeValue) : "0"(largeValue));
    g_MenuViewOffset = mode;
    mode = g_CourseIndex;
    eight = 8;
    D_801E8268 = eight;
    table = D_8009E67C;
    largeValue |= 0x4000;
    g_UiScriptProgress = 0;
    D_8009E6D4 = 0;
    D_8009E6D8 = 0;
    D_8009E6DC = 0;
    D_8009E6F4 = 0;
    D_8009E6F8 = 0;
    D_8009E6FC = 0;
    D_8009E744 = 0;
    D_8009E718 = 0;
    D_8009E71C = 0;
    g_MenuViewAngleTarget = 0x7A120;
    g_MenuViewAngle = initValue;
    g_MenuViewOffsetTarget = 0;
    D_8009B360 = largeValue;
    D_8009B364 = 0;
    D_8009B368 = table[mode & 3];

    if (mode >= 4) {
        D_8009B334 = one;
    } else {
        D_8009B334 = -1;
    }

    LoadImage(D_8007BEE4, D_801E6F2C);
    LoadImage(D_8007BEDC, D_801E444C);
    func_8001D530(g_TeamNameChars, g_TeamNameLength);
}
