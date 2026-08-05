#include "common.h"

extern long g_SndTickMode asm("D_8009A558");
extern long g_SndNoTickFlag asm("D_8009A55C");
extern long g_SndTickResolution;

long GetDMAInterruptState(long arg);

void SsSetTickMode(long spec);
void SsSetTickMode(long spec) {
    long v;
    long m;

    v = GetDMAInterruptState(spec);
    if (spec & 0x1000) {
        g_SndNoTickFlag = 1;
        g_SndTickMode = spec & 0xFFF;
    } else {
        g_SndNoTickFlag = 0;
        g_SndTickMode = spec;
    }

    m = g_SndTickMode;
    if (m < 6) {
    switch (m) {
    case 4:
        g_SndTickResolution = 50;
        g_SndTickMode = (v == 1) ? 5 : 50;
        break;
    case 1:
        g_SndTickResolution = 60;
        g_SndTickMode = (v == 0) ? 5 : 60;
        break;
    case 3:
        g_SndTickResolution = 120;
        break;
    case 2:
        g_SndTickResolution = 240;
        break;
    case 5:
        if (v == 0) {
            g_SndTickResolution = 60;
        } else if (v == 1) {
            g_SndTickResolution = 50;
        } else {
            g_SndTickResolution = 60;
        }
        break;
    case 0:
        if (v == 0) {
            g_SndTickResolution = 60;
        } else if (v == 1) {
            g_SndTickResolution = 50;
        } else {
            g_SndTickResolution = 60;
        }
        break;
    default:
        g_SndTickResolution = 60;
        break;
    }
    } else {
        g_SndTickResolution = m;
    }
}
