
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
extern u32 *g_FinalSkyOrderingTable asm("D_1F800004");
void AddPrim(void *, void *) asm("func_80064DDC");
void SetShadeTex(void *, s32) asm("func_80064EB8");
void SetPolyF4(void *) asm("func_80064F30");
void SetPolyFT4(void *) asm("func_80064F44");
void SetPolyG4(void *) asm("func_80064F58");
s32 GameSin(s32) asm("func_80068568");
s32 GameCos(s32) asm("func_80068634");
extern s16 g_SkyTileMap[][16] asm("D_8007F470");
extern s32 D_1F800068;
extern s32 g_MirrorMode asm("D_8019CACC");
extern s32 g_CourseIndex asm("D_801E428C");
extern s32 g_SkyRowBase asm("D_801E4D84");
typedef struct 
{
  u32 tag;
  u8 r0;
  u8 g0;
  u8 b0;
  u8 code;
} P_TAG;
typedef struct 
{
  P_TAG t;
  s16 x0;
  s16 y0;
  s16 x1;
  s16 y1;
  s16 x2;
  s16 y2;
  s16 x3;
  s16 y3;
} POLY_F4;
typedef struct 
{
  u16 corner[4];
} SkyTileUV;
typedef struct 
{
  u8 *packetCursor;
  u32 *orderingTable;
  s32 cameraX;
  s32 cameraY;
  s32 cameraZ;
  s32 pad14;
  s32 pitch;
  s32 yaw;
  s32 roll;
  s32 pad24[17];
  s32 mirrorFlag;
} SkyRenderScratchpad;
typedef struct 
{
  s16 xMinTop;
  s16 xMaxTop;
  s16 xMinBottom;
  s16 xMaxBottom;
  s16 yEdge0;
  s16 yEdge1;
  s16 yEdge2;
  s16 yEdge3;
} SkyClipBounds;
typedef struct 
{
  P_TAG t;
  s16 x0;
  s16 y0;
  u8 u0;
  u8 v0;
  u16 clut;
  s16 x1;
  s16 y1;
  u8 u1;
  u8 v1;
  u16 tpage;
  s16 x2;
  s16 y2;
  u8 u2;
  u8 v2;
  u16 pad1E;
  s16 x3;
  s16 y3;
  u8 u3;
  u8 v3;
  u16 pad26;
} POLY_FT4;
typedef struct 
{
  P_TAG t;
  s16 x0;
  s16 y0;
  u8 r1;
  u8 g1;
  u8 b1;
  u8 pad0;
  s16 x1;
  s16 y1;
  u8 r2;
  u8 g2;
  u8 b2;
  u8 pad1;
  s16 x2;
  s16 y2;
  u8 r3;
  u8 g3;
  u8 b3;
  u8 pad2;
  s16 x3;
  s16 y3;
} POLY_G4;
extern SkyTileUV g_SkyTileUV[] asm("D_8007F510");
typedef struct 
{
  u8 r;
  u8 g;
  u8 b;
  u8 cd;
} SkyCVec;
typedef struct 
{
  u8 chan0;
  u8 chan1;
  u8 chan2;
  u8 chan3;
} GameEnvColor;
typedef struct 
{
  GameEnvColor cur;
  GameEnvColor from;
  GameEnvColor to;
} GameEnvColorSlot;
extern GameEnvColorSlot g_EnvColors[] asm("D_801E3FB6");
inline static s32 GameRoundTerrainCoordinate(s32 value)
{
  return value / 256;
}

inline static s32 GameRoundTerrainCoordinate11(s32 value)
{
  return value / 2048;
}

void func_800418D4(void);
void func_800418D4(void)
{
  SkyRenderScratchpad *scratch = (SkyRenderScratchpad *) 0x1F800000;
  s32 sp20;
  s32 sp28;
  s32 sp30;
  s32 sp38;
  s32 sp40;
  s32 sp48;
  s32 sp50;
  s32 sp58;
  s32 sp60;
  s32 sp68;
  s32 sp70;
  s32 sp78;
  s32 sp80;
  s32 sp88;
  s32 sp90;
  s32 sp98;
  s32 spA0;
  s32 spA8;
  s32 spB0;
  s32 spB8;
  s32 spC0;
  s32 spC8;
  s32 spD0;
  s32 spE0;
  s32 temp_a0_1048;
  s32 temp_a0_1170;
  s32 temp_a0_248;
  s32 temp_a1_1150;
  s32 temp_a1_208;
  s32 temp_a3_53;
  s32 temp_fp_714;
  s32 temp_lo_117;
  s32 temp_s0_104;
  s32 temp_s1_89;
  s32 temp_s3_496;
  s32 temp_s3_86;
  s32 temp_s4_489;
  s32 temp_s4_95;
  s32 temp_s5_482;
  s32 temp_s6_474;
  s32 temp_s7_747;
  s32 negSin;
  s32 baseX1;
  s32 gridIndex;
  s32 roundedRow;
  s32 temp_v0_103;
  s32 temp_v0_431;
  s32 temp_v1_1063;
  s32 temp_v1_1140;
  s32 temp_v1_1161;
  s32 temp_v1_223;
  s32 temp_v1_257;
  s32 temp_v1_408;
  s32 temp_v1_475;
  s32 temp_v1_55;
  s32 var_a0_44;
  s32 var_a1_47;
  s32 var_a3_184;
  s32 var_fp_360;
  s32 var_s0_92;
  s32 var_s7_357;
  s32 var_v0_1007;
  s32 var_v0_112;
  s32 var_v0_127;
  s32 var_v0_38;
  s32 var_v0_69;
  s32 var_v0_854;
  s32 var_v0_928;
  s32 var_v0_941;
  s32 var_v0_947;
  SkyTileUV *temp_s1_215;
  s16 textureStageA;
  POLY_FT4 *texturedQuad;
  u8 *nextPacket;
  POLY_FT4 *texturedQuadRow;
  u8 *packetCursor;
  s32 divisionInputRow;
  s32 divisionInputX0;
  s32 divisionInputX1;
  s32 isNonNegativeRow;
  s32 isNonNegativeX0;
  s32 isNonNegativeX1;
  packetCursor = scratch->packetCursor;
  if (scratch->mirrorFlag != g_MirrorMode)
  {
    var_v0_38 = -scratch->pitch;
  }
  else
  {
    var_v0_38 = scratch->pitch;
  }
  var_a0_44 = var_v0_38 & 0xFFF;
  var_a1_47 = var_a0_44;
  if (var_a1_47 >= 0x800)
  {
    var_a1_47 -= 0x1000;
  }
  if (var_a0_44 >= 0x800)
  {
    var_a0_44 -= 0x1000;
  }
  temp_a3_53 = scratch->cameraY;
  {
    s32 adjustedRow;
    s32 leftPlusTwo;
    s32 rightPlusTwo;
    temp_v1_55 = temp_a3_53 - 0x1770;
    leftPlusTwo = var_a1_47 + 2;
    adjustedRow = temp_v1_55;
    divisionInputRow = temp_v1_55;
    isNonNegativeRow = divisionInputRow >= 0;
    if (!isNonNegativeRow)
    {
      adjustedRow = temp_a3_53 - 0x1751;
    }
    roundedRow = adjustedRow >> 5;
    var_a1_47 = leftPlusTwo + roundedRow;
    rightPlusTwo = var_a0_44 + 2;
    var_a0_44 = rightPlusTwo + roundedRow;
    var_v0_69 = scratch->mirrorFlag;
  }
  temp_v1_55 = scratch->yaw;
  if (var_v0_69 != 0)
  {
    temp_v1_55 = -temp_v1_55;
    var_v0_69 = temp_v1_55 + 0x200;
  }
  else
  {
    var_v0_69 = temp_v1_55 + 0x200;
  }
  temp_v1_55 = var_v0_69 & 0xFFF;
  var_a1_47 /= 2;
  var_a0_44 = (var_a0_44 / 2) + 0x50;
  sp60 = temp_v1_55 >> 7;
  temp_s3_86 = (s32) (((u32) ((-0x100) - ((temp_v1_55 >> 1) & 0x3F))) << 8);
  temp_s1_89 = (s32) (((u32) ((-0x80) - var_a1_47)) << 8);
  var_s0_92 = scratch->roll;
  temp_s4_95 = (s32) (((u32) ((-0x80) - var_a0_44)) << 8);
  if (g_MirrorMode == 0)
  {
    var_s0_92 = -var_s0_92;
  }
  temp_s0_104 = GameSin(var_s0_92);
  temp_v0_103 = GameCos(var_s0_92);
  var_a0_44 = temp_v0_103 * temp_s3_86;
  var_v0_112 = var_a0_44 + (temp_s0_104 * temp_s1_89);
  divisionInputX0 = var_v0_112;
  isNonNegativeX0 = divisionInputX0 >= 0;
  if (!isNonNegativeX0)
  {
    var_v0_112 += 0xFFF;
  }
  negSin = -temp_s0_104;
  temp_lo_117 = negSin * temp_s3_86;
  var_v0_112 >>= 0xC;
  var_a1_47 = 0xA000;
  sp20 = var_v0_112 + var_a1_47;
  var_v0_127 = temp_lo_117 + (temp_v0_103 * temp_s1_89);
  divisionInputX1 = var_v0_127;
  isNonNegativeX1 = divisionInputX1 >= 0;
  if (!isNonNegativeX1)
  {
    var_v0_127 += 0xFFF;
  }
  var_a3_184 = var_v0_127 >> 0xC;
  var_a0_44 += temp_s0_104 * temp_s4_95;
  sp28 = var_a3_184 + 0x7800;
  if (var_a0_44 < 0)
  {
    var_a0_44 += 0xFFF;
  }
  var_v0_127 = temp_v0_103 * temp_s4_95;
  temp_lo_117 += var_v0_127;
  var_v0_127 = var_a0_44 >> 0xC;
  baseX1 = var_v0_127 + 0xA000;
  if (temp_lo_117 < 0)
  {
    temp_lo_117 += 0xFFF;
  }
  var_a1_47 = temp_lo_117 >> 0xC;
  var_a0_44 = var_a1_47 + 0x7800;
  if (g_MirrorMode != D_1F800068)
  {
    sp28 = var_a3_184 + 0x2400;
    var_a0_44 = var_a1_47 + 0x2400;
  }
  sp30 = temp_v0_103 * 4;
  sp38 = negSin * 4;
  sp50 = temp_s0_104;
  sp58 = temp_v0_103;
  sp68 = sp20;
  sp70 = sp28;
  sp40 = temp_s0_104 * 8;
  sp48 = temp_v0_103 * 8;
  if (g_SkyRowBase != 0)
  {
    {
      s32 temp_s7_256;
      s32 var_fp_181;
      s32 spF0 = 0;
      s32 spF8 = 0;
      gridIndex = 0;
      do
      {
        var_fp_181 = 0;
        texturedQuadRow = (POLY_FT4 *) packetCursor;
        spC0 = spF8;
        spD0 = spF0;
        var_a3_184 = sp28;
        roundedRow = sp20;
        do
        {
          texturedQuad = texturedQuadRow + var_fp_181;
          textureStageA = g_SkyTileMap[(gridIndex % 2) + g_SkyRowBase][(sp60 + var_fp_181) & 0xF];
          temp_s1_215 = &g_SkyTileUV[textureStageA];
          temp_a1_208 = roundedRow - spC0;
          sp80 = GameRoundTerrainCoordinate(temp_a1_208);
          spC8 = roundedRow + sp30;
          temp_v1_223 = spC8 - spC0;
          sp88 = GameRoundTerrainCoordinate(temp_v1_223);
          sp90 = GameRoundTerrainCoordinate(temp_a1_208 + sp40);
          sp98 = GameRoundTerrainCoordinate(temp_v1_223 + sp40);
          temp_a0_248 = var_a3_184 - spD0;
          temp_s6_474 = GameRoundTerrainCoordinate(temp_a0_248);
          temp_s7_256 = var_a3_184 + sp38;
          temp_v1_257 = temp_s7_256 - spD0;
          temp_s5_482 = GameRoundTerrainCoordinate(temp_v1_257);
          temp_s4_489 = GameRoundTerrainCoordinate(temp_a0_248 + sp48);
          temp_s3_496 = GameRoundTerrainCoordinate(temp_v1_257 + sp48);
          SetPolyFT4(packetCursor);
          SetShadeTex(packetCursor, 0);
          texturedQuad->tpage = 0x18;
          *((u16 *) (&texturedQuad->u0)) = temp_s1_215->corner[0];
          packetCursor += 0x28;
          *((u16 *) (&texturedQuad->u1)) = temp_s1_215->corner[1];
          *((u16 *) (&texturedQuad->u2)) = temp_s1_215->corner[2];
          *((u16 *) (&texturedQuad->u3)) = temp_s1_215->corner[3];
          texturedQuad->x0 = sp80;
          texturedQuad->x1 = sp88;
          texturedQuad->x2 = sp90;
          *((u16 *) (&texturedQuad->x3)) = sp98;
          texturedQuad->t.r0 = 0x80;
          texturedQuad->t.g0 = 0x80;
          texturedQuad->t.b0 = 0x80;
          texturedQuad->y0 = temp_s6_474;
          texturedQuad->y1 = temp_s5_482;
          texturedQuad->y2 = temp_s4_489;
          texturedQuad->y3 = temp_s3_496;
          texturedQuad->clut = 0x798E;
          AddPrim(scratch->orderingTable + 0x2BF, texturedQuad);
          var_fp_181 += 1;
          roundedRow = spC8;
          var_a3_184 = temp_s7_256;
        }
        while (var_fp_181 < 8);
        gridIndex += 1;
        spF0 += sp48;
        spF8 += sp40;
      }
      while (gridIndex < 4);
    }
    sp30 *= 8;
    sp38 *= 8;
  }
  else
  {
    SkyClipBounds clip;
    sp28 = var_a0_44;
    {
      sp20 = baseX1;
      sp78 = GameRoundTerrainCoordinate(sp28);
      var_fp_360 = GameRoundTerrainCoordinate(sp28 + sp48);
      var_s7_357 = 0xF0;
      clip.xMinTop = (clip.xMinBottom = 0x140);
      clip.xMaxBottom = 0;
      clip.xMaxTop = 0;
      clip.yEdge0 = (sp38 > 0) ? (0xF0) : (-0xF0);
      clip.yEdge1 = (sp38 > 0) ? (-0xF0) : (0xF0);
      clip.yEdge2 = (sp38 > 0) ? (0xF0) : (-0xF0);
      clip.yEdge3 = (sp38 > 0) ? (-0xF0) : (0xF0);
      gridIndex = 0;
      texturedQuad = (POLY_FT4 *) packetCursor;
      do
      {
        sp80 = GameRoundTerrainCoordinate(sp20);
        temp_v1_408 = sp20 + sp30;
        sp88 = GameRoundTerrainCoordinate(temp_v1_408);
        sp90 = GameRoundTerrainCoordinate(sp20 + sp40);
        temp_v0_431 = GameRoundTerrainCoordinate(temp_v1_408 + sp40);
        sp98 = temp_v0_431;
        if (((((sp80 >= 0) || (sp88 >= 0)) || (sp90 >= 0)) || (temp_v0_431 >= 0)) && ((((sp80 < 0x140) || (sp88 < 0x140)) || (sp90 < 0x140)) || (sp98 < 0x140)))
        {
          temp_s6_474 = GameRoundTerrainCoordinate(sp28);
          temp_v1_475 = sp28 + sp38;
          temp_s5_482 = GameRoundTerrainCoordinate(temp_v1_475);
          temp_s4_489 = GameRoundTerrainCoordinate(sp28 + sp48);
          temp_s3_496 = GameRoundTerrainCoordinate(temp_v1_475 + sp48);
          if (sp78 < temp_s6_474)
          {
            sp78 = temp_s6_474;
          }
          if (sp78 < temp_s5_482)
          {
            sp78 = temp_s5_482;
          }
          if (temp_s4_489 < var_fp_360)
          {
            var_fp_360 = temp_s4_489;
          }
          if (temp_s3_496 < var_fp_360)
          {
            var_fp_360 = temp_s3_496;
          }
          if (temp_s4_489 < temp_s3_496)
          {
            if (temp_s4_489 < var_s7_357)
            {
              var_s7_357 = temp_s4_489;
            }
          }
          else
            if (temp_s3_496 < var_s7_357)
          {
            var_s7_357 = temp_s3_496;
          }
          if (sp80 < clip.xMinTop)
          {
            clip.xMinTop = sp80;
          }
          if (sp90 < clip.xMinBottom)
          {
            clip.xMinBottom = sp90;
          }
          if (clip.xMaxTop < sp88)
          {
            clip.xMaxTop = sp88;
          }
          if (clip.xMaxBottom < sp98)
          {
            clip.xMaxBottom = sp98;
          }
          if (sp38 > 0)
          {
            if (temp_s6_474 < clip.yEdge0)
            {
              clip.yEdge0 = temp_s6_474;
            }
            if (clip.yEdge1 < temp_s5_482)
            {
              clip.yEdge1 = temp_s5_482;
            }
            if (temp_s4_489 < clip.yEdge2)
            {
              clip.yEdge2 = temp_s4_489;
            }
            if (clip.yEdge3 < temp_s3_496)
            {
              clip.yEdge3 = temp_s3_496;
            }
          }
          else
          {
            if (clip.yEdge0 < temp_s6_474)
            {
              clip.yEdge0 = temp_s6_474;
            }
            if (temp_s5_482 < clip.yEdge1)
            {
              clip.yEdge1 = temp_s5_482;
            }
            if (clip.yEdge2 < temp_s4_489)
            {
              clip.yEdge2 = temp_s4_489;
            }
            if (temp_s3_496 < clip.yEdge3)
            {
              clip.yEdge3 = temp_s3_496;
            }
          }
          temp_lo_117 = g_SkyTileMap[0][(sp60 + gridIndex) & 0xF];
          temp_s1_215 = &g_SkyTileUV[temp_lo_117];
          SetPolyFT4(packetCursor);
          SetShadeTex(packetCursor, 0);
          texturedQuad->tpage = 0x18;
          *((u16 *) (&texturedQuad->u0)) = temp_s1_215->corner[0];
          packetCursor += 0x28;
          *((u16 *) (&texturedQuad->u1)) = temp_s1_215->corner[1];
          *((u16 *) (&texturedQuad->u2)) = temp_s1_215->corner[2];
          *((u16 *) (&texturedQuad->u3)) = temp_s1_215->corner[3];
          texturedQuad->x0 = sp80;
          texturedQuad->x1 = sp88;
          texturedQuad->x2 = sp90;
          *((u16 *) (&texturedQuad->x3)) = sp98;
          texturedQuad->t.r0 = 0x80;
          texturedQuad->t.g0 = 0x80;
          texturedQuad->t.b0 = 0x80;
          texturedQuad->y0 = temp_s6_474;
          texturedQuad->y1 = temp_s5_482;
          texturedQuad->y2 = temp_s4_489;
          texturedQuad->y3 = temp_s3_496;
          texturedQuad->clut = 0x798E;
          AddPrim(scratch->orderingTable + 0x2BF, texturedQuad++);
        }
        gridIndex += 1;
        sp20 += sp30;
        sp28 += sp38;
      }
      while (gridIndex < 8);
    }
    sp20 = sp68;
    sp28 = sp70;
    sp30 *= 8;
    sp38 *= 8;
    sp80 = GameRoundTerrainCoordinate(sp20);
    temp_fp_714 = sp20 + sp30;
    sp88 = GameRoundTerrainCoordinate(temp_fp_714);
    sp90 = GameRoundTerrainCoordinate(sp20 + sp40);
    sp98 = GameRoundTerrainCoordinate(temp_fp_714 + sp40);
    temp_s6_474 = GameRoundTerrainCoordinate(sp28);
    temp_s7_747 = sp28 + sp38;
    temp_s5_482 = GameRoundTerrainCoordinate(temp_s7_747);
    {
      u8 color;
      POLY_G4 *packetColorBase;
      s32 var_v0_762;
      temp_s4_489 = GameRoundTerrainCoordinate(sp28 + sp48);
      var_v0_762 = temp_s7_747 + sp48;
      temp_s3_496 = var_v0_762 / 256;
      nextPacket = packetCursor + 0x24;
      packetColorBase = (POLY_G4 *) packetCursor;
      SetPolyG4(packetCursor);
      packetColorBase->x0 = sp80;
      packetColorBase->x1 = sp88;
      packetColorBase->x2 = sp90;
      packetColorBase->x3 = sp98;
      packetColorBase->y0 = temp_s6_474;
      packetColorBase->y1 = temp_s5_482;
      packetColorBase->y2 = temp_s4_489;
      packetColorBase->y3 = temp_s3_496;
      color = ((SkyCVec *) (&g_EnvColors[2].cur))->r;
      packetColorBase->r1 = color;
      packetColorBase->t.r0 = color;
      color = ((SkyCVec *) (&g_EnvColors[3].cur))->r;
      packetColorBase->r3 = color;
      packetColorBase->r2 = color;
      color = ((SkyCVec *) (&g_EnvColors[2].cur))->g;
      packetColorBase->g1 = color;
      packetColorBase->t.g0 = color;
      color = ((SkyCVec *) (&g_EnvColors[3].cur))->g;
      packetColorBase->g3 = color;
      packetColorBase->g2 = color;
      packetColorBase->b1 = (color = ((SkyCVec *) (&g_EnvColors[2].cur))->b);
      packetColorBase->t.b0 = color;
      color = ((SkyCVec *) (&g_EnvColors[3].cur))->b;
      packetCursor = nextPacket;
      packetColorBase->b2 = (packetColorBase->b3 = color);
      AddPrim(scratch->orderingTable + 0x2BF, packetColorBase);
    }
    {
      u8 color;
      POLY_G4 *g4Cursor = (POLY_G4 *) packetCursor;
      s32 spE8;
      u32 *orderingTableBase;
      spE0 = sp28 - sp48;
      var_v0_941 = temp_s7_747;
      sp98 = (temp_fp_714 - sp40) / 256;
      spB0 = sp20 - sp40;
      sp90 = spB0 / 256;
      temp_s4_489 = spE0 / 256;
      nextPacket = packetCursor + 0x24;
      spE8 = var_v0_941 - sp48;
      temp_s3_496 = spE8 / 256;
      g4Cursor->y3 = temp_s3_496;
      g4Cursor->x2 = sp90;
      g4Cursor->x3 = sp98;
      g4Cursor->x0 = sp80;
      g4Cursor->x1 = sp88;
      color = ((SkyCVec *) (&g_EnvColors[1].cur))->b;
      g4Cursor->y2 = temp_s4_489;
      g4Cursor->y0 = temp_s6_474;
      g4Cursor->y1 = temp_s5_482;
      g4Cursor->b2 = color;
      g4Cursor->b3 = color;
      SetPolyG4(g4Cursor);
      color = ((SkyCVec *) (&g_EnvColors[2].cur))->r;
      g4Cursor->r1 = color;
      g4Cursor->t.r0 = color;
      color = ((SkyCVec *) (&g_EnvColors[1].cur))->r;
      g4Cursor->r3 = color;
      g4Cursor->r2 = color;
      color = ((SkyCVec *) (&g_EnvColors[2].cur))->g;
      g4Cursor->g1 = color;
      g4Cursor->t.g0 = color;
      color = ((SkyCVec *) (&g_EnvColors[1].cur))->g;
      g4Cursor->g3 = color;
      g4Cursor->g2 = color;
      color = ((SkyCVec *) (&g_EnvColors[2].cur))->b;
      do
      {
        g4Cursor->b1 = color;
      }
      while (0);
      g4Cursor->t.b0 = color;
      orderingTableBase = scratch->orderingTable;
      AddPrim(orderingTableBase + 0x2BF, g4Cursor++);
      packetCursor = (u8 *) g4Cursor;
    }
    {
      u8 packetColor;
      u8 lowerRed;
      POLY_G4 *g4Cursor = (POLY_G4 *) packetCursor;
      spC0 = sp48 * 2;
      var_v0_941 = spE0 - spC0;
      var_v0_947 = (temp_s7_747 - spC0) - sp48;
      sp80 = sp90;
      temp_s5_482 = temp_s3_496;
      temp_s3_496 = var_v0_947 / 256;
      spB0 -= sp40;
      spB0 -= sp40;
      sp88 = sp98;
      var_v0_854 = sp40 * 2;
      var_v0_928 = var_v0_854;
      var_v0_928 = (temp_fp_714 - var_v0_928) - sp40;
      sp90 = GameRoundTerrainCoordinate(spB0);
      temp_s6_474 = temp_s4_489;
      sp98 = var_v0_928 / 256;
      temp_s4_489 = var_v0_941 / 256;
      nextPacket = (u8 *) (g4Cursor + 1);
      SetPolyG4(g4Cursor);
      g4Cursor->x0 = sp80;
      g4Cursor->x1 = sp88;
      g4Cursor->x2 = sp90;
      g4Cursor->x3 = sp98;
      g4Cursor->y0 = temp_s6_474;
      g4Cursor->y1 = temp_s5_482;
      g4Cursor->y2 = temp_s4_489;
      g4Cursor->y3 = temp_s3_496;
      lowerRed = ((SkyCVec *) (&g_EnvColors[1].cur))->r;
      g4Cursor->r3 = 0;
      g4Cursor->r1 = lowerRed;
      g4Cursor->t.r0 = lowerRed;
      g4Cursor->r2 = 0;
      {
        packetColor = ((SkyCVec *) (&g_EnvColors[1].cur))->g;
        g4Cursor->g2 = (g4Cursor->g3 = 0);
        g4Cursor->t.g0 = (g4Cursor->g1 = packetColor);
        {
          u8 packetColor = ((SkyCVec *) (&g_EnvColors[1].cur))->b;
          g4Cursor->t.b0 = (g4Cursor->b1 = packetColor);
          g4Cursor->b2 = (g4Cursor->b3 = 0x10);
        }
        AddPrim(g_FinalSkyOrderingTable + 0x2BF, g4Cursor);
      }
      packetCursor = (u8 *) (g4Cursor + 1);
    }
  }
  sp60 = sp40 * 4;
  if (g_CourseIndex != 2)
  {
    u8 color;
    POLY_G4 *courseG4 = (POLY_G4 *) packetCursor;
    var_v0_854 = (sp20 + sp40) * 8;
    var_v0_1007 = var_v0_854 - sp50;
    sp80 = var_v0_1007 / 2048;
    var_v0_928 = ((sp20 + sp30) + sp40) * 8;
    sp88 = GameRoundTerrainCoordinate11(var_v0_928 - sp50);
    sp90 = GameRoundTerrainCoordinate11(var_v0_854 + sp50);
    spA0 = sp90;
    sp98 = GameRoundTerrainCoordinate11(var_v0_928 + sp50);
    temp_a0_1048 = (sp28 + sp48) * 8;
    spA8 = sp98;
    temp_s6_474 = GameRoundTerrainCoordinate11(temp_a0_1048 - sp58);
    temp_v1_1063 = ((sp28 + sp38) + sp48) * 8;
    temp_s5_482 = GameRoundTerrainCoordinate11(temp_v1_1063 - sp58);
    temp_s4_489 = GameRoundTerrainCoordinate11(temp_a0_1048 + sp58);
    spB0 = temp_s4_489;
    temp_s3_496 = GameRoundTerrainCoordinate11(temp_v1_1063 + sp58);
    SetPolyG4(courseG4);
    courseG4->x0 = sp80;
    courseG4->x1 = sp88;
    courseG4->x2 = sp90;
    courseG4->x3 = sp98;
    courseG4->y0 = temp_s6_474;
    courseG4->y1 = temp_s5_482;
    courseG4->y2 = temp_s4_489;
    courseG4->y3 = temp_s3_496;
    color = ((SkyCVec *) (&g_EnvColors[7].cur))->r;
    courseG4->r1 = color;
    courseG4->t.r0 = color;
    color = ((SkyCVec *) (&g_EnvColors[8].cur))->r;
    courseG4->r3 = color;
    courseG4->r2 = color;
    color = ((SkyCVec *) (&g_EnvColors[7].cur))->g;
    courseG4->g1 = color;
    courseG4->t.g0 = color;
    spB8 = temp_s3_496;
    color = ((SkyCVec *) (&g_EnvColors[8].cur))->g;
    courseG4->g3 = color;
    courseG4->g2 = color;
    color = ((SkyCVec *) (&g_EnvColors[7].cur))->b;
    courseG4->b1 = color;
    courseG4->t.b0 = color;
    nextPacket = (u8 *) (courseG4 + 1);
    color = ((SkyCVec *) (&g_EnvColors[8].cur))->b;
    courseG4->b3 = color;
    courseG4->b2 = color;
    AddPrim(((u8 *) scratch->orderingTable) + 0xAF8, courseG4);
    packetCursor = nextPacket;
  }
  temp_v1_1140 = sp40 * 3;
  sp90 = GameRoundTerrainCoordinate(sp20 + temp_v1_1140);
  temp_a1_1150 = sp20 + sp30;
  sp98 = GameRoundTerrainCoordinate(temp_a1_1150 + temp_v1_1140);
  temp_v1_1161 = sp48 * 3;
  temp_s4_489 = GameRoundTerrainCoordinate(sp28 + temp_v1_1161);
  temp_a0_1170 = sp28 + sp38;
  temp_s3_496 = GameRoundTerrainCoordinate(temp_a0_1170 + temp_v1_1161);
  if (g_CourseIndex == 2)
  {
    u8 color;
    POLY_G4 *courseG4 = (POLY_G4 *) packetCursor;
    sp80 = GameRoundTerrainCoordinate(sp20 + sp40);
    sp88 = GameRoundTerrainCoordinate(temp_a1_1150 + (sp40 + (sp40 - sp40)));
    temp_s6_474 = GameRoundTerrainCoordinate(sp28 + sp48);
    temp_s5_482 = GameRoundTerrainCoordinate(temp_a0_1170 + sp48);
    nextPacket = (u8 *) (courseG4 + 1);
    SetPolyG4(courseG4);
    courseG4->x0 = sp80;
    courseG4->x1 = sp88;
    courseG4->x2 = sp90;
    courseG4->x3 = sp98;
    courseG4->y0 = temp_s6_474;
    courseG4->y1 = temp_s5_482;
    courseG4->y2 = temp_s4_489;
    courseG4->y3 = temp_s3_496;
    color = ((SkyCVec *) (&g_EnvColors[5].cur))->r;
    courseG4->r1 = color;
    courseG4->t.r0 = color;
    color = ((SkyCVec *) (&g_EnvColors[6].cur))->r;
    courseG4->r3 = color;
    courseG4->r2 = color;
    color = ((SkyCVec *) (&g_EnvColors[5].cur))->g;
    courseG4->g1 = color;
    courseG4->t.g0 = color;
    color = ((SkyCVec *) (&g_EnvColors[6].cur))->g;
    courseG4->g3 = color;
    courseG4->g2 = color;
    color = ((SkyCVec *) (&g_EnvColors[5].cur))->b;
    courseG4->b1 = color;
    courseG4->t.b0 = color;
    color = ((SkyCVec *) (&g_EnvColors[6].cur))->b;
    courseG4->b3 = color;
    courseG4->b2 = color;
    AddPrim(scratch->orderingTable + 0x2BF, courseG4);
    packetCursor = nextPacket;
  }
  else
  {
    POLY_F4 *courseF4 = (POLY_F4 *) packetCursor;
    sp80 = spA0;
    sp88 = spA8;
    temp_s6_474 = spB0;
    temp_s5_482 = spB8;
    SetPolyF4(courseF4);
    courseF4->x0 = sp80;
    courseF4->x1 = sp88;
    courseF4->x2 = sp90;
    courseF4->x3 = sp98;
    courseF4->y0 = temp_s6_474;
    courseF4->y1 = temp_s5_482;
    courseF4->y2 = temp_s4_489;
    courseF4->y3 = temp_s3_496;
    courseF4->t.r0 = (u8) ((SkyCVec *) (&g_EnvColors[4].cur))->r;
    courseF4->t.g0 = (u8) ((SkyCVec *) (&g_EnvColors[4].cur))->g;
    {
      u8 *nextPacket;
      nextPacket = (u8 *) (courseF4 + 1);
      courseF4->t.b0 = (u8) ((SkyCVec *) (&g_EnvColors[4].cur))->b;
      AddPrim(scratch->orderingTable + 0x2BF, courseF4);
      packetCursor = nextPacket;
    }
  }
  scratch->packetCursor = packetCursor;
  return;
}
