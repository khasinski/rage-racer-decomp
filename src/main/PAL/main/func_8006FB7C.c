#include "common.h"

typedef struct {
    u8 tones;
    u8 pad[15];
} ProgAtr16;

typedef struct {
    u8 prior;
    u8 mode;
    u8 pad[38];
} VagAtr40;

extern u8 *D_801E79CC[];

s32 func_8007320C(s32 vab_id, s32 program, ProgAtr16 *out);
s32 func_800733D8(s32 vab_id, s32 program, s32 tone, VagAtr40 *out);
s32 func_80073820(s32 vab_id, s32 program, s32 tone, VagAtr40 *in);
s32 func_80070D70(s16 arg0, s16 arg1);

void func_8006FB7C(s32 vab, s32 prog, u32 mode) {
    u8 *ent;
    u8 *p;
    u8 *sub;
    s32 savedVab;
    s32 savedProg;
    s32 keep;
    u32 t;
    s32 i;
    s16 n;
    ProgAtr16 pa;
    VagAtr40 va;

    ent = D_801E79CC[(s16)vab] + (s16)prog * 172;
    savedVab = vab;
    sub = ent + *(u8 *)(ent + 0x12);
    savedProg = prog;
    func_8007320C(*(s16 *)(ent + 0x4C), *(u8 *)(sub + 0x2C), &pa);
    n = pa.tones;
    i = 0;
    if (n > 0) {
        p = sub;
        t = mode & 0xFF;
        __asm__("" : "=r"(t) : "0"(t));
        keep = t < 0x40;
        do {
            func_800733D8(*(s16 *)(ent + 0x4C), *(u8 *)(p + 0x2C), (s16)i, &va);
            if (keep) {
                va.mode = 2;
            } else {
                __asm__ volatile("" : "=r"(mode) : "0"(mode), "r"(i), "r"(p));
                if (((mode + 0xC0) & 0xFF) < 0x40) {
                    va.mode = 0;
                }
            }
            func_80073820(*(s16 *)(ent + 0x4C), *(u8 *)(p + 0x2C), (s16)i, &va);
            i++;
        } while (i < pa.tones);
    }
    *(s32 *)(ent + 0x88) = func_80070D70((s16)savedVab, (s16)savedProg);
}
