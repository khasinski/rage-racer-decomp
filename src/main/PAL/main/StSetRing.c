#include "common.h"
extern s32 D_801E8AAC;
extern s32 D_801F1850;
void StClearRing(void) asm("func_8006CDC0");

void StSetRing(s32 arg0, s32 arg1) asm("func_8006A058");
void StSetRing(s32 arg0, s32 arg1) { D_801E8AAC = arg0; D_801F1850 = arg1; StClearRing(); }
