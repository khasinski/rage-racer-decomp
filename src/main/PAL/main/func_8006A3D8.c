#include "common.h"
#include "psyq/cd.h"

s32 CdStatus(void);
u8 CdMode(void);
u8 CdLastCom(void);
CdlLOC *CdLastPos(void);

u32 func_8006A3D8[4] __attribute__((section(".text"))) = {
    0x240A00B0,
    0x01400008,
    0x24090007,
    0,
};

extern u8 D_8009904C;

/* CdStatus: returns the last cached CD drive status byte. */
s32 CdStatus(void) asm("func_8006A3E8");
s32 CdStatus(void) {
    return D_8009904C;
}

extern CdlLOC D_80099058;
extern u8 D_8009905C;
extern u8 D_8009905D;

/* CdMode: returns the last CD mode byte. */
u8 CdMode(void) asm("func_8006A3F8");
u8 CdMode(void) {
    return D_8009905C;
}

/* CdLastCom: returns the last CD command byte issued. */
u8 CdLastCom(void) asm("func_8006A408");
u8 CdLastCom(void) {
    return D_8009905D;
}

/* CdLastPos: returns the last reported disc position. */
CdlLOC * CdLastPos(void) asm("func_8006A418");
CdlLOC *CdLastPos(void) {
    return &D_80099058;
}

s32 func_8006BCC4_entry(void) asm("func_8006BCC4");
s32 func_8006BD14_entry(void) asm("func_8006BD14");
s32 func_8006BBD0_entry(void) asm("func_8006BBD0");

/*
 * CD reset/init sequence keyed by `mode`: mode 2 only (re)installs the CD
 * interrupt handler (CD_initintr); otherwise it runs the CD flush/sync
 * (func_8006BD14) and, for mode 1, the volume init (CD_initvol). Returns 1 on
 * success, 0 if a sub-step failed.
 */
s32 CD_init(s32 mode) asm("func_8006A428");
s32 CD_init(s32 mode) {
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
