#include <sys/types.h>

#include "common.h"

void SpuVmSeKeyOn(long seq_sep, short vab_id, short program, u_short note, u_short bend, u_short pan);
void SpuVmSeKeyOff(long vab_id, short program, short tone, u_short note);

/* Empty two-word stub. Was misnamed SsUtKeyOnV, which is the eight
 * argument function at SsUtKeyOnV in sdk/SsUtKeyOnV.c. */
void func_80076C50(void);

void func_80076B30(short vabId, short prog, u_short note, long fine, u_short voll, u_short volr) {
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

void func_80076C1C(long voice, long volLeft, long volRight) {
    SpuVmSeKeyOff(0x21, voice, volLeft, volRight);
}

void func_80076C50(void) {
}
