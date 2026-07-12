#include "common.h"
#include "game/asset.h"

extern s32 D_801E682C;
extern u32 D_801E42E4;
extern u16 D_801E436E;
extern u32 D_8019C760;
extern void (*D_8007D6D0[])(void);

void func_80017BE4(void);
void func_800268EC(void);

void func_80026AE0(void) {
    D_8007D6D0[D_801E682C]();

    if ((D_801E42E4 == 0x1E) && ((D_801E436E & 0x860) != 0)) {
        if (g_AssetLoadState != 0) {
            func_80017BE4();
            D_801E42E4 = 3;
            D_8019C760 = 0;
        } else {
            func_800268EC();
        }
    }
}
