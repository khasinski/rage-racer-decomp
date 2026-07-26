#include "common.h"
#include "psyq/gte.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/car.h"

typedef struct GearRange {
    s16 up;
    s16 down;
} GearRange;

typedef struct Vec4 {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} Vec4;

typedef struct SVec {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} SVec;

typedef struct Car {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 trackPointIndex; /* 0x30 */
    u8 pad34[0x10];
    s32 unk44;
    s32 unk48;
    u8 pad4C[4];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    u8 pad68[0x1A];
    s16 unk82;
    u8 pad84[0xC];
    s16 unk90;
    s16 unk92;
    s16 unk94;
    s16 unk96;
    s16 shiftState;  /* 0x98 */
    u16 shiftTick;   /* 0x9A */
    s16 shiftRef;    /* 0x9C */
    s16 shiftBase;   /* 0x9E */
    s32 unkA0;
    s32 speed;       /* 0xA4 */
    u8 padA8[0x10];
    s16 unkB8;
    u8 padBA[2];
    GameCarDrive drive;  /* 0xBC */
} Car;

extern u8 D_801E4369;
extern volatile u16 g_PadHeld asm("D_801E436A");
extern s16 D_801E4374;
extern s16 D_801E4376;
extern s16 D_801E4378;
extern s16 D_801E4B64;
extern s16 D_801E4B66;
extern s16 D_801E4B68[2][8];
extern s16 D_801E4B74;
extern s16 D_801E4B76;
extern s32 D_801E40B0;
extern s32 D_801E4170;
extern s32 D_801E4194;
extern s32 D_801E4BF4;
extern s16 D_801F17A4;
extern s32 D_801F17B8;
extern s32 D_801E8AA0;
extern s16 D_8019CB08;
extern s32 D_8019CAB4;
extern u8 *g_TrackPoints asm("D_8009E688");
extern s16 D_8009E830;
extern s32 D_8009E808;
extern u16 D_8007DAB0[];
extern u16 D_8007DAB2[];

s32 func_8002CD08(void *car);
void func_8002CD4C(void *car);
void func_8002A810(void *car);
void func_8001A530(Matrix *mtx, s32 angle);
void func_8001A5A0(Matrix *mtx, s32 angle);
void func_8001A4C0(Matrix *mtx, s32 angle);
void func_80069568(Matrix *lhs, Matrix *rhs);
s32 *func_80069678(void *mtx, void *vec, void *out);
void func_80069D18(void *rot, void *mtx);
void func_8002C168(void *car);
void func_80038C4C(void *car);
s32 func_80031298(void *car, s32 arg1, void *arg2);
s32 func_8002D398(void *car);
void func_80038F0C(s32 arg0, void *car);
void func_80038B04(void *car);
void func_80039280(void *car);
void func_80038FF0(void *car);
s32 func_8002A788(s32 arg0, s32 arg1);
s32 func_80068568(s32 angle);
s32 func_800632B0(void);
void func_8005D6EC(s32 id);
void func_8005C104(s32 arg0, s32 arg1, s32 arg2);
void func_8005D9F8(s32 value, s32 bank);

/*
 * Per-car physics / gear-shift driver (matched sibling of the ASM
 * func_8003BB50). Samples input, builds the car's orientation matrices, runs
 * the manual/auto gear-shift state machine (using the per-car spec block
 * g_CarSpec for top-gear/upshift/downshift-speed tables and the shift
 * cooldown timers D_801F17A4/D_801F17B8), dispatches the engine audio and the
 * boost/launch handlers, and resolves track-boundary skid via func_80031298.
 * The local Car struct and the shared GameCarDrive are a distinct hand-rolled layout (drive block
 * at +0xBC) shaped to match; they are NOT GameCarRuntime.
 */
void func_8002DEFC(Car *car) {
    Matrix m1;
    Matrix m2;
    SVec sv1;
    Matrix mScratch1;
    Vec4 vScratch1;
    Vec4 tmp;
    Vec4 vScratch2;
    Matrix mA;
    SVec sv2;
    Vec4 vout;
    s16 arr[4];
    GameCarDrive *p = &car->drive;
    s32 mode23;
    s32 limit;
    s32 slip;
    s32 skid;
    s32 crash;
    s32 revFlag;
    s32 i;
    s32 off;

    mode23 = D_801E4369 == 0x23;
    car->unkB8 = func_8002CD08(car);

    if (car->drive.manual != 0) {
        if (g_PadEdge2 & D_801E4B68[mode23][0]) {
            s32 g = car->drive.gear;

            if (g < g_CarSpec->topGear && car->drive.clutch == 0) {
                car->drive.gear = car->drive.gear + 1;
                D_801F17A4 = 0;
            }
        }
        if (g_PadEdge2 & D_801E4B68[mode23][1]) {
            s32 g = p->gear;

            if (g >= 2) {
                p->gear = p->gear - 1;
                D_801F17A4 = 0;
            }
        }
    } else {
        if (car->shiftState == 0) {
            s32 g;
            s32 idx;
            s32 tableValue;

            g = car->drive.gear;
            idx = g - 1;
            tableValue = (s32)g_CarSpec;
            tableValue += idx * 4;
            tableValue = *(s16 *)(tableValue + 0x120); /* shiftPoints[idx].downshiftSpeed */
            if (car->speed < tableValue &&
                D_801F17B8 <= 0 && car->drive.clutch == 0) {
                if (g >= 2) {
                    car->drive.gear = car->drive.gear - 1;
                    D_801F17B8 = 25;
                    D_801F17A4 = 0;
                }
            } else {
                GameCarSpec *config;
                u8 *entry;
                s32 nextGear;
                s32 speed;

                nextGear = p->gear;
                config = g_CarSpec;
                speed = car->speed;
                idx = nextGear - 1;
                entry = (u8 *)config;
                entry += idx * 4;
                /* config->shiftPoints[idx].upshiftSpeed */
                if (*(s16 *)(entry + 0x122) < speed &&
                    D_801F17B8 <= 0 && p->clutch == 0 &&
                    nextGear < config->topGear) {
                    p->gear = p->gear + 1;
                    D_801F17B8 = 25;
                    D_801F17A4 = 0;
                }
            }
        }
        if (D_801F17B8 > 0) {
            if (p->brakeBtn >= 129) {
                D_801F17B8 = D_801F17B8 - 2;
            } else {
                D_801F17B8 = D_801F17B8 - 1;
            }
        }
        if (car->speed == 0 && p->gear >= 2 && p->state98 != 3) {
            p->gear = 1;
            p->clutch = 0;
            D_801F17B8 = 0;
        }
    }

    func_8002CD4C(car);

    if (car->shiftState == 0) {
        s32 spd = car->speed;

        if (spd < 256 && p->state98 == 0) {
            p->unk90 += ((p->steerPos * 6) / 5 * p->unk32 / 256) * spd / 0x10000;
        } else if (spd < 512 && p->state98 == 3) {
            p->unk90 += ((p->steerPos * 6) / 5 * p->unk32 / 256) * spd / 0x20000;
        } else {
            p->unk90 += (p->steerPos * 6) / 5 * p->unk32 / 0x10000;
        }
    }

    if (g_RacePhase < 4) {
        if (D_801E4369 == 0x41) {
            *(volatile s16 *)&p->accelBtn = ((g_PadHeld & D_801E4B64) != 0) << 8;
            p->brakeBtn = ((g_PadHeld & D_801E4B66) != 0) << 8;
        } else if (D_801E4369 == 0x23) {
            *(volatile s16 *)&p->accelBtn = ((g_PadHeld & D_801E4B74) != 0) << 8;
            p->brakeBtn = ((g_PadHeld & D_801E4B76) != 0) << 8;
            switch (D_8019CB08) {
            case 0:
            case 5:
                *(volatile s16 *)&p->accelBtn = (D_801E4374 << 8) / 106;
                p->brakeBtn = (D_801E4376 << 8) / 106;
                break;
            case 1:
            case 6:
                *(volatile s16 *)&p->accelBtn = (D_801E4376 << 8) / 106;
                p->brakeBtn = (D_801E4374 << 8) / 106;
                break;
            case 2:
                p->brakeBtn = (D_801E4378 << 8) / 106;
                break;
            case 3:
                *(volatile s16 *)&p->accelBtn = (D_801E4376 << 8) / 106;
                p->brakeBtn = (D_801E4378 << 8) / 106;
                break;
            case 4:
            case 7:
                break;
            }
        } else {
            p->brakeBtn = 0;
            p->accelBtn = 0;
        }
    } else {
        p->accelBtn = 0;
        p->brakeBtn = 0;
    }

    func_8002A810(car);

    {
        s32 step = car->speed * 3;
        s32 spin;

        if (step > 4096) {
            step = 0x249;
        }
        spin = (step + car->unk48) & 0xFFF;
        car->unk48 = spin;
        if (car->speed > 800) {
            car->unk48 = spin | 0x1000;
        }
    }

    if (D_801E4369 == 0x23) {
        if (car->unk44 >= 4096) {
            car->unk44 = 4096;
            if (p->steerPos < -4096) {
                D_801F17A4++;
            }
        } else if (car->unk44 < -4095) {
            car->unk44 = -4096;
            if (p->steerPos > 4096) {
                D_801F17A4++;
            }
        } else {
            D_801F17A4 = -10;
        }
    } else {
        if (car->unk44 >= 4096) {
            car->unk44 = 4096;
            D_801F17A4++;
        } else if (car->unk44 < -4095) {
            car->unk44 = -4096;
            D_801F17A4++;
        } else {
            D_801F17A4 = 0;
        }
    }

    car->unk00 -= car->unk10;
    car->unk08 -= car->unk18;
    func_8001A530(&m1, car->unk24);
    func_8001A5A0(&m2, car->unk20);
    func_80069568(&m2, &m1);
    func_8001A4C0(&m2, car->unk28);
    func_80069568(&m2, &m1);

    sv1.vx = 0;
    sv1.vy = 0;
    m2.m[0][0] = m1.m[0][0];
    m2.m[0][1] = m1.m[1][0];
    m2.m[0][2] = m1.m[2][0];
    m2.m[1][0] = m1.m[0][1];
    m2.m[1][1] = m1.m[1][1];
    m2.m[1][2] = m1.m[2][1];
    m2.m[2][0] = m1.m[0][2];
    m2.m[2][1] = m1.m[1][2];
    m2.m[2][2] = m1.m[2][2];
    sv1.vz = -p->unk3E - 50;
    func_80069678(&m2, &sv1, &car->unk10);

    tmp.x = (p->accelPos * 6) / 1280 + car->unk00 + car->unk10;
    tmp.z = (p->brakePos * 6) / 1280 + car->unk08 + car->unk18;
    *(Vec4 *)&car->unk00 = tmp;
    func_8002C168(car);

    {
        s32 base = car->unk24 - 0xC00;

        slip = (base + *(s16 *)(g_TrackPoints + car->trackPointIndex * 24 + 10)) & 0xFFF;
    }
    sv2.vx = 0;
    sv2.vz = 0;
    sv2.vy = slip;
    func_80069D18(&sv2, &mA);

    arr[0] = 0;
    arr[1] = 0;
    arr[0] = -1;
    arr[1] = -1;
    for (i = 1, off = 0; off < 16; off += 4, i++) {
        sv2.vx = *(u16 *)((u8 *)D_8007DAB0 + off) * 4;
        sv2.vz = *(u16 *)((u8 *)D_8007DAB2 + off) * 4;
        sv2.vy = 0;
        func_80069678(&mA, &sv2, &vout);
        if (arr[0] < vout.x) {
            arr[2] = i;
            arr[0] = vout.x;
        } else if (vout.x < arr[1]) {
            arr[3] = i;
            arr[1] = vout.x;
        }
    }

    if (car->unk82 > 0) {
        func_80038C4C(car);
    }
    skid = func_80031298(car, car->trackPointIndex, arr);
    if ((u32)(skid - 2) < 2U && car->speed < 64) {
        skid = 0;
    }

    if (p->unk3C != 0) {
        s32 d = (g_CarSpec->revLimit + g_CarSpec->redline) / 2 - D_801E4BF4;
        if (d > 0) {
            car->unk20 += (d * func_800632B0()) / 3276700;
        }
    }

    crash = func_8002D398(car);
    if (skid != 0 || crash != 0) {
        func_80038F0C(2, car);
    }

    {
        s32 fuel = car->unk04;

        *(Vec4 *)&car->unk50 = *(Vec4 *)&car->unk20;
        car->unk28 = car->unk28 + car->unk64;
        car->unk60 = car->unk04;
        limit = fuel - 8;
    }

    if (car->shiftState != 0) {
        s32 n = car->shiftTick + 1;

        car->shiftTick = n;
        if (car->shiftState == 1) {
            s32 t = (s16)n;

            car->unk04 = car->shiftRef * t + (t * t * 72) / 100 + car->unk04;
            if (car->unk04 >= limit) {
                car->shiftState = 0;
            }
        } else if (car->shiftState == 2) {
            if (limit - car->shiftRef <= car->shiftBase) {
                car->unk04 = car->shiftBase;
            } else {
                car->shiftState = 3;
                car->shiftRef = car->shiftTick;
                car->unk04 = car->shiftBase;
            }
        } else {
            n = (s16)n - car->shiftRef;

            car->unk04 = car->shiftBase + (n * n * 216) / 100;
            if (car->unk04 >= limit) {
                car->shiftState = 0;
            }
        }

        if (car->shiftState == 0) {
            car->unk04 = limit + 8;
            car->unk90 = 0;
            car->unk94 = 0;
            func_80038F0C(1, car);
            D_801E8AA0 = 0;
            if ((s16)car->shiftTick >= 19) {
                if (g_RacePhase < 3) {
                    func_8005D6EC(0xE);
                }
            }
            if (p->state98 == 0 && (s16)car->shiftTick >= 3) {
                s32 rpm;

                GameCarSpec *props;
                s32 v = (100 - (p->gear - 1) * 4) * 10000;

                p->unk94 = v * car->speed / 100;
                D_801E8AA0 = car->shiftTick & 0x3F;
                p->unk60 = 0;
                p->unk58 = car->unkA0;
                p->unk5C = car->speed / 0x100000;
                p->unk50 = 0;
                props = g_CarSpec;
                {
                    s32 *ratios = props->gearRatio;

                    rpm = car->speed * 160 / 1168 * 10000 / ratios[p->gear];
                }
                p->unk38 = 0x14;
                p->state98 = 2;
                D_801E4BF4 = rpm;
                p->unk3C = (u16)D_801E4BF4 - (u16)p->unk78;
                {
                    s32 *loadRow = props->gearLoad;

                    p->unk2C = rpm * loadRow[p->gear] / 0x20000;
                    if (p->manual == 0) {
                        p->unk2C = p->unk2C * 985 / 1000;
                    }
                }
            }
        }
    }

    func_80038B04(car);
    func_80039280(car);

    if (skid == 0 && crash == 0) {
        car->unk04 += p->unk68;
        func_80038FF0(car);
    } else {
        slip = func_8002A788(0xC00 - *(s16 *)(g_TrackPoints + car->trackPointIndex * 24 + 10),
                             car->unkA0);
        if (crash != 0) {
            p->unk48 -= 1000;
            if (car->speed >= 81) {
                p->unk94 = p->unk94 * 98 / 100;
                car->speed = car->speed * 97 / 100;
                p->unk2C = p->unk2C * 95 / 100;
                D_801E4BF4 = D_801E4BF4 * 95 / 100;
            }
        } else {
            p->unk48 -= 5000;
            p->unk94 = (85 - func_80068568(slip) * 20 / 4096) * p->unk94 / 100;
            car->speed = (87 - func_80068568(slip) * 40 / 4096) * car->speed / 100;
            p->unk2C = p->unk2C * (85 - func_80068568(slip) * 20 / 4096) / 100;
            D_801E4BF4 = (85 - func_80068568(slip) * 20 / 4096) * D_801E4BF4 / 100;
            if (g_RacePhase < 3) {
                switch (skid) {
                case 1:
                case 3:
                    if (car->unk82 >= 15) {
                        if ((u32)(slip - 768) < 257U) {
                            if (skid == 1) {
                                func_8005D6EC(0xA);
                            } else if (car->speed >= 81) {
                                func_8005D6EC(0xD);
                            }
                        } else {
                            func_8005D6EC(g_MirrorMode == 0 ? 0xB : 0xC);
                        }
                    }
                    break;
                case 2:
                case 4:
                    if (car->unk82 >= 15) {
                        if ((u32)(slip - 768) < 257U) {
                            if (skid == 2) {
                                func_8005D6EC(0xA);
                            } else if (car->speed >= 81) {
                                func_8005D6EC(0xD);
                            }
                        } else if (g_MirrorMode == 0) {
                            func_8005D6EC(0xC);
                        } else {
                            func_8005D6EC(0xB);
                        }
                    }
                    break;
                }
            }
        }
    }

    {
        s32 d = D_8009E808;
        s32 cab = D_8019CAB4;
        s32 sum;
        s32 rpmLimit;

        d -= cab;
        if (p->clutch > 0) {
            sum = d / 2 + cab;
        } else {
            sum = d / 4 + cab;
        }
        rpmLimit = g_CarSpec->revLimit;
        D_8019CAB4 = sum;
        if (sum >= rpmLimit) {
            D_8019CAB4 = rpmLimit;
        } else if (sum < 500) {
            D_8019CAB4 = 500;
        }
    }

    if (D_8019CAB4 >= g_CarSpec->revLimit - 100 && D_8009E830 >= 129) {
        s32 r = func_800632B0();

        D_801E40B0 = g_AnimTimer & 2;
        D_801E4170 = r % 150 / 2;
    } else {
        revFlag = 0;
        if (p->unk78 == 0 && (g_AnimTimer & 8)) {
            D_801E40B0 = 0;
            D_801E4170 = func_80068568(func_800632B0() & 0xFFF) * 150 / 4096;
            if (D_801E4170 <= 0) {
                D_801E4170 = 0;
            }
            revFlag = D_801E4170 < 37;
        } else {
            D_801E4170 = 0;
            D_801E40B0 = 0;
        }
    }

    D_801E4194 = D_8019CAB4;
    if (p->unk78 != 0) {
        if (p->gear != 1) {
            revFlag = 0;
            if (D_8019CAB4 >= g_CarSpec->redline - 2000) {
                revFlag = 1;
                if (D_8019CAB4 < g_CarSpec->redline) {
                    revFlag = func_800632B0() & 1;
                }
            }
        } else {
            revFlag = 1;
        }
    }

    if (g_RacePhase >= 4) {
        func_8005C104(-1, 0, 0);
    }

    if (p->manual != 0) {
        func_8005D9F8(D_8019CAB4 + D_801E4170,
                      (0 < D_8009E830) & (p->clutch == 0) & revFlag);
    } else {
        s32 flag = 0;
        s32 vol = D_8019CAB4 + D_801E4170;

        if (D_8009E830 > 0) {
            flag = revFlag & 1;
        }
        func_8005D9F8(vol, flag);
    }

    p->gearDisp = p->gear;
}
