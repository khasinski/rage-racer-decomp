#include "common.h"
#include "psyq/gpu.h"

extern GpuCallbacks *D_800941E0;
extern u8 D_800941EA;
extern void (*D_800941E4)(char *, ...);
extern char D_80013534[];

void DrawSync(s32 arg0) asm("func_800658FC");
void DrawSync(s32 arg0) {
    if (D_800941EA >= 2) {
        D_800941E4(D_80013534, arg0);
    }
    D_800941E0->drawSync(arg0);
}
