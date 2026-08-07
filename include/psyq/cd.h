#ifndef PSYQ_CD_H
#define PSYQ_CD_H

#include <sys/types.h>

#include "common.h"
#include "psyq/cd_types.h"

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
 * CD stream ring header (Rage Racer streaming). Ring pointer g_StActiveHeader, ring
 * base g_StRingBase. `.state` is read as u_short (lhu). See StCdInterrupt.
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

/* Returns the human-readable name for CD command `cmd`, or a default string if
 * out of range. */
char *CdComstr(long cmd);
/* Returns the human-readable name for CD interrupt code `intr`. */
char *CdIntstr(long intr);
long CdSetDebug(long level);
void CdFlush(void);
long CdInit(void);
long CdStatus(void);
u_char CdMode(void);
u_char CdLastCom(void);
/* Returns the last reported disc position. */
CdlLOC *CdLastPos(void);
CdlLOC *CdIntToPos(long i, CdlLOC *p);
long CdPosToInt_Local(CdlLOC *loc);
long CdGetToc(CdlLOC *toc);
long CdGetToc2(long n, CdlLOC *toc);
void CD_initintr(void);
long CD_initvol(void);
void CD_flush(void);
long CD_getsector2(long madr, u_long size);
long CD_vol(CdlATV *vol);
long DsSearchFile(void *file, void *name);
long DS_searchdir(long type, u_char *name);
void StClearRing(void);
long StGetBackloc(CdlLOC *loc);
/* LibRef47 spells these `u_long *ring_addr, u_long ring_size` and StSetStream's
 * last two arguments as function pointers; kept as-is to match the call sites. */
void StSetRing(void *base, long size);
void StSetStream(long mode, long start_frame, long end_frame, long callback, long user_data);
u_long StFreeRing(u_long *base);
/* The libds streaming state machine: advances g_StInterruptState through states 1..0xA,
 * DMAs sector header then body, drives the StStrHeader ring. Installed via
 * CdReadyCallback behind the stub CdRead2Callback and also pumped directly from
 * UploadFmvSlice. */
void StCdInterrupt(void);

/*
 * libcd command interface. All three share the same retry-3 body over CD_cw;
 * CdControlB additionally waits on CD_sync (blocking), CdControlF sends the
 * command without collecting a result.
 */
long CdControl(long com, void *param, long result);
long CdControlF(long com, void *param);
long CdControlB(long com, void *param, long result);
long CdSync(long mode, long result);
/* PSY-Q: long CdReady(long mode, u_char *result); see CD_cw above. */
long CdReady();
/* Install a completion / data-ready callback; returns the previous one. */
long CdSyncCallback(long callback);
long CdReadyCallback(long callback);

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
/*
 * Sector geometry. A Mode 2 Form 1 data sector carries CD_SECTOR_SIZE bytes of
 * user data, so a byte count becomes a sector count as
 * `(n + CD_SECTOR_MASK) >> 11`, which is what LoadAsset does to the size it
 * read out of the RAGE.BIN index.
 */
#define CD_SECTOR_SIZE 0x800
#define CD_SECTOR_MASK 0x7FF

/*
 * CdlSetmode bit 7, double speed - the `mode` every CdRead call site in this
 * tree passes. It is not a sector-length selector: CdRead itself decodes those
 * with `mode & 0x30` (0x00 -> 0x200 words, 0x10 -> 0x246, 0x20 -> 0x249), and
 * 0x80 leaves that field zero, i.e. plain 0x800-byte sectors at 2x.
 */
#define CdlModeSpeed 0x80

long CdRead(long sectors, void *buf, long mode);
long CdReadSync(long mode, long result);
void CdReadBreak(void);
/* cdread.c's `data_ready_callback`: drains one sector per CdReady interrupt. */
void CdReadDataReadyCallback(u_char intr, long result);
/* cdread.c's `read_retry`: re-issues CdlSetmode + CdlReadN after a shell open,
 * a seek error, or the 0x4B0-vblank watchdog in CdReadSync. */
long CdReadRetry(long mode);

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
void DecDCTout(volatile u_long *buf, long size);
/* LibRef47 gives both as `long f(long mode)`; these two bodies take no argument
 * and return nothing, i.e. the blocking (mode 0) form only. */
void DecDCTinSync(void);
void DecDCToutSync(void);
void DecDCTinCallback(long callback);
void DecDCToutCallback(long callback);

long CD_init(long mode);
/* PSY-Q: long CD_sync(long mode, u_char *result); see CD_cw above. */
long CD_sync();
/* PSY-Q: long CD_ready(long mode, u_char *result); see CD_cw above. */
long CD_ready();
long CD_cw(u_char command, u_char *params, u_char *result, long async);

/* Declared identically by 65 translation units before this
 * header carried them. */

extern long g_CdCachedDir;
extern long g_CdDebugLevel;
extern volatile u_long *g_CdDmaChcr;
extern u_char g_CdLastCommand;
extern u_char g_CdModeByte;
extern volatile long g_CdReadBuffer;
extern volatile long g_CdReadMode;
extern volatile long g_CdReadSavedReadyCallback;
extern volatile long g_CdReadSavedSyncCallback;
extern volatile long g_CdReadSectorCount;
extern volatile long g_CdReadSectorWords;
extern volatile long g_CdReadStartVSync;
extern volatile u_char *g_CdReg0;
extern volatile u_char *g_CdReg1;
extern volatile u_char *g_CdReg2;
extern volatile u_char *g_CdReg3;
extern long g_CdTimeoutCounter;
extern u_char g_MdecIdctCmd[];
extern u_char g_MdecQuantCmd[];
extern long g_StRingSlot;

long CdReadInterruptStatus(void);
void MDEC_in(volatile u_long *arg0, long arg1);

/* Declared identically by 54 translation units before this
 * header carried them. */

extern long g_CdCachedShellOpenCount;
extern u_long g_CdCommandAckHasStatus[];
extern long g_CdCommandClearsReady[];
extern u_long g_CdCommandHasComplete[];
extern long g_CdCommandNeedsSetloc[];
extern long g_CdCommandParamCount[];
extern volatile u_char g_CdDataEndStatus;
extern void *g_CdDebugInfo[];
extern CdSearchDirEntry g_CdDirEntryName[];
extern volatile u_long *g_CdDmaBcr;
extern volatile u32 *g_CdDmaControl;
extern volatile u_long *g_CdDmaMadr;
extern volatile u_long *g_CdDpcr;
extern volatile long g_CdReadExpectedSector;
extern volatile long g_CdReadLastVSync;
extern volatile long g_CdReadPtr;
extern volatile long g_CdReadRemaining;
extern CdRegisterMap *volatile g_CdSpuRegs;
extern volatile u_long *g_CdromDelayReg;
extern volatile u_long *g_MdecCmdReg;
extern volatile u_long *g_MdecCtrlReg;
extern volatile u_long *g_MdecDpcr;
extern u_long g_MdecIdctTable[];
extern volatile u_long *g_MdecInDmaBcr;
extern volatile u_long *g_MdecInDmaChcr;
extern volatile u_long *g_MdecInDmaMadr;
extern volatile u_long *g_MdecOutDmaBcr;
extern volatile u_long *g_MdecOutDmaChcr;
extern volatile u32 *g_MdecOutDmaControl;
extern volatile u_long *g_MdecOutDmaMadr;
extern u_long g_MdecQuantChroma[];
extern u_long g_MdecQuantLuma[];
extern volatile StStrHeader *g_StActiveHeader;
extern long g_StBackFrame;
extern u_char g_StBackLoc[];
extern volatile u8 *g_StCdReg0;
extern volatile u8 *g_StCdReg2;
extern volatile u8 *g_StCdReg3;
extern s32 g_StCopySector;
extern StCallback g_StFrameCallback;
extern s32 g_StInterruptState;
extern u8 *g_StSectorData;

void CD_dmastart(
    s32 channel,
    u32 address,
    u32 count,
    u32 size,
    u32 control,
    u8 mode,
    u32 unused);
extern long CD_namecmp(char *a, char *b);
extern long CD_newmedia(void);
void CdDefaultReadCallback(void);
void CdDefaultReadyCallback(void);
void CdDefaultSyncCallback(void);
void CdDispatchInterrupts(void);
void CdRead2Callback(void);
u_long *DecDCTGetEnv(u_long *arg0);
u_long *DecDCTPutEnv(u_long *arg0);
void MDEC_reset(long arg0);
long MDEC_timeout(u_char *arg0);

#endif
