#include "common.h"

typedef struct Rec {
    long w0;
    long w1;
    char name[16];
} Rec;

extern long g_CdCachedShellOpenCount;
extern long g_CdShellOpenCount;
extern long g_CdDebugLevel;
extern Rec g_CdFileCache[64];

extern const char D_80013928[];
extern const char D_80013944[];
extern const char D_8001395C[];
extern const char D_80013978[];
extern const char D_80013998[];
extern const char D_800139A4[];

Rec *DsSearchFile(Rec *out, char *path) {
    char buf[32];
    char *p;
    char *b;
    long type;
    long n;
    long i;
    Rec *rec;
    char *nm;

    if (g_CdCachedShellOpenCount != g_CdShellOpenCount) {
        if (!CD_newmedia()) {
            return 0;
        }
        g_CdCachedShellOpenCount = g_CdShellOpenCount;
    }
    if (*path != '\\') {
        return 0;
    }
    type = 1;
    buf[0] = 0;
    p = path;
    n = 0;
    while (n < 8) {
        b = buf;
        while (*p != '\\') {
            if (*p == 0) {
                break;
            }
            *b++ = *p++;
        }
        if (*p == 0) {
            break;
        }
        p++;
        *b = 0;
        type = DS_searchdir(type, buf);
        if (type == -1) {
            buf[0] = 0;
            break;
        }
        n++;
    }
    if (n >= 8) {
        if (g_CdDebugLevel > 0) {
            DebugPrintf(D_80013928, path, n);
        }
        return 0;
    }
    if (buf[0] == 0) {
        if (g_CdDebugLevel > 0) {
            DebugPrintf(D_80013944, path);
        }
        return 0;
    }
    *b = 0;
    if (CD_cachefile(type) == 0) {
        if (g_CdDebugLevel > 0) {
            DebugPrintf(D_8001395C);
        }
        return 0;
    }
    if (g_CdDebugLevel >= 2) {
        DebugPrintf(D_80013978, buf);
    }
    {
        char *base = g_CdFileCache[0].name;
        rec = (Rec *)(base - 8);
        nm = base;
    }
    for (i = 0; i < 64; i++) {
        if (g_CdFileCache[i].name[0] == 0) {
            break;
        }
        if (CD_namecmp(nm, buf)) {
            if (g_CdDebugLevel >= 2) {
                DebugPrintf(D_80013998, buf);
            }
            *out = *rec;
            return rec;
        }
        rec++;
        nm += 24;
    }
    if (g_CdDebugLevel > 0) {
        DebugPrintf(D_800139A4, buf);
    }
    return 0;
}

long CD_namecmp(long lhs, long rhs) {
    return LibcStrncmp(lhs, rhs, 0xC) == 0;
}
