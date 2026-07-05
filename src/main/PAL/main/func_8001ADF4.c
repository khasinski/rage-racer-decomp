#include "common.h"

extern s32 D_801E8A98;
extern s16 D_8019CA10;
extern s32 D_801E4D18;
extern u8 *D_8019C900;
extern s32 D_801E4BC8;
extern s32 D_801E4030;

u8 *func_8001A9A8(void);
u8 *func_8001ACE4(u8 *packet);
void func_800418D4(void);
void SetDrawArea(u8 *packet, u8 *drawEnv) asm("func_80066604");
void AddPrim(u32 *ot, u32 *prim) asm("func_80064DDC");
void func_800414F0(s32 arg0, s32 arg1);
void func_80069858(void *arg0);
void func_80027FF4(void *arg0, s32 arg1, s32 arg2);
void func_8004123C(void);
void func_800389F0(void);
void func_8001ABD8(void);

void func_8001ADF4(s32 arg0) {
    register u8 **scratch asm("$16");
    register u8 *packet asm("$17");
    register u8 *prim asm("$5");

    if (arg0 >= 0x169) {
        D_801E8A98 = 1;
    }

    if (D_801E8A98 != 0) {
        if (D_8019CA10 != 0) {
            if (D_801E4D18 < 0x12) {
                D_801E4D18++;
            }
        } else if (D_801E4D18 >= -0x2B) {
            D_801E4D18--;
        }

        if (func_8001A9A8() != 0) {
            scratch = (u8 **)0x1F800000;

            func_800418D4();
            packet = func_8001ACE4(*scratch);
            SetDrawArea(packet, D_8019C900 + 0x70);
            prim = packet;
            packet += 0xC;
            AddPrim((u32 *)(D_8019C900 + 0x16C8), (u32 *)prim);
            *scratch = packet;
            func_800414F0(-0x3000, 0x6000);
            func_80069858((void *)0x1F800028);
            *(s32 *)0x1F800084 = D_801E4030;
            func_80027FF4((void *)0x1F800000, D_801E4BC8, 0x40);

            packet = *scratch;
            SetDrawArea(packet, D_8019C900);
            prim = packet;
            packet += 0xC;
            AddPrim((u32 *)(D_8019C900 + 0xBD0), (u32 *)prim);
            *scratch = packet;
            func_8004123C();
            func_800389F0();
            func_8001ABD8();
        }
    }
}
