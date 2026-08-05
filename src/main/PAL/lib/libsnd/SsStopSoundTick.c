#include "common.h"
#include "psyq/kernel.h"

typedef void (*Callback)(void);

extern long g_SndNoTickFlag;
extern Callback g_SndPrevVSyncCallback;
extern u_char g_SndTickUsesVSync;
extern u_char D_8009A569;
extern u_char g_SndTickIrq;

long KernelCallbackSlot2Wide(long arg0, Callback arg1) asm("KernelCallbackSlot2");

void SsStopSoundTick(void);

void SsStopSoundTick(void) {
    if (g_SndNoTickFlag == 0) {
        D_8009A569 = 0;
        EnterCriticalSection();

        if (g_SndTickUsesVSync != 0) {
            VSyncCallback(0);
            g_SndTickUsesVSync = 0;
        } else if (g_SndTickIrq == 0) {
            KernelCallbackSlot2Wide(0, g_SndPrevVSyncCallback);
            g_SndPrevVSyncCallback = 0;
        } else {
            KernelCallbackSlot2Wide(6, 0);
        }

        ExitCriticalSection();
        g_SndTickIrq = 0xFF;
    }
}
