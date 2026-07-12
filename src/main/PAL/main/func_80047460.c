
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
void func_8004711C(void *buf, s16 x1, s16 y1, s16 x2, s16 y2, s32 r, s32 g, s32 b, s32 code);
void func_80047460(void *buf, s32 xa, s32 ya, s32 w, s32 h, u8 r, u8 g, u8 b, u8 code)
{
  register s32 x_R19 asm("$19") = xa;
  register s32 y_R18 asm("$18") = ya;
  s16 new_var2;
  s32 x0;
  int new_var4;
  s32 x1;
  s32 ytop2;
  s32 new_var3;
  s32 ybot;
  s16 new_var;
  s32 ybi;
  new_var = (s16) x_R19;
  x0 = new_var;
  new_var3 = x_R19;
  new_var4 = w - 1;
  x1 = (s16) (new_var3 + new_var4);
  new_var2 = (s16) y_R18;
  h = (((((((((h - 1) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu) & 0xFFFFFFFFFFFFFFFFu;
  func_8004711C(buf, x0, new_var2, x1, new_var2, r, g, b, code);
  func_8004711C(buf, x0, (s16) (y_R18 + 1), x1, (s16) (y_R18 + 1), r, g, b, code);
  ytop2 = (s16) (y_R18 + 2);
  ybot = y_R18 + h;
  ybi = (s16) (ybot - 2);
  func_8004711C(buf, x0, ytop2, x0, ybi, r, g, b, code);
  func_8004711C(buf, x1, ytop2, x1, ybi, r, g, b, code);
  func_8004711C(buf, x0, (s16) ybot, x1, (s16) ybot, r, g, b, code);
  func_8004711C(buf, x0, (s16) (ybot - 1), x1, (s16) (ybot - 1), r, g, b, code);
}
