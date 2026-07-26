#include "common.h"
extern s32 D_8009B538;
extern s32 D_8009B53C;
extern s32 D_8009B540;
extern s32 D_8009B544;
void func_800631C0(s32 arg0);
void GameClearCdResultEvents(void) asm("func_8005F2AC");
void GameClearCdResultEvents(void) {
    func_800631C0(D_8009B538);
    func_800631C0(D_8009B53C);
    func_800631C0(D_8009B540);
    func_800631C0(D_8009B544);
}
