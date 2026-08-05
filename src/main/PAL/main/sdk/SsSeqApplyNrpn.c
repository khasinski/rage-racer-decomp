#include <sys/types.h>

#include "common.h"

typedef struct Arg {
    /*00*/ u_char  f00;
    /*01*/ u_char  f01;
    /*02*/ u_char  r02[4];
    /*06*/ u_char  f06;
    /*07*/ u_char  f07;
    /*08*/ u_char  r08;
    /*09*/ u_char  f09;
    /*0a*/ u_char  f0a;
    /*0b*/ u_char  r0b[5];
    /*10*/ u_short f10;
    /*12*/ u_short f12;
    /*14*/ u_char  r14[0x0c];
    /*20*/ u_short f20;
    /*22*/ u_short f22;
    /*24*/ u_short f24;
    /*26*/ u_short f26;
    /*28*/ u_short f28;
    /*2a*/ u_short f2a;
    /*2c*/ u_short f2c;
    /*2e*/ u_short f2e;
    /*30*/ u_short f30;
    /*32*/ u_short r32;
} Arg;

void SsUtGetVagAtr(short, short, short, Arg *);
void SsUtSetVagAtr(short, short, short, Arg *);
void SsUnpackAdsr(u_short, u_short, u_short *);
void SsPackAdsr(u_short *, u_short *, u_short *);
void SsUtSetReverbType(long) asm("func_80073614");
void SsUtSetReverbDepth(long, long) asm("func_80073748");
void SsUtSetReverbFeedback(long);
void SsUtSetReverbDelay(long);
void SsUtReverbOff(void) asm("func_800736E8");
void SsUtReverbOn(void);

void SsSeqApplyNrpn(short p0, short p1, short p2, Arg arg, short mode, u_char val) asm("func_800706AC");
void SsSeqApplyNrpn(short p0, short p1, short p2, Arg arg, short mode, u_char val) {
    SsUtGetVagAtr(p0, p1, p2, &arg);

    switch (mode) {
    case 0:
        arg.f00 = val;
        SsUtSetVagAtr(p0, p1, p2, &arg);
        break;
    case 1:
        arg.f01 = val;
        SsUtSetVagAtr(p0, p1, p2, &arg);
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
        arg.f06 = val;
        SsUtSetVagAtr(p0, p1, p2, &arg);
        break;
    case 3:
        arg.f07 = val;
        SsUtSetVagAtr(p0, p1, p2, &arg);
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
        SsUnpackAdsr(arg.f10, arg.f12, &arg.f20);
        switch (mode) {
        case 4:
            arg.f2a = 0;
            arg.f20 = val;
            break;
        case 5:
            arg.f2a = 1;
            arg.f20 = val;
            break;
        case 6:
            arg.f22 = val;
            break;
        case 7:
            arg.f24 = val;
            break;
        case 8:
            arg.f2c = 0;
            arg.f26 = val;
            break;
        case 9:
            arg.f2c = 1;
            arg.f26 = val;
            break;
        case 10:
            arg.f2e = 0;
            arg.f28 = val;
            break;
        case 11:
            arg.f2e = 1;
            arg.f28 = val;
            break;
        case 12:
            if (val != 0 && val < 64) {
                arg.f30 = 0;
            } else if ((u_char)(val - 64) < 64) {
                arg.f30 = 1;
            }
            break;
        case 13:
            arg.f09 = val;
            break;
        case 14:
            arg.f0a = val;
            break;
        }
        SsPackAdsr(&arg.f20, &arg.f10, &arg.f12);
        SsUtSetVagAtr(p0, p1, p2, &arg);
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
