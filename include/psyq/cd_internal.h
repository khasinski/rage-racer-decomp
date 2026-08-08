#ifndef RAGE_PC_PSYQ_CD_INTERNAL_H
#define RAGE_PC_PSYQ_CD_INTERNAL_H

#include "common.h"
#include "psyq/cd_types.h"

extern CdRawWord g_CdRootDirLba;
extern CdPathCacheRecord g_CdPathEntryLbaByDirNum[];

extern void (*g_CdReadCallback)(long, long);
extern CdCallback g_CdSyncCallback;
extern CdCallback g_CdReadyCallback;

extern long g_CdCachedDir;
extern long g_CdCachedShellOpenCount;
extern u_long g_CdCommandAckHasStatus[];
extern char *g_CdCommandNames[];
extern long g_CdCommandClearsReady[];
extern u_long g_CdCommandHasComplete[];
extern long g_CdCommandNeedsSetloc[];
extern long g_CdCommandParamCount[];
extern volatile u_char g_CdDataEndStatus;
extern long g_CdDebugLevel;
extern void *g_CdDebugInfo[];
extern CdSearchDirEntry g_CdDirEntryName[];
extern volatile u_long *g_CdDmaBcr;
extern volatile u_long *g_CdDmaChcr;
extern volatile u32 *g_CdDmaControl;
extern volatile u_long *g_CdDmaMadr;
extern volatile u_long *g_CdDpcr;
extern u_char g_CdLastCommand;
extern CdlLOC g_CdLastPos;
extern u_char g_CdModeByte;
extern volatile long g_CdReadBuffer;
extern volatile long g_CdReadExpectedSector;
extern volatile long g_CdReadLastVSync;
extern volatile long g_CdReadMode;
extern volatile long g_CdReadPtr;
extern volatile long g_CdReadRemaining;
extern volatile long g_CdReadSavedReadyCallback;
extern volatile long g_CdReadSavedSyncCallback;
extern volatile long g_CdReadSectorCount;
extern volatile long g_CdReadSectorWords;
extern volatile long g_CdReadStartVSync;
extern volatile u_char *g_CdReg0;
extern volatile u_char *g_CdReg1;
extern volatile u_char *g_CdReg2;
extern volatile u_char *g_CdReg3;
extern CdRegisterMap *volatile g_CdSpuRegs;
extern u_char g_CdStatusByte;
extern u_char g_CdErrorByte;
extern u_long g_CdShellOpenCount;
extern char *g_CdIntrNames[];
extern volatile CdIntr g_CdSyncStatus;
extern u_char g_CdSyncResult[8];
extern u_char g_CdReadyResult[8];
extern u_char g_CdDataEndResult[8];
extern long g_CdTimeoutDeadline;
extern char *g_CdTimeoutName;
extern u_char g_CdSectorBuf[0x800];
extern long g_CdTimeoutCounter;
extern volatile u_long *g_CdromDelayReg;

extern volatile u_char *D_800993D0;
extern char g_CdAlarmNameCw[];
extern short g_CdDotDotName;
extern signed char g_CdDotDotNameNul;
extern u_short g_CdDotName;
extern CdlFILE g_CdFileCache[64];
extern volatile long g_CdPathEntryParentDir[];
extern CdlDIR g_CdPathTable[];
extern long g_CdTestParamCount;
extern const char g_CdVolumeSignature[];
extern volatile u_long *g_ComDelayReg;
extern u_char *g_DmaDicr;
extern volatile u_long *g_DmaDpcr;

extern char g_FmtCdCommand[];
extern const char g_FmtCdDirNotFound[];
extern u_char g_FmtCdFileEntry[];
extern const char g_FmtCdFileFound[];
extern const char g_FmtCdFileNotFound[];
extern char g_FmtCdNoParam[];
extern const char g_FmtCdPathEntry[];
extern const char g_FmtCdPathLevelError[];
extern const char g_FmtCdSearching[];
extern char g_FmtCdTimeoutState[];
extern char g_MsgCdDataSync[];
extern const char g_MsgCdDirEntriesFound[];
extern u_char g_MsgCdDirNotFound[];
extern const char g_MsgCdDiscError[];
extern u_char g_MsgCdDiskError;
extern u_char g_MsgCdErrorCommandCode;
extern u_char g_MsgCdFilesFound[];
extern u_char g_MsgCdGetToc2Entry[];
extern u_char g_MsgCdGetToc2Error[];
extern u_char g_MsgCdInit[];
extern u_char g_MsgCdInitAddr[];
extern u_char g_MsgCdInitFailed[];
extern u_char g_MsgCdNone[];
extern const char g_MsgCdPathTableReadError[];
extern const char g_MsgCdPvdFormatError[];
extern const char g_MsgCdPvdReadError[];
extern u_char g_MsgCdReadRetry;
extern u_char g_MsgCdReadSectorError;
extern u_char g_MsgCdReadShellOpen;
extern char g_MsgCdReadyName[];
extern const char g_MsgCdSearchingDir[];
extern u_char g_MsgCdSearchingFiles[];
extern char g_MsgCdSyncName[];
extern char g_MsgCdTimeout[];
extern u_char g_MsgCdTrackRange[];
extern u_char g_MsgCdUnknownIntr;
extern u_char g_MsgCdUnknownIntrCode;
extern char g_MsgDmaStatusError[];

#endif
