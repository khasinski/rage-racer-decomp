#include "common.h"
#include "psyq/kernel.h"

extern u8 *D_80099360;
extern u8 *D_8009936C;

void func_8006A994(s32 arg0);
void func_8006A58C(s32 arg0);

void func_8006CE20(void) {
    EnterCriticalSection();
    func_8006A994(0);
    func_8006A58C(0);
    *D_80099360 = 0;
    *D_8009936C = 0;
    asm("" ::: "memory");
    ExitCriticalSection();
}
