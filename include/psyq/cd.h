#ifndef PSYQ_CD_H
#define PSYQ_CD_H

#include <sys/types.h>

#include "common.h"

/* libcd's polling deadline: a wall-clock limit, the retries left, and the name
 * the timeout message prints. Was duplicated in four CD_*.c files. */
/* libcd sync/ready completion callback: status byte plus the 8-byte result. */
typedef void (*CdCallback)(u_char status, u_char *result);

/* libcd streaming per-frame callback: takes no arguments. */
typedef void (*StCallback)(void);

typedef struct CdAlarm {
    long deadline;
    long count;
    char *name;
} CdAlarm;

/* The three status bytes CD_ready/CD_sync poll. Was duplicated in two files. */
typedef struct CdIntr {
    u_char sync;
    u_char ready;
    u_char command;
} CdIntr;

typedef struct CdlLOC {
    u_char minute;
    u_char second;
    u_char sector;
    u_char track;
} CdlLOC;

typedef struct CdlATV {
    u_char val0;
    u_char val1;
    u_char val2;
    u_char val3;
} CdlATV;

typedef struct CdRegisterMap {
    u_char pad0[0x180];
    u_short cd_left_volume;
    u_short cd_right_volume;
    u_char pad184[0x1AA - 0x184];
    u_short audio_control;
    u_char pad1AC[0x1B0 - 0x1AC];
    u_short output_left_volume;
    u_short output_right_volume;
    u_char pad1B4[0x1B8 - 0x1B4];
    u_short status_mode_a;
    u_short status_mode_b;
} CdRegisterMap;

typedef struct StRingEventRecord {
    volatile u_short state;
    u_char pad2[0x1E];
} StRingEventRecord;

typedef struct StRingClearRecord {
    long value;
    u_char pad4[0x1C];
} StRingClearRecord;

/*
 * CD stream ring header (Rage Racer streaming). Ring pointer D_8009DF1C, ring
 * base D_801E8AAC. `.state` is read as u_short (lhu). See StCdInterrupt.
 */
typedef struct StStrHeader {
    u_short state;        /* 0x00 */
    u_short mode;         /* 0x02 */
    u_short frame;        /* 0x04 */
    u_short nSectors;     /* 0x06 */
    u_short nFrames;      /* 0x08 */
    u_char pad0A[0x12];   /* 0x0A */
    CdlLOC loc;       /* 0x1C */
} StStrHeader;

typedef struct CdSearchDirEntry {
    long type;
    u_char pad4[4];
    u_char name[0x24];
} CdSearchDirEntry;

char *CdComstr(long cmd);
char *CdIntstr(long intr);
long CdSetDebug(long level);
void CdFlush(void);
long CdInit(void);
long CdStatus(void) asm("func_8006A3E8");
u_char CdMode(void);
u_char CdLastCom(void);
CdlLOC *CdLastPos(void);
CdlLOC *CdIntToPos(long i, CdlLOC *p);
long CdPosToInt_Local(CdlLOC *loc);
long CdGetToc(CdlLOC *toc);
long CdGetToc2(long n, CdlLOC *toc);
void CD_initintr(void) asm("func_8006BCC4");
long CD_initvol(void) asm("func_8006BBD0");
void CD_flush(void) asm("func_8006BAF0");
long CD_getsector2(long madr, u_long size) asm("func_8006C06C");
long CD_vol(CdlATV *vol) asm("func_8006BA68");
long DsSearchFile(void *file, void *name) asm("func_8006C25C");
long DS_searchdir(long type, u_char *name) asm("func_8006C83C");
void StClearRing(void);
long StGetBackloc(CdlLOC *loc);
/* LibRef47 spells these `u_long *ring_addr, u_long ring_size` and StSetStream's
 * last two arguments as function pointers; kept as-is to match the call sites. */
void StSetRing(void *base, long size) asm("func_8006A058");
void StSetStream(long mode, long start_frame, long end_frame, long callback, long user_data);
u_long StFreeRing(u_long *base) asm("func_8006CFF0");
/* The libds streaming state machine: advances D_80099418 through states 1..0xA,
 * DMAs sector header then body, drives the StStrHeader ring. Installed via
 * CdReadyCallback behind the stub CdRead2Callback and also pumped directly from
 * UploadFmvSlice. */
void StCdInterrupt(void) asm("func_8006D1D0");

/*
 * libcd command interface. All three share the same retry-3 body over CD_cw;
 * CdControlB additionally waits on CD_sync (blocking), CdControlF sends the
 * command without collecting a result.
 */
long CdControl(long com, void *param, long result) asm("func_8006A5A4");
long CdControlF(long com, void *param) asm("func_8006A6DC");
long CdControlB(long com, void *param, long result) asm("func_8006A808");
long CdSync(long mode, long result) asm("func_8006A534");
long CdReady(long mode, long result) asm("func_8006A554");
/* Install a completion / data-ready callback; returns the previous one. */
long CdSyncCallback(long callback) asm("func_8006A574");
long CdReadyCallback(long callback) asm("func_8006A58C");

/*
 * libcd internals. CD_init resets the drive (CD_initvol + CD_initintr + the
 * register-level reset CdResetState) and is what CdInit retries up to 5 times.
 * CdDispatchInterrupts is the IRQ2 handler installed by that reset; it drains
 * the interrupt status via CdReadInterruptStatus and fans out to the sync/ready
 * callbacks.
 */
/*
 * libcd's cdread.c, linked into the game's own .text range instead of the
 * 0x80063200+ SDK block - identified by its three surviving messages
 * "CdRead: sector error" / "CdRead: Shell open..." / "CdRead: retry...".
 * CdRead arms a multi-sector transfer and returns immediately; CdReadSync
 * polls it (mode 0 blocks, non-zero returns the sectors still outstanding).
 * The two lower entries are cdread.c's own statics, named descriptively here.
 */
long CdReadSync(long mode, long result) asm("func_80027790");
void CdReadBreak(void) asm("func_80027634");
/* cdread.c's `data_ready_callback`: drains one sector per CdReady interrupt. */
void CdReadDataReadyCallback(u_char intr, long result) asm("func_80027238");
/* cdread.c's `read_retry`: re-issues CdlSetmode + CdlReadN after a shell open,
 * a seek error, or the 0x4B0-vblank watchdog in CdReadSync. */
long CdReadRetry(long mode) asm("func_8002745C");

/* Install the DMA3 (CD-ROM) data callback; returns the previous one. */
void CdDataCallback(long callback);
/* Fetch the next ready ring frame: *addr = its data, *header = its ring entry;
 * returns 0 when one was handed out. */
long StGetNext(StRingEventRecord **addr, StRingEventRecord **header);
/* Tear the stream down: clears the CD data / ready callbacks and both kernel
 * callback slots inside a critical section. */
void StUnSetRing(void);

/*
 * libpress (MDEC) front end - thin wrappers over the MDEC_* primitives, which
 * is what identifies each one. DecDCTin/DecDCTout push a bitstream / pull the
 * decoded macroblocks; the *Callback pair installs the DMA0 (MDECin) and DMA1
 * (MDECout) completion callbacks, which is how UploadFmvSlice is reached.
 */
void DecDCTout(volatile u_long *buf, long size) asm("func_8006402C");
/* LibRef47 gives both as `long f(long mode)`; these two bodies take no argument
 * and return nothing, i.e. the blocking (mode 0) form only. */
void DecDCTinSync(void) asm("func_8006404C");
void DecDCToutSync(void) asm("func_8006406C");
void DecDCTinCallback(long callback) asm("func_8006408C");
void DecDCToutCallback(long callback) asm("func_800640B0");

long CD_init(long mode) asm("func_8006A428");
long CD_sync(long mode, u_char *result) asm("func_8006B0D4");
long CD_ready(long mode, u_char *result) asm("func_8006B354");
long CD_cw(u_char command, u_char *params, u_char *result, long async) asm("func_8006B620");


#endif
