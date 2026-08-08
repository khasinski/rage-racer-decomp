#include "common.h"
#include "psyq/cd.h"

extern long g_CdSyncCallback;
extern long g_CdReadyCallback;
extern char *g_CdCommandNames[];
extern char *g_CdIntrNames[];
extern u_char g_MsgCdNone[];

void CdFlush(void) {
    CD_flush();
}

/* CdSetDebug: sets the CD debug-verbosity `level`, returns the previous one. */
long CdSetDebug(long level) {
    long old = g_CdDebugLevel;
    g_CdDebugLevel = level;
    return old;
}

char *CdComstr(long cmd) {
    cmd &= 0xFF;
    if ((u_long)cmd >= 0x1C) {
        return (char *)g_MsgCdNone;
    }
    return g_CdCommandNames[cmd];
}

char *CdIntstr(long intr) {
    intr &= 0xFF;
    if ((u_long)intr >= 7) {
        return (char *)g_MsgCdNone;
    }
    return g_CdIntrNames[intr];
}

long CdSync(long mode, long result) {
    return CD_sync(mode, result);
}

long CdReady(long mode, long result) {
    return CD_ready(mode, result);
}

long CdSyncCallback(long callback) {
    long old = g_CdSyncCallback;

    g_CdSyncCallback = callback;
    return old;
}

long CdReadyCallback(long callback) {
    long old = g_CdReadyCallback;

    g_CdReadyCallback = callback;
    return old;
}
