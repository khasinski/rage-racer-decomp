#include <sys/types.h>

#include "common.h"
#include "psyq/cd.h"
#include "psyq/cd_internal.h"
#include "psyq/kernel.h"

long CdRead(long sectors, void *buf, long readMode) {
    long savedArg0;
    long mode;
    long value;

    value = (long)&g_CdReadMode;
    *(volatile long *)value = readMode;
    value = *(volatile long *)value;
    mode = value & 0x30;

    switch (mode) {
    case 0:
        value = 0x200;
        g_CdReadSectorWords = value;
        break;
    case 0x20:
        value = 0x249;
        g_CdReadSectorWords = value;
        break;
    default:
        value = 0x246;
        mode = (long)&g_CdReadSectorWords;
        *(volatile long *)mode = value;
        break;
    }

    mode = (long)&g_CdReadMode;
    savedArg0 = sectors;
    value = *(volatile long *)mode;
    value |= 0x20;
    *(volatile long *)mode = value;
    g_CdReadBuffer = (long)buf;
    g_CdReadSectorCount = savedArg0;
    g_CdReadSavedSyncCallback = CdSyncCallback(0);
    g_CdReadSavedReadyCallback = CdReadyCallback(0);
    g_CdReadStartVSync = VSync(-1);

    if ((CdStatus() & 0xE0) != 0) {
        CdControlB(9, 0, 0);
    }

    return CdReadRetry(0) > 0;
}

long CdReadSync(long mode, long result) {
    long savedMode;
    long savedResult;
    volatile long *state;
    long status;

    savedMode = mode;
    savedResult = result;
    state = &g_CdReadStartVSync;

    do {
        long now;

        now = VSync(-1);
        if (state[0] + 0x4B0 < now) {
            status = -1;
        } else {

        if (state[-2] < 0) {
            CdReadRetry(1);
            status = state[-7];
        } else {

        now = VSync(-1);
        if (state[-1] + 0x3C < now) {
            CdReadRetry(1);
            status = state[-7];
        } else {

        status = state[-2];

        }
        }
        }
        if (savedMode != 0) {
            break;
        }
    } while (status > 0);

    CdReady(1, savedResult);
    return status;
}

long CdReadCallback(long callback) {
    long old = g_CdReadCallback;

    g_CdReadCallback = callback;
    return old;
}
