#include "common.h"
#include "psyq/cd.h"
#include "psyq/kernel.h"
void StCdInterrupt(void);


extern long g_StBackFrame;
extern u_char g_StBackLoc[];
extern long g_StCurrentFrameCount;
extern u_char *D_80099360;
extern u_char *D_8009936C;
extern long g_StNotStream2Mode;
extern short g_StCurrentSector;
extern long g_StColorMode;
extern StCallback g_StFrameCallback;
extern long g_StEndCallback;
extern long g_StNextChannel;
extern long g_StCurrentChannel;
extern long g_StDmaBusy;
extern long g_StWriteCursor;
extern long g_StReadCursor;
extern long g_StRingSlot;
extern StStrHeader *g_StRingBase;
extern long g_StCopySource;
extern long g_StRingSize;
extern long g_StInterruptPending;

long CdControl(long com, void *param, long result);
void CdDataCallback(long arg0);
void func_8006A58C(long arg0);
void CdRead2Callback(void);
void data_ready_callback(void);
void StClearRingRange(long arg0, long arg1);
void StSetRingParams(long arg0);

/*
 * Issues CD command 0xE (set mode) with the mode byte `arg0`, and when the
 * data-ready bits (0x100) are set, installs the streaming data-ready callback
 * and the CDDA/sync handler, then issues command 0x1B (read-S). Returns the
 * second command's result.
 */
long CdRead2(long arg0);
long CdRead2(long arg0) {
    u_char byte;

    byte = arg0;
    CdControl(0xE, &byte, 0);

    if (arg0 & 0x100) {
        if (arg0 & 0x20) {
            g_StNotStream2Mode = 0;
        } else {
            g_StNotStream2Mode = 1;
        }
        CdDataCallback((long)data_ready_callback);
        func_8006A58C((long)CdRead2Callback);
    }

    return CdControl(0x1B, 0, 0);
}

void CdRead2Callback(void);
void CdRead2Callback(void) {
    StCdInterrupt();
}

void StClearRing(void) {
    g_StRingSlot = 0;
    g_StReadCursor = 0;
    g_StWriteCursor = 0;
    g_StDmaBusy = 0;
    StClearRingRange(0, g_StRingSize);
    g_StInterruptPending = 0;
    g_StCurrentSector = 0;
    g_StCurrentFrameCount = 0;
}

void StUnSetRing(void);
void StUnSetRing(void) {
    EnterCriticalSection();
    CdDataCallback(0);
    func_8006A58C(0);
    *D_80099360 = 0;
    *D_8009936C = 0;
    asm("" ::: "memory");
    ExitCriticalSection();
}

void data_ready_callback(void) {
    long index = g_StReadCursor;
    StStrHeader *base = g_StRingBase;
    StStrHeader *entry;

    entry = (StStrHeader *)((index * 32) + (long)base);
    /* RAW() keeps this store ahead of the g_StBack* writes -- see common.h. */
    RAW(entry->state) = 2;
    *(CdlLOC *)g_StBackLoc = entry->loc;
    /* +0x08 is nFrames; this path reads it as a whole word. */
    g_StBackFrame = *(long *)&entry->nFrames;
    g_StReadCursor = g_StWriteCursor;
    if (g_StFrameCallback != 0) {
        g_StFrameCallback();
    }
    g_StDmaBusy = 0;
}

long StGetBackloc(CdlLOC *arg0) {
    if (g_StNotStream2Mode != 0) {
        return -1;
    }
    CdIntToPos(CdPosToInt_Local((CdlLOC *)g_StBackLoc) + 1, arg0);
    return g_StBackFrame;
}

/* Arms CD streaming: installs the data-ready `callback`/`user_data` and mode
 * (bit 0 = one-shot vs looping). start_frame/end_frame bound the stream. */
void StSetStream(long mode, long start_frame, long end_frame, long callback, long user_data) {
    StSetRingParams(1);
    g_StCopySource = 0;
    g_StFrameCallback = (StCallback)callback;
    g_StColorMode = mode & 1;
    g_StCurrentChannel = 0;
    g_StNextChannel = 0;
    g_StCurrentSector = 0;
    g_StCurrentFrameCount = 0;
    g_StEndCallback = user_data;
}
