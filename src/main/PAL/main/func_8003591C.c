#include "common.h"

extern s32 D_8009AF80[];
extern s32 D_8009AF84;
extern s32 D_8009AF88;
extern s32 D_8009AF90;
extern s32 D_8009AF94;
extern s32 D_8009AF98;
extern s32 D_8009AFA0;
extern s16 D_8009E6A0;
extern u8 D_8009E6D4;
extern void *D_8009E67C;
extern s32 D_8009E73C;
extern s32 D_8009E740;
extern s16 D_8009E83C;
extern s32 D_8019C70C[][4][2];
extern s32 D_8019C8EC;
extern s16 D_8019CA10;
extern s32 D_801E408C;
extern s32 D_801E40D8;
extern s32 D_801E428C;
extern s32 D_801E4364;
extern s16 D_801E43FC;
extern s32 D_801E4408[][4][2];
extern s32 D_801E41E8[][4][3];
extern s32 D_801E41EC;
extern s32 D_801E41F0;
extern s32 D_801E4BA8;
extern s32 D_801E4BCC;
extern s32 D_801E4D64;
extern s16 D_801E4DAC;
extern s16 D_801E6C90;
extern s16 D_801E6E74;
extern s32 D_801E774C[][4][20];
extern s32 D_801E7A50;
extern s16 D_801E8A8C;

void func_8001FA70(void);
void func_800207E0(void);
void func_80033AA0(s32 arg0, s32 arg1);
s32 func_80034F18(s32 arg0, s32 arg1);
void func_80035258(s32 arg0);
void func_8003CA14(void *arg0);
void func_80041170(void);
void func_80042BC0(s32 arg0);
void func_80042BF0(void);
void func_80042CCC(s32 arg0);
void func_8005D6EC(s32 arg0);
void func_8005E4A4(s32 arg0);
s32 func_800632B0(void);

s32 func_8003591C(void *arg0, s32 arg1) {
    s32 value;
    s32 result;
    s16 recordIndex;
    s32 candidateTime;
    s32 count;
    u16 returnValue;
    s16 progress;
    s32 step;
    s32 tableOffset;
    s32 routeOffset;
    s32 routeCallOffset;
    s32 routeStoreOffset;
    s32 routeCompareOffset;
    s32 routeFinalOffset;
    s32 recordOffset;
    s32 resultOffset;
    s32 routeProgress;
    s32 oldTimer;
    s32 timer;
    u8 *route;

    /* Match note: GCC reserves the original 0x48 frame only when it sees
       this unevaluated seven-argument call shape. It emits no instructions. */
    if (0) func_8001674C((char *)0, 0, 0, 0, 0, 0, 0);

    route = (u8 *)arg0 + 0xBC;
    if (*(s16 *)((u8 *)arg0 + 0x168) > 0) {
        if (D_801E4364 >= *(s16 *)((u8 *)arg0 + 0x168)) {
            routeOffset = *(s16 *)((u8 *)arg0 + 0x168) * 4;
            *(s32 *)((s32)route + routeOffset + 0xAC) += 1;
            routeOffset = *(s16 *)((u8 *)arg0 + 0x168) * 4;
            if (*(s32 *)((s32)route + routeOffset + 0xAC) > 0xFFFF) {
                *(s32 *)(route + 0xAC +
                         *(s16 *)((u8 *)arg0 + 0x168) * 4) = 0x10000;
            }
            *(s32 *)((s32)route +
                     (routeStoreOffset =
                          *(s16 *)((u8 *)arg0 + 0x168) * 4) +
                     0xC4) = func_80034F18(
                (routeCallOffset = *(s16 *)((u8 *)arg0 + 0x168) * 4,
                 *(s32 *)((s32)route + routeCallOffset + 0xAC)),
                func_800632B0() % 40);
            routeCompareOffset = *(s16 *)((u8 *)arg0 + 0x168) * 4;
            if (*(s32 *)((s32)route + routeCompareOffset + 0xC4) > 0x927BE) {
                *(s32 *)(route + 0xC4 +
                         *(s16 *)((u8 *)arg0 + 0x168) * 4) = 0x927BF;
                D_8009AFA0 = 1;
            }
            routeFinalOffset = *(s16 *)((u8 *)arg0 + 0x168) * 4;
            D_801E4D64 =
                *(s32 *)((s32)route + routeFinalOffset + 0xC4);
            goto update_progress;
        }
        goto update_best;
    }
    if (D_801E4364 < *(s16 *)((u8 *)arg0 + 0x168)) {
update_best:
        if (D_801E4BA8 <
            D_8019C70C[D_801E408C][D_801E428C][arg1]) {
            D_8019C70C[D_801E408C][D_801E428C][arg1] = D_801E4BA8;
        }
    }

update_progress:
    progress = *(s16 *)(route + 0xAC);
    if (progress * D_801E40D8 <= D_8009E740 + D_8009E73C) {
        s32 progressLimit;

        progressLimit = D_801E4364;
        if (progress > progressLimit) {
            goto progress_failed;
        }
        returnValue = 1;
        *(s16 *)(route + 0xAC) = progress + 1;
        D_8009AFA0 = 0;
        D_801E7A50 &= 0xF;
        if (D_801E6C90 == 0) {
            D_801E6C90 = 2;
        }
        recordIndex = *(s16 *)(route + 0xAC);
        progressLimit = D_801E4BCC;
        recordOffset = recordIndex * 4;
        candidateTime =
            *(s32 *)((s32)route + recordOffset + 0xC0);
        tableOffset = progressLimit;
        step = candidateTime < tableOffset;
        if (step && (recordIndex != 1)) {
            routeProgress = *(u16 *)(route + 0xAC);
            *(s16 *)(route + 0xA6) = routeProgress - 2;
            resultOffset = *(s16 *)(route + 0xAC) * 4;
            result =
                *(s32 *)((s32)route + resultOffset + 0xC0);
            D_801E4BCC = candidateTime;
            D_8009AF88 = result;
            if (arg1 == 0) {
                D_8009AF98 = result;
                D_8009AF90 = D_8009AF80[0];
                D_8009AF94 = D_8009AF84;
            }

            if (D_801E4364 < *(s16 *)(route + 0xAC)) {
                goto record_done;
            }
            func_8005D6EC(0x26);
            D_801E6C90 = 0x96;
        }

record_done:
        count = D_801E4364;
        step = *(s16 *)(route + 0xAC);
        if (step == count + 1) {
                if (*(s16 *)(route + 0xA4) < 4) {
                    {
                        s32 *cursor;
                        s32 element;
                        s32 accumulated;

                        result = 0;
                        if (count > 0) {
                            cursor = (s32 *)route;
                            do {
                                element = cursor[0x32];
                                accumulated = D_801E4BA8;
                                accumulated += element;
                                D_801E4BA8 = accumulated;
                                cursor++;
                            } while (++result < count);
                        }
                    }
                if (D_801E4BA8 > 0x927BE) {
                    D_801E4BA8 = 0x927BF;
                }
                if (D_801E4408[D_801E408C][D_801E428C][arg1] >
                    D_801E4BCC) {
                    D_801E4408[D_801E408C][D_801E428C][arg1] = D_801E4BCC;
                }
                if (arg1 == 0) {
                    tableOffset = D_801E428C * 12 + D_801E408C * 48;
                    *(s32 *)((u8 *)D_801E41E8 + tableOffset) = D_8009AF90;
                    *(s32 *)((u8 *)&D_801E41EC + tableOffset) =
                        D_8009AF94;
                    *(s32 *)((u8 *)&D_801E41F0 + tableOffset) =
                        D_8009AF98;
                }
                D_801E6E74 = 4;
                func_80042CCC(8);
                func_8005D6EC(0x2B);
                goto reset_transition;
            }

            D_801E6E74 = 5;
            func_8003CA14(&D_8009E6D4);
            func_80042CCC(0x3C);
            if (*(s16 *)((u8 *)D_8009E67C + 6) != 0) {
                func_8005D6EC(0x3D);
            }
reset_transition:
            func_8005E4A4(0);
            D_801E43FC = 0;
            D_8019CA10 = 0;
            goto after_progress;
        }
        goto check_finish_transition;
    } else {
progress_failed:
        returnValue = 0;
    }

after_progress:
check_finish_transition:
    if ((D_801E4364 < *(s16 *)(route + 0xAC)) &&
        (D_801E6E74 == 4)) {
        func_80033AA0(D_801E43FC * 2, 0x29);
        timer = D_801E43FC;
        oldTimer = timer;
        timer = timer < 2;
        if (!timer) {
            returnValue = 2;
        }
        timer = oldTimer + 1;
        D_801E43FC = timer;
        if ((s16)timer == 0x3F) {
            if (D_801E4DAC != 0) {
                func_800207E0();
                if (D_8019C8EC == 1) {
                    func_80042BC0(0x10);
                } else {
                    func_80042BC0(0xC);
                }
            } else {
                D_8019C8EC = 0;
                func_80042BC0(0xD);
            }
        }
        if (D_801E43FC >= 0x83) {
            func_8001FA70();
            func_80035258(0x11);
            func_80042BF0();
            goto update_countdown;
        }
    } else if ((D_801E4DAC == 0) &&
               (((*(s32 *)((u8 *)arg0 + 0x6C) +
                  *(s32 *)((u8 *)arg0 + 0x68)) <= -D_801E40D8) ||
                ((D_8009E83C == 0) && (D_801E8A8C >= 0x3C)))) {
        D_801E6E74 = 5;
        D_801E4408[D_801E408C][D_801E428C][0] =
            D_801E774C[D_801E408C][D_801E428C][0];
        func_80042CCC(8);
        func_8005E4A4(0);
        D_801E43FC = 0;
        func_8003CA14(&D_8009E6D4);
    }

update_countdown:
    if (D_801E6C90 == 2) {
        value = D_801E4364 - *(s16 *)(route + 0xAC);
        switch (value) {
        case 2:
            func_8005D6EC(0x27);
            break;
        case 1:
            func_8005D6EC(0x28);
            break;
        case 0:
            func_8005D6EC(0x29);
            break;
        }
        D_801E6C90--;
    } else if (D_801E6C90 == 1) {
        D_801E6C90 = 0;
        D_8009E6A0 = 2;
    } else if (D_801E6C90 > 0) {
        D_801E6C90--;
    }

    func_80041170();
    return returnValue;
}
