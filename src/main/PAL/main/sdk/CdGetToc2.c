#include <sys/types.h>

#include "common.h"

extern long g_CdDebugLevel asm("D_80099048");
extern u_char D_80013678[];
extern u_char D_80013688[];
extern u_char D_800136A4[];
extern u_char D_800136B8[];

long func_8006A428(long arg0);
long func_8006A574(long arg0);
long func_8006A58C(long arg0);
long func_8006A808(long arg0, u_char *arg1, u_char *arg2);
void func_8006A360(void);
void func_8006A388(void);
void func_8006A3B0(void);
void DeliverEvent(u_long arg0, long arg1) asm("func_8006A3D8");
void CdReadCallback(void *arg0) asm("func_8002785C");

long CdGetToc2(long arg0, u_char *arg1) asm("func_8006A0AC");
long CdGetToc2(long arg0, u_char *arg1) {
    u_char command[8];
    u_char response[8];
    register u_char *toc asm("$20") = arg1;
    register long oldHandler asm("$21");
    register long firstTrack asm("$16");
    register u_char *ptr asm("$17");
    register long count asm("$18");
    register long lastTrack asm("$19");
    u_long value;

    asm("" : "=r"(toc) : "0"(toc));
    command[0] = 1;
    oldHandler = func_8006A574(0);

    if (func_8006A808(0x13, 0, response) == 0) {
        goto fail;
    }

    {
        register u_long firstBcd asm("$4");
        register u_long lastBcd asm("$5");
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
    if (func_8006A808(0x14, command, response) == 0) {
        goto fail;
    }

    value = response[1];
    asm volatile("");
    toc[0] = value;
    value = response[2];
    toc[2] = 0;
    toc[1] = value;
    count = 1;

    if (firstTrack <= lastTrack) {
        ptr = toc + 4;
        do {
            command[0] = ((firstTrack / 10) << 4) + (firstTrack % 10);
            if (func_8006A808(0x14, command, response) == 0) {
                goto fail;
            }
            count++;
            value = response[1];
            asm volatile("");
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
            register u_char *entry asm("$16");
            register u_char *fmt asm("$4");
            u_long first;
            u_long second;

            count = 0;
            asm("" : "=r"(count) : "0"(count));
            entry = toc;
            do {
                fmt = D_80013688;
                asm("" : "=r"(fmt) : "0"(fmt));
                first = entry[0];
                second = entry[1];
                entry += 4;
                count++;
                GameDebugPrintf(fmt, first, second);
            } while (count <= (long)ptr);
        }
    }

    func_8006A574(oldHandler);
    return (long)ptr;

fail:
    if (g_CdDebugLevel != 0) {
        GameDebugPrintf(D_800136A4);
    }
    func_8006A574(oldHandler);
    return 0;
}

long CdInit(void) asm("func_8006A2D0");
long CdInit(void) {
    long retries;

    retries = 4;
loop:
    if (func_8006A428(1) != 1) {
        retries--;
        if (retries != -1) {
            goto loop;
        }

        GameDebugPrintf(D_800136B8);
        return 0;
    }

    func_8006A574((long)func_8006A360);
    func_8006A58C((long)func_8006A388);
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
