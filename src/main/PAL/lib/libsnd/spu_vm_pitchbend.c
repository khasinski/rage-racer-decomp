#include <sys/types.h>
#include "common.h"
#include "psyq/snd.h"

#define SND_VOICE_COUNT_QUALIFIER volatile
#include "psyq/snd_internal.h"

long SpuVmApplyPitchBendToVoice(long voice, long note, long vab_id, long program, long bend) {
    register long raw asm("$10") = voice;
    register long value asm("$2") = 0xFFC0;
    long a4;
    register long i asm("$4");
    long t1;
    long off;
    long w, t, rem, f0;
    long bal;
    long base;
    long j, off2;
    u_char c;
    long ret;

    __asm__("" : "=r"(a4) : "0"(voice), "r"(value));
    i = (short)a4;
    t1 = bend + value;
    off = ((((i * 3) * 4) + i) * 4);

    if (*(short *)&((u_char *)g_SndVoiceState + 14)[off] == (short)note &&
        *(short *)&((u_char *)g_SndVoiceState + 22)[off] == (short)vab_id &&
        *(short *)&((u_char *)g_SndVoiceState + 18)[off] == (short)program) {

        t = *(u_short *)&((u_char *)g_SndVoiceState + 20)[off] + (g_SndCurrentProgActual * 16);
        f0 = *(u_short *)&((u_char *)g_SndVoiceState + 12)[off];
        w = (short)t1;

        if (w > 0) {
            i = w * ((u_char *)g_SndCurrentToneTable)[(((u_short)t) << 5) + 0xD];
            value = i / 63;
            base = f0 + value;
            bal = (i - value * 63) << 1;
        } else if (w < 0) {
            i = w * ((u_char *)g_SndCurrentToneTable)[(((u_short)t) << 5) + 0xC];
            value = i;
            if (i < 0) value = i + 0x3F;
            value = value >> 6;
            base = f0 + value - 1;
            value = i - value * 64;
            bal = (value << 1) + 0x7F;
        } else {
            base = f0;
            bal = 0;
        }

        j = (short)raw;
        off2 = ((((j * 3) * 4) + j) * 4);
        c = *(u_char *)&((u_char *)g_SndVoiceState + 20)[off2];
        g_SndCurrentVoice = raw;
        g_SndCurrentTone = c;
        ret = SpuVmCalculateTonePitch((u_short)base, (u_short)bal);
        *(short *)&((u_char *)g_SndVoiceRegs + 4)[j * 16] = ret;
        g_SndVoiceFlags[j] |= 4;
        return 1;
    }
    return 0;
}


long SpuVmApplyPitchBendByTone(long note, long vab_id, long program, long bend) {
    long voice;
    long x;
    long y;
    long extra;
    long i;
    long sum;
    long bound;
    register long tmp asm("$2");
    long call_x;
    long call_y;

    voice = note;
    tmp = vab_id;
    call_x = (short)tmp;
    call_y = (short)program;
    x = tmp;
    y = program;
    extra = bend;

    SpuVmVSetUp(call_x, call_y);
    i = 0;
    sum = 0;
    bound = g_SndVoiceCount;
    __asm__ volatile("");
    tmp = voice;
    g_SndCurrentSeqSep = tmp;

    if ((short)i < bound) {
        voice <<= 16;
        do {
            sum += (short)SpuVmApplyPitchBendToVoice((short)i, (short)(voice >> 16), (short)x, (short)y, (u_short)extra);
            tmp = i + 1;
            i = tmp;
            __asm__("" : "=r"(i) : "0"(i));
        } while ((short)tmp < (bound = g_SndVoiceCount));
    }

    return sum;
}
