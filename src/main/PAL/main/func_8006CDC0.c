#include "common.h"
#include "psyq/cd.h"

extern s32 D_801F1850;
extern s32 D_801E6C98;
extern s32 D_801E6C84;
extern s32 D_801E6C74;
extern s32 D_801E42C8;
extern s32 D_8019CA00;
extern s16 D_8019C790;
extern s32 D_8009E69C;

void func_8006D0AC(s32 arg0, s32 arg1);

void StClearRing(void) {
    D_801E6C98 = 0;
    D_801E6C84 = 0;
    D_801E6C74 = 0;
    D_801E42C8 = 0;
    func_8006D0AC(0, D_801F1850);
    D_8019CA00 = 0;
    D_8019C790 = 0;
    D_8009E69C = 0;
}
