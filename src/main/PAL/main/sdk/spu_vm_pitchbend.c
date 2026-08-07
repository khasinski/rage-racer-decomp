#include <sys/types.h>
#include "common.h"
#include "psyq/snd.h"

extern u_char g_SndVoiceStateNote[];
extern u_char g_SndVoiceStateProg[];
extern u_char g_SndVoiceStateTone[];
extern u_char g_SndVoiceStateVabId[];
extern u_char g_SndVoiceRegsPitch[];
extern u_char g_SndVoiceFlags[];
extern u_char * g_SndCurrentToneTable;
extern u_short g_SndCurrentVoice;

long SpuVmApplyPitchBendToVoice(long arg0, long arg1, long arg2, long arg3, long arg5) {
    register long raw asm("$10") = arg0;
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

    __asm__("" : "=r"(a4) : "0"(arg0), "r"(value));
    i = (short)a4;
    t1 = arg5 + value;
    off = ((((i * 3) * 4) + i) * 4);

    if (*(short *)&g_SndVoiceStateSeqSep[off] == (short)arg1 &&
        *(short *)&g_SndVoiceStateVabId[off] == (short)arg2 &&
        *(short *)&g_SndVoiceStateProg[off] == (short)arg3) {

        t = *(u_short *)&g_SndVoiceStateTone[off] + (g_SndCurrentProgActual * 16);
        f0 = *(u_short *)&g_SndVoiceStateNote[off];
        w = (short)t1;

        if (w > 0) {
            i = w * (u_char)g_SndCurrentToneTable[(((u_short)t) << 5) + 0xD];
            value = i / 63;
            base = f0 + value;
            bal = (i - value * 63) << 1;
        } else if (w < 0) {
            i = w * (u_char)g_SndCurrentToneTable[(((u_short)t) << 5) + 0xC];
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
        c = *(u_char *)&g_SndVoiceStateTone[off2];
        g_SndCurrentVoice = raw;
        g_SndCurrentTone = c;
        ret = SpuVmCalculateTonePitch((u_short)base, (u_short)bal);
        *(short *)&g_SndVoiceRegsPitch[j * 16] = ret;
        g_SndVoiceFlags[j] |= 4;
        return 1;
    }
    return 0;
}

extern volatile u_char g_SndVoiceCount;
extern short g_SndCurrentSeqSep;

long SpuVmApplyPitchBendByTone(long arg0, long arg1, long arg2, long arg3) {
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

    voice = arg0;
    tmp = arg1;
    call_x = (short)tmp;
    call_y = (short)arg2;
    x = tmp;
    y = arg2;
    extra = arg3;

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
