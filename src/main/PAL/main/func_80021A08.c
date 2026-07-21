#include "common.h"

extern s32 D_8007D444;
extern s32 D_8007D464;
extern s32 D_8007D484;
extern s32 D_8007D488;
extern u16 D_8007D48C;
extern s32 D_801E4408;
extern s32 D_8019C70C;
extern s32 D_801E7744;
extern s32 D_801E774C;
extern u16 D_801E7750;
extern s32 D_8019CB78;
extern s32 D_8019CB80;
extern u16 D_8019CB84;
extern s32 D_801E41E8;

void func_80021A08(void) {
    register s32 r2 asm("$2");
    register s32 r3 asm("$3");
    register s32 r4 asm("$4");
    register s32 r5 asm("$5");
    register s32 r6 asm("$6");
    register s32 r7 asm("$7");
    register s32 r8 asm("$8");
    register s32 r9 asm("$9");
    register s32 r10 asm("$10");
    register s32 r11 asm("$11");
    register s32 r12 asm("$12");
    register s32 r13 asm("$13");
    register s32 r14 asm("$14");
    register s32 r15 asm("$15");
    register s32 r16 asm("$16");
    register s32 r17 asm("$17");
    register s32 r18 asm("$18");
    register s32 r19 asm("$19");
    register s32 r20 asm("$20");
    register s32 r21 asm("$21");
    register s32 r22 asm("$22");
    register s32 r24 asm("$24");
    register s32 r25 asm("$25");
    register s32 r23 asm("$23");
    s32 r30;

    /* section 1 */
    r14 = 0;
    r18 = (s32)&D_8007D444;
    r17 = (s32)&D_8019C70C;
    r16 = (s32)&D_801E4408;
    do {
        r7 = 0;
        r15 = r14 << 5;
        r12 = r14 << 4;
        r11 = r16;
        do {
            r5 = 0;
            r13 = r7 << 3;
            r8 = r11;
            do {
                r4 = 0;
                r10 = r8;
                r2 = r12 + r18;
                r6 = r7 << 2;
                r9 = r6 + r2;
                r3 = r5 << 2;
                r2 = r15 + r17;
                r2 = r13 + r2;
                r3 = r3 + r2;
                do {
                    r2 = *(s32 *)r9;
                    *(s32 *)r10 = r2;
                    r2 = (s32)&D_8007D464;
                    __asm__("" : "=r"(r2) : "0"(r2));
                    r2 = r12 + r2;
                    r2 = r6 + r2;
                    r2 = *(s32 *)r2;
                    r4++;
                    *(s32 *)r3 = r2;
                } while (r4 < 4);
                r5++;
                r8 += 4;
            } while (r5 < 2);
            r7++;
            r11 += 8;
        } while (r7 < 4);
        r14++;
        r16 += 0x20;
    } while (r14 < 2);

    /* section 2 */
    r14 = 0;
    r22 = (s32)&D_801E7744;
    r30 = r22 + 4;
    r20 = 0;
    do {
        r7 = 0;
        r16 = r20;
        r15 = 0;
        do {
            r6 = 0;
            r19 = r15;
            r18 = (s32)&D_8019CB78;
            r21 = r18 + 4;
            r25 = r7 << 2;
            __asm__("" : "=r"(r25) : "0"(r25));
            r2 = (s32)&D_8007D444;
            r24 = r14 << 4;
            r2 = r24 + r2;
            r17 = r25 + r2;
            r10 = (s32)&D_8007D48C;
            r13 = 0;
            r12 = 0;
            r11 = r15;
            r9 = (s32)&D_8007D488;
            r8 = (s32)&D_8007D484;
            do {
                r5 = r11 + r16;
                r11 += 0x10;
                r4 = r6 << 4;
                r6++;
                r2 = r16 + r19;
                r4 = r4 + r2;
                r3 = *(s32 *)r8;
                r2 = r4 + r22;
                *(s32 *)r2 = r3;
                r3 = *(s32 *)r9;
                r2 = r4 + r30;
                *(s32 *)r2 = r3;
                r3 = *(s32 *)r8;
                r8 += 0xC;
                r2 = r4 + r18;
                *(s32 *)r2 = r3;
                r2 = *(s32 *)r9;
                r4 = r4 + r21;
                *(s32 *)r4 = r2;
                r2 = *(s32 *)r17;
                r9 += 0xC;
                r2 = r12 + r2;
                *(s32 *)((s32)&D_801E774C + r5) = r2;
                r2 = r24 + (s32)&D_8007D464;
                r2 = r25 + r2;
                r2 = *(s32 *)r2;
                r12 += 0x7D0;
                r2 = r13 + r2;
                *(s32 *)((s32)&D_8019CB80 + r5) = r2;
                r2 = *(u16 *)r10;
                r13 += 0x2710;
                *(u16 *)((s32)&D_801E7750 + r5) = r2;
                r2 = *(u16 *)r10;
                *(u16 *)((s32)&D_8019CB84 + r5) = r2;
                r10 += 0xC;
            } while (r6 < 5);
            r7++;
            r15 += 0x50;
        } while (r7 < 4);
        r14++;
        r20 += 0x140;
    } while (r14 < 2);

    /* section 3 */
    r14 = 0;
    r8 = (s32)&D_8007D444;
    r9 = (s32)&D_801E41E8;
    do {
        r7 = 0;
        r4 = r9;
        do {
            r6 = 0;
            r2 = r7 << 2;
            r5 = r2 + r8;
            r3 = r4;
            do {
                r2 = *(s32 *)r5;
                r6++;
                *(s32 *)r3 = r2;
                r3 += 4;
            } while (r6 < 3);
            r7++;
            r4 += 0xC;
        } while (r7 < 4);
        r8 += 0x10;
        r14++;
        r9 += 0x30;
    } while (r14 < 2);

    __asm__("" : : "r"(r23));
}
