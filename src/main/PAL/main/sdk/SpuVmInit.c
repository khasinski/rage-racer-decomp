#include <sys/types.h>

#include "common.h"

void _spu_setTransferCompletionFlag(long);
void SpuInitMalloc(long, u_char *);
void SsUtFlush(void);

extern u_char g_SpuMallocArea[];

extern volatile u_short D_801E4B5C;
extern volatile u_short g_SndDamper;
extern volatile u_short g_SndVabOpenCount;

extern u_short g_SndVoiceRegs[];
extern u_char g_SndVoiceFlags[] asm("D_8009E0A0");
extern u_char g_SndVabStatus[];

extern volatile u_char g_SndVoiceCount;
extern volatile u_short g_SndCurrentVoice;
extern volatile u_short D_801F2A08;
extern volatile u_short D_801F2A0C;
extern volatile u_short D_8009E670;
extern volatile u_short D_8009E674;
extern u_short *g_SndSpuRegs;

extern u_char g_SndVoiceState[];
extern u_char g_SndVoiceStateAge[];
extern u_char g_SndVoiceStatePitch[];
extern u_char g_SndVoiceStateEnvx[];
extern u_char D_8009E0C0[];
extern u_char D_8009E0C2[];
extern u_char g_SndVoiceStateSeqSep[];
extern u_char g_SndVoiceStateProgActual[];
extern u_char g_SndVoiceStateProg[];
extern u_char g_SndVoiceStateTone[];
extern u_char g_SndVoiceStateStatus[];
extern u_char g_SndVoiceStateAutoVol[];
extern u_char D_8009E0D6[];
extern u_char D_8009E0D8[];
extern u_char D_8009E0DA[];
extern u_char D_8009E0DC[];
extern u_char g_SndVoiceStateAutoPan[];
extern u_char g_SndVoiceStatePanStep[];
extern u_char g_SndVoiceStatePanCounter[];
extern u_char g_SndVoiceStatePanCounterReload[];
extern u_char g_SndVoiceStateStartPan[];

extern volatile u_short g_SndReverbAttrDepthLeft;
extern volatile u_short g_SndReverbAttrDepthRight;
extern volatile u_short g_SndReverbOnLow;
extern volatile u_short g_SndReverbOnHigh;
extern volatile u_long g_SndReverbAttr;
extern volatile u_long g_SndReverbAttrMode;
extern volatile u_char g_SndReservedVoiceCount;
extern volatile u_short g_SndMonoMode;
extern volatile u_short g_SndVabProgMax;

void SpuVmInit(long arg0);
void SpuVmInit(long arg0) {
    s16 i;
    long ff;
    long one;
    long index;
    long offset;
    s16 shifted;
    long eighteen;
    long mindex;
    u_long lowMask;
    u_long highMask;
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
        asm volatile("" ::: "memory");
        SpuInitMalloc(0x20, p);
    }

    for (i = 0; (u_short)i < 192; i++) g_SndVoiceRegs[(u_short)i] = 0;
    for (i = 0; (u_short)i < 24; i++) g_SndVoiceFlags[(u_short)i] = 0;
    g_SndVabOpenCount = 0;
    for (i = 0; (u_short)i < 16; i++) g_SndVabStatus[(u_short)i] = 0;

    n = (u8)arg0;
    if (n >= 24) {
        g_SndVoiceCount = 24;
    } else {
        g_SndVoiceCount = n;
    }
    if (g_SndVoiceCount != 0) {
            i = 0;
            ff = 0xFF;
            one = 1;
            do {
                index = (u_short)i;
                shifted = index * 8;
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
                *(short *)&g_SndVoiceStatePanStep[offset] = 0;
                *(short *)&g_SndVoiceStatePanCounter[offset] = 0;
                *(short *)&g_SndVoiceStatePanCounterReload[offset] = 0;
                *(short *)&g_SndVoiceStateStartPan[offset] = 0;
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
                __asm__ volatile("");
                /* These barriers are load-bearing. Without them `combine` folds
                 * the single-use `zero_extend(mem)` that defines the second
                 * operand into the `ior`, tripping its "complex expression
                 * first" rule and swapping the operands; retail keeps the
                 * written mask-first order. */
                asm("" : "=r"(lowBits) : "0"(lowBits));
                lowBits = lowMask | lowBits;
                asm("" : "=r"(highBits) : "0"(highBits));
                highBits = highMask | highBits;
                D_801F2A08 = lowBits;
                bits = bits & ~lowBits;
                D_801F2A0C = highBits;
                D_8009E670 = bits;
                bits = D_8009E674;
                cond = g_SndVoiceCount;
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
