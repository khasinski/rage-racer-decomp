#include <sys/types.h>

#include "common.h"
#include "psyq/snd.h"
#include "game/audio.h"

extern u_char *g_SndSeqTable[];
extern short g_SndCurrentSeqSep;
extern volatile u_char g_SndVoiceCount;
extern u_char g_SndVoiceRegs[];

short SpuVmSetSeqVol(short seq_sep, u_short left, u_short right, short mode) {
    short i;
    u_char *base;
    u_char *entry;
    long x;
    long y;
    long temp;
    short pos;
    u_long offset;
    u_long index;

    base = *(u_char **)((u_char *)g_SndSeqTable + ((seq_sep & 0xFF) << 2));
    g_SndCurrentSeqSep = seq_sep;
    index = (seq_sep & 0xFF00) >> 8;
    entry = (u_char *)((index * 0xAC) + (long)base);

    *(u_short *)(entry + 0x74) = left;
    offset = *(u_short *)(entry + 0x74);
    *(u_short *)(entry + 0x76) = right;
    temp = seq_sep;
    if (offset >= 0x80) {
        *(u_short *)(entry + 0x74) = 0x7F;
    }

    offset = *(u_short *)(entry + 0x76);
    if (offset >= 0x80) {
        *(u_short *)(entry + 0x76) = 0x7F;
    }

    x = left * 0x81;
    y = right * 0x81;

    if (mode == 1) {
        for (i = 0; i < g_SndVoiceCount; i++) {
            offset = (i * 12 + i) * 4;
            if (*(u_short *)&g_SndVoiceStateSeqSep[offset] == (u_short)temp) {
                pos = i * 8;
                ((u_short *)g_SndVoiceRegs)[pos] = x;
                ((u_short *)g_SndVoiceRegs)[pos + 1] = y;
                g_SndVoiceFlags[i] |= 3;
            }
        }
    }
    return g_SndCurrentSeqSep;
}

long SpuVmGetSeqVol(long seq_sep, short *volLeft, short *volRight) {
    long index;
    long offset;
    u_char *base;
    u_char *ptr;
    short *status;

    status = &g_SndCurrentSeqSep;
    offset = (seq_sep & 0xFF) << 2;
    base = *(u_char **)((u_char *)g_SndSeqTable + offset);
    *status = seq_sep;
    index = (seq_sep & 0xFF00) >> 8;

    ptr = (u_char *)((index * 0xAC) + (long)base);
    *volLeft = *(u_short *)(ptr + 0x74);
    *volRight = *(u_short *)(ptr + 0x76);
    return *status;
}

short SpuVmGetSeqVolLeft(long seq_sep) {
    long index;
    long offset;
    u_char *ptr;

    offset = (seq_sep & 0xFF) << 2;
    ptr = *(u_char **)((u_char *)g_SndSeqTable + offset);
    g_SndCurrentSeqSep = seq_sep;
    index = (seq_sep & 0xFF00) >> 8;

    return *(short *)(ptr + (index * 0xAC) + 0x74);
}

short SpuVmGetSeqVolRight(long seq_sep) {
    long index;
    long offset;
    u_char *ptr;

    offset = (seq_sep & 0xFF) << 2;
    ptr = *(u_char **)((u_char *)g_SndSeqTable + offset);
    g_SndCurrentSeqSep = seq_sep;
    index = (seq_sep & 0xFF00) >> 8;

    return *(short *)(ptr + (index * 0xAC) + 0x76);
}
