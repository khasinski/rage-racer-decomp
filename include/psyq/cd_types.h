#ifndef PSYQ_CD_TYPES_H
#define PSYQ_CD_TYPES_H

#include "common.h"

typedef struct CdlLOC {
    u_char minute;
    u_char second;
    u_char sector;
    u_char track;
} CdlLOC;

typedef struct CdlFILE {
    CdlLOC pos;
    long size;
    char name[16];
} CdlFILE;

typedef union CdlLBA {
    long sector;
    CdlLOC pos;
} CdlLBA;

typedef struct CdlDIR {
    long number;
    long parent_number;
    CdlLBA lba;
    char name[32];
} CdlDIR;

#endif
