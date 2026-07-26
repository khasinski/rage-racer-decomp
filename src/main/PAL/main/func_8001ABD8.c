#include "common.h"
#include "game/render.h"

extern Matrix D_8009AF00;
extern u8 * volatile D_8019C900;
extern s32 D_801E6828;
extern s32 D_8019C86C;
extern s32 D_801E4BC8;
extern s32 D_8009EC94;

void func_80069A58(s32 arg0, s32 arg1);
void func_80069A78(s32 arg0);

/*
 * Sibling of func_8001A9A8: closes the mirror pass and restores the full-screen
 * main viewport render state (mode 0xA, full 0x140x0xF0 clip rect, prim base),
 * flips the ordering flag back, pulls the depth back (-= 0x800) and restores the
 * saved main-view matrix from D_8009AF00.
 */
void func_8001ABD8(void) {
    GameScratchpadRenderState *scratch;
    register s32 v0reg asm("$2");
    register s32 v1reg asm("$3");

    scratch = (GameScratchpadRenderState *)0x1F800000;

    func_80069A58(0xA0, 0x78);
    func_80069A78(0x140);

    scratch->mode = 0xA;
    scratch->x1 = 0x140;
    scratch->y1 = 0xF0;
    D_801E6828 = (s32)&D_8019C86C;
    __asm__("" : : : "memory");
    v0reg = (s32)D_8019C900;
    v1reg = (s32)&D_8009EC94;
    D_801E4BC8 = v1reg;
    v1reg = scratch->depth;
    scratch->x0 = 0;
    scratch->y0 = 0;
    v0reg += 0xCC;
    scratch->primData = (void *)v0reg;
    __asm__("" : "=r"(v0reg) : "0"(v0reg) : "memory");
    v0reg = scratch->orderingFlag;
    scratch->depth = v1reg - 0x800;
    scratch->orderingFlag = v0reg ^ 1;
    scratch->matrix = D_8009AF00;
}
