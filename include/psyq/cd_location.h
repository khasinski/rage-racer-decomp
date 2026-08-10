#ifndef PSYQ_CD_LOCATION_H
#define PSYQ_CD_LOCATION_H

#include "common.h"

typedef struct CdlLOC {
    u8 minute;
    u8 second;
    u8 sector;
    u8 track;
} CdlLOC;

#endif
