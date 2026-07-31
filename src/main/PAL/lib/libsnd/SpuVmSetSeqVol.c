#include <sys/types.h>

#include "common.h"

extern u_char *g_SndSeqTable[] asm("D_801E79CC");
extern short g_SndCurrentSeqSep asm("D_801E4BE6");
extern volatile u_char D_801E42F8;
extern u_char g_SndVoiceRegs[] asm("D_8009DF20");
extern volatile u_char g_SndVoiceFlags[] asm("D_8009E0A0");
extern u_char g_SndVoiceStateSeqSep[] asm("D_8009E0C6");

short SpuVmSetSeqVol() asm("func_80076C58");
long SpuVmGetSeqVol(long arg0, short *arg1, short *arg2) asm("func_80076DCC");
short SpuVmGetSeqVolLeft(long arg0) asm("func_80076E38");
short SpuVmGetSeqVolRight(long arg0) asm("func_80076E88");

short SpuVmSetSeqVol(arg0, arg1, arg2, arg3)
short arg0;
u_short arg1;
u_short arg2;
short arg3;
{
    short i;
    register u_char *base;
    u_char *entry;
    long x;
    long y;
    long temp;
    short pos;
    u_long offset;
    u_long index;

    base = *(u_char **)((u_char *)g_SndSeqTable + ((arg0 & 0xFF) << 2));
    g_SndCurrentSeqSep = arg0;
    index = (arg0 & 0xFF00) >> 8;
    entry = (u_char *)((index * 0xAC) + (long)base);

    *(u_short *)(entry + 0x74) = arg1;
    offset = *(u_short *)(entry + 0x74);
    *(u_short *)(entry + 0x76) = arg2;
    temp = arg0;
    if (offset >= 0x80) {
        *(u_short *)(entry + 0x74) = 0x7F;
    }

    offset = *(u_short *)(entry + 0x76);
    if (offset >= 0x80) {
        *(u_short *)(entry + 0x76) = 0x7F;
    }

    x = arg1 * 0x81;
    y = arg2 * 0x81;

    if (arg3 == 1) {
        for (i = 0; i < D_801E42F8; i++) {
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

long SpuVmGetSeqVol(long arg0, short *arg1, short *arg2) {
    long index;
    long offset;
    u_char *base;
    u_char *ptr;
    short *status;

    status = &g_SndCurrentSeqSep;
    offset = (arg0 & 0xFF) << 2;
    base = *(u_char **)((u_char *)g_SndSeqTable + offset);
    *status = arg0;
    index = (arg0 & 0xFF00) >> 8;

    ptr = (u_char *)((index * 0xAC) + (long)base);
    *arg1 = *(u_short *)(ptr + 0x74);
    *arg2 = *(u_short *)(ptr + 0x76);
    return *status;
}

short SpuVmGetSeqVolLeft(long arg0) {
    long index;
    long offset;
    u_char *ptr;

    offset = (arg0 & 0xFF) << 2;
    ptr = *(u_char **)((u_char *)g_SndSeqTable + offset);
    g_SndCurrentSeqSep = arg0;
    index = (arg0 & 0xFF00) >> 8;

    return *(short *)(ptr + (index * 0xAC) + 0x74);
}

short SpuVmGetSeqVolRight(long arg0) {
    long index;
    long offset;
    u_char *ptr;

    offset = (arg0 & 0xFF) << 2;
    ptr = *(u_char **)((u_char *)g_SndSeqTable + offset);
    g_SndCurrentSeqSep = arg0;
    index = (arg0 & 0xFF00) >> 8;

    return *(short *)(ptr + (index * 0xAC) + 0x76);
}
