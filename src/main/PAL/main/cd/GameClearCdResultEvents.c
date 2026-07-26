#include "common.h"
#include "psyq/kernel.h"
#include "game/memcard.h"

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

extern s32 D_8009B548;
extern s32 D_8009B54C;
extern s32 D_8009B550;
extern s32 D_8009B554;
void GameClearCdCompleteEvents(void) asm("func_8005F304");
void GameClearCdCompleteEvents(void) {
    func_800631C0(D_8009B548);
    func_800631C0(D_8009B54C);
    func_800631C0(D_8009B550);
    func_800631C0(D_8009B554);
}

extern s32 D_8019C864;

s32 GamePollCdResultEventWithTimeout(void) asm("func_8005F35C");
s32 GamePollCdResultEventWithTimeout(void) {
    register s32 result asm("$16");
    register s32 ready asm("$17");
    s32 count;

    ready = 1;
    if (TestEvent(D_8009B538) == ready) {
        result = 1;
    }
    if (TestEvent(D_8009B53C) == ready) {
        result = 2;
    }
    if (TestEvent(D_8009B540) == ready) {
        result = 3;
    }
    if (TestEvent(D_8009B544) == ready) {
        result = 4;
    }

    count = D_8019C864;
    D_8019C864 = count + 1;
    if (count >= 90) {
        result = 2;
    }

    return result;
}

s32 GamePollCdResultEventLimit(s32 limit) asm("func_8005F420");
s32 GamePollCdResultEventLimit(s32 limit) {
    register s32 i asm("$16");
    register s32 ready asm("$17");
    volatile s32 stack[2];

    i = 0;
    if (limit > 0) {
        ready = 1;
        do {
            if (TestEvent(D_8009B538) == ready) {
                return 1;
            }
            if (TestEvent(D_8009B53C) == ready) {
                return 2;
            }
            if (TestEvent(D_8009B540) == ready) {
                return 3;
            }
            if (TestEvent(D_8009B544) == ready) {
                return 4;
            }
            i++;
        } while (i < limit);
    }

    return 0;
}

s32 GameWaitCdResultEvent(void) asm("func_8005F4D8");
s32 GameWaitCdResultEvent(void) {
    register s32 ready asm("$16");

    ready = 1;
    while (1) {
        if (TestEvent(D_8009B538) == ready) {
            return 1;
        }
        if (TestEvent(D_8009B53C) == ready) {
            return 2;
        }
        if (TestEvent(D_8009B540) == ready) {
            return 3;
        }
        if (TestEvent(D_8009B544) == ready) {
            return 4;
        }
    }
}

s32 GameWaitCdCompleteEvent(void) asm("func_8005F55C");
s32 GameWaitCdCompleteEvent(void) {
    register s32 ready asm("$16");

    ready = 1;
    while (1) {
        if (TestEvent(D_8009B548) == ready) {
            return 1;
        }
        if (TestEvent(D_8009B54C) == ready) {
            return 2;
        }
        if (TestEvent(D_8009B550) == ready) {
            return 3;
        }
        if (TestEvent(D_8009B554) == ready) {
            return 4;
        }
    }
}

extern s32 D_801E7A54;
void func_80063DCC(s32 arg0);
void func_80063DDC(void);
void func_80063180(void);
void func_8005F5E0(void) { func_80063DCC(1); func_80063DDC(); func_80063180(); D_801E7A54 = 0; }

extern s32 D_8019C768;

void GameAdvanceSaveHeaderCounter(void) {
    if (D_8019C768 == 0x80) {
        D_801E7A54++;
    } else {
        D_801E7A54 += 2;
    }
}
