#include "common.h"
#include "game/track.h"
#include "game/race.h"
#include "game/car.h"
#include "game/render.h"

void UpdateCarDriving();
void func_80030030();
void UpdateCarAirborne();
void UpdateCarStandingStart();
s32 rsin(s32) asm("func_80068568");
s32 rcos() asm("func_80068634");
extern s32 g_ShiftTargetSpeed asm("D_8007DA74");
extern s32 g_RoadGrade asm("D_8007DA78");
extern u8 *g_TrackArcCenters asm("D_8019C7D0");
/* Deliberately raw: the only initialiser anywhere is InitPlayerCar writing
 * zero, and the only other write is this decrement, so the boost it would add
 * can never fire. See docs/names.md 15g. */
extern s32 D_8019C998;
extern s32 g_StandingStartSpin asm("D_8019CA04");
/*
 * Rpm-band indexes into the car spec's two rpm->value curves, rebuilt per car
 * by InitPlayerCar: entry b holds how many curve points sit at or below rpm
 * band b, where band = drive->rpm / 1000. `Start` is the same table one
 * halfword earlier, i.e. band b - 1, so [Start[b], End[b]) is the range of
 * curve points this rpm can land in. The first curve (spec +0x40 / +0x44 x,
 * +0x00 / +0x04 y) yields the torque term; the second (spec +0xA8 / +0xAC x,
 * +0x80 / +0x84 y) a 0..100 percentage that is subtracted from it.
 */
extern s16 g_TorqueBandStart asm("D_801E4112");
extern s16 g_TorqueBandEnd asm("D_801E4114");
extern s16 g_TorqueLossBandStart asm("D_801E4152");
extern s16 g_TorqueLossBandEnd asm("D_801E4154");
extern u8 g_PadType asm("D_801E4369");
extern s16 g_GripLossTimer asm("D_801E4BA0");
extern s32 g_ShiftTargetRpm asm("D_801E4BF4");
/* Divisor of the speed-squared drag term: drag = v^2 / (spec->unk110 * 1000 /
 * this). Reset to 1000 at the end of every frame, so writers elsewhere
 * (func_8002CB30, CollidePlayerWithCars) change the drag for exactly one frame.
 * NOTE: docs/names.md 15g listed this as written-but-never-read; that is
 * wrong, the read is right here. */
extern s16 g_DragScale asm("D_801E4FB4");
/*
 * AI target-speed / drivetrain physics driver (called by UpdatePlayerCar). Reads
 * the per-car spec block g_CarSpec to compute a target speed, applies steering
 * assist and RPM, and dispatches the state98 motion handlers. `base` is the car
 * runtime, accessed via raw byte-offset pointer arithmetic (e.g.
 * *(s16*)((u8*)base+0x132)) with the drive sub-block at base+0xBC; the raw
 * offsets are what make it match, so it is intentionally left as void*.
 */
void UpdateCarDrivetrain(void *base);
void UpdateCarDrivetrain(void *base) {
  u8 *var_a2;
  s16 temp_a0;
  s16 temp_a0_2;
  s16 temp_a1;
  s32 temp_t1;
  s16 temp_v0_3;
  s16 temp_v0_5;
  s16 temp_a0_5;
  int new_var2;
  int steeringNonnegative;
  int secondNonnegative;
  s16 temp_v0_6;
  s16 temp_v0_7;
  s32 temp_v0_15;
  s16 temp_v1;
  s16 temp_v1_11;
  s16 temp_v1_2;
  s16 temp_v1_3;
  s16 temp_v1_4;
  s16 temp_v1_5;
  s32 var_a0_2;
  s32 temp_a0_3;
  s32 temp_a0_4;
  s32 temp_a0_7;
  s32 temp_a0_8;
  s32 temp_a0_9;
  s32 temp_a1_2;
  s32 temp_a1_3;
  s32 temp_a2;
  s32 temp_lo;
  s32 temp_lo_2;
  s32 temp_s2;
  s32 temp_lo_3;
  s32 temp_s0;
  s32 temp_s1;
  s32 temp_t0;
  s32 temp_t0_2;
  s32 temp_t2;
  s32 temp_v0;
  s32 temp_v0_10;
  s32 temp_v0_11;
  s32 temp_v0_12;
  s32 temp_v0_13;
  s32 shiftTargetRpm;
  s32 temp_v0_2;
  u8 *new_var3;
  s32 temp_v0_4;
  s32 temp_v0_9;
  s32 temp_v1_10;
  s32 temp_v1_12;
  s32 temp_v1_13;
  s32 temp_v1_15;
  s32 temp_v1_18;
  s32 temp_v1_19;
  s32 temp_v1_20;
  s32 temp_v1_21;
  s32 temp_v1_22;
  s32 temp_v1_7;
  s32 temp_v1_8;
  s32 temp_v1_9;
  s32 var_a0;
  s32 var_a0_3;
  s32 var_a0_4;
  s32 var_a0_5;
  s32 var_a0_6;
  s32 var_a1;
  s32 var_a1_2;
  s32 var_a1_3;
  s32 var_a1_4;
  int new_var;
  s32 var_a1_5;
  s32 var_a1_7;
  s32 var_s0;
  s32 var_s1;
  s32 var_s2;
  s32 var_s5;
  s32 var_s6;
  s32 var_s7;
  s32 var_t3;
  s32 var_v0_10;
  s32 var_v0_11;
  s32 var_v0_12;
  s32 var_v0_3;
  s32 var_v0_4;
  s32 var_v0_5;
  s32 var_v0_6;
  s32 var_v0_7;
  s32 var_v0_8;
  s32 var_v0_9;
  s32 var_v1;
  s32 var_v1_2;
  s32 var_v1_4;
  s32 var_v1_7;
  s32 var_v1_8;
  s32 var_v1_9;
  s32 coefficientBase;
  s32 coefficient;
  s32 temp_a0_6;
  s32 temp_v0_8;
  u16 temp_v1_16;
  u16 temp_v1_23;
  u16 var_v0;
  s16 var_v0_2;
  void *temp_s3;
  void *temp_v1_17;
  void *temp_v1_6;
  void *var_a3;
  void *var_v1_3;
  void *car;
  u8 *config;
  car = base;
  base = (u8 *)g_GearTorqueCurve;
  config = (u8 *)g_CarSpec;
  temp_v1 = *(s16 *)(((u8 *)car) + 0x132);
  new_var3 = ((u8 *)(config + (temp_v1 * 4))) + 0xCC;
  var_a2 = (temp_v1 * 64) + ((u8 *)base);
  var_s7 = *(s32 *)new_var3;
  temp_s3 = car + 0xBC;
  if (g_RacePhase < 2)
  {
    *(s16 *)(((u8 *)car) + 0xEC) = temp_v1;
    var_s7 = *(s32 *)(((u8 *)config) + 0xD0);
    var_a2 = base;
  }
  else
    if (((*(s32 *)(((u8 *)car) + 0x154)) == 3) && (((*(s16 *)(((u8 *)car) + 0x15C)) < 0x40) || ((*(s16 *)(((u8 *)car) + 0x15E)) >= 0x80)))
  {
    var_a2 = base;
  }
  temp_v1_2 = *(s16 *)(((u8 *)temp_s3) + 0x9C);
  if (temp_v1_2 == 0)
  {
    if ((*(s16 *)(((u8 *)temp_s3) + 0xA0)) >= 0x85)
    {
      *(s16 *)(((u8 *)temp_s3) + 0x9C) = 1;
    }
  }
  else
    if (temp_v1_2 == 1)
  {
    *(s16 *)(((u8 *)temp_s3) + 0x9C) = 2;
  }
  else
    if ((*(s16 *)(((u8 *)temp_s3) + 0xA0)) < 0x7C)
  {
    *(s16 *)(((u8 *)temp_s3) + 0x9C) = 0;
  }
  temp_v1_3 = *(s16 *)(((u8 *)temp_s3) + 0x9E);
  if (temp_v1_3 == 0)
  {
    if ((*(s16 *)(((u8 *)temp_s3) + 0xA2)) >= 0x85)
    {
      *(s16 *)(((u8 *)temp_s3) + 0x9E) = 1;
    }
  }
  else
    if (temp_v1_3 == 1)
  {
    *(s16 *)(((u8 *)temp_s3) + 0x9E) = 2;
  }
  else
    if ((*(s16 *)(((u8 *)temp_s3) + 0xA2)) < 0x7C)
  {
    *(s16 *)(((u8 *)temp_s3) + 0x9E) = 0;
  }
  temp_v0 = (*(s16 *)(((u8 *)temp_s3) + 0xA0)) * 0x64;
  var_a0 = temp_v0 >> 8;
  if (temp_v0 < 0)
  {
    var_a0 = ((s32) (temp_v0 + 0xFF)) >> 8;
  }
  var_a1 = 0x17C - var_a0;
  var_a1 += ((s32) ((*(s16 *)(((u8 *)temp_s3) + 0xA2)) * 0x64)) / 256;
  if ((*(s32 *)(((u8 *)temp_s3) + 0x98)) == 1)
  {
    temp_v1_4 = *(s16 *)(((u8 *)temp_s3) + 0x40);
    temp_v0_2 = (*(u16 *)(((u8 *)((((*((s32 *)(((u8 *)car) + 0x30))) * 3) * 8) + ((u8 *)g_TrackPoints))) + 0x14)) & 3;
    if (temp_v1_4 != temp_v0_2)
    {
      if (temp_v1_4 != 0)
      {
        var_v0 = (*(u16 *)(((u8 *)temp_s3) + 0x42)) - 1;
        goto block_29;
      }
      if (temp_v0_2 == 0)
      {
        goto block_27;
      }
    }
    else
    {
      block_27:
      if ((*(s16 *)(((u8 *)temp_s3) + 0x40)) != 0)
      {
        var_v0 = (*(u16 *)(((u8 *)temp_s3) + 0x42)) + 2;
        block_29:
        *(u16 *)(((u8 *)temp_s3) + 0x42) = var_v0;

      }

    }
    temp_v1_5 = (s16) (*(u16 *)(((u8 *)temp_s3) + 0x42));
    if (temp_v1_5 >= 0x1F)
    {
      *(s16 *)(((u8 *)temp_s3) + 0x42) = 0x1E;
    }
    else if (temp_v1_5 < (-0x1E))
    {
      *(s16 *)(((u8 *)temp_s3) + 0x42) = -0x1E;
    }
    var_a1 += (g_CarSpec->unk112) - (((s16) (*(u16 *)(((u8 *)temp_s3) + 0x42))) * 0xA);
    *(s16 *)(((u8 *)temp_s3) + 0x32) = (s16) var_a1;
  }
  else
  {
    temp_v1_6 = ((*(s32 *)(((u8 *)car) + 0x30)) * 0x18) + ((u8 *)g_TrackPoints);
    temp_a0 = *(s16 *)(((u8 *)temp_s3) + 0x40);
    if ((temp_a0 != ((*(u16 *)(((u8 *)temp_v1_6) + 0x14)) & 3)) && (temp_a0 != 0))
    {
      var_a0_2 = *(s16 *)(((u8 *)temp_v1_6) + 0xE);
      if (var_a0_2 < (-0x32))
      {
        var_a0_2 = -0x32;
      }
      else
        if (var_a0_2 >= 0x33)
      {
        var_a0_2 = 0x32;
      }
      if (((*(u16 *)(((u8 *)(((*((s32 *)(((u8 *)car) + 0x30))) * 0x18) + ((u8 *)g_TrackPoints))) + 0x14)) & 3) == 1)
      {
        var_v0_3 = (-(var_a0_2 * 0x3C)) / 20;
      }
      else
      {
        var_v0_3 = var_a0_2 * 3;
      }
      var_a1 += var_v0_3;
    }
    *(s16 *)(((u8 *)temp_s3) + 0x32) = (s16) (((s32) ((*(s16 *)(((u8 *)temp_s3) + 0x32)) + ((var_a1 * (*(s32 *)(((u8 *)temp_s3) + 0x88))) / 1000))) / 2);
  }
  temp_lo = var_s7 * (*(s32 *)(((u8 *)temp_s3) + 0x78));
  var_s5 = 0;
  temp_v1_7 = *(s32 *)(((u8 *)temp_s3) + 0x94);
  var_t3 = temp_lo - temp_v1_7;
  temp_v1_8 = *(s32 *)(((u8 *)temp_s3) + 0x98);
  var_s1 = 0;
  if (temp_v1_8 == 1)
  {
    var_v0_4 = var_t3;
    if (var_t3 < 0)
    {
      var_v0_4 = var_t3 + 0xFFF;
    }
    var_s1 = var_v0_4;
    var_s1 = var_s1 >> 0xC;
  }
  else
    if (var_t3 >= (-0x30D3))
  {
    if (var_t3 > 0x186A0)
    {
      var_v1 = var_t3;
      if (var_t3 < 0)
      {
        var_v1 = var_t3 + 0xFF;
      }
      var_s1 = ((var_v1 >> 8) * 0x46) / 200;
    }
  }
  else
    if (temp_v1_8 == 3)
  {
    var_s1 = (temp_lo - temp_v1_7) / 768;
  }
  else
  {
    var_v0_5 = var_t3;
    if (var_t3 < 0)
    {
      var_v0_5 = var_t3 + 0x7FF;
    }
    var_s1 = var_v0_5 >> 0xB;
  }
  temp_a0_2 = ((GameCarSpec *)config)->revLimit;
  if ((*(s32 *)(((u8 *)temp_s3) + 0x78)) >= temp_a0_2)
  {
    var_s2 = 0;
    var_t3 = ((temp_a0_2 - (*(s32 *)(((u8 *)temp_s3) + 0x78))) * 4) / 5;
  }
  else
  {
    temp_t2 = (*(s32 *)(((u8 *)temp_s3) + 0x78)) / 1000;
    if (temp_t2 == 0)
    {
      var_v1_2 = 0;
    }
    else
    {
      temp_v0_3 = *((&g_TorqueBandStart) + temp_t2);
      if (temp_v0_3 == 0)
      {
        var_v1_2 = 0;
      }
      else
      {
        var_v1_2 = temp_v0_3 - 1;
      }
    }
    temp_t1 = *((&g_TorqueBandEnd) + temp_t2);
    var_a1_2 = var_v1_2;
    if (var_a1_2 < temp_t1)
    {
      temp_t0 = *(s32 *)(((u8 *)temp_s3) + 0x78);
      var_a3 = (void *)((var_a1_2 * 4) + ((s32) var_a2));
      var_v1_3 = (void *)((var_a1_2 * 4) + ((s32) config));
      loop_68:
      temp_a0_3 = *(s32 *)(((u8 *)var_v1_3) + 0x40);

      if ((temp_t0 >= temp_a0_3) && ((var_a2 = *(u8 **)(((u8 *)var_v1_3) + 0x44), (((s32) var_a2) < temp_t0) == 0)))
      {
        var_a1_3 = ((s32) var_a2) - temp_a0_3;
        if (var_a1_3 <= 0)
        {
          var_a1_3 = 1;
        }
        var_a0 = ((temp_t0 - temp_a0_3) * (*(s32 *)(((u8 *)var_a3) + 4)));
        var_a0 += (((s32) var_a2) - temp_t0) * (*(s32 *)(((u8 *)var_a3) + 0));
        var_t3 = var_a0 / ((s32) (var_a1_3 * 0xA));
      }
      else
      {
        var_a3 += 4;
        var_a1_2 += 1;
        var_v1_3 += 4;
        if (var_a1_2 < temp_t1)
        {
          goto loop_68;
        }
      }
    }
    if (var_t3 < 0)
    {
      var_t3 = 0;
    }
    if (temp_t2 == 0)
    {
      var_v1_4 = 0;
    }
    else
    {
      temp_v0_5 = *((&g_TorqueLossBandStart) + temp_t2);
      var_v1_4 = 0;
      if (temp_v0_5 != 0)
      {
        var_v1_4 = temp_v0_5 - 1;
      }
    }
    temp_t1 = *((&g_TorqueLossBandEnd) + temp_t2);
    var_a1_4 = var_v1_4;
    var_s2 = 0;
    if (var_a1_4 < temp_t1)
    {
      temp_t0_2 = *(s32 *)(((u8 *)temp_s3) + 0x78);
      var_a2 = (u8 *)((var_a1_4 * 4) + ((s32) config));
      loop_83:
      temp_a0_4 = *(s32 *)(((u8 *)var_a2) + 0xA8);

      if (temp_t0_2 >= temp_a0_4)
      {
        var_a3 = *(void **)(((u8 *)var_a2) + 0xAC);
        var_a1_4 += 1;
        if (((s32) var_a3) >= temp_t0_2)
        {
          var_a1_5 = ((s32) var_a3) - temp_a0_4;
          if (var_a1_5 <= 0)
          {
            var_a1_5 = 1;
          }
          var_s2 = ((s32) (((temp_t0_2 - temp_a0_4) * (*(s32 *)(((u8 *)var_a2) + 0x84))) + ((((s32) var_a3) - temp_t0_2) * (*(s32 *)(((u8 *)var_a2) + 0x80))))) / var_a1_5;
          var_v0_8 = var_s2 < 0x64;
        }
        else
        {
          goto block_89;
        }
      }
      else
      {
        var_a1_4 = var_a1_4 + 1;
        block_89:
        var_a2 += 4;

        if (var_a1_4 >= temp_t1)
        {
          goto block_90;
        }
        goto loop_83;
      }
    }
    else
    {
      block_90:
      var_v0_8 = var_s2 < 0x64;

    }
    if (var_v0_8 == 0)
    {
      var_s2 = 0x64;
    }
    else
      if (var_s2 <= 0)
    {
      var_s2 = 0;
    }
    if (((*(s16 *)(((u8 *)temp_s3) + 0x76)) == 1) && ((*(s32 *)(((u8 *)temp_s3) + 0x78)) < (g_CarSpec->redline)))
    {
      var_s2 *= 2;
    }
  }
  temp_v1_10 = *(s32 *)(((u8 *)temp_s3) + 0x98);
  if ((temp_v1_10 == 1) || (temp_v1_10 == 3))
  {
    *(s16 *)(((u8 *)temp_s3) + 0x38) = 0;
    *(u16 *)(((u8 *)temp_s3) + 0x34) = 0U;
  }
  else
  {
    if (temp_v1_10 == 2)
    {
      temp_v0_6 = *(s16 *)(((u8 *)temp_s3) + 0x38);
      new_var = temp_v0_6 >= 0;
      if (new_var)
      {
        temp_v0_7 = temp_v0_6 - 1;
        *(s16 *)(((u8 *)temp_s3) + 0x38) = temp_v0_7;
        var_s1 = 0;
        if (temp_v0_7 < 0)
        {
          *(s16 *)(((u8 *)temp_s3) + 0x38) = 0;
        }
        temp_a1 = *(s16 *)(((u8 *)temp_s3) + 0x76);
        if ((*(s16 *)(((u8 *)temp_s3) + 0x30)) != temp_a1)
        {
          shiftTargetRpm = ((s32) ((((*(s32 *)(((u8 *)car) + 0xA4)) * 0xA0) / 1168) * 0x2710)) / ((s32) (*(s32 *)(((u8 *)((u8 *)g_CarSpec - (-(temp_a1 * 4)))) + 0xE4)));
          temp_v1_23 = *(u16 *)(((u8 *)temp_s3) + 0x78);
          g_ShiftTargetRpm = shiftTargetRpm;
          *(s16 *)(((u8 *)temp_s3) + 0x3C) = (s16) (((u16) g_ShiftTargetRpm) - temp_v1_23);
        }
        temp_t1 = ((*(s16 *)(((u8 *)temp_s3) + 0x3C)) * (*(s16 *)(((u8 *)temp_s3) + 0x38))) / 20;
        var_v0_9 = temp_t1;
        var_v0_9 = var_v0_9 + g_ShiftTargetRpm;
        goto block_129;
      }
    }
    temp_a0_5 = *(s16 *)(((u8 *)temp_s3) + 0x76);
    if ((*(s16 *)(((u8 *)temp_s3) + 0x30)) != temp_a0_5)
    {
      switch (0) { default:
      var_a2 = (u8 *)(((s32) ((*(s32 *)(((u8 *)car) + 0xA4)) * 0x2710)) / ((s32) (((*(s32 *)(((u8 *)(config - (-(temp_a0_5 * 4)))) + 0xE4)) * 0x490) / 160)));
      temp_a1_3 = *(u16 *)(((u8 *)car) + 0xA8);
      temp_a0_6 = temp_a1_3;
      temp_v0_15 = *(s16 *)(((u8 *)temp_s3) + 0x74);
      *(u16 *)(((u8 *)temp_s3) + 0x2C) = temp_a0_6;
      g_ShiftTargetSpeed = (s32) var_a2;
      if (temp_v0_15 != 0)
      {
        temp_v1_11 = *(s16 *)(((u8 *)temp_s3) + 0x76);
        if (((*(s16 *)(((u8 *)temp_s3) + 0x30)) < temp_v1_11) && (g_RoadGrade < 0))
        {
          if (temp_v1_11 < 4)
          {
            break;
          }
          if (temp_v1_11 == 4)
          {
            var_v0_10 = (-g_RoadGrade) / 120;
            temp_a0_6 = (s32) ((u32) temp_a0_6 << 16);
            temp_a0_6 >>= 16;
          }
          else
            if (temp_v1_11 == 5)
          {
            var_v0_10 = (-g_RoadGrade) / 48;
            temp_a0_6 = (s32) ((u32) temp_a0_6 << 16);
            temp_a0_6 >>= 16;
          }
          else
            if (temp_v1_11 >= 6)
          {
            var_v0_10 = (g_RoadGrade * (-7)) / 240;
            temp_a0_6 = (s32) ((u32) temp_a0_6 << 16);
            temp_a0_6 >>= 16;
          }
          else
          {
            break;
          }
          temp_v1_12 = 0x64 - var_v0_10;
          *(u16 *)(((u8 *)temp_s3) + 0x2C) = (u16) ((temp_a0_6 * temp_v1_12) / 100);
          g_ShiftTargetSpeed = (temp_v1_12 * ((s32) var_a2)) / 100;
        }
      }
      }
      var_a0_3 = g_ShiftTargetSpeed;

      var_s1 = 0;
      if ((*(s16 *)(((u8 *)temp_s3) + 0x30)) > (*(s16 *)(((u8 *)temp_s3) + 0x76)))
      {
        var_a0_3 += 0x1F4;
      }
      g_ShiftTargetSpeed = var_a0_3;
      {
        u16 targetSpeed = (u16) g_ShiftTargetSpeed;
        u16 currentSpeed = *(u16 *)(((u8 *)temp_s3) + 0x78);
        *(u16 *)(((u8 *)temp_s3) + 0x34) = 0xAU;
        *(s16 *)(((u8 *)temp_s3) + 0x2E) = 0;
        *(s16 *)(((u8 *)temp_s3) + 0x36) = (s16) (targetSpeed - currentSpeed);
      }
    }
    else
    {
      {
        s32 countdown = --(*(u16 *)(((u8 *)temp_s3) + 0x34));
      if (((s16) countdown) <= 0)
      {
        *(s16 *)(((u8 *)temp_s3) + 0x2E) = 1;
        *(u16 *)(((u8 *)temp_s3) + 0x2C) = 0U;
        *(u16 *)(((u8 *)temp_s3) + 0x34) = 0U;
      }
      else
        if ((*(s16 *)(((u8 *)temp_s3) + 0x74)) != 0)
      {
        *(s32 *)(((u8 *)temp_s3) + 0x78) = (s32) (g_ShiftTargetSpeed - (((*(s16 *)(((u8 *)temp_s3) + 0x36)) * ((s16) countdown)) / 15));
      }
      else
      {
        temp_a0_7 = (*(s16 *)(((u8 *)temp_s3) + 0x36)) * ((s16) countdown);
        switch (0) { default:
        var_v1_4 = temp_a0_7 / 10;
        *(s32 *)(((u8 *)temp_s3) + 0x78) = g_ShiftTargetSpeed - var_v1_4;
        break;
        block_129:
        *(s32 *)(((u8 *)temp_s3) + 0x78) = var_v0_9;
        }

      }
      }
    }
  }
  var_v1_7 = (var_t3 * (*(s16 *)(((u8 *)temp_s3) + 0xA0))) * (*(s16 *)(((u8 *)temp_s3) + 0x2E));
  if (var_v1_7 < 0)
  {
    var_v1_7 += 0xFF;
  }
  var_s6 = var_v1_7 >> 8;
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
    var_s5 += ((s32) (*(s32 *)(((u8 *)temp_s3) + 0x78))) / 256;
  }
  var_s1 += ((s32) ((*(s16 *)(((u8 *)temp_s3) + 0xA2)) * (*(s32 *)(((u8 *)temp_s3) + 0x78)))) / 8192;
  if (var_t3 > 0)
  {
    if ((*(s16 *)(((u8 *)temp_s3) + 0xA0)) < 0x7F)
    {
      var_s1 += var_t3 / 2;
    }
  }
  else
  {
    var_s1 -= var_t3 / 2;
  }
  temp_v0_9 = GetAngleDistance(*(s32 *)(((u8 *)car) + 0x24), *(s32 *)(((u8 *)car) + 0xA0), (s32) var_a2, var_a3);
  *(s32 *)(((u8 *)temp_s3) + 0x4C) = temp_v0_9;
  if (temp_v0_9 >= 0x401)
  {
    *(s32 *)(((u8 *)temp_s3) + 0x4C) = (s32) (0x800 - temp_v0_9);
  }
  var_s5 += ((s32) (*(s32 *)(((u8 *)temp_s3) + 0x4C))) / 256;
  if (((*(s32 *)(((u8 *)temp_s3) + 0x98)) != 1) && (g_PadType == 0x41))
  {
    var_a1_4 = ((g_CarSpec->unk10E) * (*(s32 *)(((u8 *)temp_s3) + 0x88))) / 1000;
    if (var_a1_4 <= 0)
    {
      var_a1_4 = 1;
    }
    temp_a0_7 = *(s32 *)(((u8 *)temp_s3) + 0x1C);
    new_var2 = temp_a0_7 >= 0;
    if (new_var2)
    {
      var_s5 += ((s32) ((temp_a0_7 * 5) / 6)) / var_a1_4;
    }
    else
    {
      var_s5 -= ((s32) ((temp_a0_7 * 5) / 6)) / var_a1_4;
    }
  }
  temp_a0_8 = GetAngleDistance(*(s32 *)(((u8 *)car) + 0xA0), 0xC00 - (*(s16 *)(((u8 *)(((*((s32 *)(((u8 *)car) + 0x30))) * 0x18) + ((u8 *)g_TrackPoints))) + 0xA)));
  var_a0 = temp_a0_8;
  temp_a1_2 = *(s32 *)(((u8 *)car) + 0x30);
  temp_a2 = *(s32 *)(((u8 *)car) + 0x38);
  temp_t0 = (*(s16 *)(((u8 *)((temp_a1_2 * 0x18) + ((u8 *)g_TrackPoints))) + 0xC)) * (0x400 - temp_a2);
  temp_a1_2 += 1;
  var_v0_11 = temp_t0 + ((*(s16 *)(((u8 *)(((temp_a1_2 % ((s32) g_TrackPointCount)) * 0x18) + ((u8 *)g_TrackPoints))) + 0xC)) * temp_a2);
  secondNonnegative = var_v0_11 >= 0;
  if (!secondNonnegative)
  {
    var_v0_11 += 0x3FF;
  }
  var_s0 = var_v0_11 >> 0xA;
  temp_lo_2 = var_s0 * rcos(var_a0);
  var_s0 = temp_lo_2 >> 0xC;
  if (temp_lo_2 < 0)
  {
    var_s0 = ((s32) (temp_lo_2 + 0xFFF)) >> 0xC;
  }
  if (var_s0 < (-0xEE))
  {
    var_s0 = -0xEE;
  }
  else
    if (var_s0 >= 0xEF)
  {
    var_s0 = 0xEE;
  }
  temp_v1_13 = (-rsin(var_s0)) * 0x708;
  g_RoadGrade = var_s0;
  var_a0 = temp_v1_13 / 0xA000;
  steeringNonnegative = var_s0 >= 0;
  if (!steeringNonnegative)
  {
    var_s5 += var_a0;
  }
  else
  {
    var_s5 += var_a0 / 10;
  }
  if ((g_RacePhase == 2) && ((*(s32 *)(((u8 *)temp_s3) + 0x98)) == 3))
  {
    var_s5 += (g_StandingStartSpin & 0x1F) * 5;
  }
  {
    s32 counter = D_8019C998;
    if (counter > 0)
    {
      s32 baseValue = var_s5 + 0xC8;
      var_s5 = baseValue + (counter * 0x14);
      D_8019C998 = counter - 1;
    }
  }
  if ((*(s32 *)(((u8 *)temp_s3) + 0x98)) == 1)
  {
    var_s6 = (var_s6 * 4) / 5;
  }
  var_a0_3 = (temp_v1_15 = ((*(s32 *)(((u8 *)car) + 0xA4)) * 0xA0) / 1168);
  var_v0_12 = (s32) ((g_CarSpec->unk110) * 0x3E8);
  var_a1_7 = var_v0_12 / ((s16) g_DragScale);
  if (var_a1_7 <= 0)
  {
    var_a1_7 = 1;
  }
  var_s5 += ((s32) (temp_v1_15 * temp_v1_15)) / var_a1_7;
  g_DragScale = 0x3E8;
  if ((*(s16 *)(((u8 *)car) + 0x98)) == 0)
  {
    var_s5 = (var_s5 * (0x64 - var_s2)) / 100;
  }
  else
  {
    var_s6 *= 2;
    var_s5 = 0;
  }
  if (((*(s16 *)(((u8 *)temp_s3) + 0x38)) <= 0) && (((s16) (*(u16 *)(((u8 *)temp_s3) + 0x34))) <= 0))
  {
    *(s32 *)(((u8 *)temp_s3) + 0x78) = (s32) (((var_s6 - var_s1) - var_s5) + (*(s32 *)(((u8 *)temp_s3) + 0x78)));
  }
  temp_v0_11 = *(s32 *)(((u8 *)temp_s3) + 0x78);
  if (temp_v0_11 < 0)
  {
    *(s32 *)(((u8 *)temp_s3) + 0x78) = 0;
  }
  else
    if (temp_v0_11 >= 0x3A99)
  {
    *(s32 *)(((u8 *)temp_s3) + 0x78) = 0x3A98;
  }
  temp_lo_3 = var_s7 * (*(s32 *)(((u8 *)temp_s3) + 0x78));
  *(s32 *)(((u8 *)temp_s3) + 0x94) = temp_lo_3;
  if ((*(s32 *)(((u8 *)temp_s3) + 0x98)) == 1)
  {
    temp_v1_18 = *(s32 *)(((u8 *)car) + 0x30);
    temp_v1_16 = *(u16 *)(((u8 *)((temp_v1_18 * 0x18) + ((u8 *)g_TrackPoints))) + 0x14);
    var_v0_12 = temp_v1_16 % 4;
    if (var_v0_12 > 0)
    {
      temp_v1_17 = (((((s32) (temp_v1_16 << 0x10)) >> 13) >> 7) * 0xC) + g_TrackArcCenters;
      temp_s2 = (*(s32 *)(((u8 *)car) + 0)) - (*(s32 *)(((u8 *)temp_v1_17) + 0));
      temp_s1 = (*(s32 *)(((u8 *)car) + 8)) - (*(s32 *)(((u8 *)temp_v1_17) + 4));
      temp_v0_12 = Atan2(temp_s2, temp_s1);
      temp_s0 = rcos(temp_v0_12);
      temp_v0_13 = (temp_s0 * temp_s2) + (rsin(temp_v0_12) * temp_s1);
      var_a0 = temp_v0_13 >> 0xC;
      if (temp_v0_13 < 0)
      {
        var_a0 = ((s32) (temp_v0_13 + 0xFFF)) >> 0xC;
      }
    }
    else
    {
      var_a0 = (g_CarSpec->unk10C) * 0x64;
    }
    if ((var_a0 <= 0) || ((var_a0_5 = (g_CarSpec->unk10C) * 0x64, var_a0_5 <= 0)))
    {
      var_a0_5 = (g_CarSpec->unk10C) * 0x64;
    }
    var_a0_6 = ((s32) ((g_CarSpec->unk10C) * 0x64)) / var_a0_5;
    if (var_a0_6 <= 0)
    {
      var_a0_6 = 1;
    }
    var_a1_7 = (*(s16 *)(((u8 *)temp_s3) + 0xA2)) * 0x14;
    coefficientBase = 0x26FC - var_a0_6;
    coefficient = coefficientBase - (var_s5 * 2);
    if (var_a1_7 < 0)
    {
      var_a1_7 += 0xFF;
    }
    *(s32 *)(((u8 *)car) + 0xA4) = (s32) (((coefficient - (var_a1_7 >> 8)) * (*(s32 *)(((u8 *)car) + 0xA4))) / 10000);
    temp_v1_18 = *(s32 *)(((u8 *)temp_s3) + 0x94);
    if (temp_v1_18 < 0)
    {
      temp_v1_18 += 0x1FFFFF;
    }
    var_v0_12 = temp_v1_18 >> 0x15;
    *(s32 *)(((u8 *)car) + 0xA8) = var_v0_12;
  }
  else
  {
    if ((*(s16 *)(((u8 *)car) + 0x98)) != 0)
    {
      temp_v1_19 = *(s32 *)(((u8 *)car) + 0xA4);
      *(s32 *)(((u8 *)car) + 0xA8) = 0;
      var_v1_8 = (temp_v1_19 * 0x3E7) / 1000;
    }
    else
    {
      if (((s16) (*(u16 *)(((u8 *)temp_s3) + 0x34))) > 0)
      {
        *(s32 *)(((u8 *)car) + 0xA8) = (s32) ((s16) (*(u16 *)(((u8 *)temp_s3) + 0x2C)));
      }
      else
      {
        var_v1_9 = temp_lo_3;
        if ((*(s16 *)(((u8 *)temp_s3) + 0x38)) > 0)
        {
          *(s32 *)(((u8 *)car) + 0xA8) = (s32) ((s16) (*(u16 *)(((u8 *)temp_s3) + 0x2C)));
        }
        else
        {
          if (var_v1_9 < 0)
          {
            var_v1_9 += 0x1FFFF;
          }
          temp_v1_20 = var_v1_9 >> 0x11;
          *(s32 *)(((u8 *)car) + 0xA8) = temp_v1_20;
          if ((*(s16 *)(((u8 *)temp_s3) + 0x74)) == 0)
          {
            *(s32 *)(((u8 *)car) + 0xA8) = (s32) (((g_CarSpec->unk102) * temp_v1_20) / 1000);
          }
        }
      }
      if (g_GripLossTimer > 0)
      {
        *(s32 *)(((u8 *)car) + 0xA8) /= 2;
      }
      temp_v1_21 = *(s32 *)(((u8 *)car) + 0xA4);
      var_v1_8 = (temp_v1_21 * 0x5E) / 100;
    }
    *(s32 *)(((u8 *)car) + 0xA4) = var_v1_8;
  }
  if ((*(s32 *)(((u8 *)car) + 0xA4)) < 8)
  {
    *(s32 *)(((u8 *)car) + 0xA0) = (s32) (*(s32 *)(((u8 *)car) + 0x24));
  }
  if (g_RacePhase >= 2)
  {
    temp_v1_22 = *(s32 *)(((u8 *)temp_s3) + 0x98);
    switch (temp_v1_22)
    {
      case 0:
        UpdateCarDriving(car, var_a1_7);
        break;

      case 1:
        func_80030030(car, var_a1_7);
        break;

      case 2:
        UpdateCarAirborne(car, var_a1_7);
        break;

      case 3:
        UpdateCarStandingStart(car, var_a1_7);
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
