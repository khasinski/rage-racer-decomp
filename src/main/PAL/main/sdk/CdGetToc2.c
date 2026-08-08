#include <sys/types.h>

#include "common.h"

extern long g_CdDebugLevel;
extern u_char D_80013678[];
extern u_char D_80013688[];
extern u_char D_800136A4[];
extern u_char D_800136B8[];

long CD_init(long mode);
long CdSyncCallback(long callback);
long CdReadyCallback(long callback);
long CdControlB(long com, u_char *param, u_char *result);
void CdDefaultSyncCallback(void);
void CdDefaultReadyCallback(void);
void CdDefaultReadCallback(void);
void DeliverEvent(u_long event, long spec);
void CdReadCallback(void *arg0);

long CdGetToc2(long arg0, u_char *arg1) {
    u_char *fmt;
    u_char command[8];
    u_char response[8];
    u_char *toc = arg1;
    long oldHandler;
    long firstTrack;
    register u_char *ptr asm("$17");
    long count;
    long lastTrack;
    u_long value;

    command[0] = 1;
    oldHandler = CdSyncCallback(0);

    if (CdControlB(0x13, 0, response) == 0) {
    } else {

    {
        u_char firstBcd = response[1];
        u_char lastBcd = response[2];

        firstTrack = (firstBcd >> 4) * 10 + (firstBcd & 0xF);
        lastTrack = (lastBcd >> 4) * 10 + (lastBcd & 0xF);
    }

    if (g_CdDebugLevel >= 2) {
        DebugPrintf(D_80013678, firstTrack, lastTrack);
    }

    command[0] = 0;
    if (CdControlB(0x14, command, response) == 0) {
    } else {

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
    if (g_CdDebugLevel != 0) {
        DebugPrintf(D_800136A4);
    }
    CdSyncCallback(oldHandler);
    return 0;
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
            u_long first;
            u_long second;

            count = 0;
            entry = toc;
            do {
                fmt = D_80013688;
                first = entry[0];
                second = entry[1];
                DebugPrintf(fmt, first, second);
                entry += 4;
                count++;
            } while (count <= (long)ptr);
        }
    }

    CdSyncCallback(oldHandler);
    return (long)ptr;

    }
    }
    if (g_CdDebugLevel != 0) {
        DebugPrintf(D_800136A4);
    }
    CdSyncCallback(oldHandler);
    return 0;
}

long CdInit(void) {
    long retries;

    retries = 4;
    while (CD_init(1) != 1) {
        retries--;
        if (retries == -1) {
            DebugPrintf(D_800136B8);
            return 0;
        }
    }

    CdSyncCallback((long)CdDefaultSyncCallback);
    CdReadyCallback((long)CdDefaultReadyCallback);
    CdReadCallback(CdDefaultReadCallback);
    return 1;
}

void CdDefaultSyncCallback(void) {
    DeliverEvent(0xF0000003, 0x20);
}

void CdDefaultReadyCallback(void) {
    DeliverEvent(0xF0000003, 0x40);
}

void CdDefaultReadCallback(void) {
    DeliverEvent(0xF0000003, 0x40);
}
