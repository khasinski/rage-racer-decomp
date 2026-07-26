#include "common.h"
#include "game/render.h"


void AddPrim(void *ot, void *prim) asm("func_80064DDC");
void *func_80017390(void *arg0, void *arg1, s32 arg2);

void func_80032E9C(s32 arg0) {
    s32 count;
    s32 i;
    s32 offset;
    void **scratch;

    count = 9;
    if (arg0 != 0) {
        count = 0xC;
    }

    i = 6;
    if (i < count) {
        offset = 0x23770;
        do {
            u8 *base = g_DrawBuffer;

            i++;
            AddPrim(base + 0xCC, (void *)(offset + (s32)base));
            offset += 0x14;
        } while (i < count);
    }

    scratch = (void **)0x1F800000;
    *scratch = func_80017390(g_DrawBuffer + 0xCC, *scratch, 9);
}
