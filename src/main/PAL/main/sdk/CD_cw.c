#include <sys/types.h>

#include "common.h"
#include "psyq/cd.h"

extern CdCallback g_CdSyncCallback;
extern CdCallback g_CdReadyCallback;
extern long g_CdDebugLevel;
extern u_char g_CdLastPos[4];
extern u_char g_CdModeByte;
extern u_char g_CdLastCommand;
extern char *g_CdCommandNames[];
extern char *g_CdIntrNames[];
extern long g_CdCommandClearsReady[];
extern long g_CdCommandParamCount[];
/* CD-ROM controller ports 0x1F801800..03, from the data segment. Port 0 is
 * the index/status register; 1..3 are bank-switched by the index it holds. */
extern volatile u_char *g_CdReg0;
extern volatile u_char *g_CdReg1;
extern volatile u_char *g_CdReg2;
extern volatile CdIntr g_CdSyncStatus;
extern u_char g_CdSyncResult[8];
extern u_char g_CdReadyResult[8];
extern CdAlarm g_CdTimeoutDeadline;
extern char D_80013814[];
extern char D_80013824[];
extern char D_800138B0[];
extern char D_800138B8[];
extern char D_800138C8[];

void LibcPutString(char *text);
long CdReadInterruptStatus(void);
long CD_sync(long mode, u_char *result);
long VSync(long mode);
long GetKernelStatus(void);

static inline void setAlarm(char *name) {
    g_CdTimeoutDeadline.deadline = VSync(-1) + 0x3C0;
    g_CdTimeoutDeadline.count = 0;
    g_CdTimeoutDeadline.name = name;
}

static inline long getAlarm(void) {
    if (g_CdTimeoutDeadline.deadline < VSync(-1) ||
        g_CdTimeoutDeadline.count++ > 0x3C0000) {
        LibcPutString(D_80013814);
        DebugPrintf(D_80013824, g_CdTimeoutDeadline.name, g_CdCommandNames[g_CdLastCommand],
                      g_CdIntrNames[g_CdSyncStatus.sync], g_CdIntrNames[g_CdSyncStatus.ready]);
        CD_flush();
        return -1;
    }
    return 0;
}

long CD_cw(u_char command, u_char *params, u_char *result, long async) {
    long i;
    long interrupt;
    long remaining;
    u_char interruptState;
    u_char *destination;
    u_char *source;

    if (g_CdDebugLevel >= 2) {
        DebugPrintf(D_800138B0, g_CdCommandNames[command]);
    }

    if (g_CdCommandParamCount[command] != 0 && params == 0) {
        if (g_CdDebugLevel > 0) {
            DebugPrintf(D_800138B8, g_CdCommandNames[command]);
        }
        return -2;
    }

    CD_sync(0, 0);

    if (command == 2) {
        for (i = 0; i < 4; i++) {
            g_CdLastPos[i] = params[i];
        }
    }

    g_CdSyncStatus.sync = 0;
    if (g_CdCommandClearsReady[command] != 0) {
        g_CdSyncStatus.ready = 0;
    }
    *g_CdReg0 = 0;

    for (i = 0; i < g_CdCommandClearsReady[0x40 + command]; i++) {
        *g_CdReg2 = params[i];
    }

    g_CdLastCommand = command;
    *g_CdReg1 = command;

    if (async != 0) {
        return 0;
    }

    setAlarm(D_800138C8);

    while (g_CdSyncStatus.sync == 0) {
        if (getAlarm() != 0) {
            return -1;
        }

        if (GetKernelStatus() != 0) {
            interruptState = *g_CdReg0 % 4;
            while ((interrupt = CdReadInterruptStatus()) != 0) {
                if ((interrupt & 4) != 0 && g_CdReadyCallback != 0) {
                    g_CdReadyCallback(g_CdSyncStatus.ready, g_CdReadyResult);
                }
                if ((interrupt & 2) != 0 && g_CdSyncCallback != 0) {
                    g_CdSyncCallback(g_CdSyncStatus.sync, g_CdSyncResult);
                }
            }
            *g_CdReg0 = interruptState;
        }
    }

    destination = result;
    if (g_CdSyncStatus.sync == 2 && command == 0xE) {
        g_CdModeByte = *params;
    }

    source = g_CdSyncResult;
    if (destination != 0) {
        remaining = 7;
        do {
            *destination++ = *source++;
            remaining--;
        } while (remaining != -1);
    }

    return -(g_CdSyncStatus.sync == 5);
}
