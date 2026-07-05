#include "common.h"

typedef struct Rect {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} Rect;

void LoadImage(Rect *arg0, void *arg1) asm("func_80065B24");
s32 GetClut(s32 arg0, s32 arg1) asm("func_80064C7C");

s32 LoadClut2(void *arg0, s32 arg1, s32 arg2) asm("func_800649D8");

s32 LoadClut2(void *arg0, s32 arg1, s32 arg2) {
    Rect rect;

    rect.x = arg1;
    rect.y = arg2;
    rect.w = 0x100;
    rect.h = 1;

    LoadImage(&rect, arg0);
    return GetClut(arg1, arg2) & 0xFFFF;
}
