#include <sys/types.h>

#include "common.h"
#include "psyq/snd.h"

/* Empty two-word stub. Was misnamed SsUtKeyOnV, which is the eight
 * argument function at SsUtKeyOnV in sdk/SsUtKeyOnV.c. */
/*
 * Keys a sound effect on through the SE pseudo-sequence (seq_sep 0x21),
 * converting the caller's left/right volume pair into the volume + pan pair
 * SpuVmSeKeyOn wants: the louder side becomes the volume, and the quieter side
 * its share of the 0..0x7F pan sweep. Left as func_ because the libsnd entry
 * point it corresponds to takes a different argument list; see the note on
 * func_80076C50 below.
 */
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

/* The matching key-off on the same SE pseudo-sequence. */
void func_80076C1C(long voice, long volLeft, long volRight) {
    SpuVmSeKeyOff(0x21, (short)voice, (short)volLeft, (u_short)volRight);
}

void func_80076C50(void) {
}
