#include <sys/types.h>
#include "common.h"
#include "psyq/cd.h"
typedef struct 
{
  long f0;
  u_char rest[40];
} Rec44;
extern long g_CdCachedDir;
extern long g_CdDebugLevel;
extern CdlFILE g_CdFileCache[64];
extern Rec44 D_8009C0F0[];
extern u_char g_CdSectorBuf[0x800];
extern u_char D_80013A94[];
extern u_char D_80013AB4[];
extern u_char D_80013AD8[];
extern u_char D_80013AF4[];
extern u_short D_80013AD0;
extern short D_80013AD4;
extern signed char D_80013AD6;
long cd_read(long sectors, long addr, void *mode);
void LibcMemcpy(char *dst, u_char *src, long n);
/* Fills the 64-entry file cache from one ISO directory, converting each
 * record's extent to a CdlLOC. Owns its three "CD_cachefile: ..." messages. */
long CD_cachefile(long dir);
long CD_cachefile(long arg0)
{
  u_char *p;
  long i;
  u_long lba;
  if (arg0 == g_CdCachedDir)
  {
    return 1;
  }
  if (cd_read(1, D_8009C0F0[arg0].f0, g_CdSectorBuf) != 1)
  {
    if (g_CdDebugLevel > 0)
    {
      DebugPrintf(D_80013A94);
    }
    return -1;
  }
  if (g_CdDebugLevel >= 2)
  {
    DebugPrintf(D_80013AB4);
  }
  i = 0;
  p = g_CdSectorBuf;
  while (p < (g_CdSectorBuf + 0x800))
  {
    if ((*p) == 0)
    {
      break;
    }
    __builtin_memcpy(&lba, p + 2, 4);
 do { CdIntToPos(lba & 0xFFFFFFFFFFFFFFFFu, &g_CdFileCache[i].pos); __builtin_memcpy(&g_CdFileCache[i].size, p + 0xA, 4); switch (i) { case 0: *(u_short *)g_CdFileCache[0].name = D_80013AD0; break; case 1: { long hi = D_80013AD4; long lo = D_80013AD6; *(short *)g_CdFileCache[1].name = hi; *(volatile u_char *)&g_CdFileCache[1].name[2] = lo; break; } default: LibcMemcpy(g_CdFileCache[i].name, p + 0x21, p[0x20]); g_CdFileCache[i].name[p[0x20]] = 0; break; } if (g_CdDebugLevel >= 2) { DebugPrintf(D_80013AD8, g_CdFileCache[i].pos.minute, g_CdFileCache[i].pos.second, g_CdFileCache[i].pos.sector, g_CdFileCache[i].size, g_CdFileCache[i].name); } } while (0);
    p = p + (*p);
    i++;
    if (i >= 0x40)
    {
      break;
    }
  }

  g_CdCachedDir = arg0;
  if (0x40 > i)
  {
    g_CdFileCache[i].name[0] = 0;
  }
  if (2 <= g_CdDebugLevel)
  {
    DebugPrintf(D_80013AF4, i);
  }
  return 1;
}
