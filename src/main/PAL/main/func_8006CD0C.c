#include "common.h"
#include "psyq/cd.h"
#include "psyq/kernel.h"

typedef void (*Callback)(void);

typedef struct {
    s16 state;
    u8 pad2[6];
    s32 value;
    u8 padC[0x10];
    u8 tail[4];
} Entry;

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
extern Entry *D_801E8AAC;
extern s32 D_801E8274;
extern s32 D_801F1850;
extern s32 D_8019CA00;

s32 func_8006A5A4(s32 arg0, void *arg1, s32 arg2);
void func_8006A994(s32 arg0);
void func_8006A58C(s32 arg0);
void func_8006CDA0(void);
void data_ready_callback(void) asm("func_8006CE78");
void func_8006D0AC(s32 arg0, s32 arg1);
void func_8006D1B0(s32 arg0);
void func_8006D1D0(void);

s32 func_8006CD0C(s32 arg0) {
    u8 byte;

    byte = arg0;
    func_8006A5A4(0xE, &byte, 0);

    if (arg0 & 0x100) {
        if (arg0 & 0x20) {
            D_8019C7A0 = 0;
        } else {
            D_8019C7A0 = 1;
        }
        func_8006A994((s32)data_ready_callback);
        func_8006A58C((s32)func_8006CDA0);
    }

    return func_8006A5A4(0x1B, 0, 0);
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
    register Entry *base asm("$3") = D_801E8AAC;
    register Entry *entry asm("$2");

    entry = (Entry *)((index << 5) + (s32)base);
    entry->state = 2;
    __asm__ volatile(
        "lui    $6,%%hi(D_8009DF14)\n"
        "addiu  $6,$6,%%lo(D_8009DF14)\n"
        "lwl    $3,0x1F(%0)\n"
        "lwr    $3,0x1C(%0)\n"
        "nop\n"
        "swl    $3,0x3($6)\n"
        "swr    $3,0x0($6)"
        :
        : "r"(entry)
        : "$3", "$6", "memory");
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

void StSetStream(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_8006D1B0(1);
    D_801E8274 = 0;
    D_8019C994 = (Callback)arg3;
    D_8019C79C = arg0 & 1;
    D_801E4190 = 0;
    D_801E3E08 = 0;
    D_8019C790 = 0;
    D_8009E69C = 0;
    D_8019C9A0 = arg4;
}
