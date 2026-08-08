#include <sys/types.h>

#include "psyq/snd.h"

void SsSeqApplyNrpn(
    short p0,
    short p1,
    short p2,
    VagAtr tone_attr,
    SndAdsr adsr,
    short mode,
    u_char val) {
    SsUtGetVagAtr(p0, p1, p2, &tone_attr);

    switch (mode) {
    case 0:
        tone_attr.prior = val;
        SsUtSetVagAtr(p0, p1, p2, &tone_attr);
        break;
    case 1:
        tone_attr.mode = val;
        SsUtSetVagAtr(p0, p1, p2, &tone_attr);
        if (val == 0) {
            SsUtReverbOff();
            break;
        }
        if (val == 1) {
            break;
        }
        if (val == 2) {
            break;
        }
        if (val == 3) {
            break;
        }
        if (val == 4) {
            SsUtReverbOn();
            break;
        }
        break;
    case 2:
        tone_attr.min = val;
        SsUtSetVagAtr(p0, p1, p2, &tone_attr);
        break;
    case 3:
        tone_attr.max = val;
        SsUtSetVagAtr(p0, p1, p2, &tone_attr);
        break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        SsUnpackAdsr(tone_attr.adsr1, tone_attr.adsr2, (u_short *)&adsr);
        switch (mode) {
        case 4:
            adsr.sustain_direction = 0;
            adsr.attack_mode = val;
            break;
        case 5:
            adsr.sustain_direction = 1;
            adsr.attack_mode = val;
            break;
        case 6:
            adsr.attack_rate = val;
            break;
        case 7:
            adsr.decay_rate = val;
            break;
        case 8:
            adsr.sustain_rate = 0;
            adsr.sustain_level = val;
            break;
        case 9:
            adsr.sustain_rate = 1;
            adsr.sustain_level = val;
            break;
        case 10:
            adsr.release_mode = 0;
            adsr.sustain_mode = val;
            break;
        case 11:
            adsr.release_mode = 1;
            adsr.sustain_mode = val;
            break;
        case 12:
            if (val != 0 && val < 64) {
                adsr.release_rate = 0;
            } else if ((u_char)(val - 64) < 64) {
                adsr.release_rate = 1;
            }
            break;
        case 13:
            tone_attr.vibT = val;
            break;
        case 14:
            tone_attr.porW = val;
            break;
        }
        SsPackAdsr((u_short *)&adsr, &tone_attr.adsr1, &tone_attr.adsr2);
        SsUtSetVagAtr(p0, p1, p2, &tone_attr);
        break;
    case 15:
        SsUtSetReverbType(val);
        break;
    case 16: {
        long a0r = (u16)val;
        long a1r = (u8)a0r;
        SsUtSetReverbDepth(a0r, a1r);
        break;
    }
    case 17:
        SsUtSetReverbFeedback(val);
        break;
    case 18:
    case 19:
        SsUtSetReverbDelay(val);
        break;
    case 20:
    case 21:
    case 22:
        break;
    }
}
