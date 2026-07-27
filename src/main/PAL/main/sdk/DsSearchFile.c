#include "common.h"

typedef struct {
    long w0;
    long w1;
    char name[16];
} Rec;

extern long D_8009934C;
extern long D_80099054;
extern long D_80099048;
extern Rec D_8009BB14[64];

extern const char D_80013928[];
extern const char D_80013944[];
extern const char D_8001395C[];
extern const char D_80013978[];
extern const char D_80013998[];
extern const char D_800139A4[];

extern long func_8006C560(void);
extern long func_8006C83C(long type, char *name);
extern long func_8006C8E4(long arg0);
extern long CD_namecmp(char *a, char *b) asm("func_8006C53C");

Rec * DsSearchFile(Rec *out, char *path) asm("func_8006C25C");
Rec *DsSearchFile(Rec *out, char *path) {
    char buf[32];
    char *p;
    char *b;
    long type;
    long n;
    long i;
    Rec *rec;
    char *nm;

    if (D_8009934C != D_80099054) {
        if (!func_8006C560()) {
            return 0;
        }
        D_8009934C = D_80099054;
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
                goto after_tokens;
            }
            *b++ = *p++;
        }
        if (*p == 0) {
            goto after_tokens;
        }
        p++;
        *b = 0;
        type = func_8006C83C(type, buf);
        if (type == -1) {
            buf[0] = 0;
            break;
        }
        n++;
    }
after_tokens:
    if (n >= 8) {
        if (D_80099048 > 0) {
            GameDebugPrintf(D_80013928, path, n);
        }
        return 0;
    }
    if (buf[0] == 0) {
        if (D_80099048 > 0) {
            GameDebugPrintf(D_80013944, path);
        }
        return 0;
    }
    *b = 0;
    if (func_8006C8E4(type) == 0) {
        if (D_80099048 > 0) {
            GameDebugPrintf(D_8001395C);
        }
        return 0;
    }
    if (D_80099048 >= 2) {
        GameDebugPrintf(D_80013978, buf);
    }
    {
        char *base = D_8009BB14[0].name;
        rec = (Rec *)(base - 8);
        nm = base;
    }
    for (i = 0; i < 64; i++) {
        if (D_8009BB14[i].name[0] == 0) {
            break;
        }
        if (CD_namecmp(nm, buf)) {
            if (D_80099048 >= 2) {
                GameDebugPrintf(D_80013998, buf);
            }
            *out = *rec;
            return rec;
        }
        rec++;
        nm += 24;
    }
    if (D_80099048 > 0) {
        GameDebugPrintf(D_800139A4, buf);
    }
    return 0;
}
