#include "common.h"
#include "game/vector.h"
#include "psyq/gte.h"
#include "game/state.h"
#include "game/menu.h"
#include "game/race.h"
#include "game/car.h"
#include "game/render.h"

typedef struct GearRange {
    s16 up;
    s16 down;
} GearRange;


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

extern u8 g_PadType;
extern volatile u16 g_PadHeld;
/*
 * The NeGcon's three analog channels, already zeroed against the calibration
 * captured by the controller-config screen: button I, button II and the left
 * shoulder. Full scale is 106 (0x6A), which is why every use divides by 106 --
 * UpdatePadState feeds the digital pad through the same three slots by writing
 * a flat 0x6A when the mapped button is held.
 */
extern s16 g_NegconAnalogI;
extern s16 g_NegconAnalogII;
extern s16 g_NegconAnalogL;
/*
 * The live button mapping LoadPadButtonMapping installs: eight u16 masks
 * for the standard pad at 0x801E4B60 then eight for the NeGcon at 0x801E4B70.
 * These are masks 2 and 3 of each row; g_PadShiftMasks is the [pad/NeGcon][up,
 * down] view of masks 4 and 5, which is why its row stride is eight halfwords.
 */
extern s16 g_PadAccelMask;
extern s16 g_PadBrakeMask;
extern s16 g_PadShiftMasks[2][8];
extern s16 g_NegconAccelMask;
extern s16 g_NegconBrakeMask;
extern s32 g_TachoNeedleFlash;
extern s32 g_EngineRpmJitter;
extern s32 D_801E4194;
extern s32 g_ShiftTargetRpm;
extern s16 g_SteerHoldFrames;
extern s32 g_AutoShiftCooldown;
/* Latched from car->shiftTick & 0x3F when a gear change lands under power, and
 * cleared on every other shift and on touchdown. Its only effect is to raise
 * the volume of continuous sound 0 (flag + 25) while the car is airborne. */
extern s32 g_ShiftSoundLevel;
extern s16 g_NegconMappingIndex;
extern s32 g_EngineRpm;
extern u8 *g_TrackPoints;
/* g_PlayerCar drive +0xA0 / +0xA4: the throttle the input layer produced this
 * frame (0..0x100) and the drivetrain's own rpm, which g_EngineRpm is slewed
 * towards by a half (clutch in) or a quarter (clutch out) of the gap. */
extern s16 g_PlayerThrottle;
extern s32 g_PlayerTargetRpm;
extern u16 g_CarCornerOffsetX[];
extern u16 g_CarCornerOffsetZ[];

s32 IsCarFacingBackwards(void *car);
void UpdateCarBodyRoll(void *car);
void UpdateCarDrivetrain(void *car);
void AccumulateLapProgress(void *car);
void ApplyCarKnockback(void *car);
s32 UpdateCarTrackState(void *car, s32 arg1, void *arg2);
s32 func_8002D398(void *car);
void StartCarBodyKick(s32 arg0, void *car);
void UpdateCarTiltCounter(void *car);
void UpdateCarCrestHop(void *car);
void UpdateCarBodyKick(void *car);
s32 rsin(s32 angle) asm("func_80068568");
s32 Random15(void);
void PlaySoundCue(s32 cue);
void SetIndexedEffectVoice(s32 arg0, s32 arg1, s32 arg2);
void UpdateLoadedAudioVoices(s32 value, s32 bank);

/*
 * Per-car physics / gear-shift driver (matched sibling of the ASM
 * UpdateAttractCars). Samples input, builds the car's orientation matrices, runs
 * the manual/auto gear-shift state machine (using the per-car spec block
 * g_CarSpec for top-gear/upshift/downshift-speed tables and the shift
 * cooldown timers g_SteerHoldFrames/g_AutoShiftCooldown), dispatches the engine audio and the
 * boost/launch handlers, and resolves track-boundary skid via UpdateCarTrackState.
 * The local Car struct and the shared GameCarDrive are a distinct hand-rolled layout (drive block
 * at +0xBC) shaped to match; they are NOT GameCarRuntime.
 */
void UpdatePlayerCar(Car *car) asm("func_8002DEFC");
void UpdatePlayerCar(Car *car) {
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

    mode23 = g_PadType == 0x23;
    car->unkB8 = IsCarFacingBackwards(car);

    if (car->drive.manual != 0) {
        if (g_PadEdge2 & g_PadShiftMasks[mode23][0]) {
            s32 g = car->drive.gear;

            if (g < g_CarSpec->topGear && car->drive.clutch == 0) {
                car->drive.gear = car->drive.gear + 1;
                g_SteerHoldFrames = 0;
            }
        }
        if (g_PadEdge2 & g_PadShiftMasks[mode23][1]) {
            s32 g = p->gear;

            if (g >= 2) {
                p->gear = p->gear - 1;
                g_SteerHoldFrames = 0;
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
                g_AutoShiftCooldown <= 0 && car->drive.clutch == 0) {
                if (g >= 2) {
                    car->drive.gear = car->drive.gear - 1;
                    g_AutoShiftCooldown = 25;
                    g_SteerHoldFrames = 0;
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
                    g_AutoShiftCooldown <= 0 && p->clutch == 0 &&
                    nextGear < config->topGear) {
                    p->gear = p->gear + 1;
                    g_AutoShiftCooldown = 25;
                    g_SteerHoldFrames = 0;
                }
            }
        }
        if (g_AutoShiftCooldown > 0) {
            if (p->brakeBtn >= 129) {
                g_AutoShiftCooldown = g_AutoShiftCooldown - 2;
            } else {
                g_AutoShiftCooldown = g_AutoShiftCooldown - 1;
            }
        }
        if (car->speed == 0 && p->gear >= 2 && p->state98 != 3) {
            p->gear = 1;
            p->clutch = 0;
            g_AutoShiftCooldown = 0;
        }
    }

    UpdateCarBodyRoll(car);

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
        if (g_PadType == 0x41) {
            *(volatile s16 *)&p->accelBtn = ((g_PadHeld & g_PadAccelMask) != 0) << 8;
            p->brakeBtn = ((g_PadHeld & g_PadBrakeMask) != 0) << 8;
        } else if (g_PadType == 0x23) {
            *(volatile s16 *)&p->accelBtn = ((g_PadHeld & g_NegconAccelMask) != 0) << 8;
            p->brakeBtn = ((g_PadHeld & g_NegconBrakeMask) != 0) << 8;
            switch (g_NegconMappingIndex) {
            case 0:
            case 5:
                *(volatile s16 *)&p->accelBtn = (g_NegconAnalogI << 8) / 106;
                p->brakeBtn = (g_NegconAnalogII << 8) / 106;
                break;
            case 1:
            case 6:
                *(volatile s16 *)&p->accelBtn = (g_NegconAnalogII << 8) / 106;
                p->brakeBtn = (g_NegconAnalogI << 8) / 106;
                break;
            case 2:
                p->brakeBtn = (g_NegconAnalogL << 8) / 106;
                break;
            case 3:
                *(volatile s16 *)&p->accelBtn = (g_NegconAnalogII << 8) / 106;
                p->brakeBtn = (g_NegconAnalogL << 8) / 106;
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

    UpdateCarDrivetrain(car);

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

    if (g_PadType == 0x23) {
        if (car->unk44 >= 4096) {
            car->unk44 = 4096;
            if (p->steerPos < -4096) {
                g_SteerHoldFrames++;
            }
        } else if (car->unk44 < -4095) {
            car->unk44 = -4096;
            if (p->steerPos > 4096) {
                g_SteerHoldFrames++;
            }
        } else {
            g_SteerHoldFrames = -10;
        }
    } else {
        if (car->unk44 >= 4096) {
            car->unk44 = 4096;
            g_SteerHoldFrames++;
        } else if (car->unk44 < -4095) {
            car->unk44 = -4096;
            g_SteerHoldFrames++;
        } else {
            g_SteerHoldFrames = 0;
        }
    }

    car->unk00 -= car->unk10;
    car->unk08 -= car->unk18;
    BuildRotMatrixY(&m1, car->unk24);
    BuildRotMatrixX(&m2, car->unk20);
    MulMatrix2(&m2, &m1);
    BuildRotMatrixZ(&m2, car->unk28);
    MulMatrix2(&m2, &m1);

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
    ApplyMatrix(&m2, &sv1, &car->unk10);

    tmp.x = (p->accelPos * 6) / 1280 + car->unk00 + car->unk10;
    tmp.z = (p->brakePos * 6) / 1280 + car->unk08 + car->unk18;
    *(Vec4 *)&car->unk00 = tmp;
    AccumulateLapProgress(car);

    {
        s32 base = car->unk24 - 0xC00;

        slip = (base + *(s16 *)(g_TrackPoints + car->trackPointIndex * 24 + 10)) & 0xFFF;
    }
    sv2.vx = 0;
    sv2.vz = 0;
    sv2.vy = slip;
    RotMatrix(&sv2, &mA);

    arr[0] = 0;
    arr[1] = 0;
    arr[0] = -1;
    arr[1] = -1;
    for (i = 1, off = 0; off < 16; off += 4, i++) {
        sv2.vx = *(u16 *)((u8 *)g_CarCornerOffsetX + off) * 4;
        sv2.vz = *(u16 *)((u8 *)g_CarCornerOffsetZ + off) * 4;
        sv2.vy = 0;
        ApplyMatrix(&mA, &sv2, &vout);
        if (arr[0] < vout.x) {
            arr[2] = i;
            arr[0] = vout.x;
        } else if (vout.x < arr[1]) {
            arr[3] = i;
            arr[1] = vout.x;
        }
    }

    if (car->unk82 > 0) {
        ApplyCarKnockback(car);
    }
    skid = UpdateCarTrackState(car, car->trackPointIndex, arr);
    if ((u32)(skid - 2) < 2U && car->speed < 64) {
        skid = 0;
    }

    if (p->unk3C != 0) {
        s32 d = (g_CarSpec->revLimit + g_CarSpec->redline) / 2 - g_ShiftTargetRpm;
        if (d > 0) {
            car->unk20 += (d * Random15()) / 3276700;
        }
    }

    crash = func_8002D398(car);
    if (skid != 0 || crash != 0) {
        StartCarBodyKick(2, car);
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
            StartCarBodyKick(1, car);
            g_ShiftSoundLevel = 0;
            if ((s16)car->shiftTick >= 19) {
                if (g_RacePhase < 3) {
                    PlaySoundCue(0xE);
                }
            }
            if (p->state98 == 0 && (s16)car->shiftTick >= 3) {
                s32 rpm;

                GameCarSpec *props;
                s32 v = (100 - (p->gear - 1) * 4) * 10000;

                p->unk94 = v * car->speed / 100;
                g_ShiftSoundLevel = car->shiftTick & 0x3F;
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
                g_ShiftTargetRpm = rpm;
                p->unk3C = (u16)g_ShiftTargetRpm - (u16)p->unk78;
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

    UpdateCarTiltCounter(car);
    UpdateCarCrestHop(car);

    if (skid == 0 && crash == 0) {
        car->unk04 += p->unk68;
        UpdateCarBodyKick(car);
    } else {
        slip = GetAngleDistance(0xC00 - *(s16 *)(g_TrackPoints + car->trackPointIndex * 24 + 10),
                             car->unkA0);
        if (crash != 0) {
            p->unk48 -= 1000;
            if (car->speed >= 81) {
                p->unk94 = p->unk94 * 98 / 100;
                car->speed = car->speed * 97 / 100;
                p->unk2C = p->unk2C * 95 / 100;
                g_ShiftTargetRpm = g_ShiftTargetRpm * 95 / 100;
            }
        } else {
            p->unk48 -= 5000;
            p->unk94 = (85 - rsin(slip) * 20 / 4096) * p->unk94 / 100;
            car->speed = (87 - rsin(slip) * 40 / 4096) * car->speed / 100;
            p->unk2C = p->unk2C * (85 - rsin(slip) * 20 / 4096) / 100;
            g_ShiftTargetRpm = (85 - rsin(slip) * 20 / 4096) * g_ShiftTargetRpm / 100;
            if (g_RacePhase < 3) {
                switch (skid) {
                case 1:
                case 3:
                    if (car->unk82 >= 15) {
                        if ((u32)(slip - 768) < 257U) {
                            if (skid == 1) {
                                PlaySoundCue(0xA);
                            } else if (car->speed >= 81) {
                                PlaySoundCue(0xD);
                            }
                        } else {
                            PlaySoundCue(g_MirrorMode == 0 ? 0xB : 0xC);
                        }
                    }
                    break;
                case 2:
                case 4:
                    if (car->unk82 >= 15) {
                        if ((u32)(slip - 768) < 257U) {
                            if (skid == 2) {
                                PlaySoundCue(0xA);
                            } else if (car->speed >= 81) {
                                PlaySoundCue(0xD);
                            }
                        } else if (g_MirrorMode == 0) {
                            PlaySoundCue(0xC);
                        } else {
                            PlaySoundCue(0xB);
                        }
                    }
                    break;
                }
            }
        }
    }

    {
        s32 d = g_PlayerTargetRpm;
        s32 cab = g_EngineRpm;
        s32 sum;
        s32 rpmLimit;

        d -= cab;
        if (p->clutch > 0) {
            sum = d / 2 + cab;
        } else {
            sum = d / 4 + cab;
        }
        rpmLimit = g_CarSpec->revLimit;
        g_EngineRpm = sum;
        if (sum >= rpmLimit) {
            g_EngineRpm = rpmLimit;
        } else if (sum < 500) {
            g_EngineRpm = 500;
        }
    }

    if (g_EngineRpm >= g_CarSpec->revLimit - 100 && g_PlayerThrottle >= 129) {
        s32 r = Random15();

        g_TachoNeedleFlash = g_AnimTimer & 2;
        g_EngineRpmJitter = r % 150 / 2;
    } else {
        revFlag = 0;
        if (p->unk78 == 0 && (g_AnimTimer & 8)) {
            g_TachoNeedleFlash = 0;
            g_EngineRpmJitter = rsin(Random15() & 0xFFF) * 150 / 4096;
            if (g_EngineRpmJitter <= 0) {
                g_EngineRpmJitter = 0;
            }
            revFlag = g_EngineRpmJitter < 37;
        } else {
            g_EngineRpmJitter = 0;
            g_TachoNeedleFlash = 0;
        }
    }

    D_801E4194 = g_EngineRpm;
    if (p->unk78 != 0) {
        if (p->gear != 1) {
            revFlag = 0;
            if (g_EngineRpm >= g_CarSpec->redline - 2000) {
                revFlag = 1;
                if (g_EngineRpm < g_CarSpec->redline) {
                    revFlag = Random15() & 1;
                }
            }
        } else {
            revFlag = 1;
        }
    }

    if (g_RacePhase >= 4) {
        SetIndexedEffectVoice(-1, 0, 0);
    }

    if (p->manual != 0) {
        UpdateLoadedAudioVoices(g_EngineRpm + g_EngineRpmJitter,
                      (0 < g_PlayerThrottle) & (p->clutch == 0) & revFlag);
    } else {
        s32 flag = 0;
        s32 vol = g_EngineRpm + g_EngineRpmJitter;

        if (g_PlayerThrottle > 0) {
            flag = revFlag & 1;
        }
        UpdateLoadedAudioVoices(vol, flag);
    }

    p->gearDisp = p->gear;
}

extern s16 g_TrackZoneDark;
extern s32 g_EnvScriptClock;

s32 DrawTachometer(s32 rpm, s32 arg1, s32 type, s32 amt);

s32 DrawPlayerTachometer(void);
s32 DrawPlayerTachometer(void) {
    s32 value;
    s32 arg2;
    s32 arg3;

    if (g_TrackZoneDark != 3) {
        value = g_EnvScriptClock;
        arg3 = value - 0x1154;
        if ((u32)arg3 < 0x434C) {
            if ((u32)arg3 < 0x80) {
                arg2 = 3;
            } else {
                arg3 = value - 0x5420;
                if ((u32)arg3 < 0x80) {
                    arg2 = 1;
                } else {
                    arg2 = 0;
                    arg3 = 0;
                }
            }
        } else {
            arg2 = 2;
            arg3 = 0;
        }
    } else {
        arg2 = 2;
        arg3 = 0;
    }

    return DrawTachometer(g_EngineRpm + g_EngineRpmJitter, g_TachoNeedleFlash, arg2, arg3);
}
