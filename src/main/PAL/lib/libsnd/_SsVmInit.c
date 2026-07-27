#include "psyq/snd.h"

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
extern u_char g_SndVoiceStateSeqSep[] asm("D_8009E0C6");
extern u_char g_SndVoiceStateProgActual[] asm("D_8009E0C8");
extern u_char g_SndVoiceStateProg[] asm("D_8009E0CA");
extern u_char g_SndVoiceStateTone[] asm("D_8009E0CC");
extern u_char g_SndVoiceStateStatus[] asm("D_8009E0D3");

void _SsVmInit(void) {
    /* This pin is load-bearing: removing it changes .text. */
    register long i asm("$8");
    long ff;
    long one;
    /* This pin is load-bearing: removing it changes .text. */
    register long index asm("$4");
    long offset;
    long shifted;
    /* These pins are load-bearing: removing any one changes .text. */
    register long eighteen asm("$3");
    u_long lowMask;
    register u_long highMask asm("$6");
    register u_long lowBits asm("$3");
    register u_long highBits asm("$4");
    long next;
    volatile u_short *spu;
    u_long bits;

    i = 0;
    if (D_801E42F8 != 0) {
        ff = 0xFF;
        one = 1;
        do {
            index = (short)i;
            offset = index * 0x34;
            eighteen = 0x18;
            shifted = index << 19;
            *(short *)&g_SndVoiceStateAge[offset] = eighteen;
            *(short *)&g_SndVoiceState[offset] = ff;
            g_SndVoiceStateStatus[offset] = 0;
            *(short *)&g_SndVoiceStatePitch[offset] = 0;
            *(short *)&g_SndVoiceStateEnvx[offset] = 0;
            *(short *)&g_SndVoiceStateSeqSep[offset] = ff;
            *(short *)&g_SndVoiceStateProgActual[offset] = 0;
            *(short *)&g_SndVoiceStateProg[offset] = 0;
            *(short *)&g_SndVoiceStateTone[offset] = ff;

            shifted = shifted >> 15;
            spu = (volatile u_short *)(shifted + (long)g_SndSpuRegs);
            spu[3] = 0x200;
            spu[2] = 0x1000;
            spu[4] = 0x80FF;
            spu[0] = 0;
            spu[1] = 0;
            spu[5] = 0x4000;

            g_SndCurrentVoice = i;
            lowBits = g_SndCurrentVoice;
            /* This barrier is load-bearing: it hides the halfword load, whose
             * known-zero high bits would otherwise let gcc drop the mask. */
            asm("" : "=r"(lowBits) : "0"(lowBits));
            index = lowBits & 0xFFFF;
            if ((u_long)index < 0x10) {
                lowMask = one << index;
                highMask = 0;
            } else {
                lowMask = 0;
                offset = index - 0x10;
                highMask = one << offset;
            }

            next = i + 1;
            i = next;
            lowBits &= 0xFFFF;
            offset = lowBits * 0x34;
            g_SndVoiceStateStatus[offset] = 0;
            lowBits = D_801F2A08;
            highBits = D_801F2A0C;
            __asm__ volatile("" ::: "memory");
            next <<= 16;
            *(short *)&g_SndVoiceStatePitch[offset] = 0;
            *(short *)&g_SndVoiceState[offset] = 0;

            bits = D_8009E670;
            /* These barriers are load-bearing. Without them `combine` folds the
             * single-use `zero_extend(mem)` that defines the second operand into
             * the `ior`, tripping its "complex expression first" rule and
             * swapping the operands; retail keeps the written mask-first order. */
            asm volatile("");
            lowBits = lowMask | lowBits;
            D_801F2A08 = lowBits;
            D_8009E670 = bits & ~lowBits;
            bits = D_8009E674;
            asm("" : "=r"(highBits) : "0"(highBits));
            highBits = highMask | highBits;
            D_801F2A0C = highBits;
            D_8009E674 = bits & ~highBits;
        } while ((next >> 16) < D_801E42F8);
    }
}
