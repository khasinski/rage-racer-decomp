#include "common.h"
#include <stdio.h>
#include <string.h>
#include "psyq/cd.h"

typedef struct W4 {
    u_char b[4];
} W4;

extern u_char g_CdSectorBuf[];
extern W4 g_CdRootDirLba;
extern CdlDIR g_CdPathTable[];

extern const char D_800139B4[];
extern const char D_800139E0[];
extern const char D_800139E8[];
extern const char D_80013A18[];
extern const char D_80013A3C[];
extern const char D_80013A5C[];
extern const char D_80013A70[];

/*
 * Reads and parses the disc's directory into the Entry table g_CdPathTable[128].
 * Loads sector 16 (the volume descriptor) via cd_read, validates the "CD"
 * signature, follows it to the path/directory sector, then walks the packed
 * variable-length records (record length in *p, name at p+8) copying each into
 * an Entry (index, header word, flags, name). Returns 1 on success, 0 on error.
 * g_CdDebugLevel is the debug-verbosity level gating the printf logging.
 */
/* Rebuilds the ISO path-table cache after a disc change: reads the primary
 * volume descriptor at sector 16, checks "CD001", then parses up to 128
 * path-table records. Owns all four of its own "CD_newmedia: ..." messages. */
long CD_newmedia(void) {
    u_char *p;
    long i;
    long r;
    W4 hdr;

    r = cd_read(1, 16, g_CdSectorBuf);
    if (r != 1) {
        if (g_CdDebugLevel > 0) {
            printf((u8 *)D_800139B4);
        }
        return 0;
    }
    if (strncmp((u8 *)&g_CdSectorBuf[1], (u8 *)D_800139E0, 5) != 0) {
        if (g_CdDebugLevel > 0) {
            printf((u8 *)D_800139E8);
        }
        return 0;
    }
    hdr = g_CdRootDirLba;
    if (cd_read(1, *(long *)&hdr, g_CdSectorBuf) != r) {
        if (g_CdDebugLevel > 0) {
            printf((u8 *)D_80013A18, *(long *)&hdr);
        }
        return 0;
    }
    p = g_CdSectorBuf;
    if (g_CdDebugLevel >= 2) {
        printf((u8 *)D_80013A3C);
    }
    i = 0;
    while (p < &g_CdSectorBuf[0x800]) {
        long n;
        long d;
        if (*p == 0) {
            break;
        }
        *(W4 *)&g_CdPathTable[i].lba = *(W4 *)(p + 2);
        g_CdPathTable[i].parent_number = *(u_char *)(p + 6);
        g_CdPathTable[i].number = i + 1;
        memcpy(g_CdPathTable[i].name, p + 8, *p);
        g_CdPathTable[i].name[*p] = 0;
        n = *p;
        d = (n & 1) + 8;
        p += n + d;
        if (g_CdDebugLevel >= 2) {
            printf((u8 *)D_80013A5C, g_CdPathTable[i].lba.sector,
                          g_CdPathTable[i].number, g_CdPathTable[i].parent_number,
                          g_CdPathTable[i].name);
        }
        i++;
        if (i >= 128) {
            break;
        }
    }
    if (i < 128) {
        g_CdPathTable[i].parent_number = 0;
    }
    g_CdCachedDir = 0;
    if (g_CdDebugLevel >= 2) {
        printf((u8 *)D_80013A70, i);
    }
    return 1;
}

extern volatile long D_8009C118[];

long DS_searchdir(long type, u_char *name) {
    long i = 0;
    u_char *entryName = (u_char *)g_CdDirEntryName;
    long offset = 0;
    long entryType;

    while (i < 0x80) {
        entryType = *(long *)((u_char *)D_8009C118 + offset);
        if (entryType != 0) {
            if (entryType == type && strcmp((u8 *)name, (u8 *)entryName) == 0) {
                return i + 1;
            }

            entryName += sizeof(CdSearchDirEntry);
            i++;
            offset += sizeof(CdSearchDirEntry);
        } else {
            return -1;
        }
    }

    return -1;
}
