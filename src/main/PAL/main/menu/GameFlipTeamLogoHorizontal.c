#include "common.h"
#include "psyq/gpu.h"

extern u32 g_TeamLogoCanvas[] asm("D_801E6F2C");

void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");

void GameFlipTeamLogoHorizontal(void) asm("func_8004BCE4");
void GameFlipTeamLogoHorizontal(void) {
    register u32 *base asm("$14");
    register s32 row asm("$13");
    register s32 highStart asm("$15");
    register s32 rowOffset asm("$9");
    register u32 *lowBase asm("$8");
    register s32 pairOffset asm("$11");
    register s32 highIndex asm("$7");
    register s32 nibble asm("$4");
    register u32 lowPacked asm("$5");
    register u32 highPacked asm("$6");
    register u32 lowWord asm("$12");
    register u32 highWord asm("$10");
    register u32 shift asm("$2");
    register u32 lowNibble asm("$3");

    GamePlaySoundCue(8);

    base = g_TeamLogoCanvas;
    row = 0;
    highStart = 7;
    do {
        rowOffset = row << 5;
        lowBase = base;
        pairOffset = 0;
        highIndex = highStart;
        do {
            nibble = 0;
            lowPacked = 0;
            highPacked = 0;
            lowWord = *(u32 *)(rowOffset + (s32)lowBase);
            highWord = *(u32 *)(rowOffset + (s32)&base[highIndex]);
            do {
                lowPacked <<= 4;
                shift = nibble << 2;
                lowNibble = (lowWord >> shift) & 0xF;
                asm("" : "=r"(lowNibble) : "0"(lowNibble));
                highPacked = (highPacked << 4) | ((highWord >> shift) & 0xF);
                nibble++;
                lowPacked |= lowNibble;
            } while (nibble < 8);
            shift = rowOffset + (s32)lowBase;
            lowBase++;
            pairOffset += 4;
            lowNibble = highIndex << 2;
            lowNibble += (s32)base;
            lowNibble = rowOffset + lowNibble;
            *(u32 *)lowNibble = lowPacked;
            *(u32 *)shift = highPacked;
            highIndex--;
        } while (pairOffset < 0x10);
        row++;
    } while (row < 0x40);
}

void GameRotateTeamLogoCcw(void) asm("func_8004BDB4");
void GameRotateTeamLogoCcw(void) {
    register s32 i asm("$8");
    register s32 j asm("$9");
    register s32 k asm("$7");
    register s32 limit asm("$12");
    register u32 *base asm("$13");
    register u32 *srcStart asm("$10");
    register u32 *src asm("$6");
    register u32 *stackBase asm("$11");
    register u32 *dst asm("$4");
    register s32 shift asm("$5");
    register u32 value1 asm("$3");
    register u32 value2 asm("$2");
    u32 saved[512];

    asm(".globl func_8004BDEC\nfunc_8004BDEC = func_8004BDB4 + 0x38");

    GamePlaySoundCue(8);

    base = g_TeamLogoCanvas;
    i = 0;
    limit = 7;
    do {
        j = 0;
        value2 = limit - i;
        value2 <<= 2;
        srcStart = (u32 *)(value2 + (s32)base);
        do {
            k = 0;
            src = srcStart;
            do {
                dst = (u32 *)((((i * 8 + k) * 8 + j) << 2) + (s32)(stackBase = saved));
                shift = (limit - k) << 2;
                *dst = 0;
                value1 = (src[0x38] >> shift) & 0xF;
                value1 <<= 4;
                *dst = value1;
                value2 = (src[0x30] >> shift) & 0xF;
                value2 |= value1;
                value2 <<= 4;
                *dst = value2;
                value1 = (src[0x28] >> shift) & 0xF;
                value1 |= value2;
                value1 <<= 4;
                *dst = value1;
                value2 = (src[0x20] >> shift) & 0xF;
                value2 |= value1;
                value2 <<= 4;
                *dst = value2;
                value1 = (src[0x18] >> shift) & 0xF;
                value1 |= value2;
                value1 <<= 4;
                *dst = value1;
                value2 = (src[0x10] >> shift) & 0xF;
                value2 |= value1;
                value2 <<= 4;
                *dst = value2;
                value1 = (src[0x08] >> shift) & 0xF;
                value1 |= value2;
                value1 <<= 4;
                *dst = value1;
                value2 = (src[0x00] >> shift) & 0xF;
                k++;
                value2 |= value1;
                *dst = value2;
            } while (k < 8);
            j++;
            srcStart += 0x40;
        } while (j < 8);
        i++;
    } while (i < 8);

    i = 0;
    dst = base;
    value1 = (u32)stackBase;
    do {
        value2 = *(u32 *)value1;
        value1 += 4;
        i++;
        *dst = value2;
        dst++;
    } while (i < 512);
}

void GameRotateTeamLogoCw(void) asm("func_8004BF48");
void GameRotateTeamLogoCw(void) {
    register s32 i asm("$8");
    register s32 j asm("$9");
    register s32 k asm("$7");
    register u32 *rowBase asm("$12");
    register u32 *base asm("$13");
    register u32 *srcStart asm("$10");
    register u32 *src asm("$6");
    register u32 *stackBase asm("$11");
    register u32 *dst asm("$4");
    register s32 shift asm("$5");
    register u32 value1 asm("$3");
    register u32 value2 asm("$2");
    u32 saved[512];

    GamePlaySoundCue(8);

    i = 0;
    base = g_TeamLogoCanvas;
    rowBase = base;
    do {
        j = 0;
        srcStart = rowBase;
        do {
            k = 0;
            src = srcStart;
            do {
                dst = (u32 *)((i * 8 + k) * 8);
                asm("" : "=r"(dst) : "0"(dst));
                dst = (u32 *)((s32)dst + 7);
                dst = (u32 *)((s32)dst - j);
                dst = (u32 *)(((s32)dst << 2) + (s32)(stackBase = saved));
                *dst = 0;
                value1 = src[0x00];
                shift = k << 2;
                value1 = (value1 >> shift) & 0xF;
                value1 <<= 4;
                *dst = value1;
                value2 = (src[0x08] >> shift) & 0xF;
                value2 |= value1;
                value2 <<= 4;
                *dst = value2;
                value1 = (src[0x10] >> shift) & 0xF;
                value1 |= value2;
                value1 <<= 4;
                *dst = value1;
                value2 = (src[0x18] >> shift) & 0xF;
                value2 |= value1;
                value2 <<= 4;
                *dst = value2;
                value1 = (src[0x20] >> shift) & 0xF;
                value1 |= value2;
                value1 <<= 4;
                *dst = value1;
                value2 = (src[0x28] >> shift) & 0xF;
                value2 |= value1;
                value2 <<= 4;
                *dst = value2;
                value1 = (src[0x30] >> shift) & 0xF;
                value1 |= value2;
                value1 <<= 4;
                *dst = value1;
                value2 = src[0x38];
                k++;
                value2 = (value2 >> shift) & 0xF;
                value2 |= value1;
                *dst = value2;
            } while (k < 8);
            j++;
            srcStart += 0x40;
        } while (j < 8);
        i++;
        rowBase++;
    } while (i < 8);

    i = 0;
    dst = base;
    value1 = (u32)stackBase;
    do {
        value2 = *(u32 *)value1;
        value1 += 4;
        i++;
        *dst = value2;
        dst++;
    } while (i < 512);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/menu/GameFlipTeamLogoHorizontal", func_8004C0D8);

extern s32 g_TeamLogoClutRect asm("D_8007BEDC");
extern s32 D_8007F964;
void LoadImage(Rect *rect, void *data) asm("func_80065B24");

void func_8004CED0(void) { LoadImage((Rect *)&g_TeamLogoClutRect, &D_8007F964); }

extern s32 g_TeamLogoClut asm("D_801E444C");

void func_8004CF00(void) { LoadImage((Rect *)&g_TeamLogoClutRect, &g_TeamLogoClut); }

extern s32 D_8007FB20;

void func_800468FC(void *ot, s32 x, s32 y, s32 w, s32 h);
void func_80047330(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u32 r0, u32 g0, u32 b0, u8 r1, u8 g1, u8 b1, u8 alpha);
void func_80047024(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u32 r, u32 g, u32 b, u8 alpha);
void func_80064F58(u8 *prim);
void func_80064E90(u8 *prim, s32 arg1);
void AddPrim(void *ot, void *prim) asm("func_80064DDC");

typedef struct {
    s16 b[33];
} Blob;

extern Blob D_80011898;
extern Blob D_800118DC;

void func_8004CF30(s32 arg) {
    void *s3;
    Blob l1;
    Blob l2;

    s3 = (void *)(*(s32 *)0x1F800004 + 0xAFC);
    l1 = D_80011898;
    l2 = D_800118DC;

    if (arg == 0) {
        D_8007FB20 = 0;
        return;
    }
    if (arg < 0) {
        D_8007FB20 += arg;
        if (D_8007FB20 < 0) {
            D_8007FB20 = 0;
        }
    }
    if (D_8007FB20 > 0) {
        s32 s0;
        s32 s1;
        s32 s2;
        u8 *prim;

        func_800468FC(s3, 0, 0, 0x140, 0x1E0);

        s0 = 0;
        s2 = 0;
        s1 = 0x00300000;
        do {
            register s32 cnt asm("$3") = D_8007FB20;
            register s32 m11 asm("$2") = cnt * 11;
            register u32 u0 asm("$8");
            register u8 c1 asm("$3");
            register u32 c0 asm("$2");
            __asm__("" : "=r"(m11) : "0"(m11));
            u0 = (u32)m11 >> 8;
            __asm__("" : "=r"(u0) : "0"(u0));
            c1 = (u32)(cnt * 75) >> 8;
            c0 = u0 & 0xFF;
            func_80047330(s3, s1 >> 16, 0xAA, s2 >> 16, 0x1E0, c0, c0, c0, c1, c1, c1, 0x60);
            s2 += 0x000A0000;
            s1 += 0x00070000;
            s0++;
        } while (s0 < 0x21);

        s0 = 0;
        do {
            s32 x0 = l1.b[s0];
            s32 y0 = l2.b[s0];
            s16 x1 = (0xA0 - (u16)l1.b[s0]) * 2;
            s32 v = (((s32)((u16)l2.b[s0] - 0xAA) << 7) / 309 + 0x16) * D_8007FB20;
            register s32 vv asm("$2") = v;
            register u32 sh asm("$8");
            register u32 col asm("$2");
            __asm__("" : "=r"(vv) : "0"(vv));
            sh = (u32)vv >> 9;
            __asm__("" : "=r"(sh) : "0"(sh));
            col = sh & 0xFF;
            func_80047024(s3, x0, y0, x1, 2, col, col, col, 0x60);
            s0++;
        } while (s0 < 0x21);

        prim = *(u8 **)0x1F800000;
        func_80064F58(prim);
        func_80064E90(prim, 0);
        {
            register s32 x asm("$3") = D_8007FB20;
            register s32 q0 asm("$2") = x / 5 + (x >> 31);
            register u8 col asm("$8");
            __asm__("" : "=r"(q0) : "0"(q0));
            col = q0 - (x >> 31);
            *(s16 *)(prim + 0x20) = 0x13F;
            *(s16 *)(prim + 0x10) = 0x13F;
            *(s16 *)(prim + 0x12) = 0x28;
            *(s16 *)(prim + 0x0A) = 0x28;
            *(s16 *)(prim + 0x18) = 0;
            *(s16 *)(prim + 0x08) = 0;
            *(s16 *)(prim + 0x22) = 0x1DF;
            *(s16 *)(prim + 0x1A) = 0x1DF;
            prim[0x0C] = 0;
            prim[0x04] = 0;
            prim[0x0D] = 0;
            prim[0x05] = 0;
            prim[0x0E] = 0;
            prim[0x06] = 0;
            prim[0x1C] = col;
            prim[0x14] = col;
            prim[0x1D] = col;
            prim[0x15] = col;
            prim[0x1E] = col;
            prim[0x16] = col;
        }
        {
            u8 *oldPrim = prim;
            prim += 0x24;
            AddPrim(s3, oldPrim);
        }
        *(u8 **)0x1F800000 = prim;
        func_800468FC(s3, 0x48, 0, 0x140, 0x1E0);
    }
    if (arg > 0) {
        D_8007FB20 += arg;
        if (D_8007FB20 >= 0x201) {
            D_8007FB20 = 0x200;
        }
    }
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/main/menu/GameFlipTeamLogoHorizontal", func_8004D384);
