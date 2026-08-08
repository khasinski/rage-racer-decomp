#include "common.h"
#include "psyq/kernel.h"
#include "game/memcard.h"
#include "game/memcard_internal.h"

void ClearMemoryCardHwEvents(void) {
    TestEvent(g_McHwEventIoe);
    TestEvent(g_McHwEventError);
    TestEvent(g_McHwEventTimeout);
    TestEvent(g_McHwEventNew);
}

void ClearMemoryCardSwEvents(void) {
    TestEvent(g_McSwEventIoe);
    TestEvent(g_McSwEventError);
    TestEvent(g_McSwEventTimeout);
    TestEvent(g_McSwEventNew);
}

s32 PollMemoryCardHwEvent(void) {
    s32 result;
    s32 ready;
    s32 count;

    ready = 1;
    if (TestEvent(g_McHwEventIoe) == 1) {
        result = 1;
    }
    if (TestEvent(g_McHwEventError) == 1) {
        result = 2;
    }
    if (TestEvent(g_McHwEventTimeout) == 1) {
        result = 3;
    }
    if (TestEvent(g_McHwEventNew) == 1) {
        result = 4;
    }

    count = g_McPollTicks;
    g_McPollTicks = count + 1;
    if (count >= 90) {
        result = 2;
    }

    return result;
}

s32 PollMemoryCardHwEventLimit(s32 limit) {
    s32 i;
    i = 0;
    while (i < limit) {
        if (TestEvent(g_McHwEventIoe) == 1) {
            return 1;
        }
        if (TestEvent(g_McHwEventError) == 1) {
            return 2;
        }
        if (TestEvent(g_McHwEventTimeout) == 1) {
            return 3;
        }
        if (TestEvent(g_McHwEventNew) == 1) {
            return 4;
        }
        i++;
    }

    return 0;
}

s32 WaitMemoryCardHwEvent(void) {
    s32 ready;

    ready = 1;
    while (1) {
        if (TestEvent(g_McHwEventIoe) == 1) {
            return 1;
        }
        if (TestEvent(g_McHwEventError) == 1) {
            return 2;
        }
        if (TestEvent(g_McHwEventTimeout) == 1) {
            return 3;
        }
        if (TestEvent(g_McHwEventNew) == 1) {
            return 4;
        }
    }
}

s32 WaitMemoryCardSwEvent(void) {
    s32 ready;

    ready = 1;
    while (1) {
        if (TestEvent(g_McSwEventIoe) == 1) {
            return 1;
        }
        if (TestEvent(g_McSwEventError) == 1) {
            return 2;
        }
        if (TestEvent(g_McSwEventTimeout) == 1) {
            return 3;
        }
        if (TestEvent(g_McSwEventNew) == 1) {
            return 4;
        }
    }
}
void RestartMemoryCard(void) { InitCARD(1); StartCARD(); BiosBuInit(); g_SaveElapsedTicks = 0; }


void AdvanceSaveHeaderCounter(void) {
    if (g_FrameSyncThreshold == 0x80) {
        g_SaveElapsedTicks++;
    } else {
        g_SaveElapsedTicks += 2;
    }
}
