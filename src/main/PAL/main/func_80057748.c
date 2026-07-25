#include "common.h"
/*
 * func_80057748: sound/menu state machine. The redundant
 * `if (D_801E436E) { ... } else { ... }` around the state>0 block is a
 * deliberate no-op guard (both arms identical): its presence forces GCC 2.6.3
 * to rematerialize the literal 2 at both the direction ternary and the switch
 * case instead of CSE-ing it into a saved register, reproducing retail codegen.
 * Verified byte-exact; both arms are semantically identical so behaviour is
 * unchanged. This is the accepted resolution while the legal-only permuter
 * searches for a cleaner shape.
 */
#include "psyq/gpu.h"
#include "game/race.h"

extern s32 D_8009B300;
extern s32 D_8009B308;
extern s32 D_8009B314;
extern s32 D_8009B318;
extern s32 D_8009B340;
extern s32 D_8009B344;
extern u8 D_8009B2F0;
extern s32 D_8009B2F8;
extern s32 D_8019CB0C;
extern s32 D_8019C9F0;
extern s32 D_8019C9F8;
extern s32 D_8019CAB8;
extern void *D_801E8A44;
extern s32 D_801F1804;
extern u16 D_801E436E;
extern u16 D_801E444C[];
extern Rect D_8007BEDC;
extern u8 D_80082844;
extern u8 D_80082790;
extern u8 D_80081C14;
extern u8 D_80082460;
extern u8 D_80082574;
extern u8 D_80082664;
extern u8 D_80011BA0;

void func_80046A2C(void *ot, s32 x0, s32 y0, s32 x1, s32 y1, s32 u0, s32 v0,
                   s32 r, s32 g, s32 b, s32 clut, s32 sh, s32 st, s32 flags);
s32 func_800487D8(u8 *commands, s32 *progress, s32 step);
void func_800489AC(s32 arg0, s32 arg1, s32 arg2);
void func_80048B88(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                   s32 a7, s32 a8, s32 a9, s32 a10);
void func_80048D64(s32 x0, s32 y0, s32 x1, s32 y1, s32 useFlash);
void func_8004A248(s32 arg0, s32 arg1);
void func_8004B8B4(s32 arg0, s32 arg1);
void func_8004C0D8(void);
void func_8004E368(s32 arg0, s32 arg1);
void func_8005D6EC(s32 arg0);
void func_8005EA14(void);
void func_8005EA6C(void);
void func_80065B24(Rect *rect, void *data);

void func_80057748(void)
{
  void *ot;
  s32 state;
  s32 sel;
  s32 edge;
  s32 cnt;
  int new_var;
  ot = *((void **) 0x1F800004);
  D_8019CB0C = 0;
  state = D_8009B308;
  if (state == 0)
  {
    func_8004B8B4(-13, -21);
    func_800487D8(&D_80082844, &D_8009B2F8, -1);
    func_800487D8(&D_80082790, &D_8009B2F8, 0);
    func_800487D8(D_801E8A44, &D_8009B2F8, 0);
    func_8004A248(1, -1);
    func_800489AC(D_8019C9F0, 2, D_801F1804);
    func_800487D8(&D_80081C14, &D_8019C9F0, 0);
    if ((func_800487D8(&D_80082460, &D_8019C9F0, 1) != 0) && (D_8009B2F8 <= 0))
    {
      D_8009B314 = 1;
      D_8009B318 = -1;
      if (D_801E436E & 0x1000)
      {
        func_8005D6EC(1);
        D_801F1804 = (D_801F1804 > 0) ? (D_801F1804 - 1) : (2);
      }
      if (D_801E436E & 0x4000)
      {
        func_8005D6EC(1);
        D_801F1804 = (D_801F1804 < 2) ? (D_801F1804 + 1) : (0);
      }
      edge = D_801E436E;
      if (edge & 0x860)
      {
        sel = D_801F1804;
        if (sel == 0)
        {
          func_8005D6EC(2);
          D_8009B308 = -1;
          D_8009B2F0 = 0;
          D_8009B2F8 = 0;
          D_801E8A44 = &D_80082574;
        }
        else
          if (sel == 1)
        {
          func_8005D6EC(2);
          func_8005EA14();
          D_8009B308 = -3;
          D_8019CAB8 = 0;
          D_8009B2F8 = 0;
          D_801E8A44 = &D_80082664;
        }
        else
          if (sel == 2)
        {
          func_8005D6EC(3);
          D_8009B308 = sel;
          D_8009B318 = sel;
        }
      }
      else
        if (edge & 0x90)
      {
        func_8005D6EC(3);
        D_8009B308 = 2;
        D_8009B318 = 2;
      }
    }
  }
  else
    if (state < 0)
  {
    if (state == (-1))
    {
      u16 *pad;
      func_800487D8(&D_80082844, &D_8009B2F8, 0);
      func_800487D8(&D_80082790, &D_8009B2F8, 0);
      if (func_800487D8(D_801E8A44, &D_8009B2F8, 1) != 0)
      {
        if (D_801E436E & 0x860)
        {
          if (D_8009B2F0 != 0)
          {
            func_8005D6EC(2);
            D_8009B308 = -2;
            D_8009B300 = 0x23;
          }
          else
          {
            func_8005D6EC(3);
            D_8009B308 = 0;
          }
        }
        pad = (u16 *) (&D_801E436E);
        if ((*pad) & 0x90)
        {
          func_8005D6EC(3);
          D_8009B308 = 0;
        }
        if ((*pad) & 0x8000)
        {
          if (D_8009B2F0 == 0)
          {
            func_8005D6EC(1);
            D_8009B2F0 = 1;
          }
        }
        if (D_801E436E & 0x2000)
        {
          if (D_8009B2F0 != 0)
          {
            func_8005D6EC(1);
            D_8009B2F0 = 0;
          }
        }
        func_80048D64((D_8009B2F0 != 0) ? (0xB8) : (0xDA), 0x44, 0x20, 0x20, 0);
        func_80046A2C(ot, 0xC0, 0x4C, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
        func_80046A2C(ot, 0xE3, 0x4C, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
        new_var = 0x20;
        func_80048B88(0xB8, 0x44, 0x20, new_var, 0x95, 0x25, 0x1E, 0, 0, 0, (s32) (&D_80011BA0));
        func_80048B88(0xDA, 0x44, 0x20, 0x20, 0x3A, 0x1E, 0x95, 0, 0, 0, (s32) (&D_80011BA0));
      }
      func_8004A248(1, 0);
    }
    else
      if (state == (-2))
    {
      cnt = D_8009B300;
      if (cnt <= 0)
      {
        func_800487D8(&D_80082844, &D_8009B2F8, -1);
        func_800487D8(&D_80082790, &D_8009B2F8, 0);
        func_800487D8(D_801E8A44, &D_8009B2F8, 0);
        if (D_8009B2F8 <= 0)
        {
          D_8009B308 = 1;
          D_8009B318 = 1;
        }
      }
      else
      {
        D_8009B300 = cnt - 1;
        func_800487D8(&D_80082844, &D_8009B2F8, 0);
        func_800487D8(&D_80082790, &D_8009B2F8, 0);
        func_800487D8(D_801E8A44, &D_8009B2F8, 1);
        func_80048D64((D_8009B2F0 != 0) ? (0xB8) : (0xDA), 0x44, 0x20, 0x20, 1);
        func_80046A2C(ot, 0xC0, 0x4C, 0x10, 0x10, 0x9D, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
        func_80046A2C(ot, 0xE3, 0x4C, 0x10, 0x10, 0xAD, 0x7C, 0, 0, 0, 0x244, 1, 1, 0x3B);
        func_80048B88(0xB8, 0x44, 0x20, 0x20, 0x95, 0x25, 0x1E, 0, 0, 0, (s32) (&D_80011BA0));
        func_80048B88(0xDA, 0x44, 0x20, 0x20, 0x3A, 0x1E, 0x95, 0, 0, 0, (s32) (&D_80011BA0));
      }
      func_8004A248(1, 0);
    }
    else
      if (state == (-3))
    {
      func_8004B8B4(9, 0x15);
      if (func_800487D8(D_801E8A44, &D_8009B2F8, 1) != 0)
      {
        if (D_801E436E & 0x800)
        {
          func_8005D6EC(3);
          func_8005EA6C();
          D_8009B308 = -4;
        }
        func_8004C0D8();
      }
      if (D_8009B2F8 >= 8)
      {
        D_8009B314 = 0;
      }
      func_8004A248(1, 1);
    }
    else
    {
      func_8004B8B4(-13, -21);
      func_800487D8(D_801E8A44, &D_8009B2F8, -1);
      func_8004A248(1, -1);
      if (D_8009B2F8 < 7)
      {
        D_8009B314 = 1;
      }
      if (D_8009B2F8 <= 0)
      {
        D_8009B308 = 0;
      }
    }
    func_800489AC(D_8019C9F0, 2, D_801F1804);
    func_800487D8(&D_80081C14, &D_8019C9F0, 0);
    func_800487D8(&D_80082460, &D_8019C9F0, 1);
  }
  else
  {
    if (D_801E436E)
    {
      D_8009B340 = -1;
      D_8009B344 = 7;
      func_8004A248((state == 2) ? (-1) : (1), 0);
      func_800487D8(&D_80081C14, &D_8019C9F0, -1);
      func_800487D8(&D_80082460, &D_8019C9F0, 0);
      func_800489AC(D_8019C9F0, 2, D_801F1804);
    }
    else
    {
      D_8009B340 = -1;
      D_8009B344 = 7;
      func_8004A248((state == 2) ? (-1) : (1), 0);
      func_800487D8(&D_80081C14, &D_8019C9F0, -1);
      func_800487D8(&D_80082460, &D_8019C9F0, 0);
      func_800489AC(D_8019C9F0, 2, D_801F1804);
    }
    if (D_8019C9F0 <= 0)
    {
      switch (D_8009B308)
      {
        case 1:
          D_8019C9F8 = 8;
          D_8009B340 = 8;
          func_8004E368(0, 0);
          break;

        case 2:
          D_8019C9F8 = 6;
          D_8009B340 = 6;
          D_801F1804 = 0;
          D_801E444C[0] = 0;
          func_80065B24(&D_8007BEDC, D_801E444C);
          break;

        default:
            break;

      }

      D_8019C9F0 = 0;
      D_8009B308 = 0;
    }
  }
}
