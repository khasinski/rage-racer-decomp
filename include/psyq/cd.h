#ifndef PSYQ_CD_H
#define PSYQ_CD_H

#include "common.h"

typedef struct CdlLOC {
    u8 minute;
    u8 second;
    u8 sector;
    u8 track;
} CdlLOC;

typedef struct CdlATV {
    u8 val0;
    u8 val1;
    u8 val2;
    u8 val3;
} CdlATV;

typedef struct CdRegisterMap {
    u8 pad0[0x180];
    u16 cd_left_volume;
    u16 cd_right_volume;
    u8 pad184[0x1AA - 0x184];
    u16 audio_control;
    u8 pad1AC[0x1B0 - 0x1AC];
    u16 output_left_volume;
    u16 output_right_volume;
    u8 pad1B4[0x1B8 - 0x1B4];
    u16 status_mode_a;
    u16 status_mode_b;
} CdRegisterMap;

typedef struct StHEADER_RR {
    s16 id;
    u8 pad2[4];
    s16 nSectors;
    u8 pad8[0x18];
} StHEADER_RR;

typedef struct StRingEntry {
    volatile s16 state;
    u8 pad2[4];
    s16 nSectors;
    s32 value;
    u8 padC[0x10];
    CdlLOC loc;
} StRingEntry;

typedef struct StRingEventRecord {
    volatile u16 state;
    u8 pad2[0x1E];
} StRingEventRecord;

typedef struct StRingClearRecord {
    s32 value;
    u8 pad4[0x1C];
} StRingClearRecord;

/*
 * CD stream ring header (Rage Racer streaming). Ring pointer D_8009DF1C, ring
 * base D_801E8AAC. `.state` is read as u16 (lhu). See func_8006D1D0.
 */
typedef struct StStrHeader {
    u16 state;        /* 0x00 */
    u16 mode;         /* 0x02 */
    u16 frame;        /* 0x04 */
    u16 nSectors;     /* 0x06 */
    u16 nFrames;      /* 0x08 */
    u8 pad0A[0x12];   /* 0x0A */
    CdlLOC loc;       /* 0x1C */
} StStrHeader;

typedef struct CdSearchDirEntry {
    s32 type;
    u8 pad4[4];
    u8 name[0x24];
} CdSearchDirEntry;

char *CdComstr(s32 cmd) asm("func_8006A4CC");
char *CdIntstr(s32 intr) asm("func_8006A500");
s32 CdSetDebug(s32 level) asm("func_8006A4B4");
void CdFlush(void) asm("func_8006A494");
s32 CdInit(void) asm("func_8006A2D0");
s32 CdStatus(void) asm("func_8006A3E8");
u8 CdMode(void) asm("func_8006A3F8");
u8 CdLastCom(void) asm("func_8006A408");
CdlLOC *CdLastPos(void) asm("func_8006A418");
CdlLOC *CdIntToPos(s32 i, CdlLOC *p) asm("func_8006A9D8");
s32 CdPosToInt_Local(CdlLOC *loc) asm("func_8006AADC");
s32 CdGetToc(CdlLOC *toc) asm("func_8006A088");
s32 CdGetToc2(s32 n, CdlLOC *toc) asm("func_8006A0AC");
void CD_initintr(void) asm("func_8006BCC4");
int CD_initvol(void) asm("func_8006BBD0");
void CD_flush(void) asm("func_8006BAF0");
s32 CD_getsector2(s32 madr, u32 size) asm("func_8006C06C");
s32 CD_vol(CdlATV *vol) asm("func_8006BA68");
int DsSearchFile(void *file, void *name) asm("func_8006C25C");
int DS_searchdir(s32 type, u8 *name) asm("func_8006C83C");
void StClearRing(void) asm("func_8006CDC0");
s32 StGetBackloc(CdlLOC *loc) asm("func_8006CF08");
void StSetRing(void *base, s32 size) asm("func_8006A058");
void StSetStream(s32 mode, s32 start_frame, s32 end_frame, s32 callback, s32 user_data) asm("func_8006CF68");
u32 StFreeRing(u32 *base) asm("func_8006CFF0");
/* The libds streaming state machine: advances D_80099418 through states 1..0xA,
 * DMAs sector header then body, drives the StStrHeader ring. Installed via
 * CdReadyCallback behind the stub func_8006CDA0 and also pumped directly from
 * func_8001EBC8. */
void StCdInterrupt(void) asm("func_8006D1D0");

/*
 * libcd command interface. All three share the same retry-3 body over CD_cw;
 * CdControlB additionally waits on CD_sync (blocking), CdControlF sends the
 * command without collecting a result.
 */
s32 CdControl(s32 com, void *param, s32 result) asm("func_8006A5A4");
s32 CdControlF(s32 com, void *param) asm("func_8006A6DC");
s32 CdControlB(s32 com, void *param, s32 result) asm("func_8006A808");
s32 CdSync(s32 mode, s32 result) asm("func_8006A534");
s32 CdReady(s32 mode, s32 result) asm("func_8006A554");
/* Install a completion / data-ready callback; returns the previous one. */
s32 CdSyncCallback(s32 callback) asm("func_8006A574");
s32 CdReadyCallback(s32 callback) asm("func_8006A58C");

/*
 * libcd internals. CD_init resets the drive (CD_initvol + CD_initintr + the
 * register-level reset func_8006BD14) and is what CdInit retries up to 5 times.
 * func_8006C17C is the IRQ2 handler installed by that reset; it drains the
 * interrupt status via func_8006AB5C and fans out to the sync/ready callbacks.
 */
s32 CD_init(s32 mode) asm("func_8006A428");
s32 CD_sync(s32 mode, s32 result) asm("func_8006B0D4");
s32 CD_ready(s32 mode, s32 result) asm("func_8006B354");
s32 CD_cw(s32 com, void *param, s32 result, s32 wait) asm("func_8006B620");


#endif
