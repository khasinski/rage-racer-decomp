#include "common.h"
#include "psyq/kernel.h"

typedef void (*Callback)(void);

extern long g_SndNoTickFlag asm("D_8009A55C");
extern Callback g_SndPrevVSyncCallback asm("D_8009A564");
extern u_char g_SndTickUsesVSync asm("D_8009A568");
extern u_char D_8009A569;
extern u_char g_SndTickIrq asm("D_8009A56A");

long func_8006DF64(long arg0, Callback arg1);
void func_8006DFC4(long arg0);

void SsStopSoundTick(void) asm("func_80072260");

void SsStopSoundTick(void) {
    if (g_SndNoTickFlag == 0) {
        D_8009A569 = 0;
        EnterCriticalSection();

        if (g_SndTickUsesVSync != 0) {
            func_8006DFC4(0);
            g_SndTickUsesVSync = 0;
        } else if (g_SndTickIrq == 0) {
            func_8006DF64(0, g_SndPrevVSyncCallback);
            g_SndPrevVSyncCallback = 0;
        } else {
            func_8006DF64(6, 0);
        }

        ExitCriticalSection();
        g_SndTickIrq = 0xFF;
    }
}
