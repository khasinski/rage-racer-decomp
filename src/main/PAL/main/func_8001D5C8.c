#include "common.h"

/*
 * Members declare these three halfwords as u16, s16 and volatile u16;
 * reconciled to the most specific form.  Only func_8001D748 and
 * func_8001D900 read them, the others just store.
 */
extern volatile u16 D_8019CB38;
extern volatile u16 D_8019CB3A;
extern volatile u16 D_8019CB3C;


void func_8001D5C8(u32 arg0, u32 arg1) {
    u32 a;
    u32 b;

    a = arg0 >> 1;
    b = arg1 >> 1;
    a &= 0x3DEF;
    b &= 0x3DEF;
    D_8019CB38 = (a + b) | 0x8000;
}

/*
 * Exact PAL 0x0DDF0 wrapper. The best non-wrapper C proposal is still
 * target-sized but differs in late /3 quotient register allocation.
 */

void func_8001D5F0(u32 arg0, u32 arg1) {
    __asm__ volatile(
        ".set   noat\n"
        "andi   $11,$4,0x1F\n"
        "andi   $12,$5,0x1F\n"
        "lui    $6,0x5555\n"
        "ori    $6,$6,0x5556\n"
        "addu   $9,$11,$12\n"
        "sll    $7,$9,1\n"
        "mult   $7,$6\n"
        "andi   $4,$4,0xFFFF\n"
        "srl    $2,$4,5\n"
        "andi   $10,$2,0x1F\n"
        "andi   $5,$5,0xFFFF\n"
        "srl    $2,$5,5\n"
        "andi   $3,$2,0x1F\n"
        "mfhi   $11\n"
        "addu   $8,$10,$3\n"
        "sll    $3,$8,1\n"
        "mult   $3,$6\n"
        "srl    $4,$4,10\n"
        "andi   $13,$4,0x1F\n"
        "srl    $5,$5,10\n"
        "andi   $2,$5,0x1F\n"
        "mfhi   $10\n"
        "addu   $4,$13,$2\n"
        "sll    $2,$4,1\n"
        "mult   $2,$6\n"
        "mfhi   $5\n"
        "nop\n"
        "nop\n"
        "mult   $9,$6\n"
        "mfhi   $12\n"
        "nop\n"
        "nop\n"
        "mult   $8,$6\n"
        "sra    $7,$7,31\n"
        "subu   $11,$11,$7\n"
        "sra    $3,$3,31\n"
        "subu   $10,$10,$3\n"
        "sra    $2,$2,31\n"
        "sll    $3,$10,5\n"
        "mfhi   $7\n"
        "subu   $13,$5,$2\n"
        "sll    $2,$13,10\n"
        "mult   $4,$6\n"
        "addu   $2,$2,$3\n"
        "addu   $2,$2,$11\n"
        "addiu  $5,$0,-0x8000\n"
        "addu   $2,$2,$5\n"
        "sra    $9,$9,31\n"
        "lui    $1,%hi(D_8019CB38)\n"
        "sh     $2,%lo(D_8019CB38)($1)\n"
        "subu   $12,$12,$9\n"
        "sra    $8,$8,31\n"
        "subu   $3,$7,$8\n"
        "sra    $4,$4,31\n"
        "sll    $3,$3,5\n"
        "mfhi   $2\n"
        "subu   $2,$2,$4\n"
        "sll    $2,$2,10\n"
        "addu   $2,$2,$3\n"
        "addu   $2,$2,$12\n"
        "addu   $2,$2,$5\n"
        "lui    $1,%hi(D_8019CB3A)\n"
        "sh     $2,%lo(D_8019CB3A)($1)\n"
        ".set   at\n"
    );
}


void func_8001D6F4(u32 arg0, u32 arg1) {
    u32 a;
    u32 b;
    u32 c;
    u32 high;
    s32 bias;

    a = arg0 >> 1;
    b = arg1 >> 1;
    a &= 0x3DEF;
    b &= 0x3DEF;
    c = a + b;
    high = c;
    bias = 0x8000;
    high += bias;
    c >>= 1;
    c &= 0x3DEF;
    a += c;
    /* Preserve the bias while keeping its two GCC live ranges separate. */
    a -= bias++;
    c += b;
    c -= --bias;

    D_8019CB3A = high;
    D_8019CB38 = a;
    D_8019CB3C = c;
}

/*
 * func_8001D748.
 *
 * The store to raw+0x7162 written just before the call is scheduled by the
 * compiler into the jal delay slot; no hand-written asm is needed.
 */

extern u16 D_8007D30C[];
extern u16 D_8007D330[];
extern volatile u16 D_8007D354[];
extern volatile u16 D_8007D378[];

void func_8001D5C8(u32 arg0, u32 arg1);
void func_8001D5F0(u32 arg0, u32 arg1);
void func_8001D6F4(u32 arg0, u32 arg1);

void func_8001D748(u32 arg0, u32 arg1) {
    register u32 raw asm("$2");
    register u16 *base asm("$16");
    register u16 s1 asm("$17");
    register u16 s2 asm("$18");
    volatile u16 *idx;
    volatile u16 *color;
    u16 c;
    s32 i;

    raw = arg1;
    arg0 <<= 1;
    base = (u16 *)(raw + 0x7060);
    __asm__("" : "=r"(raw), "=r"(base), "=r"(arg0) : "0"(raw), "1"(base), "2"(arg0));
    s1 = *(u16 *)((u8 *)D_8007D30C + arg0);
    s2 = *(u16 *)((u8 *)D_8007D330 + arg0);

    *(u16 *)(raw + 0x7162) = s1;
    func_8001D5C8(s1, s2);

    i = 0;
    color = &D_8019CB38;
    idx = D_8007D354;
    for (; i < 9; idx++) {
        i++;
        base[idx[0] + 0] = s1;
        base[idx[0] + 1] = color[0];
        base[idx[0] + 2] = s2;
    }

    func_8001D5F0(s1, s2);

    i = 0;
    color = &D_8019CB38;
    idx = D_8007D378;
    for (; i < 4; idx++) {
        i++;
        base[idx[0] + 0] = s1;
        base[idx[0] + 1] = color[0];
        base[idx[0] + 2] = color[1];
        base[idx[0] + 3] = s2;
    }

    func_8001D6F4(s1, s2);

    ((volatile u16 *)base)[0x2C1] = s1;
    ((volatile u16 *)base)[0x2C2] = D_8019CB38;
    ((volatile u16 *)base)[0x2C3] = D_8019CB3A;
    c = D_8019CB3C;
    ((volatile u16 *)base)[0x2C5] = s2;
    ((volatile u16 *)base)[0x2C4] = c;
}

extern u32 D_8009E698;
extern u32 g_CarModelSlot asm("D_8009E87C");

void func_8001D748(u32 arg0, u32 arg1);
void func_80017B5C(u32 arg0);

void func_8001D8C4(u32 arg0) {
    func_8001D748(arg0, *(u32 *)(D_8009E698 + 0x24));
    func_80017B5C(g_CarModelSlot);
}

extern u16 D_8007D30C[];
extern u16 D_8007D330[];
extern volatile u16 D_8007D368[];
extern volatile u16 D_8007D378[];

void func_8001D5C8(u32 arg0, u32 arg1);
void func_8001D5F0(u32 arg0, u32 arg1);
void func_8001D6F4(u32 arg0, u32 arg1);

void func_8001D900(u32 arg0, u32 arg1) {
    u16 *base;
    u16 s1;
    u16 s2;
    volatile u16 *idx;
    volatile u16 *color;
    u16 c;
    s32 i;

    base = (u16 *)(arg1 + 0x7060);
    s1 = D_8007D30C[arg0];
    s2 = D_8007D330[arg0];

    func_8001D5C8(s1, s2);

    i = 0;
    color = &D_8019CB38;
    idx = D_8007D368;
    for (; i < 8; idx++) {
        i++;
        base[idx[0] + 3] = s1;
        base[idx[0] + 4] = color[0];
        base[idx[0] + 5] = s2;
    }

    func_8001D5F0(s1, s2);

    i = 0;
    color = &D_8019CB38;
    idx = D_8007D378;
    for (; i < 4; idx++) {
        i++;
        base[idx[0] + 4] = s1;
        base[idx[0] + 5] = color[0];
        base[idx[0] + 6] = color[1];
        base[idx[0] + 7] = s2;
    }

    func_8001D6F4(s1, s2);

    ((volatile u16 *)base)[0x2C6] = s1;
    ((volatile u16 *)base)[0x2C7] = D_8019CB38;
    ((volatile u16 *)base)[0x2C8] = D_8019CB3A;
    c = D_8019CB3C;
    ((volatile u16 *)base)[0x2CA] = s2;
    ((volatile u16 *)base)[0x2C9] = c;
}

extern u32 D_8009E698;
extern u32 g_CarModelSlot asm("D_8009E87C");

void func_8001D900(u32 arg0, u32 arg1);
void func_80017B5C(u32 arg0);

void func_8001DA74(u32 arg0) {
    func_8001D900(arg0, *(u32 *)(D_8009E698 + 0x24));
    func_80017B5C(g_CarModelSlot);
}
