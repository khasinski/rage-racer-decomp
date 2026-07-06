#include "common.h"
#include "game/car.h"

extern GameCarRuntime D_801F1854[];

void func_80017A10(s32 arg0);
void func_8001DFC0(GameCarRuntime *arg0);

void func_80038A88(void) {
    func_80017A10(1);
    func_8001DFC0(D_801F1854);
}
