#include <sys/types.h>

#include "common.h"

void _spu_setTransferCompletionFlag(long) asm("func_8007B294");
void SpuInitMalloc(long, u_char *) asm("func_80079B60");
void SsUtFlush(void) asm("func_80075FA4");

extern u_char g_SpuMallocArea[] asm("D_8009E5E0");

extern volatile u_short D_801E4B5C;
extern volatile u_short g_SndDamper asm("D_8019CA68");
extern volatile u_short g_SndVabOpenCount asm("D_801F17AC");

extern u_short g_SndVoiceRegs[] asm("D_8009DF20");
extern u_char g_SndVoiceFlags[] asm("D_8009E0A0");
extern u_char g_SndVabStatus[] asm("D_801E4CFC");

extern volatile u_char D_801E42F8;
extern volatile u_short g_SndCurrentVoice asm("D_801E4BEA");
extern volatile u_short D_801F2A08;
extern volatile u_short D_801F2A0C;
extern volatile u_short D_8009E670;
extern volatile u_short D_8009E674;
extern u_short *g_SndSpuRegs asm("D_8009A588");

extern u_char g_SndVoiceState[] asm("D_8009E0B8");
extern u_char g_SndVoiceStateAge[] asm("D_8009E0BA");
extern u_char g_SndVoiceStatePitch[] asm("D_8009E0BC");
extern u_char g_SndVoiceStateEnvx[] asm("D_8009E0BE");
extern u_char D_8009E0C0[];
extern u_char D_8009E0C2[];
extern u_char g_SndVoiceStateSeqSep[] asm("D_8009E0C6");
extern u_char g_SndVoiceStateProgActual[] asm("D_8009E0C8");
extern u_char g_SndVoiceStateProg[] asm("D_8009E0CA");
extern u_char g_SndVoiceStateTone[] asm("D_8009E0CC");
extern u_char g_SndVoiceStateStatus[] asm("D_8009E0D3");
extern u_char g_SndVoiceStateAutoVol[] asm("D_8009E0D4");
extern u_char D_8009E0D6[];
extern u_char D_8009E0D8[];
extern u_char D_8009E0DA[];
extern u_char D_8009E0DC[];
extern u_char g_SndVoiceStateAutoPan[] asm("D_8009E0E0");
extern u_char D_8009E0E2[];
extern u_char D_8009E0E4[];
extern u_char D_8009E0E6[];
extern u_char D_8009E0E8[];

extern volatile u_short g_SndReverbAttrDepthLeft asm("D_8019C780");
extern volatile u_short g_SndReverbAttrDepthRight asm("D_8019C782");
extern volatile u_short g_SndReverbOnLow asm("D_8009E680");
extern volatile u_short g_SndReverbOnHigh asm("D_8009E684");
extern volatile u_long g_SndReverbAttr asm("D_8019C778");
extern volatile u_long g_SndReverbAttrMode asm("D_8019C77C");
extern volatile u_char g_SndReservedVoiceCount asm("D_801E4D88");
extern volatile u_short g_SndMonoMode asm("D_801E3FB0");
extern volatile u_short g_SndVabProgMax asm("D_801E40D0");

void SpuVmInit(long arg0) asm("func_80075710");
void SpuVmInit(long arg0) {
    /* This pin is load-bearing: removing it changes .text. */
    s16 i;
    long ff;
    long one;
    long index;
    long offset;
    /* This pin is load-bearing: removing it changes .text. */
    s16 shifted;
    long eighteen;
    long mindex;
    u_long lowMask;
    u_long highMask;
    /* These pins are load-bearing: removing any one changes .text. */
    register u_long lowBits asm("$3");
    register u_long highBits asm("$4");
    volatile u_short *spu;
    u_long bits;
    u_long n;
    u_long cond;

    {
        u_char *p = g_SpuMallocArea;
        _spu_setTransferCompletionFlag(0);
        D_801E4B5C = 0;
        g_SndDamper = 0;
        /* This barrier is load-bearing: removing it changes .text. */
        asm volatile("" ::: "memory");
        SpuInitMalloc(0x20, p);
    }

    for (i = 0; (u_short)i < 192; i++) g_SndVoiceRegs[(u_short)i] = 0;
    for (i = 0; (u_short)i < 24; i++) g_SndVoiceFlags[(u_short)i] = 0;
    g_SndVabOpenCount = 0;
    for (i = 0; (u_short)i < 16; i++) g_SndVabStatus[(u_short)i] = 0;

    n = arg0 & 0xff;
    if (n >= 24) {
        D_801E42F8 = 24;
    } else {
        D_801E42F8 = n;
    }
    if (D_801E42F8 != 0) {
            i = 0;
            ff = 0xFF;
            one = 1;
            do {
                index = (u_short)i;
                shifted = index << 3;
                offset = index * 0x34;
                eighteen = 0x18;   *(short *)&g_SndVoiceStateAge[offset] = eighteen;
                eighteen = -1;     *(short *)&g_SndVoiceStateSeqSep[offset] = eighteen;
                *(short *)&g_SndVoiceState[offset] = ff;
                g_SndVoiceStateStatus[offset] = 0;
                *(short *)&g_SndVoiceStatePitch[offset] = 0;
                *(short *)&g_SndVoiceStateEnvx[offset] = 0;
                *(short *)&g_SndVoiceStateProgActual[offset] = 0;
                *(short *)&g_SndVoiceStateProg[offset] = 0;
                *(short *)&g_SndVoiceStateTone[offset] = ff;
                *(short *)&D_8009E0C0[offset] = 0;
                eighteen = 0x40;   D_8009E0C2[offset] = eighteen;
                *(short *)&g_SndVoiceStateAutoVol[offset] = 0;
                *(short *)&D_8009E0D6[offset] = 0;
                *(short *)&D_8009E0D8[offset] = 0;
                *(short *)&D_8009E0DA[offset] = 0;
                *(short *)&g_SndVoiceStateAutoPan[offset] = 0;
                *(short *)&D_8009E0E2[offset] = 0;
                *(short *)&D_8009E0E4[offset] = 0;
                *(short *)&D_8009E0E6[offset] = 0;
                *(short *)&D_8009E0E8[offset] = 0;
                *(short *)&D_8009E0DC[offset] = 0;

                spu = (volatile u_short *)&g_SndSpuRegs[(u_short)shifted];
                spu[3] = 0x200;
                spu[2] = 0x1000;
                spu[4] = 0x80FF;
                spu[0] = 0;
                spu[1] = 0;
                spu[5] = 0x4000;

                g_SndCurrentVoice = i;
                lowBits = g_SndCurrentVoice;
                /* This barrier is load-bearing: it hides the halfword load,
                 * whose known-zero high bits would otherwise let gcc drop the
                 * mask. */
                mindex = lowBits & 0xFFFF;
                if ((u_long)mindex < 0x10) {
                    lowMask = one << mindex;
                    highMask = 0;
                } else {
                    lowMask = 0;
                    offset = mindex - 0x10;
                    highMask = one << offset;
                }

                lowBits &= 0xFFFF;
                offset = lowBits * 0x34;
                g_SndVoiceStateStatus[offset] = 0;
                lowBits = D_801F2A08;
                highBits = D_801F2A0C;
                i++;
                *(short *)&g_SndVoiceStatePitch[offset] = 0;
                *(short *)&g_SndVoiceState[offset] = 0;

                bits = D_8009E670;
                /* This barrier is load-bearing: removing it changes .text. */
                __asm__ volatile("");
                /* These barriers are load-bearing. Without them `combine` folds
                 * the single-use `zero_extend(mem)` that defines the second
                 * operand into the `ior`, tripping its "complex expression
                 * first" rule and swapping the operands; retail keeps the
                 * written mask-first order. */
                /* These barriers are load-bearing: removing any one changes .text. */
                asm("" : "=r"(lowBits) : "0"(lowBits));
                lowBits = lowMask | lowBits;
                asm("" : "=r"(highBits) : "0"(highBits));
                highBits = highMask | highBits;
                D_801F2A08 = lowBits;
                bits = bits & ~lowBits;
                D_801F2A0C = highBits;
                D_8009E670 = bits;
                bits = D_8009E674;
                cond = D_801E42F8;
                D_8009E674 = bits & ~highBits;
            } while ((u_short)i < cond);
    }

    g_SndReverbAttrDepthLeft = 0x3FFF;
    g_SndReverbAttrDepthRight = 0x3FFF;
    D_8009E670 = 0;
    D_8009E674 = 0;
    D_801F2A08 = 0;
    g_SndReverbOnLow = 0;
    g_SndReverbOnHigh = 0;
    g_SndReverbAttr = 0;
    g_SndReverbAttrMode = 0;
    g_SndReservedVoiceCount = 0;
    g_SndMonoMode = 0;
    g_SndVabProgMax = 0x80;
    SsUtFlush();
}
