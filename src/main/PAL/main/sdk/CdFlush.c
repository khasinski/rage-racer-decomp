#include "common.h"
#include "psyq/cd.h"

extern long g_CdDebugLevel;
extern long g_CdSyncCallback;
extern long g_CdReadyCallback;
extern char *g_CdCommandNames[];
extern char *g_CdIntrNames[];
extern u_char D_800136D0[];

void CD_flush(void);
long CD_sync(long arg0, long arg1);
long CD_ready(long arg0, long arg1);

void CdFlush(void) {
    CD_flush();
}

/* CdSetDebug: sets the CD debug-verbosity `level`, returns the previous one. */
long CdSetDebug(long level) {
    long old = g_CdDebugLevel;
    g_CdDebugLevel = level;
    return old;
}

/* CdComstr: returns the human-readable name for CD command `cmd` (or a default
 * string if out of range). */
char * CdComstr(long cmd);
char *CdComstr(long cmd) {
    cmd &= 0xFF;
    if ((u_long)cmd >= 0x1C) {
        return (char *)D_800136D0;
    }
    return g_CdCommandNames[cmd];
}

/* CdIntstr: returns the human-readable name for CD interrupt code `intr`. */
char * CdIntstr(long intr);
char *CdIntstr(long intr) {
    intr &= 0xFF;
    if ((u_long)intr >= 7) {
        return (char *)D_800136D0;
    }
    return g_CdIntrNames[intr];
}

long CdSync(long arg0, long arg1) {
    return CD_sync(arg0, arg1);
}

long CdReady(long arg0, long arg1) {
    return CD_ready(arg0, arg1);
}

long CdSyncCallback(long arg0) {
    long old = g_CdSyncCallback;

    g_CdSyncCallback = arg0;
    return old;
}

long CdReadyCallback(long arg0) {
    long old = g_CdReadyCallback;

    g_CdReadyCallback = arg0;
    return old;
}
