#include <sys/types.h>

typedef struct 
{
  u_char min;
  u_char sec;
  u_char frame;
  u_char pad;
  long size;
  char name[16];
} Entry;
typedef struct 
{
  long f0;
  u_char rest[40];
} Rec44;
extern long D_80099348;
extern long D_80099048;
extern Entry D_8009BB14[64];
extern Rec44 D_8009C0F0[];
extern u_char D_8009D714[0x800];
extern u_char D_80013A94[];
extern u_char D_80013AB4[];
extern u_char D_80013AD8[];
extern u_char D_80013AF4[];
extern u_short D_80013AD0;
extern short D_80013AD4;
extern signed char D_80013AD6;
extern void GameDebugPrintf() asm("func_8001674C");
long func_8006CB88(long arg0, long arg1, void *arg2);
void func_8006A9D8(u_long arg0, Entry *arg1);
void func_8006CBF4(char *dst, u_char *src, long n);
/* Fills the 64-entry file cache from one ISO directory, converting each
 * record's extent to a CdlLOC. Owns its three "CD_cachefile: ..." messages. */
long CD_cachefile(long dir) asm("func_8006C8E4");
long CD_cachefile(long arg0)
{
  u_char *p;
  long i;
  u_long lba;
  if (arg0 == D_80099348)
  {
    return 1;
  }
  if (func_8006CB88(1, D_8009C0F0[arg0].f0, D_8009D714) != 1)
  {
    if (D_80099048 > 0)
    {
      GameDebugPrintf(D_80013A94);
    }
    return -1;
  }
  if (D_80099048 >= 2)
  {
    GameDebugPrintf(D_80013AB4);
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
 do { func_8006A9D8(lba & 0xFFFFFFFFFFFFFFFFu, &D_8009BB14[i]); __builtin_memcpy(&D_8009BB14[i].size, p + 0xA, 4); switch (i) { case 0: *((u_short *) D_8009BB14[0].name) = D_80013AD0; break; case 1: { long hi = D_80013AD4; long lo = D_80013AD6; *((short *) D_8009BB14[1].name) = hi; *(volatile u_char *)&D_8009BB14[1].name[2] = lo; break; } default: func_8006CBF4(D_8009BB14[i].name, p + 0x21, p[0x20]); D_8009BB14[i].name[p[0x20]] = 0; break; } if (D_80099048 >= 2) { GameDebugPrintf(D_80013AD8, D_8009BB14[i].min, D_8009BB14[i].sec, D_8009BB14[i].frame, D_8009BB14[i].size, D_8009BB14[i].name); } } while (0);
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
    GameDebugPrintf(D_80013AF4, i);
  }
  return 1;
}
