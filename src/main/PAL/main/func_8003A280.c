#include "common.h"
#include "game/car.h"

extern u8 D_801F1854[];
extern u8 D_8009E744[];
extern s32 D_801E40D8;
extern s32 D_801E42E4;
extern s32 D_8009E778;

void func_8003A280(GameCarRuntime *car, s32 arg1) {
    register u8 *state asm("$8") = (u8 *)&car->field_BC;
    register s32 acc8 asm("$24") = 0;
    register s32 acc9 asm("$25") = 0;
    register s32 i asm("$10") = 0;
    register s32 k11 asm("$17") = 0xB;
    register u8 *base asm("$16") = D_8009E744;
    register s32 carProgress asm("$22");
    register s32 carField34 asm("$15");
    register s32 carA4low asm("$21");
    register u8 *block asm("$11");
    register s32 track asm("$12");
    s32 t6;
    s32 field34minus;
    s32 field34plus;
    register s32 trackMinus asm("$18");
    s32 total;
    s32 sums[4];

    carProgress = car->trackProgress;
    carField34 = car->field_34;
    carA4low = (u16)car->field_A4;
    block = D_801F1854 + 0xA4;
    car->field_120 = 0;
    car->field_10C = 0;
    sums[3] = 0;
    sums[2] = 0;
    sums[1] = 0;
    sums[0] = 0;
    track = D_801E40D8;
    {
        s32 tmp = car->field_A4 << 1;
        t6 = tmp + 0xC00;
    }
    field34minus = (s16)(carField34 - 0x30);
    field34plus = (s16)(carField34 + 0x30);
    trackMinus = track - 0x400;

    for (; i < 12; i++, block += 0x19C) {
        s32 otherField34;
        s32 otherA4;
        s32 a2;
        register s32 t1 asm("$9");
        s32 diff;
        register s32 angleDiff asm("$3");
        s32 angleSaved;
        s16 bucket;
        s32 val;

        if (D_801E42E4 != 0xC && i == k11) {
            break;
        }
        if (i == arg1) {
            continue;
        }
        if (*(s16 *)(block + 8) == -1) {
            continue;
        }

        if (i == k11) {
            register s32 op asm("$2") = *(s32 *)(base + 0);
            a2 = op + track;
            otherField34 = *(s32 *)(base - 0x3C);
            if (arg1 >= 4) {
                goto loadA4;
            }
            otherA4 = 0;
            goto afterA4;
        loadA4:
            otherA4 = *(u16 *)(base + 0x34);
        afterA4:
            t1 = 0;
            t6 = 0x1800 - (D_8009E778 << 1);
        } else {
            register s32 op asm("$2");
            otherField34 = *(s32 *)(block - 0x70);
            otherA4 = *(u16 *)block;
            op = *(s32 *)(block - 0x34);
            a2 = op + track;
            t1 = *(u16 *)(state + 0x48);
        }

        diff = (a2 - carProgress) % track;
        angleDiff = otherField34 - carField34;
        angleSaved = angleDiff;
        __asm__("" : "=r"(angleSaved) : "0"(angleSaved));
        otherA4 -= carA4low;

        if (diff > 0 && diff < t6) {
            (*(u16 *)(state + 0x50))++;
            if (field34minus < otherField34 && otherField34 < field34plus) {
                if (!((s16)otherA4 > 0 && t1 == 0)) {
                    *(s16 *)(state + 0x48) = 1;
                    val = angleDiff + 0x30;
                    if (val < 0) {
                        val = angleDiff + 0x4F;
                    }
                    bucket = val >> 5;
                    if (i == k11) {
                        if (diff < 0xC00) {
                            s32 s = sums[bucket] + 0xC00;
                            sums[bucket] = s - diff;
                        } else {
                            sums[bucket] = sums[bucket] + 0xC00;
                        }
                    } else {
                        sums[bucket] = (t6 - diff) + sums[bucket];
                    }
                }
            }
            if (diff < 0x200) {
                s32 ad = (s16)angleSaved;
                if (ad >= 0x41) {
                    register s32 s asm("$2") = acc9 + 0xC00;
                    acc9 = s - diff;
                } else if (ad < -0x40) {
                    register s32 s asm("$2") = acc8 + 0xC00;
                    acc8 = s - diff;
                }
            }
        } else {
            if (trackMinus < diff) {
                (*(u16 *)(state + 0x50))++;
            }
        }
    }

    total = sums[0] + sums[1] + sums[2];
    sums[3] = total;
    if (total > 0) {
        if (acc8 == 0 && carField34 >= 0x51) {
            s32 f104 = *(s16 *)(state + 0x48);
            *(s16 *)(state + 0x62) = -0x50;
            *(s16 *)(state + 0x64) = -8 - (f104 << 1);
        } else if (acc9 == 0 && carField34 < -0x50) {
            s32 f104 = *(s16 *)(state + 0x48);
            *(s16 *)(state + 0x62) = 0x50;
            *(s16 *)(state + 0x64) = (f104 << 1) + 8;
        } else if (sums[0] <= sums[1] && sums[0] <= sums[2] && acc8 == 0) {
            s32 f104 = *(s16 *)(state + 0x48);
            *(s16 *)(state + 0x62) = -0x50;
            *(s16 *)(state + 0x64) = -6 - (f104 << 1);
        } else if (sums[2] <= sums[1] && sums[2] <= sums[0] && acc9 == 0) {
            s32 f104 = *(s16 *)(state + 0x48);
            *(s16 *)(state + 0x62) = 0x50;
            *(s16 *)(state + 0x64) = (f104 << 1) + 6;
        }
        __asm__("" : : "r"(carField34));
        if (sums[3] >= 0x3E9) {
            register s32 fv asm("$3") = *(s16 *)(state + 0x74);
            register s32 d asm("$2") = ((fv << 4) - fv) << 1;
            *(s16 *)(state + 0x74) = d / 100;
        }
    } else {
        *(s16 *)(state + 0x64) = 0;
        *(s16 *)(state + 0x48) = 0;
        *(s16 *)(state + 0x62) = *(u16 *)(state + 0x60);
    }

    *(s16 *)(state + 0x60) = *(u16 *)(state + 0x60) + *(u16 *)(state + 0x64);
}
