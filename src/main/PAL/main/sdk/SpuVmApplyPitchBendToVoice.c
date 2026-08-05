#include <sys/types.h>

#include "common.h"

extern u_char g_SndVoiceStateNote[];
extern u_char g_SndVoiceStateSeqSep[];
extern u_char g_SndVoiceStateProg[];
extern u_char g_SndVoiceStateTone[];
extern u_char g_SndVoiceStateVabId[];
extern u_char g_SndVoiceRegsPitch[];
extern u_char g_SndVoiceFlags[];
extern u_char g_SndCurrentProgActual;
extern u_char *g_SndCurrentToneTable;
extern u_short g_SndCurrentVoice;
extern u_char g_SndCurrentTone;

long SpuVmCalculateTonePitch(long arg0, long arg1);

long SpuVmApplyPitchBendToVoice(long arg0, long arg1, long arg2, long arg3, long arg5) {
    register long raw asm("$10") = arg0;
    register long cst asm("$2") = 0xFFC0;
    long a4;
    register long i asm("$4");
    long t1;
    long off;
    long w, t, rem, f0;
    register long prod asm("$4");
    register long q asm("$2");
    long bal;
    long base;
    long j, off2;
    u_char c;
    long ret;

    __asm__("" : "=r"(a4) : "0"(arg0), "r"(cst));
    i = (short)a4;
    t1 = arg5 + cst;
    off = ((((i * 3) * 4) + i) * 4);

    if (*(short *)&g_SndVoiceStateSeqSep[off] == (short)arg1 &&
        *(short *)&g_SndVoiceStateVabId[off] == (short)arg2 &&
        *(short *)&g_SndVoiceStateProg[off] == (short)arg3) {

        t = *(u_short *)&g_SndVoiceStateTone[off] + (g_SndCurrentProgActual * 16);
        f0 = *(u_short *)&g_SndVoiceStateNote[off];
        w = (short)t1;

        if (w > 0) {
            prod = w * (u_char)g_SndCurrentToneTable[(((u_short)t) << 5) + 0xD];
            q = prod / 63;
            base = f0 + q;
            bal = (prod - q * 63) << 1;
        } else if (w < 0) {
            prod = w * (u_char)g_SndCurrentToneTable[(((u_short)t) << 5) + 0xC];
            q = prod;
            if (prod < 0) q = prod + 0x3F;
            q = q >> 6;
            base = f0 + q - 1;
            {
                register long r asm("$2");
                r = prod - q * 64;
                bal = (r << 1) + 0x7F;
            }
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
