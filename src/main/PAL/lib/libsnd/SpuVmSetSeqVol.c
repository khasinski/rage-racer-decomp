#include <sys/types.h>

#include "common.h"

extern u_char *g_SndSeqTable[] asm("D_801E79CC");
extern short g_SndCurrentSeqSep asm("D_801E4BE6");
extern volatile u_char D_801E42F8;
extern u_char g_SndVoiceRegs[] asm("D_8009DF20");
extern volatile u_char g_SndVoiceFlags[] asm("D_8009E0A0");
extern u_char g_SndVoiceStateSeqSep[] asm("D_8009E0C6");

short SpuVmSetSeqVol(long arg0, u_short arg1, u_short arg2, short arg3) asm("func_80076C58");
long SpuVmGetSeqVol(long arg0, short *arg1, short *arg2) asm("func_80076DCC");
short SpuVmGetSeqVolLeft(long arg0) asm("func_80076E38");
short SpuVmGetSeqVolRight(long arg0) asm("func_80076E88");

short SpuVmSetSeqVol(long arg0, u_short arg1, u_short arg2, short arg3) {
    register u_long offset asm("$2");
    register u_char *base asm("$8");
    register u_char *entry asm("$3");
    register u_long index asm("$3");
    register u_short x asm("$9");
    register u_short y asm("$8");

    offset = (arg0 & 0xFF) << 2;
    base = *(u_char **)((u_char *)g_SndSeqTable + offset);
    __asm__ volatile("addiu $sp,$sp,-8" ::: "memory");
    g_SndCurrentSeqSep = arg0;
    arg0 <<= 16;
    index = ((u_long)arg0) >> 24;
    offset = index * 0xAC;
    entry = (u_char *)(offset + (long)base);

    *(u_short *)(entry + 0x74) = arg1;
    offset = *(volatile u_short *)(entry + 0x74);
    arg0 >>= 16;
    *(u_short *)(entry + 0x76) = arg2;
    if (offset >= 0x80) {
        *(u_short *)(entry + 0x74) = 0x7F;
    }

    offset = *(volatile u_short *)(entry + 0x76);
    if (offset >= 0x80) {
        *(u_short *)(entry + 0x76) = 0x7F;
    }

    index = arg1 & 0xFFFF;
    offset = index << 7;
    x = offset + index;
    index = arg2 & 0xFFFF;
    offset = index << 7;
    y = offset + index;

    if (arg3 == 1) {
        register long i asm("$6");
        register u_char *dst asm("$7");
        register u_char *dstY asm("$10");
        register long signedIndex asm("$5");

        i = 0;
        if (D_801E42F8 != 0) {
            arg0 &= 0xFFFF;
            dst = g_SndVoiceRegs;
            dstY = dst + 2;
            do {
                offset = i << 16;
                signedIndex = (long)offset >> 16;
                offset = signedIndex * 3;
                offset *= 4;
                offset += signedIndex;
                offset *= 4;

                if (*(u_short *)&g_SndVoiceStateSeqSep[offset] != arg0) {
                    offset = i + 1;
                } else {
                    offset = signedIndex << 19;
                    offset = (long)offset >> 15;
                    *(u_short *)(offset + (long)dst) = x;
                    *(u_short *)(offset + (long)dstY) = y;
                    offset = g_SndVoiceFlags[signedIndex];
                    offset |= 3;
                    g_SndVoiceFlags[signedIndex] = offset;
                    offset = i + 1;
                }

                i = offset;
                __asm__("" : "=r"(offset) : "0"(offset));
            } while (((long)(offset << 16) >> 16) < D_801E42F8);
        }
    }

    offset = g_SndCurrentSeqSep;
    __asm__ volatile("addiu $sp,$sp,8" ::: "memory");
    return offset;
}

long SpuVmGetSeqVol(long arg0, short *arg1, short *arg2) {
    long index;
    register long offset asm("$2");
    register u_char *base asm("$3");
    register u_char *ptr asm("$2");
    register short *status asm("$7");

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
    register long offset asm("$2");
    register u_char *ptr asm("$3");

    offset = (arg0 & 0xFF) << 2;
    ptr = *(u_char **)((u_char *)g_SndSeqTable + offset);
    g_SndCurrentSeqSep = arg0;
    index = (arg0 & 0xFF00) >> 8;

    return *(short *)(ptr + (index * 0xAC) + 0x74);
}

short SpuVmGetSeqVolRight(long arg0) {
    long index;
    register long offset asm("$2");
    register u_char *ptr asm("$3");

    offset = (arg0 & 0xFF) << 2;
    ptr = *(u_char **)((u_char *)g_SndSeqTable + offset);
    g_SndCurrentSeqSep = arg0;
    index = (arg0 & 0xFF00) >> 8;

    return *(short *)(ptr + (index * 0xAC) + 0x76);
}
