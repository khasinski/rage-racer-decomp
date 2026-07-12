
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
extern s32 D_801E408C;
extern u8 *D_801E4150;
void func_800393AC(u8 *car, s16 arg1);
void func_800396FC(u8 *car, s32 gear)
{
  u8 *p[2];
  u16 lim[4];
  u16 val[2];
  register u8 *sub_R9 asm("$9");
  s32 rpm;
  s32 g0;
  s32 raw;
  u8 *tbl;
  s32 f;
  register s32 lo_R7 asm("$7");
  s32 hi;
  s32 range;
  register s32 d_R3 asm("$3");
  s32 pitch;
  s32 q;
  register s32 v20_R4 asm("$4");
  s32 cnt;
  s32 one;
  int new_var;
  raw = *((s32 *) (car + 0x70));
  rpm = raw >> 4;
  g0 = *((s16 *) (car + 0x138));
  sub_R9 = car + 0xBC;
  if (rpm < 0x20)
  {
    *((s16 *) (car + 0x138)) = 0;
  }
  if (g0 < 0)
  {
    *((s16 *) (car + 0x138)) = 0;
  }
  tbl = D_801E4150 + ((D_801E408C * 576) + 0x474);
  p[0] = tbl + (g0 * 12);
  p[1] = tbl + ((g0 * 12) + 12);
  lim[0] = *((u16 *) p[0]);
  lim[1] = *((u16 *) p[1]);
  if (gear < 4)
  {
    val[0] = *((u16 *) ((p[0] + (gear * 2)) + 4));
    val[1] = *((u16 *) ((p[1] + (gear * 2)) + 4));
  }
  else
  {
    f = 0x55 - gear;
    val[0] = ((*((s16 *) (p[0] + 0xA))) * f) / 100;
    val[1] = ((*((s16 *) (p[1] + 0xA))) * f) / 100;
  }
  pitch = 0;
  lo_R7 = *((s16 *) (&lim[0]));
  if (rpm < lo_R7)
  {
    goto L2;
  }
  hi = *((s16 *) (&lim[1]));
  one = hi;
  if (one < rpm)
  {
    goto L2_inc;
  }
  range = hi - lo_R7;
  pitch = *((u16 *) (p[0] + 2));
  if (!(range > 0))
  {
    range = 1;
  }
  d_R3 = rpm - lo_R7;
  v20_R4 = (new_var = *((s16 *) (&val[0])));
  q = (((*((s16 *) (&val[1]))) - new_var) * d_R3) / range;
  *((s16 *) (sub_R9 + 0x74)) = ((((new_var + q) * 1168) / 160) * 6) / 100;
  goto OUT;
  L2:
  if ((*((s16 *) (&lim[1]))) < rpm)
  {
    L2_inc:
    __asm__ volatile("" : : : "memory");
    cnt = *((u16 *) (sub_R9 + 0x7C));
    d_R3 = 1;
    q = d_R3;
    g0 = q;
    *((s16 *) (sub_R9 + 0x7E)) = g0;
    cnt = cnt + 1;
  }
  else
  {
    cnt = *((u16 *) (sub_R9 + 0x7C));
    one = 1;
    __asm__("" : "=r"(one) : "0"(one));
    *((s16 *) (sub_R9 + 0x7E)) = one;
    cnt = cnt - 1;
  }

  *((u16 *) (sub_R9 + 0x7C)) = cnt;
  __asm__ volatile("");
  if (rpm < 0x20)
  {
    *((u16 *) (sub_R9 + 0x7C)) = 0;
  }
  OUT:
  if ((*((s16 *) (sub_R9 + 0x7E))) != 0)
  {
    func_800393AC(car, (s16) pitch);
  }

}
