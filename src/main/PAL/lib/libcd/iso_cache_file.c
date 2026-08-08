#include <sys/types.h>
#include <stdio.h>
#include <string.h>
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
extern Rec44 g_CdPathEntryLbaByDirNum[];
extern u_char g_CdSectorBuf[0x800];
extern u_char g_MsgCdDirNotFound[];
extern u_char g_MsgCdSearchingFiles[];
extern u_char g_FmtCdFileEntry[];
extern u_char g_MsgCdFilesFound[];
extern u_short g_CdDotName;
extern short g_CdDotDotName;
extern signed char g_CdDotDotNameNul;
/* Fills the 64-entry file cache from one ISO directory, converting each
 * record's extent to a CdlLOC. Owns its three "CD_cachefile: ..." messages. */
long CD_cachefile(long dir)
{
  u_char *p;
  long i;
  u_long lba;
  if (dir == g_CdCachedDir)
  {
    return 1;
  }
  if (cd_read(1, g_CdPathEntryLbaByDirNum[dir].f0, g_CdSectorBuf) != 1)
  {
    if (g_CdDebugLevel > 0)
    {
      printf((u8 *)g_MsgCdDirNotFound);
    }
    return -1;
  }
  if (g_CdDebugLevel >= 2)
  {
    printf((u8 *)g_MsgCdSearchingFiles);
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
 do { CdIntToPos(lba & 0xFFFFFFFFFFFFFFFFu, &g_CdFileCache[i].pos); __builtin_memcpy(&g_CdFileCache[i].size, p + 0xA, 4); switch (i) { case 0: *(u_short *)g_CdFileCache[0].name = g_CdDotName; break; case 1: { long hi = g_CdDotDotName; long lo = g_CdDotDotNameNul; *(short *)g_CdFileCache[1].name = hi; *(volatile u_char *)&g_CdFileCache[1].name[2] = lo; break; } default: memcpy(g_CdFileCache[i].name, p + 0x21, p[0x20]); g_CdFileCache[i].name[p[0x20]] = 0; break; } if (g_CdDebugLevel >= 2) { printf((u8 *)g_FmtCdFileEntry, g_CdFileCache[i].pos.minute, g_CdFileCache[i].pos.second, g_CdFileCache[i].pos.sector, g_CdFileCache[i].size, g_CdFileCache[i].name); } } while (0);
    p = p + (*p);
    i++;
    if (i >= 0x40)
    {
      break;
    }
  }

  g_CdCachedDir = dir;
  if (0x40 > i)
  {
    g_CdFileCache[i].name[0] = 0;
  }
  if (2 <= g_CdDebugLevel)
  {
    printf((u8 *)g_MsgCdFilesFound, i);
  }
  return 1;
}
