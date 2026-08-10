#include "common.h"
#include <stdio.h>
#include "game/screens.h"
#include "game/race.h"
#include "game/render.h"
#include "game/save_internal.h"
#include "game/records_internal.h"

void InitRecordTables(void) {
    RaceRecordAddress recordAddress;
    RaceRecordAddress r2;
    RaceRecordAddress r3;
    RaceRecordAddress r4;
    register RaceRecordAddress r5 asm("$5");
    s32 r6;
    register s32 r7 asm("$7");
    RaceRecordAddress r8;
    RaceRecordAddress r9;
    RaceRecordAddress r10;
    s32 r11;
    s32 r12;
    s32 r13;
    register s32 r14 asm("$14");
    s32 r15;
    RaceRecordAddress r16;
    RaceRecordAddress r17;
    register RaceRecordAddress r18 asm("$18");
    s32 r19;
    s32 r20;
    register s32 r21 asm("$21");
    RaceRecordAddress rankingBaseAddress;
    register s32 r24 asm("$24");
    register s32 r25 asm("$25");
    register s32 r23 asm("$23");
    s32 r30;

    r14 = 0;
    r18.wordPointer = &g_DefaultLapTimes;
    r17.wordPointer = &g_BestTotalTimes[0][0][0];
    r16.wordPointer = &g_BestLapTimes[0][0][0];
    do {
        r7 = 0;
        r15 = r14 * 32;
        r12 = r14 << 4;
        r11 = r16.byteOffset;
        do {
            r5.value = 0;
            r13 = r7 * 8;
            r8.byteOffset = r11;
            do {
                r4.value = 0;
                r10.byteOffset = r8.byteOffset;
                r2.byteOffset = r12 + r18.byteOffset;
                r6 = r7 * 4;
                r9.byteOffset = r6 + r2.byteOffset;
                r3.byteOffset = r5.value * 4;
                r2.byteOffset = r15 + r17.byteOffset;
                r2.byteOffset = r13 + r2.byteOffset;
                r3.byteOffset = r3.byteOffset + r2.byteOffset;
                do {
                    r2.value = *r9.wordPointer;
                    *r10.wordPointer = r2.value;
                    r2.wordPointer = &g_DefaultTotalTimes;
                    r2.byteOffset = r12 + r2.byteOffset;
                    r2.byteOffset = r6 + r2.byteOffset;
                    r2.value = *r2.wordPointer;
                    r4.value++;
                    *r3.wordPointer = r2.value;
                } while (r4.value < 4);
                r5.value++;
                r8.wordPointer++;
            } while (r5.value < 2);
            r7++;
            r11 += 8;
        } while (r7 < 4);
        r14++;
        r16.wordPointer += 8;
    } while (r14 < 2);

    r14 = 0;
    rankingBaseAddress.pointer = &g_RankingRecords[0][0][0];
    r30 = rankingBaseAddress.byteOffset + 4;
    r20 = 0;
    do {
        r7 = 0;
        r16.byteOffset = r20;
        r15 = 0;
        do {
            r6 = 0;
            r19 = r15;
            r18.pointer = &g_TimeRecords[0][0][0];
            r21 = r18.byteOffset + 4;
            r25 = r7 * 4;
            __asm__("" : "=r"(r25) : "0"(r25));
            r2.wordPointer = &g_DefaultLapTimes;
            r24 = r14 << 4;
            r2.byteOffset = r24 + r2.byteOffset;
            r17.byteOffset = r25 + r2.byteOffset;
            r10.halfwordPointer = &g_DefaultRecordCars;
            r13 = 0;
            r12 = 0;
            r11 = r15;
            r9.wordPointer = &g_DefaultRecordTimes;
            r8.wordPointer = &g_DefaultRecordRows;
            do {
                r5.byteOffset = r11 + r16.byteOffset;
                r11 += 0x10;
                r4.byteOffset = r6 * 16;
                r6++;
                r2.byteOffset = r16.byteOffset + r19;
                r4.byteOffset = r4.byteOffset + r2.byteOffset;
                r3.value = *r8.wordPointer;
                r2.byteOffset = r4.byteOffset + rankingBaseAddress.byteOffset;
                *r2.wordPointer = r3.value;
                r3.value = *r9.wordPointer;
                r2.byteOffset = r4.byteOffset + r30;
                *r2.wordPointer = r3.value;
                r3.value = *r8.wordPointer;
                r8.wordPointer += 3;
                r2.byteOffset = r4.byteOffset + r18.byteOffset;
                *r2.wordPointer = r3.value;
                r2.value = *r9.wordPointer;
                r4.byteOffset = r4.byteOffset + r21;
                *r4.wordPointer = r2.value;
                r2.value = *r17.wordPointer;
                r9.wordPointer += 3;
                r2.value = r12 + r2.value;
                recordAddress.pointer = &g_RankingRecords[0][0][0];
                recordAddress.byteOffset += r5.byteOffset;
                recordAddress.pointer->raceTime = r2.value;
                r2.wordPointer = &g_DefaultTotalTimes;
                r2.byteOffset = r24 + r2.byteOffset;
                r2.byteOffset = r25 + r2.byteOffset;
                r2.value = *r2.wordPointer;
                r12 += 0x7D0;
                r2.value = r13 + r2.value;
                recordAddress.pointer = &g_TimeRecords[0][0][0];
                recordAddress.byteOffset += r5.byteOffset;
                recordAddress.pointer->raceTime = r2.value;
                r2.value = *r10.halfwordPointer;
                r13 += 0x2710;
                recordAddress.pointer = &g_RankingRecords[0][0][0];
                recordAddress.byteOffset += r5.byteOffset;
                recordAddress.pointer->carIndex = r2.value;
                r2.value = *r10.halfwordPointer;
                recordAddress.pointer = &g_TimeRecords[0][0][0];
                recordAddress.byteOffset += r5.byteOffset;
                recordAddress.pointer->carIndex = r2.value;
                r10.halfwordPointer += 6;
            } while (r6 < 5);
            r7++;
            r15 += 0x50;
        } while (r7 < 4);
        r14++;
        r20 += 0x140;
    } while (r14 < 2);

    r14 = 0;
    r8.wordPointer = &g_DefaultLapTimes;
    r9.wordPointer = &g_BestSectorTimes[0][0][0];
    do {
        r7 = 0;
        r4.byteOffset = r9.byteOffset;
        do {
            r6 = 0;
            r2.value = r7 * 4;
            r5.byteOffset = r2.value + r8.byteOffset;
            r3.byteOffset = r4.byteOffset;
            do {
                r2.value = *r5.wordPointer;
                r6++;
                *r3.wordPointer = r2.value;
                r3.wordPointer++;
            } while (r6 < 3);
            r7++;
            r4.wordPointer += 3;
        } while (r7 < 4);
        r8.wordPointer += 4;
        r14++;
        r9.wordPointer += 12;
    } while (r14 < 2);

    __asm__("" : : "r"(r23));
}

void *FormatLapTime(void *dst, s32 value) {
    s32 minutes = value / 60000;
    s32 ticks = value / 1000;
    s32 seconds = ticks - (minutes * 60);
    s32 fraction = value - (ticks * 1000);

    sprintf(dst, g_FmtLapTime, minutes, seconds, fraction);
    return dst;
}

void DrawCourseIntro(void) {
    DrawProportionalText(0x10, 0x1C, g_TextTimeAttack, 0x7812);
    DrawText8x8Trans(0x10, 0x39, g_TextCourseIn, 0x78CC);
    DrawResultScreen();
}
