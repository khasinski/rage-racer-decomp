#include <sys/types.h>

#include "common.h"

typedef struct {
    u_char sync;
    u_char ready;
    u_char command;
} CdState;

typedef struct {
    long deadline;
    long count;
    char *name;
} CdAlarm;

typedef void (*CdCallback)(u_char status, u_char *result);

extern CdCallback g_CdSyncCallback asm("D_8009903C");
extern CdCallback g_CdReadyCallback asm("D_80099040");
extern long g_CdDebugLevel asm("D_80099048");
extern u_char g_CdLastPos[4] asm("D_80099058");
extern u_char g_CdModeByte asm("D_8009905C");
extern u_char D_8009905D;
extern char *g_CdCommandNames[] asm("D_80099060");
extern char *g_CdIntrNames[] asm("D_800990E0");
extern long g_CdCommandClearsReady[] asm("D_80099180");
extern long g_CdCommandParamCount[] asm("D_80099280");
/* CD-ROM controller ports 0x1F801800..03, from the data segment. Port 0 is
 * the index/status register; 1..3 are bank-switched by the index it holds. */
extern volatile u_char *g_CdReg0 asm("D_80099300");
extern volatile u_char *g_CdReg1 asm("D_80099304");
extern volatile u_char *g_CdReg2 asm("D_80099308");
extern volatile CdState g_CdSyncStatus asm("D_80099318");
extern u_char g_CdSyncResult[8] asm("D_8009BAF0");
extern u_char g_CdReadyResult[8] asm("D_8009BAF8");
extern CdAlarm g_CdTimeoutDeadline asm("D_8009BB08");
extern char D_80013814[];
extern char D_80013824[];
extern char D_800138B0[];
extern char D_800138B8[];
extern char D_800138C8[];

void func_80063C38(char *text);
long func_8006AB5C(void);
long func_8006B0D4(long mode, u_char *result);
void func_8006BAF0(void);
long VSync(long mode) asm("func_8006DD30");
long func_8006E088(void);

static inline void setAlarm(char *name) {
    g_CdTimeoutDeadline.deadline = VSync(-1) + 0x3C0;
    g_CdTimeoutDeadline.count = 0;
    g_CdTimeoutDeadline.name = name;
}

static inline long getAlarm(void) {
    if (g_CdTimeoutDeadline.deadline < VSync(-1) ||
        g_CdTimeoutDeadline.count++ > 0x3C0000) {
        func_80063C38(D_80013814);
        GameDebugPrintf(D_80013824, g_CdTimeoutDeadline.name, g_CdCommandNames[D_8009905D],
                      g_CdIntrNames[g_CdSyncStatus.sync], g_CdIntrNames[g_CdSyncStatus.ready]);
        func_8006BAF0();
        return -1;
    }
    return 0;
}

long CD_cw(u_char command, u_char *params, u_char *result, long async) asm("func_8006B620");
long CD_cw(u_char command, u_char *params, u_char *result, long async) {
    long i;
    long interrupt;
    long remaining;
    u_char interruptState;
    u_char *destination;
    u_char *source;

    if (g_CdDebugLevel >= 2) {
        GameDebugPrintf(D_800138B0, g_CdCommandNames[command]);
    }

    if (g_CdCommandParamCount[command] != 0 && params == 0) {
        if (g_CdDebugLevel > 0) {
            GameDebugPrintf(D_800138B8, g_CdCommandNames[command]);
        }
        return -2;
    }

    func_8006B0D4(0, 0);

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

    D_8009905D = command;
    *g_CdReg1 = command;

    if (async != 0) {
        return 0;
    }

    setAlarm(D_800138C8);

    while (g_CdSyncStatus.sync == 0) {
        if (getAlarm() != 0) {
            return -1;
        }

        if (func_8006E088() != 0) {
            interruptState = *g_CdReg0 & 3;
            while ((interrupt = func_8006AB5C()) != 0) {
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
