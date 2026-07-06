#include "common.h"
#include "game/render.h"

extern s32 D_801E8A98;
extern s16 D_8019CA10;
extern s32 D_8009E870;
extern s16 D_801E4DAC;
extern s16 D_801E6E74;
extern Matrix D_8009AF00;
extern Matrix D_8019CB18;
extern u8 *D_8019C900;
extern s16 D_8019CEAA;
extern s16 D_801C0692;
extern s16 D_8019CEAE;
extern s16 D_801C0696;
extern s32 D_801E6828;
extern s32 D_8019C7E4;
extern s32 D_801E4BC8;
extern s32 D_8009E888;
extern s32 D_801E4D18;

void func_80069A58(s32 arg0, s32 arg1);
void func_80069A78(s32 arg0);

s32 func_8001A9A8(void) {
    GameScratchpadRenderState *scratch;
    s32 active;
    register s32 v0reg asm("$2");
    register s32 v1reg asm("$3");
    register s32 y0 asm("$4");

    active = 0;
    scratch = (GameScratchpadRenderState *)0x1F800000;

    if ((D_801E8A98 != 0) &&
        (D_8019CA10 != 0) &&
        (D_8009E870 == 0) &&
        (D_801E4DAC != 0) &&
        (D_801E6E74 == 2)) {
        active = 1;
    }

    if (active != 0) {
        D_8009AF00 = scratch->matrix;
        scratch->matrix = D_8019CB18;

        func_80069A58(0xA0, 0x24);
        func_80069A78(0xC0);

        scratch->mode = 9;
        scratch->x0 = 0x56;
        __asm__("" ::: "memory");
        v0reg = (s32)D_8019C900;
        y0 = *(u16 *)&D_801E4D18;
        scratch->x1 = 0xEA;
        __asm__("" ::: "memory");
        v1reg = D_801E4D18;
        v0reg += 0xBCC;
        scratch->primData = (void *)v0reg;
        v0reg = scratch->orderingFlag;
        scratch->y0 = y0;
        v0reg ^= 1;
        scratch->orderingFlag = v0reg;
        v0reg = y0 + 0x24;
        scratch->y1 = v0reg;

        if (v1reg > 0) {
            D_8019CEAA = y0;
            v0reg = y0 + 0xF0;
        } else {
            v0reg = 0xF0;
            D_8019CEAA = 0;
        }
        D_801C0692 = v0reg;
        __asm__("" ::: "memory");

        v0reg = D_801E4D18;
        v1reg = v0reg + 0x24;
        if (v1reg > 0) {
            v0reg = v1reg - D_8019CEAA;
            D_8019CEAE = v0reg;
            D_801C0696 = v0reg;
        } else {
            D_8019CEAE = 0;
            D_801C0696 = 0;
        }

        D_801E6828 = (s32)&D_8019C7E4;
        D_801E4BC8 = (s32)&D_8009E888;
        scratch->depth += 0x800;
    }

    return active;
}
