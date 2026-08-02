#include "common.h"
#include "psyq/cd.h"

long CdStatus(void);
u_char CdMode(void);
u_char CdLastCom(void);
CdlLOC *CdLastPos(void);

u_long DeliverEvent[4] asm("func_8006A3D8") __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090007,
    0,
};

extern u_char g_CdStatusByte asm("D_8009904C");

/* CdStatus: returns the last cached CD drive status byte. */
long CdStatus(void) asm("func_8006A3E8");
long CdStatus(void) {
    return g_CdStatusByte;
}

extern CdlLOC g_CdLastPos asm("D_80099058");
extern u_char g_CdModeByte asm("D_8009905C");
extern u_char D_8009905D;

/* CdMode: returns the last CD mode byte. */
u_char CdMode(void) asm("func_8006A3F8");
u_char CdMode(void) {
    return g_CdModeByte;
}

/* CdLastCom: returns the last CD command byte issued. */
u_char CdLastCom(void) asm("func_8006A408");
u_char CdLastCom(void) {
    return D_8009905D;
}

/* CdLastPos: returns the last reported disc position. */
CdlLOC * CdLastPos(void) asm("func_8006A418");
CdlLOC *CdLastPos(void) {
    return &g_CdLastPos;
}

long func_8006BCC4_entry(void) asm("func_8006BCC4");
long func_8006BD14_entry(void) asm("func_8006BD14");
long func_8006BBD0_entry(void) asm("func_8006BBD0");

/*
 * CD reset/init sequence keyed by `mode`: mode 2 only (re)installs the CD
 * interrupt handler (CD_initintr); otherwise it runs the CD flush/sync
 * (func_8006BD14) and, for mode 1, the volume init (CD_initvol). Returns 1 on
 * success, 0 if a sub-step failed.
 */
long CD_init(long mode) asm("func_8006A428");
long CD_init(long mode) {
    if (mode == 2) {
        func_8006BCC4_entry();
        return 1;
    }

    if (func_8006BD14_entry() != 0) {
        return 0;
    }
    if (mode != 1) {
        return 1;
    }

    if (func_8006BBD0_entry() != 0) {
        return 0;
    }

    asm("");
    return 1;
}
