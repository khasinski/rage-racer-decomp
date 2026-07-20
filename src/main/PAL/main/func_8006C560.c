#include "common.h"

typedef struct {
    u8 b[4];
} W4;

typedef struct {
    s32 index;    /* +0  */
    s32 field6;   /* +4  */
    s32 word2;    /* +8  */
    char name[32];/* +12 */
} Entry;

extern u8 D_8009D714[];
extern W4 D_8009D7A0;
extern Entry D_8009C114[];
extern s32 D_80099048;
extern s32 D_80099348;

extern const char D_800139B4[];
extern const char D_800139E0[];
extern const char D_800139E8[];
extern const char D_80013A18[];
extern const char D_80013A3C[];
extern const char D_80013A5C[];
extern const char D_80013A70[];

extern int func_8006CB88(int, int, void *);
extern int func_8006CC8C(void *, const char *, int);
extern void func_8006CBF4(char *, u8 *, int);
extern int func_8001674C();

int func_8006C560(void) {
    u8 *p;
    int i;
    int r;
    W4 hdr;

    r = func_8006CB88(1, 16, D_8009D714);
    if (r != 1) {
        if (D_80099048 > 0) {
            func_8001674C(D_800139B4);
        }
        return 0;
    }
    if (func_8006CC8C(&D_8009D714[1], D_800139E0, 5) != 0) {
        if (D_80099048 > 0) {
            func_8001674C(D_800139E8);
        }
        return 0;
    }
    hdr = D_8009D7A0;
    if (func_8006CB88(1, *(int *)&hdr, D_8009D714) != r) {
        if (D_80099048 > 0) {
            func_8001674C(D_80013A18, *(int *)&hdr);
        }
        return 0;
    }
    p = D_8009D714;
    if (D_80099048 >= 2) {
        func_8001674C(D_80013A3C);
    }
    i = 0;
    while (p < &D_8009D714[0x800]) {
        int n;
        int d;
        if (*p == 0) {
            break;
        }
        *(W4 *)&D_8009C114[i].word2 = *(W4 *)(p + 2);
        D_8009C114[i].field6 = *(u8 *)(p + 6);
        D_8009C114[i].index = i + 1;
        func_8006CBF4(D_8009C114[i].name, p + 8, *p);
        D_8009C114[i].name[*p] = 0;
        n = *p;
        d = (n & 1) + 8;
        p += n + d;
        if (D_80099048 >= 2) {
            func_8001674C(D_80013A5C, D_8009C114[i].word2,
                          D_8009C114[i].index, D_8009C114[i].field6,
                          D_8009C114[i].name);
        }
        i++;
        if (i >= 128) {
            break;
        }
    }
    if (i < 128) {
        D_8009C114[i].field6 = 0;
    }
    D_80099348 = 0;
    if (D_80099048 >= 2) {
        func_8001674C(D_80013A70, i);
    }
    return 1;
}
