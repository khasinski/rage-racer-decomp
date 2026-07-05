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

char *CdComstr(s32 cmd) asm("func_8006A4CC");
char *CdIntstr(s32 intr) asm("func_8006A500");
CdlLOC *CdIntToPos(s32 i, CdlLOC *p) asm("func_8006A9D8");
s32 CdPosToInt_Local(CdlLOC *loc) asm("func_8006AADC");
void CD_initintr(void) asm("func_8006BCC4");
int CD_initvol(void) asm("func_8006BBD0");
void CD_flush(void) asm("func_8006BAF0");
s32 CD_getsector2(s32 madr, u32 size) asm("func_8006C06C");
s32 CD_vol(CdlATV *vol) asm("func_8006BA68");
int DsSearchFile(void *file, void *name) asm("func_8006C25C");
int DS_searchdir(s32 type, u8 *name) asm("func_8006C83C");
void StClearRing(void) asm("func_8006CDC0");
s32 StGetBackloc(CdlLOC *loc) asm("func_8006CF08");
void StSetStream(s32 mode, s32 start_frame, s32 end_frame, s32 callback, s32 user_data) asm("func_8006CF68");
u32 StFreeRing(u32 *base) asm("func_8006CFF0");

#endif
