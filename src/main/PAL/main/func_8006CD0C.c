#include "common.h"
#include "psyq/cd.h"
#include "psyq/kernel.h"

typedef void (*Callback)(void);

extern s32 D_8009DF18;
extern u8 D_8009DF14[];
extern s32 D_8009E69C;
extern u8 *D_80099360;
extern u8 *D_8009936C;
extern s32 D_8019C7A0;
extern s16 D_8019C790;
extern s32 D_8019C79C;
extern Callback D_8019C994;
extern s32 D_8019C9A0;
extern s32 D_801E3E08;
extern s32 D_801E4190;
extern s32 D_801E42C8;
extern s32 D_801E6C74;
extern s32 D_801E6C84;
extern s32 D_801E6C98;
extern StRingEntry *D_801E8AAC;
extern s32 D_801E8274;
extern s32 D_801F1850;
extern s32 D_8019CA00;

s32 CdControl(s32 com, void *param, s32 result) asm("func_8006A5A4");
void func_8006A994(s32 arg0);
void func_8006A58C(s32 arg0);
void func_8006CDA0(void);
void data_ready_callback(void) asm("func_8006CE78");
void func_8006D0AC(s32 arg0, s32 arg1);
void func_8006D1B0(s32 arg0);
void func_8006D1D0(void);

/*
 * Issues CD command 0xE (set mode) with the mode byte `arg0`, and when the
 * data-ready bits (0x100) are set, installs the streaming data-ready callback
 * and the CDDA/sync handler, then issues command 0x1B (read-S). Returns the
 * second command's result.
 */
s32 func_8006CD0C(s32 arg0) {
    u8 byte;

    byte = arg0;
    CdControl(0xE, &byte, 0);

    if (arg0 & 0x100) {
        if (arg0 & 0x20) {
            D_8019C7A0 = 0;
        } else {
            D_8019C7A0 = 1;
        }
        func_8006A994((s32)data_ready_callback);
        func_8006A58C((s32)func_8006CDA0);
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

void func_8006CE20(void) {
    EnterCriticalSection();
    func_8006A994(0);
    func_8006A58C(0);
    *D_80099360 = 0;
    *D_8009936C = 0;
    asm("" ::: "memory");
    ExitCriticalSection();
}

void data_ready_callback(void) {
    register s32 index asm("$2") = D_801E6C84;
    register StRingEntry *base asm("$3") = D_801E8AAC;
    register StRingEntry *entry asm("$2");

    entry = (StRingEntry *)((index << 5) + (s32)base);
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

s32 StGetBackloc(CdlLOC *arg0) {
    if (D_8019C7A0 != 0) {
        return -1;
    }
    CdIntToPos(CdPosToInt_Local((CdlLOC *)D_8009DF14) + 1, arg0);
    return D_8009DF18;
}

/* Arms CD streaming: installs the data-ready `callback`/`user_data` and mode
 * (bit 0 = one-shot vs looping). start_frame/end_frame bound the stream. */
void StSetStream(s32 mode, s32 start_frame, s32 end_frame, s32 callback, s32 user_data) {
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
