#include "common.h"
#include "psyq/cd.h"
#include "psyq/kernel.h"
void StCdInterrupt(void);


extern long g_StBackFrame asm("D_8009DF18");
extern u_char g_StBackLoc[] asm("D_8009DF14");
extern long D_8009E69C;
extern u_char *D_80099360;
extern u_char *D_8009936C;
extern long g_StNotStream2Mode asm("D_8019C7A0");
extern short D_8019C790;
extern long g_StColorMode asm("D_8019C79C");
extern StCallback g_StFrameCallback asm("D_8019C994");
extern long D_8019C9A0;
extern long D_801E3E08;
extern long D_801E4190;
extern long g_StDmaBusy asm("D_801E42C8");
extern long D_801E6C74;
extern long D_801E6C84;
extern long g_StRingSlot asm("D_801E6C98");
extern StStrHeader *g_StRingBase asm("D_801E8AAC");
extern long D_801E8274;
extern long g_StRingSize asm("D_801F1850");
extern long g_StInterruptPending asm("D_8019CA00");

long CdControl(long com, void *param, long result);
void CdDataCallback(long arg0);
void func_8006A58C(long arg0);
void CdRead2Callback(void);
void data_ready_callback(void);
void StClearRingRange(long arg0, long arg1) asm("func_8006D0AC");
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
    D_801E6C84 = 0;
    D_801E6C74 = 0;
    g_StDmaBusy = 0;
    StClearRingRange(0, g_StRingSize);
    g_StInterruptPending = 0;
    D_8019C790 = 0;
    D_8009E69C = 0;
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
    long index = D_801E6C84;
    StStrHeader *base = g_StRingBase;
    StStrHeader *entry;

    entry = (StStrHeader *)((index * 32) + (long)base);
    /* RAW() keeps this store ahead of the g_StBack* writes -- see common.h. */
    RAW(entry->state) = 2;
    *(CdlLOC *)g_StBackLoc = entry->loc;
    /* +0x08 is nFrames; this path reads it as a whole word. */
    g_StBackFrame = *(long *)&entry->nFrames;
    D_801E6C84 = D_801E6C74;
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
    D_801E8274 = 0;
    g_StFrameCallback = (StCallback)callback;
    g_StColorMode = mode & 1;
    D_801E4190 = 0;
    D_801E3E08 = 0;
    D_8019C790 = 0;
    D_8009E69C = 0;
    D_8019C9A0 = user_data;
}
