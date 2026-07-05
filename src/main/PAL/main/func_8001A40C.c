#include "common.h"
#include "psyq/gpu.h"

extern Rect D_8007C710;
extern Rect D_8007C718;
extern GpuRectPacked D_8007C720;
extern s16 D_8019CABC;
extern u16 D_801E444C[];

void LoadImage(Rect *rect, void *data) asm("func_80065B24");
void StoreImage(Rect *rect, void *data) asm("func_80065B88");
s32 MoveImage(GpuRectPacked *rect, u32 x, u32 y) asm("func_80065BEC");
void DrawSync(s32 mode) asm("func_800658FC");

void func_8001A40C(void *dst) {
    D_801E444C[0] = 0x8000;
    LoadImage(&D_8007C718, D_801E444C);

    if (D_8019CABC != 0) {
        MoveImage(&D_8007C720, 0x3F0, 0xE2);
    }

    StoreImage(&D_8007C710, dst);
    DrawSync(0);
    D_801E444C[0] = 0;
}
