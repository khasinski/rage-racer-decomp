#include "common.h"

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
