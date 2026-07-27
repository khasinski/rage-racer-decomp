#include "common.h"
#include "psyq/kernel.h"
#include "game/memcard.h"

extern s32 g_McHwEventIoe asm("D_8009B538");
extern s32 g_McHwEventError asm("D_8009B53C");
extern s32 g_McHwEventTimeout asm("D_8009B540");
extern s32 g_McHwEventNew asm("D_8009B544");
void func_800631C0(s32 arg0);
void GameClearMemoryCardHwEvents(void) asm("func_8005F2AC");
void GameClearMemoryCardHwEvents(void) {
    func_800631C0(g_McHwEventIoe);
    func_800631C0(g_McHwEventError);
    func_800631C0(g_McHwEventTimeout);
    func_800631C0(g_McHwEventNew);
}

extern s32 g_McSwEventIoe asm("D_8009B548");
extern s32 g_McSwEventError asm("D_8009B54C");
extern s32 g_McSwEventTimeout asm("D_8009B550");
extern s32 g_McSwEventNew asm("D_8009B554");
void GameClearMemoryCardSwEvents(void) asm("func_8005F304");
void GameClearMemoryCardSwEvents(void) {
    func_800631C0(g_McSwEventIoe);
    func_800631C0(g_McSwEventError);
    func_800631C0(g_McSwEventTimeout);
    func_800631C0(g_McSwEventNew);
}

extern s32 g_McPollTicks asm("D_8019C864");

s32 GamePollMemoryCardHwEvent(void) asm("func_8005F35C");
s32 GamePollMemoryCardHwEvent(void) {
    register s32 result asm("$16");
    register s32 ready asm("$17");
    s32 count;

    ready = 1;
    if (TestEvent(g_McHwEventIoe) == ready) {
        result = 1;
    }
    if (TestEvent(g_McHwEventError) == ready) {
        result = 2;
    }
    if (TestEvent(g_McHwEventTimeout) == ready) {
        result = 3;
    }
    if (TestEvent(g_McHwEventNew) == ready) {
        result = 4;
    }

    count = g_McPollTicks;
    g_McPollTicks = count + 1;
    if (count >= 90) {
        result = 2;
    }

    return result;
}

s32 GamePollMemoryCardHwEventLimit(s32 limit) asm("func_8005F420");
s32 GamePollMemoryCardHwEventLimit(s32 limit) {
    register s32 i asm("$16");
    register s32 ready asm("$17");
    volatile s32 stack[2];

    i = 0;
    if (limit > 0) {
        ready = 1;
        do {
            if (TestEvent(g_McHwEventIoe) == ready) {
                return 1;
            }
            if (TestEvent(g_McHwEventError) == ready) {
                return 2;
            }
            if (TestEvent(g_McHwEventTimeout) == ready) {
                return 3;
            }
            if (TestEvent(g_McHwEventNew) == ready) {
                return 4;
            }
            i++;
        } while (i < limit);
    }

    return 0;
}

s32 GameWaitMemoryCardHwEvent(void) asm("func_8005F4D8");
s32 GameWaitMemoryCardHwEvent(void) {
    register s32 ready asm("$16");

    ready = 1;
    while (1) {
        if (TestEvent(g_McHwEventIoe) == ready) {
            return 1;
        }
        if (TestEvent(g_McHwEventError) == ready) {
            return 2;
        }
        if (TestEvent(g_McHwEventTimeout) == ready) {
            return 3;
        }
        if (TestEvent(g_McHwEventNew) == ready) {
            return 4;
        }
    }
}

s32 GameWaitMemoryCardSwEvent(void) asm("func_8005F55C");
s32 GameWaitMemoryCardSwEvent(void) {
    register s32 ready asm("$16");

    ready = 1;
    while (1) {
        if (TestEvent(g_McSwEventIoe) == ready) {
            return 1;
        }
        if (TestEvent(g_McSwEventError) == ready) {
            return 2;
        }
        if (TestEvent(g_McSwEventTimeout) == ready) {
            return 3;
        }
        if (TestEvent(g_McSwEventNew) == ready) {
            return 4;
        }
    }
}

extern s32 g_SaveElapsedTicks asm("D_801E7A54");
void InitCARD(s32 arg0) asm("func_80063DCC");
void StartCARD(void) asm("func_80063DDC");
void func_80063180(void);
void func_8005F5E0(void) { InitCARD(1); StartCARD(); func_80063180(); g_SaveElapsedTicks = 0; }

extern s32 g_FrameSyncThreshold asm("D_8019C768");

void GameAdvanceSaveHeaderCounter(void) {
    if (g_FrameSyncThreshold == 0x80) {
        g_SaveElapsedTicks++;
    } else {
        g_SaveElapsedTicks += 2;
    }
}
