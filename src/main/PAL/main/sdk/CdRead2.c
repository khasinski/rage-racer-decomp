#include "common.h"
#include "psyq/cd.h"
#include "psyq/kernel.h"

typedef void (*Callback)(void);

extern long D_8009DF18;
extern u_char D_8009DF14[];
extern long D_8009E69C;
extern u_char *D_80099360;
extern u_char *D_8009936C;
extern long D_8019C7A0;
extern short D_8019C790;
extern long D_8019C79C;
extern Callback D_8019C994;
extern long D_8019C9A0;
extern long D_801E3E08;
extern long D_801E4190;
extern long D_801E42C8;
extern long D_801E6C74;
extern long D_801E6C84;
extern long D_801E6C98;
extern StRingEntry *D_801E8AAC;
extern long D_801E8274;
extern long D_801F1850;
extern long D_8019CA00;

long CdControl(long com, void *param, long result) asm("func_8006A5A4");
void CdDataCallback(long arg0) asm("func_8006A994");
void func_8006A58C(long arg0);
void func_8006CDA0(void);
void data_ready_callback(void) asm("func_8006CE78");
void func_8006D0AC(long arg0, long arg1);
void func_8006D1B0(long arg0);
void func_8006D1D0(void);

/*
 * Issues CD command 0xE (set mode) with the mode byte `arg0`, and when the
 * data-ready bits (0x100) are set, installs the streaming data-ready callback
 * and the CDDA/sync handler, then issues command 0x1B (read-S). Returns the
 * second command's result.
 */
long CdRead2(long arg0) asm("func_8006CD0C");
long CdRead2(long arg0) {
    u_char byte;

    byte = arg0;
    CdControl(0xE, &byte, 0);

    if (arg0 & 0x100) {
        if (arg0 & 0x20) {
            D_8019C7A0 = 0;
        } else {
            D_8019C7A0 = 1;
        }
        CdDataCallback((long)data_ready_callback);
        func_8006A58C((long)func_8006CDA0);
    }

    return CdControl(0x1B, 0, 0);
}

void func_8006CDA0(void) {
    func_8006D1D0();
}

void StClearRing(void) {
    D_801E6C98 = 0;
    D_801E6C84 = 0;
    D_801E6C74 = 0;
    D_801E42C8 = 0;
    func_8006D0AC(0, D_801F1850);
    D_8019CA00 = 0;
    D_8019C790 = 0;
    D_8009E69C = 0;
}

void StUnSetRing(void) asm("func_8006CE20");
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
    register long index asm("$2") = D_801E6C84;
    register StRingEntry *base asm("$3") = D_801E8AAC;
    register StRingEntry *entry asm("$2");

    entry = (StRingEntry *)((index << 5) + (long)base);
    entry->state = 2;
    *(CdlLOC *)D_8009DF14 = entry->loc;
    __asm__ volatile("" ::: "memory");
    D_8009DF18 = entry->value;
    D_801E6C84 = D_801E6C74;
    if (D_8019C994 != 0) {
        D_8019C994();
    }
    D_801E42C8 = 0;
}

long StGetBackloc(CdlLOC *arg0) {
    if (D_8019C7A0 != 0) {
        return -1;
    }
    CdIntToPos(CdPosToInt_Local((CdlLOC *)D_8009DF14) + 1, arg0);
    return D_8009DF18;
}

/* Arms CD streaming: installs the data-ready `callback`/`user_data` and mode
 * (bit 0 = one-shot vs looping). start_frame/end_frame bound the stream. */
void StSetStream(long mode, long start_frame, long end_frame, long callback, long user_data) {
    func_8006D1B0(1);
    D_801E8274 = 0;
    D_8019C994 = (Callback)callback;
    D_8019C79C = mode & 1;
    D_801E4190 = 0;
    D_801E3E08 = 0;
    D_8019C790 = 0;
    D_8009E69C = 0;
    D_8019C9A0 = user_data;
}
