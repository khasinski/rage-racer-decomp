#include "common.h"
#include "psyq/kernel.h"


void func_80078838(void);
void func_80071B0C(void);

void func_80071BF4(void) {
    KernelCallbackSlot3();
    func_80078838();
    func_80071B0C();
}
