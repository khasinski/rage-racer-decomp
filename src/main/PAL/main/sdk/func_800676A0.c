#include "common.h"

typedef struct {
    void (*cb)(s32, s32);
    s32 arg;
    s32 tag;
    s32 params[21];
} QEntry;

extern QEntry D_801E5024[];
extern volatile s32 D_800942EC;
extern s32 D_800942F0;
extern s32 D_800942F4;
extern u8 D_800942DC[];
extern s32 D_800942E0;
extern s32 D_800942E4;
extern volatile u32 *D_800942C8;
extern volatile u32 *D_800942BC;
extern u8 D_800941F0[];
extern volatile u8 D_800941E9;
extern s32 D_800941F4;

extern void func_80067F04(void);
extern s32 func_80067F38(void);
extern void func_80067984(void);
extern s32 func_8006E0B0(s32);
extern void func_8006DF94(s32, void *);

s32 func_800676A0(void (*cb)(s32, s32), s32 arg, s32 size, s32 tag) {
    s32 i;
    s32 *src;
    s32 ret;

    func_80067F04();
    while ((((D_800942EC + 1) & 0x3f)) == D_800942F0) {
        if (func_80067F38() != 0) {
            return -1;
        }
        func_80067984();
    }

    ret = func_8006E0B0(0);
    *(volatile s32 *)D_800941F0 = 1;
    D_800942F4 = ret;

    if (D_800941E9 != 0) {
        if (D_800942EC != D_800942F0) goto enqueue;
        if ((*D_800942C8 & 0x01000000) != 0) goto enqueue;
        if (D_800941F4 != 0) goto enqueue;
    }

    do {
    } while ((*D_800942BC & 0x04000000) == 0);

    cb(arg, tag);
    *(void (*volatile *)(s32, s32))D_800942DC = cb;
    D_800942E0 = arg;
    D_800942E4 = tag;
    func_8006E0B0(D_800942F4);
    return 0;

enqueue:
    func_8006DF94(2, (void *)func_80067984);

    if (size != 0) {
        u8 *pbase = (u8 *)D_801E5024 + 12;
        i = 0;
        src = (s32 *)arg;
        while (i < size / 4) {
            *(s32 *)(pbase + D_800942EC * 96 + i * 4) = *src;
            src++;
            i++;
            asm("");
        }
        D_801E5024[D_800942EC].arg = (s32)D_801E5024[D_800942EC].params;
    } else {
        D_801E5024[D_800942EC].arg = arg;
    }

    D_801E5024[D_800942EC].tag = tag;
    asm("");
    D_801E5024[D_800942EC].cb = cb;
    asm("");

    D_800942EC = (D_800942EC + 1) & 0x3f;
    func_8006E0B0(D_800942F4);
    func_80067984();
    return (D_800942EC - D_800942F0) & 0x3f;
}
