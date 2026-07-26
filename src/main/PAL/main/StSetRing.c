#include "common.h"
extern s32 D_801E8AAC;
extern s32 D_801F1850;
void func_8006CDC0(void);

/* StSetRing: installs the stream ring buffer (`base`, `size`) then clears it. */
void func_8006A058(s32 base, s32 size) { D_801E8AAC = base; D_801F1850 = size; func_8006CDC0(); }
