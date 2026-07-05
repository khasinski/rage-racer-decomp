#include "common.h"
#include "psyq/gte.h"

typedef struct ScratchpadRenderState {
    s32 pad0;
    void *primData;
    u8 pad8[0x14];
    s32 depth;
    u8 pad20[8];
    Matrix matrix;
    u8 pad48[0x20];
    s32 orderingFlag;
    s32 mode;
    u8 pad70[8];
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
} ScratchpadRenderState;

extern Matrix D_8009AF00;
extern u8 * volatile D_8019C900;
extern s32 D_801E6828;
extern s32 D_8019C86C;
extern s32 D_801E4BC8;
extern s32 D_8009EC94;

void func_80069A58(s32 arg0, s32 arg1);
void func_80069A78(s32 arg0);

void func_8001ABD8(void) {
    ScratchpadRenderState *scratch;
    register s32 v0reg asm("$2");
    register s32 v1reg asm("$3");

    scratch = (ScratchpadRenderState *)0x1F800000;

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
