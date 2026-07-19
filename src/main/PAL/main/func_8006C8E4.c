
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct 
{
  u8 min;
  u8 sec;
  u8 frame;
  u8 pad;
  s32 size;
  char name[16];
} Entry;
typedef struct 
{
  s32 f0;
  u8 rest[40];
} Rec44;
extern s32 D_80099348;
extern s32 D_80099048;
extern Entry D_8009BB14[64];
extern Rec44 D_8009C0F0[];
extern u8 D_8009D714[0x800];
extern u8 D_80013A94[];
extern u8 D_80013AB4[];
extern u8 D_80013AD8[];
extern u8 D_80013AF4[];
extern u16 D_80013AD0;
extern s16 D_80013AD4;
extern s8 D_80013AD6;
s32 func_8006CB88(s32 arg0, s32 arg1, void *arg2);
void func_8006A9D8(u32 arg0, Entry *arg1);
void func_8006CBF4(char *dst, u8 *src, s32 n);
extern void func_8001674C();
s32 func_8006C8E4(s32 arg0)
{
  u8 *p;
  s32 i;
  u32 lba;
  if (arg0 == D_80099348)
  {
    return 1;
  }
  if (func_8006CB88(1, D_8009C0F0[arg0].f0, D_8009D714) != 1)
  {
    if (D_80099048 > 0)
    {
      func_8001674C(D_80013A94);
    }
    return -1;
  }
  if (D_80099048 >= 2)
  {
    func_8001674C(D_80013AB4);
  }
  i = 0;
  p = D_8009D714;
  while (p < (D_8009D714 + 0x800))
  {
    if ((*p) == 0)
    {
      break;
    }
    __builtin_memcpy(&lba, p + 2, 4);
 do { func_8006A9D8(lba & 0xFFFFFFFFFFFFFFFFu, &D_8009BB14[i]); __builtin_memcpy(&D_8009BB14[i].size, p + 0xA, 4); switch (i) { case 0: *((u16 *) D_8009BB14[0].name) = D_80013AD0; break; case 1: { s32 hi = D_80013AD4; s32 lo = D_80013AD6; *((s16 *) D_8009BB14[1].name) = hi; D_8009BB14[1].name[2] = lo; asm(""); break; } default: func_8006CBF4(D_8009BB14[i].name, p + 0x21, p[0x20]); D_8009BB14[i].name[p[0x20]] = 0; break; } if (D_80099048 >= 2) { func_8001674C(D_80013AD8, D_8009BB14[i].min, D_8009BB14[i].sec, D_8009BB14[i].frame, D_8009BB14[i].size, D_8009BB14[i].name); } } while (0);
    p = p + (*p);
    i++;
    if (i >= 0x40)
    {
      break;
    }
  }

  D_80099348 = arg0;
  if (0x40 > i)
  {
    D_8009BB14[i].name[0] = 0;
  }
  if (2 <= D_80099048)
  {
    func_8001674C(D_80013AF4, i);
  }
  return 1;
}
