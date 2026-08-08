#include "common.h"
#include "game/track.h"
#include "game/race.h"
#include "game/car.h"
#include "game/render.h"

void UpdateCarAirborne();
s32 rsin(s32);
s32 rcos();
extern u8 *g_TrackArcCenters;
extern s16 g_TorqueBandEnd;
extern s16 g_TorqueLossBandEnd;
extern u8 g_PadType;
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
void UpdateCarDrivetrain(void *base) {
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
  void *drive;
  void *arcCentre;
  void *trackPoint;
  void *curveSlot;
  void *specSlot;
  void *car;
  u8 *config;
  car = base;
  base = (u8 *)g_GearTorqueCurve;
  config = (u8 *)g_CarSpec;
  gear = *(s16 *)(((u8 *)car) + 0x132);
  gearRatioSlot = ((u8 *)(config + (gear * 4))) + 0xCC;
  gearCurve = (gear * 64) + ((u8 *)base);
  gearRatio = *(s32 *)gearRatioSlot;
  drive = car + 0xBC;
  if (g_RacePhase < 2)
  {
    *(s16 *)(((u8 *)car) + 0xEC) = gear;
    gearRatio = *(s32 *)(((u8 *)config) + 0xD0);
    gearCurve = base;
  }
  else
    if (((*(s32 *)(((u8 *)car) + 0x154)) == 3) && (((*(s16 *)(((u8 *)car) + 0x15C)) < 0x40) || ((*(s16 *)(((u8 *)car) + 0x15E)) >= 0x80)))
  {
    gearCurve = base;
  }
  leftWheelState = *(s16 *)(((u8 *)drive) + 0x9C);
  if (leftWheelState == 0)
  {
    if ((*(s16 *)(((u8 *)drive) + 0xA0)) >= 0x85)
    {
      *(s16 *)(((u8 *)drive) + 0x9C) = 1;
    }
  }
  else
    if (leftWheelState == 1)
  {
    *(s16 *)(((u8 *)drive) + 0x9C) = 2;
  }
  else
    if ((*(s16 *)(((u8 *)drive) + 0xA0)) < 0x7C)
  {
    *(s16 *)(((u8 *)drive) + 0x9C) = 0;
  }
  rightWheelState = *(s16 *)(((u8 *)drive) + 0x9E);
  if (rightWheelState == 0)
  {
    if ((*(s16 *)(((u8 *)drive) + 0xA2)) >= 0x85)
    {
      *(s16 *)(((u8 *)drive) + 0x9E) = 1;
    }
  }
  else
    if (rightWheelState == 1)
  {
    *(s16 *)(((u8 *)drive) + 0x9E) = 2;
  }
  else
    if ((*(s16 *)(((u8 *)drive) + 0xA2)) < 0x7C)
  {
    *(s16 *)(((u8 *)drive) + 0x9E) = 0;
  }
  frontLoad = (*(s16 *)(((u8 *)drive) + 0xA0)) * 0x64;
  frontLoadScaled = frontLoad >> 8;
  if (frontLoad < 0)
  {
    frontLoadScaled = ((s32) (frontLoad + 0xFF)) >> 8;
  }
  gripBudget = 0x17C - frontLoadScaled;
  gripBudget += ((s32) ((*(s16 *)(((u8 *)drive) + 0xA2)) * 0x64)) / 256;
  if ((*(s32 *)(((u8 *)drive) + 0x98)) == 1)
  {
    driveCurveMode = *(s16 *)(((u8 *)drive) + 0x40);
    pointCurveMode = (*(u16 *)(((u8 *)((((*((s32 *)(((u8 *)car) + 0x30))) * 3) * 8) + ((u8 *)g_TrackPoints))) + 0x14)) & 3;
    if (driveCurveMode != pointCurveMode)
    {
      if (driveCurveMode != 0)
      {
        steerBiasNext = (*(u16 *)(((u8 *)drive) + 0x42)) - 1;
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
      if ((*(s16 *)(((u8 *)drive) + 0x40)) != 0)
      {
        steerBiasNext = (*(u16 *)(((u8 *)drive) + 0x42)) + 2;
        block_29:
        *(u16 *)(((u8 *)drive) + 0x42) = steerBiasNext;

      }

    }
    steerBias = (s16) (*(u16 *)(((u8 *)drive) + 0x42));
    if (steerBias >= 0x1F)
    {
      *(s16 *)(((u8 *)drive) + 0x42) = 0x1E;
    }
    else if (steerBias < (-0x1E))
    {
      *(s16 *)(((u8 *)drive) + 0x42) = -0x1E;
    }
    gripBudget += (g_CarSpec->unk112) - (((s16) (*(u16 *)(((u8 *)drive) + 0x42))) * 0xA);
    *(s16 *)(((u8 *)drive) + 0x32) = (s16) gripBudget;
  }
  else
  {
    trackPoint = ((*(s32 *)(((u8 *)car) + 0x30)) * 0x18) + ((u8 *)g_TrackPoints);
    curveModeNow = *(s16 *)(((u8 *)drive) + 0x40);
    if ((curveModeNow != ((*(u16 *)(((u8 *)trackPoint) + 0x14)) & 3)) && (curveModeNow != 0))
    {
      camber = *(s16 *)(((u8 *)trackPoint) + 0xE);
      if (camber < (-0x32))
      {
        camber = -0x32;
      }
      else
        if (camber >= 0x33)
      {
        camber = 0x32;
      }
      if (((*(u16 *)(((u8 *)(((*((s32 *)(((u8 *)car) + 0x30))) * 0x18) + ((u8 *)g_TrackPoints))) + 0x14)) & 3) == 1)
      {
        camberLean = (-(camber * 0x3C)) / 20;
      }
      else
      {
        camberLean = camber * 3;
      }
      gripBudget += camberLean;
    }
    *(s16 *)(((u8 *)drive) + 0x32) = (s16) (((s32) ((*(s16 *)(((u8 *)drive) + 0x32)) + ((gripBudget * (*(s32 *)(((u8 *)drive) + 0x88))) / 1000))) / 2);
  }
  gearTorque = gearRatio * (*(s32 *)(((u8 *)drive) + 0x78));
  steerLoad = 0;
  loadTorque = *(s32 *)(((u8 *)drive) + 0x94);
  netTorque = gearTorque - loadTorque;
  driveMode = *(s32 *)(((u8 *)drive) + 0x98);
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
  if ((*(s32 *)(((u8 *)drive) + 0x78)) >= revLimit)
  {
    bandScale = 0;
    netTorque = ((revLimit - (*(s32 *)(((u8 *)drive) + 0x78))) * 4) / 5;
  }
  else
  {
    bandIndex = (*(s32 *)(((u8 *)drive) + 0x78)) / 1000;
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
    bandEnd = *((&g_TorqueBandEnd) + bandIndex);
    bandSlot = bandBase;
    if (bandSlot < bandEnd)
    {
      engineSpeed = *(s32 *)(((u8 *)drive) + 0x78);
      curveSlot = (void *)((bandSlot * 4) + ((s32) gearCurve));
      specSlot = (void *)((bandSlot * 4) + ((s32) config));
      loop_68:
      bandTorque = *(s32 *)(((u8 *)specSlot) + 0x40);

      if ((engineSpeed >= bandTorque) && ((gearCurve = *(u8 **)(((u8 *)specSlot) + 0x44), (((s32) gearCurve) < engineSpeed) == 0)))
      {
        bandCurve = ((s32) gearCurve) - bandTorque;
        if (bandCurve <= 0)
        {
          bandCurve = 1;
        }
        frontLoadScaled = ((engineSpeed - bandTorque) * (*(s32 *)(((u8 *)curveSlot) + 4)));
        frontLoadScaled += (((s32) gearCurve) - engineSpeed) * (*(s32 *)(((u8 *)curveSlot) + 0));
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
    bandEnd = *((&g_TorqueLossBandEnd) + bandIndex);
    assistStep = lossBase;
    bandScale = 0;
    if (assistStep < bandEnd)
    {
      engineSpeedLoss = *(s32 *)(((u8 *)drive) + 0x78);
      gearCurve = (u8 *)((assistStep * 4) + ((s32) config));
      loop_83:
      lossTorque = *(s32 *)(((u8 *)gearCurve) + 0xA8);

      if (engineSpeedLoss >= lossTorque)
      {
        curveSlot = *(void **)(((u8 *)gearCurve) + 0xAC);
        assistStep += 1;
        if (((s32) curveSlot) >= engineSpeedLoss)
        {
          lossCurve = ((s32) curveSlot) - lossTorque;
          if (lossCurve <= 0)
          {
            lossCurve = 1;
          }
          bandScale = ((s32) (((engineSpeedLoss - lossTorque) * (*(s32 *)(((u8 *)gearCurve) + 0x84))) + ((((s32) curveSlot) - engineSpeedLoss) * (*(s32 *)(((u8 *)gearCurve) + 0x80))))) / lossCurve;
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
    if (((*(s16 *)(((u8 *)drive) + 0x76)) == 1) && ((*(s32 *)(((u8 *)drive) + 0x78)) < (g_CarSpec->redline)))
    {
      bandScale *= 2;
    }
  }
  shiftMode = *(s32 *)(((u8 *)drive) + 0x98);
  if ((shiftMode == 1) || (shiftMode == 3))
  {
    *(s16 *)(((u8 *)drive) + 0x38) = 0;
    *(u16 *)(((u8 *)drive) + 0x34) = 0U;
  }
  else
  {
    if (shiftMode == 2)
    {
      shiftTimer = *(s16 *)(((u8 *)drive) + 0x38);
      shiftTimerActive = shiftTimer >= 0;
      if (shiftTimerActive)
      {
        shiftTimerNext = shiftTimer - 1;
        *(s16 *)(((u8 *)drive) + 0x38) = shiftTimerNext;
        accel = 0;
        if (shiftTimerNext < 0)
        {
          *(s16 *)(((u8 *)drive) + 0x38) = 0;
        }
        targetGear = *(s16 *)(((u8 *)drive) + 0x76);
        if ((*(s16 *)(((u8 *)drive) + 0x30)) != targetGear)
        {
          shiftTargetRpm = ((s32) ((((*(s32 *)(((u8 *)car) + 0xA4)) * 0xA0) / 1168) * 0x2710)) / ((s32) (*(s32 *)(((u8 *)((u8 *)g_CarSpec - (-(targetGear * 4)))) + 0xE4)));
          currentSpeed = *(u16 *)(((u8 *)drive) + 0x78);
          g_ShiftTargetRpm = shiftTargetRpm;
          *(s16 *)(((u8 *)drive) + 0x3C) = (s16) (((u16) g_ShiftTargetRpm) - currentSpeed);
        }
        bandEnd = ((*(s16 *)(((u8 *)drive) + 0x3C)) * (*(s16 *)(((u8 *)drive) + 0x38))) / 20;
        shiftedSpeed = bandEnd;
        shiftedSpeed = shiftedSpeed + g_ShiftTargetRpm;
        goto block_129;
      }
    }
    targetGearAgain = *(s16 *)(((u8 *)drive) + 0x76);
    if ((*(s16 *)(((u8 *)drive) + 0x30)) != targetGearAgain)
    {
      switch (0) { default:
      gearCurve = (u8 *)(((s32) ((*(s32 *)(((u8 *)car) + 0xA4)) * 0x2710)) / ((s32) (((*(s32 *)(((u8 *)(config - (-(targetGearAgain * 4)))) + 0xE4)) * 0x490) / 160)));
      wheelSpeed = *(u16 *)(((u8 *)car) + 0xA8);
      wheelSpeedScaled = wheelSpeed;
      assistEnabled = *(s16 *)(((u8 *)drive) + 0x74);
      *(u16 *)(((u8 *)drive) + 0x2C) = wheelSpeedScaled;
      g_ShiftTargetSpeed = (s32) gearCurve;
      if (assistEnabled != 0)
      {
        targetGearCheck = *(s16 *)(((u8 *)drive) + 0x76);
        if (((*(s16 *)(((u8 *)drive) + 0x30)) < targetGearCheck) && (g_RoadGrade < 0))
        {
          if (targetGearCheck < 4)
          {
            break;
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
            break;
          }
          gradeScale = 0x64 - gradePenalty;
          *(u16 *)(((u8 *)drive) + 0x2C) = (u16) ((wheelSpeedScaled * gradeScale) / 100);
          g_ShiftTargetSpeed = (gradeScale * ((s32) gearCurve)) / 100;
        }
      }
      }
      shiftTargetSpeed = g_ShiftTargetSpeed;

      accel = 0;
      if ((*(s16 *)(((u8 *)drive) + 0x30)) > (*(s16 *)(((u8 *)drive) + 0x76)))
      {
        shiftTargetSpeed += 0x1F4;
      }
      g_ShiftTargetSpeed = shiftTargetSpeed;
      {
        u16 targetSpeed = (u16) g_ShiftTargetSpeed;
        u16 currentSpeed = *(u16 *)(((u8 *)drive) + 0x78);
        *(u16 *)(((u8 *)drive) + 0x34) = 0xAU;
        *(s16 *)(((u8 *)drive) + 0x2E) = 0;
        *(s16 *)(((u8 *)drive) + 0x36) = (s16) (targetSpeed - currentSpeed);
      }
    }
    else
    {
      {
        s32 countdown = --(*(u16 *)(((u8 *)drive) + 0x34));
      if (((s16) countdown) <= 0)
      {
        *(s16 *)(((u8 *)drive) + 0x2E) = 1;
        *(u16 *)(((u8 *)drive) + 0x2C) = 0U;
        *(u16 *)(((u8 *)drive) + 0x34) = 0U;
      }
      else
        if ((*(s16 *)(((u8 *)drive) + 0x74)) != 0)
      {
        *(s32 *)(((u8 *)drive) + 0x78) = (s32) (g_ShiftTargetSpeed - (((*(s16 *)(((u8 *)drive) + 0x36)) * ((s16) countdown)) / 15));
      }
      else
      {
        shiftRemaining = (*(s16 *)(((u8 *)drive) + 0x36)) * ((s16) countdown);
        switch (0) { default:
        lossBase = shiftRemaining / 10;
        *(s32 *)(((u8 *)drive) + 0x78) = g_ShiftTargetSpeed - lossBase;
        break;
        block_129:
        *(s32 *)(((u8 *)drive) + 0x78) = shiftedSpeed;
        }

      }
      }
    }
  }
  throttleTorque = (netTorque * (*(s16 *)(((u8 *)drive) + 0xA0))) * (*(s16 *)(((u8 *)drive) + 0x2E));
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
  if ((*(s16 *)(((u8 *)car) + 0x98)) == 0)
  {
    steerLoad += ((s32) (*(s32 *)(((u8 *)drive) + 0x78))) / 256;
  }
  accel += ((s32) ((*(s16 *)(((u8 *)drive) + 0xA2)) * (*(s32 *)(((u8 *)drive) + 0x78)))) / 8192;
  if (netTorque > 0)
  {
    if ((*(s16 *)(((u8 *)drive) + 0xA0)) < 0x7F)
    {
      accel += netTorque / 2;
    }
  }
  else
  {
    accel -= netTorque / 2;
  }
  headingError = GetAngleDistance(*(s32 *)(((u8 *)car) + 0x24), *(s32 *)(((u8 *)car) + 0xA0), (s32) gearCurve, curveSlot);
  *(s32 *)(((u8 *)drive) + 0x4C) = headingError;
  if (headingError >= 0x401)
  {
    *(s32 *)(((u8 *)drive) + 0x4C) = (s32) (0x800 - headingError);
  }
  steerLoad += ((s32) (*(s32 *)(((u8 *)drive) + 0x4C))) / 256;
  if (((*(s32 *)(((u8 *)drive) + 0x98)) != 1) && (g_PadType == 0x41))
  {
    assistStep = ((g_CarSpec->unk10E) * (*(s32 *)(((u8 *)drive) + 0x88))) / 1000;
    if (assistStep <= 0)
    {
      assistStep = 1;
    }
    shiftRemaining = *(s32 *)(((u8 *)drive) + 0x1C);
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
  trackHeadingError = GetAngleDistance(*(s32 *)(((u8 *)car) + 0xA0), 0xC00 - (*(s16 *)(((u8 *)(((*((s32 *)(((u8 *)car) + 0x30))) * 0x18) + ((u8 *)g_TrackPoints))) + 0xA)));
  frontLoadScaled = trackHeadingError;
  pointIndex = *(s32 *)(((u8 *)car) + 0x30);
  lateralOffset = *(s32 *)(((u8 *)car) + 0x38);
  engineSpeed = (*(s16 *)(((u8 *)((pointIndex * 0x18) + ((u8 *)g_TrackPoints))) + 0xC)) * (0x400 - lateralOffset);
  pointIndex += 1;
  lateralSum = engineSpeed + ((*(s16 *)(((u8 *)(((pointIndex % ((s32) g_TrackPointCount)) * 0x18) + ((u8 *)g_TrackPoints))) + 0xC)) * lateralOffset);
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
  if ((g_RacePhase == 2) && ((*(s32 *)(((u8 *)drive) + 0x98)) == 3))
  {
    steerLoad += (g_StandingStartSpin & 0x1F) * 5;
  }
  {
    s32 counter = D_8019C998;
    if (counter > 0)
    {
      s32 baseValue = steerLoad + 0xC8;
      steerLoad = baseValue + (counter * 0x14);
      D_8019C998 = counter - 1;
    }
  }
  if ((*(s32 *)(((u8 *)drive) + 0x98)) == 1)
  {
    throttleAccel = (throttleAccel * 4) / 5;
  }
  shiftTargetSpeed = (roadSpeed = ((*(s32 *)(((u8 *)car) + 0xA4)) * 0xA0) / 1168);
  dragBase = (s32) ((g_CarSpec->unk110) * 0x3E8);
  dragTerm = dragBase / ((s16) g_DragScale);
  if (dragTerm <= 0)
  {
    dragTerm = 1;
  }
  steerLoad += ((s32) (roadSpeed * roadSpeed)) / dragTerm;
  g_DragScale = 0x3E8;
  if ((*(s16 *)(((u8 *)car) + 0x98)) == 0)
  {
    steerLoad = (steerLoad * (0x64 - bandScale)) / 100;
  }
  else
  {
    throttleAccel *= 2;
    steerLoad = 0;
  }
  if (((*(s16 *)(((u8 *)drive) + 0x38)) <= 0) && (((s16) (*(u16 *)(((u8 *)drive) + 0x34))) <= 0))
  {
    *(s32 *)(((u8 *)drive) + 0x78) = (s32) (((throttleAccel - accel) - steerLoad) + (*(s32 *)(((u8 *)drive) + 0x78)));
  }
  speedForPath = *(s32 *)(((u8 *)drive) + 0x78);
  if (speedForPath < 0)
  {
    *(s32 *)(((u8 *)drive) + 0x78) = 0;
  }
  else
    if (speedForPath >= 0x3A99)
  {
    *(s32 *)(((u8 *)drive) + 0x78) = 0x3A98;
  }
  gearTorqueLate = gearRatio * (*(s32 *)(((u8 *)drive) + 0x78));
  *(s32 *)(((u8 *)drive) + 0x94) = gearTorqueLate;
  if ((*(s32 *)(((u8 *)drive) + 0x98)) == 1)
  {
    arcPointIndex = *(s32 *)(((u8 *)car) + 0x30);
    arcFlags = *(u16 *)(((u8 *)((arcPointIndex * 0x18) + ((u8 *)g_TrackPoints))) + 0x14);
    dragBase = arcFlags % 4;
    if (dragBase > 0)
    {
      arcCentre = (((((s32) (arcFlags << 0x10)) >> 13) >> 7) * 0xC) + g_TrackArcCenters;
      toCentreX = (*(s32 *)(((u8 *)car) + 0)) - (*(s32 *)(((u8 *)arcCentre) + 0));
      toCentreZ = (*(s32 *)(((u8 *)car) + 8)) - (*(s32 *)(((u8 *)arcCentre) + 4));
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
    dragTerm = (*(s16 *)(((u8 *)drive) + 0xA2)) * 0x14;
    coefficientBase = 0x26FC - downforce;
    coefficient = coefficientBase - (steerLoad * 2);
    if (dragTerm < 0)
    {
      dragTerm += 0xFF;
    }
    *(s32 *)(((u8 *)car) + 0xA4) = (s32) (((coefficient - (dragTerm >> 8)) * (*(s32 *)(((u8 *)car) + 0xA4))) / 10000);
    arcPointIndex = *(s32 *)(((u8 *)drive) + 0x94);
    if (arcPointIndex < 0)
    {
      arcPointIndex += 0x1FFFFF;
    }
    dragBase = arcPointIndex >> 0x15;
    *(s32 *)(((u8 *)car) + 0xA8) = dragBase;
  }
  else
  {
    if ((*(s16 *)(((u8 *)car) + 0x98)) != 0)
    {
      speedA = *(s32 *)(((u8 *)car) + 0xA4);
      *(s32 *)(((u8 *)car) + 0xA8) = 0;
      speedScaled = (speedA * 0x3E7) / 1000;
    }
    else
    {
      if (((s16) (*(u16 *)(((u8 *)drive) + 0x34))) > 0)
      {
        *(s32 *)(((u8 *)car) + 0xA8) = (s32) ((s16) (*(u16 *)(((u8 *)drive) + 0x2C)));
      }
      else
      {
        torqueLate = gearTorqueLate;
        if ((*(s16 *)(((u8 *)drive) + 0x38)) > 0)
        {
          *(s32 *)(((u8 *)car) + 0xA8) = (s32) ((s16) (*(u16 *)(((u8 *)drive) + 0x2C)));
        }
        else
        {
          if (torqueLate < 0)
          {
            torqueLate += 0x1FFFF;
          }
          torqueShifted = torqueLate >> 0x11;
          *(s32 *)(((u8 *)car) + 0xA8) = torqueShifted;
          if ((*(s16 *)(((u8 *)drive) + 0x74)) == 0)
          {
            *(s32 *)(((u8 *)car) + 0xA8) = (s32) (((g_CarSpec->unk102) * torqueShifted) / 1000);
          }
        }
      }
      if (g_GripLossTimer > 0)
      {
        *(s32 *)(((u8 *)car) + 0xA8) /= 2;
      }
      speedB = *(s32 *)(((u8 *)car) + 0xA4);
      speedScaled = (speedB * 0x5E) / 100;
    }
    *(s32 *)(((u8 *)car) + 0xA4) = speedScaled;
  }
  if ((*(s32 *)(((u8 *)car) + 0xA4)) < 8)
  {
    *(s32 *)(((u8 *)car) + 0xA0) = (s32) (*(s32 *)(((u8 *)car) + 0x24));
  }
  if (g_RacePhase >= 2)
  {
    driveModeLate = *(s32 *)(((u8 *)drive) + 0x98);
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
    *(s32 *)(((u8 *)car) + 0xA4) = 0;
  }
  if ((*(s32 *)(((u8 *)car) + 0xA4)) < 8)
  {
    *(s32 *)(((u8 *)car) + 0xA0) = (s32) (*(s32 *)(((u8 *)car) + 0x24));
  }
}
