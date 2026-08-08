#include "common.h"
#include "psyq/cd.h"

extern u_char g_CdStatusByte;

/* CdStatus: returns the last cached CD drive status byte. */
long CdStatus(void) {
    return g_CdStatusByte;
}

extern CdlLOC g_CdLastPos;

/* CdMode: returns the last CD mode byte. */
u_char CdMode(void) {
    return g_CdModeByte;
}

/* CdLastCom: returns the last CD command byte issued. */
u_char CdLastCom(void) {
    return g_CdLastCommand;
}

CdlLOC *CdLastPos(void) {
    return &g_CdLastPos;
}

/*
 * CD reset/init sequence keyed by `mode`: mode 2 only (re)installs the CD
 * interrupt handler (CD_initintr); otherwise it runs the CD flush/sync
 * (CdResetState) and, for mode 1, the volume init (CD_initvol). Returns 1 on
 * success, 0 if a sub-step failed.
 */
long CD_init(long mode) {
    if (mode == 2) {
        CD_initintr();
        return 1;
    }

    if (CdResetState() != 0) {
        return 0;
    }
    if (mode != 1) {
        return 1;
    }

    if (CD_initvol() != 0) {
        return 0;
    }

    asm("");
    return 1;
}
