
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
extern u8 *D_801E4150;
extern s32 D_801E408C;
extern s32 D_801E40D8;
void func_800385FC(u8 *ent, s32 pos, s32 *arr)
{
  register s32 pos2_R10 asm("$10");
  register s32 idx_R8 asm("$8");
  register u8 *base_R9 asm("$9");
  register u8 *ent2_R7 asm("$7");
  register u8 *sub_R6 asm("$6");
  s32 c;
  u16 w;
  pos2_R10 = pos;
  __asm__("" : "=r"(pos2_R10) : "0"(pos2_R10));
  idx_R8 = arr[pos2_R10];
  base_R9 = D_801E4150;
  ent2_R7 = ent;
  if (!(idx_R8 < 12))
  {
    idx_R8 = 0;
  }
  {
    register s32 lev1_R3 asm("$3");
    unsigned int idxoff1_R4;
    register u8 *p1_R4 asm("$4");
    lev1_R3 = D_801E408C;
    idxoff1_R4 = idx_R8;
    idxoff1_R4 = idxoff1_R4 * 16;
    p1_R4 = base_R9 + (idxoff1_R4 + (lev1_R3 * 192));
    *((s16 *) (ent2_R7 + 0x124)) = ((*((s16 *) (p1_R4 + 0x8F4))) * 1168) / 160;
    *((u16 *) (ent2_R7 + 0x126)) = *((u16 *) (p1_R4 + 0x8F6));
    *((u16 *) (ent2_R7 + 0x128)) = *((u16 *) (p1_R4 + 0x8F8));
    *((u16 *) (ent2_R7 + 0x12A)) = *((u16 *) (p1_R4 + 0x8FA));
    *((u16 *) (ent2_R7 + 0x12C)) = *((u16 *) (p1_R4 + 0x8FC));
  }
  __asm__ volatile("");
  c = *((s16 *) (ent2_R7 + 0x128));
  sub_R6 = ent2_R7 + 0xBC;
  *((s16 *) (ent2_R7 + 0x12E)) = 0;
  if (c < 0)
  {
    *((s16 *) (ent2_R7 + 0x128)) = 0;
  }
  else
    if (!(c < 11))
  {
    *((s16 *) (ent2_R7 + 0x128)) = 10;
  }
  if ((*((s16 *) (sub_R6 + 0x6E))) < 0)
  {
    *((s16 *) (sub_R6 + 0x6E)) = 0;
  }
  c = *((s16 *) (sub_R6 + 0x70));
  if (c <= 0)
  {
    *((s16 *) (sub_R6 + 0x70)) = 0;
  }
  else
    if (!(c < 16))
  {
    *((s16 *) (sub_R6 + 0x70)) = 15;
  }
  {
    register s32 lev2_R2 asm("$2");
    register s32 idxoff2_R4 asm("$4");
    register u8 *p2_R3 asm("$3");
    lev2_R2 = D_801E408C;
    idxoff2_R4 = idx_R8 * 16;
    p2_R3 = base_R9 + (idxoff2_R4 + (lev2_R2 * 192));
    w = *((u16 *) (p2_R3 + 0x8FE));
    *((s16 *) (sub_R6 + 0x76)) = w;
    if (((s16) w) < 0x3D)
    {
      *((s16 *) (sub_R6 + 0x76)) = 0x3C;
    }
    lev2_R2 = D_801E408C;
    __asm__("" : "=r"(idxoff2_R4) : "0"(idxoff2_R4));
    p2_R3 = base_R9 + (idxoff2_R4 + (lev2_R2 * 192));
    w = *((u16 *) (p2_R3 + 0x900));
    *((s16 *) (sub_R6 + 0x78)) = w;
    if (((s16) w) <= 0)
    {
      *((s16 *) (sub_R6 + 0x78)) = 0;
    }
  }
  {
    register s32 v_R3 asm("$3");
    v_R3 = *((s16 *) (sub_R6 + 0x68));
    *((s16 *) (sub_R6 + 0x74)) = (v_R3 * 6) / 100;
  }
  if (pos2_R10 >= 4)
  {
    register s32 d_R5 asm("$5");
    register s32 pm4_R3 asm("$3");
    d_R5 = D_801E40D8;
    pm4_R3 = pos2_R10 - 4;
    *((s32 *) (sub_R6 + 0x5C)) = (d_R5 / 12) + ((d_R5 / 40) * pm4_R3);
  }
  else
  {
    *((s32 *) (sub_R6 + 0x5C)) = D_801E40D8 / 12;
  }
}
