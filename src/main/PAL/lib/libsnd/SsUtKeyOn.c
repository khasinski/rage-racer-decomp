#include <sys/types.h>

#include "common.h"

void SpuVmSeKeyOn(long arg0, short arg1, short arg2, u_short arg3, u_short arg4, u_short arg5);
void SpuVmSeKeyOff(long vab_id, short program, short tone, u_short note);

/* Empty two-word stub. Was misnamed SsUtKeyOnV, which is the eight
 * argument function at SsUtKeyOnV in sdk/SsUtKeyOnV.c. */
void func_80076C50(void);

void func_80076B30(short vabId, short prog, u_short note, long arg3, u_short voll, u_short volr) {
    u_short volume;
    u_short pan;

    if (voll == volr) {
        pan = 0x40;
        volume = voll;
    } else if (volr < voll) {
        volume = voll;
        pan = (volr * 0x40) / voll;
    } else {
        volume = volr;
        pan = 0x7F - ((voll * 0x40) / volr);
    }

    SpuVmSeKeyOn(0x21, vabId, prog, note, volume, pan);
}

void func_80076C1C(long arg0, long arg1, long arg2) {
    SpuVmSeKeyOff(0x21, arg0, arg1, arg2);
}

void func_80076C50(void) {
}
