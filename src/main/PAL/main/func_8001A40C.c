#include "common.h"
#include "psyq/gpu.h"

extern Rect D_8007C710;
extern Rect D_8007C718;
extern GpuRectPacked D_8007C720;
extern s16 D_8019CABC;
extern u16 D_801E444C[];

void func_80065B24(Rect *rect, void *data);
void func_80065B88(Rect *rect, void *data);
s32 func_80065BEC(GpuRectPacked *rect, u32 x, u32 y);
void func_800658FC(s32 mode);

void func_8001A40C(void *dst) {
    D_801E444C[0] = 0x8000;
    func_80065B24(&D_8007C718, D_801E444C);

    if (D_8019CABC != 0) {
        func_80065BEC(&D_8007C720, 0x3F0, 0xE2);
    }

    func_80065B88(&D_8007C710, dst);
    func_800658FC(0);
    D_801E444C[0] = 0;
}
