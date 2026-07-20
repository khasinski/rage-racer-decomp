#include "common.h"

typedef struct {
    s32 w0;
    s32 w1;
    char name[16];
} Rec;

extern s32 D_8009934C;
extern s32 D_80099054;
extern s32 D_80099048;
extern Rec D_8009BB14[64];

extern const char D_80013928[];
extern const char D_80013944[];
extern const char D_8001395C[];
extern const char D_80013978[];
extern const char D_80013998[];
extern const char D_800139A4[];

extern int func_8006C560(void);
extern int func_8006C83C(int type, char *name);
extern int func_8006C8E4(int arg0);
extern int func_8006C53C(char *a, char *b);
extern void func_8001674C();

Rec *func_8006C25C(Rec *out, char *path) {
    char buf[32];
    char *p;
    char *b;
    int type;
    int n;
    int i;
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
            func_8001674C(D_80013928, path, n);
        }
        return 0;
    }
    if (buf[0] == 0) {
        if (D_80099048 > 0) {
            func_8001674C(D_80013944, path);
        }
        return 0;
    }
    *b = 0;
    if (func_8006C8E4(type) == 0) {
        if (D_80099048 > 0) {
            func_8001674C(D_8001395C);
        }
        return 0;
    }
    if (D_80099048 >= 2) {
        func_8001674C(D_80013978, buf);
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
        if (func_8006C53C(nm, buf)) {
            if (D_80099048 >= 2) {
                func_8001674C(D_80013998, buf);
            }
            *out = *rec;
            return rec;
        }
        rec++;
        nm += 24;
    }
    if (D_80099048 > 0) {
        func_8001674C(D_800139A4, buf);
    }
    return 0;
}
