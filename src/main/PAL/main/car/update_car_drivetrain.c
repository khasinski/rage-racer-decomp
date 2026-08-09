#include "common.h"
#include "game/state.h"
#include "game/track.h"
#include "game/race.h"
#include "game/car.h"
#include "game/car_internal.h"
#include "game/track_internal.h"
#include "game/render.h"

/*
 * AI target-speed / drivetrain physics driver (called by UpdatePlayerCar). Reads
 * the per-car spec block g_CarSpec to compute a target speed, applies steering
 * assist and RPM, and dispatches the state98 motion handlers. `base` is the car
 * runtime, accessed via raw byte-offset pointer arithmetic (e.g.
 * *(s16*)((u8*)base+0x132)) with the drive sub-block at base+0xBC; the raw
 * offsets are what make it match, so it is intentionally left as void*.
 */
/*
 * Note on `gearCurve`: m2c merged two values into one temporary, so it starts
 * out as the per-gear torque curve pointer and is later reused to carry the
 * shift target speed. Splitting it changes the register assignment.
 */
void UpdateCarDrivetrain(PlayerCarRuntime *carArg) {
  u8 *gearCurve;
  s16 curveModeNow;
  s16 revLimit;
  s16 targetGear;
  s32 bandEnd;
  s16 bandStart;
  s16 lossStart;
  s16 targetGearAgain;
  int assistArmed;
  int steeringNonnegative;
  int secondNonnegative;
  s16 shiftTimer;
  s16 shiftTimerNext;
  s32 assistEnabled;
  s16 gear;
  s16 targetGearCheck;
  s16 leftWheelState;
  s16 rightWheelState;
  s16 driveCurveMode;
  s16 steerBias;
  s32 camber;
  s32 bandTorque;
  s32 lossTorque;
  s32 shiftRemaining;
  s32 trackHeadingError;
  s32 pointIndex;
  s32 wheelSpeed;
  s32 lateralOffset;
  s32 gearTorque;
  s32 dragProduct;
  s32 toCentreX;
  s32 gearTorqueLate;
  s32 cosCentreAngle;
  s32 toCentreZ;
  s32 engineSpeed;
  s32 engineSpeedLoss;
  s32 bandIndex;
  s32 frontLoad;
  s32 speedForPath;
  s32 centreAngle;
  s32 radialDistance;
  s32 shiftTargetRpm;
  s32 pointCurveMode;
  u8 *gearRatioSlot;
  s32 headingError;
  s32 shiftMode;
  s32 gradeScale;
  s32 sideForce;
  s32 roadSpeed;
  s32 arcPointIndex;
  s32 speedA;
  s32 torqueShifted;
  s32 speedB;
  s32 driveModeLate;
  s32 loadTorque;
  s32 driveMode;
  s32 frontLoadScaled;
  s32 shiftTargetSpeed;
  s32 downforceScale;
  s32 downforce;
  s32 gripBudget;
  s32 bandSlot;
  s32 bandCurve;
  s32 assistStep;
  int shiftTimerActive;
  s32 lossCurve;
  s32 dragTerm;
  s32 slipAngle;
  s32 accel;
  s32 bandScale;
  s32 steerLoad;
  s32 throttleAccel;
  s32 gearRatio;
  s32 netTorque;
  s32 gradePenalty;
  s32 lateralSum;
  s32 dragBase;
  s32 camberLean;
  s32 netTorqueRoundedA;
  s32 netTorqueRoundedC;
  s32 lossBelowLimit;
  s32 shiftedSpeed;
  s32 netTorqueRoundedB;
  s32 bandBase;
  s32 lossBase;
  s32 throttleTorque;
  s32 speedScaled;
  s32 torqueLate;
  s32 coefficientBase;
  s32 coefficient;
  s32 wheelSpeedScaled;
  u16 arcFlags;
  u16 currentSpeed;
  u16 steerBiasNext;
  GameCarDrive *drive;
  void *arcCentre;
  GameTrackPoint *trackPoint;
  void *curveSlot;
  void *specSlot;
  PlayerCarRuntime *car;
  u8 *config;
  void *base;
  car = carArg;
  base = (u8 *)g_GearTorqueCurve;
  config = (u8 *)g_CarSpec;
  gear = car->drive.gear;
  gearRatioSlot = config + (gear * 4);
  gearCurve = (gear * 64) + ((u8 *)base);
  gearRatio = ((GameCarSpec *)gearRatioSlot)->gearLoad[0];
  drive = &car->drive;
  if (g_RacePhase < 2)
  {
    car->drive.gearDisp = gear;
    gearRatio = ((GameCarSpec *)config)->gearLoad[1];
    gearCurve = base;
  }
  else
    if ((car->drive.state98 == 3) && ((car->drive.accelBtn < 0x40) || (car->drive.brakeBtn >= 0x80)))
  {
    gearCurve = base;
  }
  leftWheelState = drive->unk9C;
  if (leftWheelState == 0)
  {
    if (drive->accelBtn >= 0x85)
    {
      drive->unk9C = 1;
    }
  }
  else
    if (leftWheelState == 1)
  {
    drive->unk9C = 2;
  }
  else
    if (drive->accelBtn < 0x7C)
  {
    drive->unk9C = 0;
  }
  rightWheelState = drive->unk9E;
  if (rightWheelState == 0)
  {
    if (drive->brakeBtn >= 0x85)
    {
      drive->unk9E = 1;
    }
  }
  else
    if (rightWheelState == 1)
  {
    drive->unk9E = 2;
  }
  else
    if (drive->brakeBtn < 0x7C)
  {
    drive->unk9E = 0;
  }
  frontLoad = drive->accelBtn * 0x64;
  frontLoadScaled = frontLoad >> 8;
  if (frontLoad < 0)
  {
    frontLoadScaled = ((s32) (frontLoad + 0xFF)) >> 8;
  }
  gripBudget = 0x17C - frontLoadScaled;
  gripBudget += ((s32) (drive->brakeBtn * 0x64)) / 256;
  if (drive->state98 == 1)
  {
    driveCurveMode = drive->unk40;
    pointCurveMode = g_TrackPoints[car->trackPointIndex].arcRef & 3;
    if (driveCurveMode != pointCurveMode)
    {
      if (driveCurveMode != 0)
      {
        steerBiasNext = (u16)drive->unk42 - 1;
        goto block_29;
      }
      if (pointCurveMode == 0)
      {
        goto block_27;
      }
    }
    else
    {
      block_27:
      if (drive->unk40 != 0)
      {
        steerBiasNext = (u16)drive->unk42 + 2;
        block_29:
        drive->unk42 = steerBiasNext;

      }

    }
    steerBias = drive->unk42;
    if (steerBias >= 0x1F)
    {
      drive->unk42 = 0x1E;
    }
    else if (steerBias < (-0x1E))
    {
      drive->unk42 = -0x1E;
    }
    gripBudget += g_CarSpec->unk112 - drive->unk42 * 0xA;
    drive->unk32 = (s16)gripBudget;
  }
  else
  {
    trackPoint = &g_TrackPoints[car->trackPointIndex];
    curveModeNow = drive->unk40;
    if ((curveModeNow != (trackPoint->arcRef & 3)) && (curveModeNow != 0))
    {
      camber = trackPoint->field_E;
      if (camber < (-0x32))
      {
        camber = -0x32;
      }
      else
        if (camber >= 0x33)
      {
        camber = 0x32;
      }
      if ((g_TrackPoints[car->trackPointIndex].arcRef & 3) == 1)
      {
        camberLean = (-(camber * 0x3C)) / 20;
      }
      else
      {
        camberLean = camber * 3;
      }
      gripBudget += camberLean;
    }
    drive->unk32 = (s16)((drive->unk32 + (gripBudget * drive->unk88) / 1000) / 2);
  }
  gearTorque = gearRatio * drive->unk78;
  steerLoad = 0;
  loadTorque = drive->unk94;
  netTorque = gearTorque - loadTorque;
  driveMode = drive->state98;
  accel = 0;
  if (driveMode == 1)
  {
    netTorqueRoundedA = netTorque;
    if (netTorque < 0)
    {
      netTorqueRoundedA = netTorque + 0xFFF;
    }
    accel = netTorqueRoundedA;
    accel = accel >> 0xC;
  }
  else
    if (netTorque >= (-0x30D3))
  {
    if (netTorque > 0x186A0)
    {
      netTorqueRoundedB = netTorque;
      if (netTorque < 0)
      {
        netTorqueRoundedB = netTorque + 0xFF;
      }
      accel = ((netTorqueRoundedB >> 8) * 0x46) / 200;
    }
  }
  else
    if (driveMode == 3)
  {
    accel = (gearTorque - loadTorque) / 768;
  }
  else
  {
    netTorqueRoundedC = netTorque;
    if (netTorque < 0)
    {
      netTorqueRoundedC = netTorque + 0x7FF;
    }
    accel = netTorqueRoundedC >> 0xB;
  }
  revLimit = ((GameCarSpec *)config)->revLimit;
  if (drive->unk78 >= revLimit)
  {
    bandScale = 0;
    netTorque = ((revLimit - drive->unk78) * 4) / 5;
  }
  else
  {
    bandIndex = drive->unk78 / 1000;
    if (bandIndex == 0)
    {
      bandBase = 0;
    }
    else
    {
      bandStart = *((&g_TorqueBandStart) + bandIndex);
      if (bandStart == 0)
      {
        bandBase = 0;
      }
      else
      {
        bandBase = bandStart - 1;
      }
    }
    bandEnd = g_TorqueBandEnd[bandIndex];
    bandSlot = bandBase;
    if (bandSlot < bandEnd)
    {
      engineSpeed = drive->unk78;
      curveSlot = (void *)((bandSlot * 4) + ((s32) gearCurve));
      specSlot = (void *)((bandSlot * 4) + ((s32) config));
      loop_68:
      bandTorque = ((GameCarSpec *)specSlot)->torqueBand.values[0];

      if ((engineSpeed >= bandTorque) && ((gearCurve = (u8 *)((GameCarSpec *)specSlot)->torqueBand.values[1], (((s32) gearCurve) < engineSpeed) == 0)))
      {
        s32 *curveValues = curveSlot;
        bandCurve = ((s32) gearCurve) - bandTorque;
        if (bandCurve <= 0)
        {
          bandCurve = 1;
        }
        frontLoadScaled = (engineSpeed - bandTorque) * curveValues[1];
        frontLoadScaled += (((s32) gearCurve) - engineSpeed) * curveValues[0];
        netTorque = frontLoadScaled / ((s32) (bandCurve * 0xA));
      }
      else
      {
        curveSlot += 4;
        bandSlot += 1;
        specSlot += 4;
        if (bandSlot < bandEnd)
        {
          goto loop_68;
        }
      }
    }
    if (netTorque < 0)
    {
      netTorque = 0;
    }
    if (bandIndex == 0)
    {
      lossBase = 0;
    }
    else
    {
      lossStart = *((&g_TorqueLossBandStart) + bandIndex);
      lossBase = 0;
      if (lossStart != 0)
      {
        lossBase = lossStart - 1;
      }
    }
    bandEnd = g_TorqueLossBandEnd[bandIndex];
    assistStep = lossBase;
    bandScale = 0;
    if (assistStep < bandEnd)
    {
      engineSpeedLoss = drive->unk78;
      gearCurve = (u8 *)((assistStep * 4) + ((s32) config));
      loop_83:
      lossTorque = ((GameCarSpec *)gearCurve)->torqueLossRpm[0];

      if (engineSpeedLoss >= lossTorque)
      {
        curveSlot = (void *)((GameCarSpec *)gearCurve)->torqueLossRpm[1];
        assistStep += 1;
        if (((s32) curveSlot) >= engineSpeedLoss)
        {
          lossCurve = ((s32) curveSlot) - lossTorque;
          if (lossCurve <= 0)
          {
            lossCurve = 1;
          }
          bandScale = ((s32) (((engineSpeedLoss - lossTorque) * ((GameCarSpec *)gearCurve)->torqueLossValue[1]) + ((((s32) curveSlot) - engineSpeedLoss) * ((GameCarSpec *)gearCurve)->torqueLossValue[0]))) / lossCurve;
          lossBelowLimit = bandScale < 0x64;
        }
        else
        {
          goto block_89;
        }
      }
      else
      {
        assistStep = assistStep + 1;
        block_89:
        gearCurve += 4;

        if (assistStep >= bandEnd)
        {
          goto block_90;
        }
        goto loop_83;
      }
    }
    else
    {
      block_90:
      lossBelowLimit = bandScale < 0x64;

    }
    if (lossBelowLimit == 0)
    {
      bandScale = 0x64;
    }
    else
      if (bandScale <= 0)
    {
      bandScale = 0;
    }
    if ((drive->gear == 1) && (drive->unk78 < g_CarSpec->redline))
    {
      bandScale *= 2;
    }
  }
  shiftMode = drive->state98;
  if ((shiftMode == 1) || (shiftMode == 3))
  {
    drive->unk38 = 0;
    drive->clutch = 0;
  }
  else
  {
    if (shiftMode == 2)
    {
      shiftTimer = drive->unk38;
      shiftTimerActive = shiftTimer >= 0;
      if (shiftTimerActive)
      {
        shiftTimerNext = shiftTimer - 1;
        drive->unk38 = shiftTimerNext;
        accel = 0;
        if (shiftTimerNext < 0)
        {
          drive->unk38 = 0;
        }
        targetGear = drive->gear;
        if (drive->gearDisp != targetGear)
        {
          shiftTargetRpm = ((s32)(((car->speed * 0xA0) / 1168) * 0x2710)) /
                           ((GameCarSpec *)((u8 *)g_CarSpec - (-(targetGear * 4))))->gearRatio[0];
          currentSpeed = (u16)drive->unk78;
          g_ShiftTargetRpm = shiftTargetRpm;
          drive->unk3C = (s16)((u16)g_ShiftTargetRpm - currentSpeed);
        }
        bandEnd = drive->unk3C * drive->unk38 / 20;
        shiftedSpeed = bandEnd;
        shiftedSpeed = shiftedSpeed + g_ShiftTargetRpm;
        goto block_129;
      }
    }
    targetGearAgain = drive->gear;
    if (drive->gearDisp != targetGearAgain)
    {
      gearCurve = (u8 *)((car->speed * 0x2710) /
                         (((GameCarSpec *)(config - (-(targetGearAgain * 4))))->gearRatio[0] * 0x490 / 160));
      wheelSpeed = (u16)car->field_A8;
      wheelSpeedScaled = wheelSpeed;
      assistEnabled = drive->manual;
      drive->unk2C = wheelSpeedScaled;
      g_ShiftTargetSpeed = (s32) gearCurve;
      if (assistEnabled != 0)
      {
        targetGearCheck = drive->gear;
        if ((drive->gearDisp < targetGearCheck) && (g_RoadGrade < 0))
        {
          if (targetGearCheck < 4)
          {
            goto grade_adjust_done;
          }
          if (targetGearCheck == 4)
          {
            gradePenalty = (-g_RoadGrade) / 120;
            wheelSpeedScaled = (s32) ((u32) wheelSpeedScaled << 16);
            wheelSpeedScaled >>= 16;
          }
          else
            if (targetGearCheck == 5)
          {
            gradePenalty = (-g_RoadGrade) / 48;
            wheelSpeedScaled = (s32) ((u32) wheelSpeedScaled << 16);
            wheelSpeedScaled >>= 16;
          }
          else
            if (targetGearCheck >= 6)
          {
            gradePenalty = (g_RoadGrade * (-7)) / 240;
            wheelSpeedScaled = (s32) ((u32) wheelSpeedScaled << 16);
            wheelSpeedScaled >>= 16;
          }
          else
          {
            goto grade_adjust_done;
          }
          gradeScale = 0x64 - gradePenalty;
          drive->unk2C = (u16)((wheelSpeedScaled * gradeScale) / 100);
          g_ShiftTargetSpeed = (gradeScale * ((s32) gearCurve)) / 100;
        }
      }
grade_adjust_done:
      shiftTargetSpeed = g_ShiftTargetSpeed;

      accel = 0;
      if (drive->gearDisp > drive->gear)
      {
        shiftTargetSpeed += 0x1F4;
      }
      g_ShiftTargetSpeed = shiftTargetSpeed;
      {
        u16 targetSpeed = (u16) g_ShiftTargetSpeed;
        u16 currentSpeed = (u16)drive->unk78;
        drive->clutch = 0xA;
        drive->unk2E = 0;
        drive->unk36 = (s16)(targetSpeed - currentSpeed);
      }
    }
    else
    {
      {
        s32 countdown = --drive->clutch;
      if (((s16) countdown) <= 0)
      {
        drive->unk2E = 1;
        drive->unk2C = 0;
        drive->clutch = 0;
      }
      else
        if (drive->manual != 0)
      {
        drive->unk78 = g_ShiftTargetSpeed - drive->unk36 * (s16)countdown / 15;
      }
      else
      {
        shiftRemaining = drive->unk36 * (s16)countdown;
        lossBase = shiftRemaining / 10;
        drive->unk78 = g_ShiftTargetSpeed - lossBase;
        goto shift_interpolation_done;
        block_129:
        drive->unk78 = shiftedSpeed;
shift_interpolation_done:

      }
      }
    }
  }
  throttleTorque = netTorque * drive->accelBtn * drive->unk2E;
  if (throttleTorque < 0)
  {
    throttleTorque += 0xFF;
  }
  throttleAccel = throttleTorque >> 8;
  if (g_GripLossTimer > 0)
  {
    g_GripLossTimer -= 1;
  }
  else
  {
    g_GripLossTimer = 0;
  }
  if (car->shiftState == 0)
  {
    steerLoad += drive->unk78 / 256;
  }
  accel += drive->brakeBtn * drive->unk78 / 8192;
  if (netTorque > 0)
  {
    if (drive->accelBtn < 0x7F)
    {
      accel += netTorque / 2;
    }
  }
  else
  {
    accel -= netTorque / 2;
  }
  headingError = GetAngleDistance(car->field_24, car->headingAngle, (s32) gearCurve, curveSlot);
  drive->unk4C = headingError;
  if (headingError >= 0x401)
  {
    drive->unk4C = 0x800 - headingError;
  }
  steerLoad += drive->unk4C / 256;
  if ((drive->state98 != 1) && (g_PadType == 0x41))
  {
    assistStep = g_CarSpec->unk10E * drive->unk88 / 1000;
    if (assistStep <= 0)
    {
      assistStep = 1;
    }
    shiftRemaining = drive->steerPos;
    assistArmed = shiftRemaining >= 0;
    if (assistArmed)
    {
      steerLoad += ((s32) ((shiftRemaining * 5) / 6)) / assistStep;
    }
    else
    {
      steerLoad -= ((s32) ((shiftRemaining * 5) / 6)) / assistStep;
    }
  }
  trackHeadingError = GetAngleDistance(car->headingAngle,
                                       0xC00 - g_TrackPoints[car->trackPointIndex].angle);
  frontLoadScaled = trackHeadingError;
  pointIndex = car->trackPointIndex;
  lateralOffset = car->field_38;
  engineSpeed = g_TrackPoints[pointIndex].field_C * (0x400 - lateralOffset);
  pointIndex += 1;
  lateralSum = engineSpeed +
               g_TrackPoints[pointIndex % (s32)g_TrackPointCount].field_C * lateralOffset;
  secondNonnegative = lateralSum >= 0;
  if (!secondNonnegative)
  {
    lateralSum += 0x3FF;
  }
  slipAngle = lateralSum >> 0xA;
  dragProduct = slipAngle * rcos(frontLoadScaled);
  slipAngle = dragProduct >> 0xC;
  if (dragProduct < 0)
  {
    slipAngle = ((s32) (dragProduct + 0xFFF)) >> 0xC;
  }
  if (slipAngle < (-0xEE))
  {
    slipAngle = -0xEE;
  }
  else
    if (slipAngle >= 0xEF)
  {
    slipAngle = 0xEE;
  }
  sideForce = (-rsin(slipAngle)) * 0x708;
  g_RoadGrade = slipAngle;
  frontLoadScaled = sideForce / 0xA000;
  steeringNonnegative = slipAngle >= 0;
  if (!steeringNonnegative)
  {
    steerLoad += frontLoadScaled;
  }
  else
  {
    steerLoad += frontLoadScaled / 10;
  }
  if ((g_RacePhase == 2) && (drive->state98 == 3))
  {
    steerLoad += (g_StandingStartSpin & 0x1F) * 5;
  }
  {
    s32 counter = g_DriveBoostTimer;
    if (counter > 0)
    {
      s32 baseValue = steerLoad + 0xC8;
      steerLoad = baseValue + (counter * 0x14);
      g_DriveBoostTimer = counter - 1;
    }
  }
  if (drive->state98 == 1)
  {
    throttleAccel = (throttleAccel * 4) / 5;
  }
  shiftTargetSpeed = (roadSpeed = car->speed * 0xA0 / 1168);
  dragBase = (s32) ((g_CarSpec->unk110) * 0x3E8);
  dragTerm = dragBase / ((s16) g_DragScale);
  if (dragTerm <= 0)
  {
    dragTerm = 1;
  }
  steerLoad += ((s32) (roadSpeed * roadSpeed)) / dragTerm;
  g_DragScale = 0x3E8;
  if (car->shiftState == 0)
  {
    steerLoad = (steerLoad * (0x64 - bandScale)) / 100;
  }
  else
  {
    throttleAccel *= 2;
    steerLoad = 0;
  }
  if ((drive->unk38 <= 0) && (drive->clutch <= 0))
  {
    drive->unk78 = throttleAccel - accel - steerLoad + drive->unk78;
  }
  speedForPath = drive->unk78;
  if (speedForPath < 0)
  {
    drive->unk78 = 0;
  }
  else
    if (speedForPath >= 0x3A99)
  {
    drive->unk78 = 0x3A98;
  }
  gearTorqueLate = gearRatio * drive->unk78;
  drive->unk94 = gearTorqueLate;
  if (drive->state98 == 1)
  {
    arcPointIndex = car->trackPointIndex;
    arcFlags = g_TrackPoints[arcPointIndex].arcRef;
    dragBase = arcFlags % 4;
    if (dragBase > 0)
    {
      arcCentre = &g_TrackArcCenters[(s16)arcFlags >> 4];
      toCentreX = car->x - ((GameTrackArcCenter *)arcCentre)->x;
      toCentreZ = car->z - ((GameTrackArcCenter *)arcCentre)->z;
      centreAngle = Atan2(toCentreX, toCentreZ);
      cosCentreAngle = rcos(centreAngle);
      radialDistance = (cosCentreAngle * toCentreX) + (rsin(centreAngle) * toCentreZ);
      frontLoadScaled = radialDistance >> 0xC;
      if (radialDistance < 0)
      {
        frontLoadScaled = ((s32) (radialDistance + 0xFFF)) >> 0xC;
      }
    }
    else
    {
      frontLoadScaled = (g_CarSpec->unk10C) * 0x64;
    }
    if ((frontLoadScaled <= 0) || ((downforceScale = (g_CarSpec->unk10C) * 0x64, downforceScale <= 0)))
    {
      downforceScale = (g_CarSpec->unk10C) * 0x64;
    }
    downforce = ((s32) ((g_CarSpec->unk10C) * 0x64)) / downforceScale;
    if (downforce <= 0)
    {
      downforce = 1;
    }
    dragTerm = drive->brakeBtn * 0x14;
    coefficientBase = 0x26FC - downforce;
    coefficient = coefficientBase - (steerLoad * 2);
    if (dragTerm < 0)
    {
      dragTerm += 0xFF;
    }
    car->speed = (coefficient - (dragTerm >> 8)) * car->speed / 10000;
    arcPointIndex = drive->unk94;
    if (arcPointIndex < 0)
    {
      arcPointIndex += 0x1FFFFF;
    }
    dragBase = arcPointIndex >> 0x15;
    car->field_A8 = dragBase;
  }
  else
  {
    if (car->shiftState != 0)
    {
      speedA = car->speed;
      car->field_A8 = 0;
      speedScaled = (speedA * 0x3E7) / 1000;
    }
    else
    {
      if (drive->clutch > 0)
      {
        car->field_A8 = drive->unk2C;
      }
      else
      {
        torqueLate = gearTorqueLate;
        if (drive->unk38 > 0)
        {
          car->field_A8 = drive->unk2C;
        }
        else
        {
          if (torqueLate < 0)
          {
            torqueLate += 0x1FFFF;
          }
          torqueShifted = torqueLate >> 0x11;
          car->field_A8 = torqueShifted;
          if (drive->manual == 0)
          {
            car->field_A8 = g_CarSpec->unk102 * torqueShifted / 1000;
          }
        }
      }
      if (g_GripLossTimer > 0)
      {
        car->field_A8 /= 2;
      }
      speedB = car->speed;
      speedScaled = (speedB * 0x5E) / 100;
    }
    car->speed = speedScaled;
  }
  if (car->speed < 8)
  {
    car->headingAngle = car->field_24;
  }
  if (g_RacePhase >= 2)
  {
    driveModeLate = drive->state98;
    switch (driveModeLate)
    {
      case 0:
        UpdateCarDriving(car, dragTerm);
        break;

      case 1:
        UpdateCarLaunch(car, dragTerm);
        break;

      case 2:
        UpdateCarAirborne(car, dragTerm);
        break;

      case 3:
        UpdateCarStandingStart(car, dragTerm);
        break;

    }

  }
  else
  {
    car->speed = 0;
  }
  if (car->speed < 8)
  {
    car->headingAngle = car->field_24;
  }
}
