#include "common.h"
#include "game/screens.h"

extern s32 g_DefaultLapTimes asm("D_8007D444");
extern s32 g_DefaultTotalTimes asm("D_8007D464");
extern s32 g_DefaultRecordRows asm("D_8007D484");
/* Split symbols of g_DefaultRecordRows, whose records are 12 bytes: the name
 * at +0, the seed time at +4 and the car index at +8. */
extern s32 g_DefaultRecordTimes asm("D_8007D488");
extern u16 g_DefaultRecordCars asm("D_8007D48C");
extern s32 g_BestLapTimes asm("D_801E4408");
extern s32 g_BestTotalTimes asm("D_8019C70C");
extern s32 g_RankingRecords asm("D_801E7744");
extern s32 g_RankingTimes asm("D_801E774C");
extern u16 g_RankingCars asm("D_801E7750");
extern s32 g_TimeRecords asm("D_8019CB78");
extern s32 g_TimeRecordTimes asm("D_8019CB80");
extern u16 g_TimeRecordCars asm("D_8019CB84");
extern s32 g_BestSectorTimes asm("D_801E41E8");
extern char g_FmtLapTime[] asm("D_80010EB4");
void LibcSprintf(void *dst, void *fmt, s32 arg0, s32 arg1, s32 arg2) asm("func_800632F0");
extern char g_TextTimeAttack[] asm("D_80010EC4");
extern char g_TextCourseIn[] asm("D_80010ED0");
void func_80016EA0(s32 arg0, s32 arg1, char *arg2, s32 arg3);
void func_80016A18(s32 arg0, s32 arg1, char *arg2, s32 arg3);
void func_800200D0(void);

void GameInitRecordTables(void) asm("func_80021A08");
void GameInitRecordTables(void) {
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
    r18 = (s32)&g_DefaultLapTimes;
    r17 = (s32)&g_BestTotalTimes;
    r16 = (s32)&g_BestLapTimes;
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
                    r2 = (s32)&g_DefaultTotalTimes;
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
    r22 = (s32)&g_RankingRecords;
    r30 = r22 + 4;
    r20 = 0;
    do {
        r7 = 0;
        r16 = r20;
        r15 = 0;
        do {
            r6 = 0;
            r19 = r15;
            r18 = (s32)&g_TimeRecords;
            r21 = r18 + 4;
            r25 = r7 << 2;
            __asm__("" : "=r"(r25) : "0"(r25));
            r2 = (s32)&g_DefaultLapTimes;
            r24 = r14 << 4;
            r2 = r24 + r2;
            r17 = r25 + r2;
            r10 = (s32)&g_DefaultRecordCars;
            r13 = 0;
            r12 = 0;
            r11 = r15;
            r9 = (s32)&g_DefaultRecordTimes;
            r8 = (s32)&g_DefaultRecordRows;
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
                *(s32 *)((s32)&g_RankingTimes + r5) = r2;
                r2 = r24 + (s32)&g_DefaultTotalTimes;
                r2 = r25 + r2;
                r2 = *(s32 *)r2;
                r12 += 0x7D0;
                r2 = r13 + r2;
                *(s32 *)((s32)&g_TimeRecordTimes + r5) = r2;
                r2 = *(u16 *)r10;
                r13 += 0x2710;
                *(u16 *)((s32)&g_RankingCars + r5) = r2;
                r2 = *(u16 *)r10;
                *(u16 *)((s32)&g_TimeRecordCars + r5) = r2;
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
    r8 = (s32)&g_DefaultLapTimes;
    r9 = (s32)&g_BestSectorTimes;
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

void *GameFormatLapTime(void *dst, s32 value) {
    s32 minutes = value / 60000;
    s32 ticks = value / 1000;
    s32 seconds = ticks - (minutes * 60);
    s32 fraction = value - (ticks * 1000);

    LibcSprintf(dst, g_FmtLapTime, minutes, seconds, fraction);
    return dst;
}

void GameDrawCourseIntro(void) {
    func_80016EA0(0x10, 0x1C, g_TextTimeAttack, 0x7812);
    func_80016A18(0x10, 0x39, g_TextCourseIn, 0x78CC);
    func_800200D0();
}
