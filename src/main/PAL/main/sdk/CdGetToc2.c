#include <sys/types.h>

#include "common.h"

extern long g_CdDebugLevel asm("D_80099048");
extern u_char D_80013678[];
extern u_char D_80013688[];
extern u_char D_800136A4[];
extern u_char D_800136B8[];

long CD_init(long arg0) asm("func_8006A428");
long CdSyncCallback(long arg0) asm("func_8006A574");
long CdReadyCallback(long arg0) asm("func_8006A58C");
long CdControlB(long arg0, u_char *arg1, u_char *arg2) asm("func_8006A808");
void func_8006A360(void);
void func_8006A388(void);
void func_8006A3B0(void);
void DeliverEvent(u_long arg0, long arg1) asm("func_8006A3D8");
void CdReadCallback(void *arg0) asm("func_8002785C");

long CdGetToc2(long arg0, u_char *arg1) asm("func_8006A0AC");
long CdGetToc2(long arg0, u_char *arg1) {
    u_char command[8];
    u_char response[8];
    u_char *toc = arg1;
    long oldHandler;
    long firstTrack;
    /* This pin is load-bearing: removing it changes .text. */
    register u_char *ptr asm("$17");
    long count;
    long lastTrack;
    u_long value;

    command[0] = 1;
    oldHandler = CdSyncCallback(0);

    if (CdControlB(0x13, 0, response) == 0) {
        goto fail;
    }

    {
        /* These pins are load-bearing: removing any one changes .text. */
        register u_long firstBcd asm("$4");
        u_long lastBcd;
        register u_long high asm("$3");

        firstBcd = response[1];
        lastBcd = response[2];
        high = firstBcd >> 4;
        value = high << 2;
        value += high;
        value <<= 1;
        firstBcd &= 0xF;
        firstTrack = value + firstBcd;
        value = lastBcd >> 4;
        high = value << 2;
        high += value;
        high <<= 1;
        lastBcd &= 0xF;
        lastTrack = high + lastBcd;
    }

    if (g_CdDebugLevel >= 2) {
        GameDebugPrintf(D_80013678, firstTrack, lastTrack);
    }

    command[0] = 0;
    if (CdControlB(0x14, command, response) == 0) {
        goto fail;
    }

    value = response[1];
    toc[0] = value;
    value = response[2];
    toc[2] = 0;
    toc[1] = value;
    count = 1;

    if (firstTrack <= lastTrack) {
        ptr = toc + 4;
        do {
            command[0] = ((firstTrack / 10) << 4) + (firstTrack % 10);
            if (CdControlB(0x14, command, response) == 0) {
                goto fail;
            }
            count++;
            value = response[1];
            ptr[0] = value;
            value = response[2];
            firstTrack++;
            ptr[2] = 0;
            ptr[1] = value;
            ptr += 4;
        } while (firstTrack <= lastTrack);
    }

    ptr = (u_char *)(count - 1);
    if (g_CdDebugLevel >= 2) {
        if ((long)ptr >= 0) {
            u_char *entry;
            /* This pin is load-bearing: removing it changes .text. */
            register u_char *fmt asm("$4");
            u_long first;
            u_long second;

            count = 0;
            entry = toc;
            do {
                fmt = D_80013688;
                /* This barrier is load-bearing: removing it changes .text. */
                asm("" : "=r"(fmt) : "0"(fmt));
                first = entry[0];
                second = entry[1];
                entry += 4;
                count++;
                GameDebugPrintf(fmt, first, second);
            } while (count <= (long)ptr);
        }
    }

    CdSyncCallback(oldHandler);
    return (long)ptr;

fail:
    if (g_CdDebugLevel != 0) {
        GameDebugPrintf(D_800136A4);
    }
    CdSyncCallback(oldHandler);
    return 0;
}

long CdInit(void) asm("func_8006A2D0");
long CdInit(void) {
    long retries;

    retries = 4;
loop:
    if (CD_init(1) != 1) {
        retries--;
        if (retries != -1) {
            goto loop;
        }

        GameDebugPrintf(D_800136B8);
        return 0;
    }

    CdSyncCallback((long)func_8006A360);
    CdReadyCallback((long)func_8006A388);
    CdReadCallback(func_8006A3B0);
    return 1;
}

void func_8006A360(void) {
    DeliverEvent(0xF0000003, 0x20);
}

void func_8006A388(void) {
    DeliverEvent(0xF0000003, 0x40);
}

void func_8006A3B0(void) {
    DeliverEvent(0xF0000003, 0x40);
}
