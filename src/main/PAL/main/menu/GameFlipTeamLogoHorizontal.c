#include "common.h"
#include "psyq/gpu.h"

extern u32 g_TeamLogoCanvas[] asm("D_801E6F2C");

void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");

void GameFlipTeamLogoHorizontal(void) asm("func_8004BCE4");
void GameFlipTeamLogoHorizontal(void) {
    u32 *base;
    s32 row;
    s32 highStart;
    s32 rowOffset;
    /* These pins are load-bearing: removing any one changes .text. */
    register u32 *lowBase asm("$8");
    s32 pairOffset;
    register s32 highIndex asm("$7");
    s32 nibble;
    u32 lowPacked;
    /* This pin is load-bearing: removing it changes .text. */
    register u32 highPacked asm("$6");
    u32 lowWord;
    u32 highWord;
    /* These pins are load-bearing: removing any one changes .text. */
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
    s32 i;
    s32 j;
    s32 k;
    s32 limit;
    u32 *base;
    u32 *srcStart;
    /* These pins are load-bearing: removing any one changes .text. */
    register u32 *src asm("$6");
    register u32 *stackBase asm("$11");
    register u32 *dst asm("$4");
    s32 shift;
    register u32 value1 asm("$3");
    u32 value2;
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
    s32 i;
    s32 j;
    s32 k;
    u32 *rowBase;
    u32 *base;
    u32 *srcStart;
    /* These pins are load-bearing: removing any one changes .text. */
    register u32 *src asm("$6");
    register u32 *stackBase asm("$11");
    register u32 *dst asm("$4");
    s32 shift;
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
                /* This barrier is load-bearing: removing it changes .text. */
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

typedef union {
    s32 value;
    u16 lo;
} TeamLogoColorSlot;

void func_8004B9B8(s32 repeat);
void func_8004BA50(void);
void func_8004BAE4(void);
void func_8004BBA8(void);
void func_8004BC68(void);
extern u16 g_PadHeld asm("D_801E436A");
extern u16 g_PadEdge asm("D_801E4370");
extern u16 g_PadEdge2 asm("D_801E436E");
extern u8 D_8007F930;
extern s32 D_8007F934;
extern s32 D_8007F938;
extern s32 D_8007F93C;
extern s32 D_8007F940;
extern s32 D_8007F944;
extern s32 D_8007F94C;
extern TeamLogoColorSlot D_8007F950;
extern s32 D_8007F954;
extern s32 D_8007F958;
extern s32 D_8007FB14;
extern s32 D_8007FB18;
extern s32 D_8007FB1C;
extern s32 D_8019CAB8;
extern u16 g_TeamLogoClut[] asm("D_801E444C");

void func_8004C0D8(void) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_5;
    s32 temp_v1_9;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a0_4;
    s32 var_a0_7;
    s32 var_a1;
    s32 var_a3;
    s32 var_a3_2;
    s32 var_t0;
    s32 var_t0_2;
    s32 var_v0_3;
    s32 var_v1;
    s32 var_v1_2;
    s32 var_v1_3;
    s32 var_v1_5;
    u16 *temp_v1;
    u16 temp_a1;
    u16 var_v0_5;
    u32 temp_v1_3;
    u32 temp_v1_4;
    u32 temp_v1_5;
    u32 var_v0_7;
    u16 *temp_a1_5;
    s32 var_a2;

    {
        s32 input;
        s32 held;
        s32 repeat;
        u16 *state = &g_PadHeld;

        input = (*state & 5) ? 0 : 3;
        if (*state & D_8007FB18) {
            held = D_8007FB14;
            repeat = 0x14;
            if (held < input + repeat) {
                repeat = held + 1;
            }
            D_8007FB14 = repeat;
        } else {
            D_8007FB14 = 0;
        }
    }
    {
        u16 held = g_PadHeld;

        D_8007FB18 = held & 0xF000;
        if (!(held & 0x20)) {
            D_8019CAB8 = 1;
        }
    }
    if (D_8007F930 != 0) {
        if (g_PadEdge2 & 0x100) {
            D_8007FB1C = D_8007F944;
            var_a0_2 = 0;
            if (D_8007F944 < 2) {
                var_a0_2 = D_8007F944 + 1;
            }
            D_8007F944 = var_a0_2;
        }
    } else {
        D_8007F944 = 1;
    }
    if (D_8007F954 == 1) {
        u16 *input = &g_PadEdge2;

        if (*input & 0x60) {
            GamePlaySoundCue(2);
            D_8007F954 = 0;
            D_8019CAB8 = 0;
        }
        {
            s32 mask = 0xF;

            if ((*input & 0x100) &&
                ((g_PadHeld & mask) == mask)) {
                D_8007F930 = D_8007F930 == 0;
                D_8007F944 = D_8007FB1C;
            }
        }
        if ((D_8007FB14 == 0x14) || (D_8007FB14 == 1)) {
            if (g_PadHeld & 0x8000) {
                TeamLogoColorSlot output;
                s32 selected;

                GamePlaySoundCue(1);
                selected = D_8007F950.value;
                var_v1 = 0xF;
                if (selected >= 2) {
                    var_v1 = selected - 1;
                }
                output.value = var_v1;
                D_8007F950 = output;
            }
            if (g_PadHeld & 0x2000) {
                TeamLogoColorSlot output;
                s32 selected;

                GamePlaySoundCue(1);
                selected = D_8007F950.value;
                var_v1_2 = 1;
                if (selected < 0xF) {
                    var_v1_2 = selected + 1;
                }
                output.value = var_v1_2;
                D_8007F950 = output;
            }
        }
        if (D_8007F930 != 0) {
            if (g_PadHeld & 0xA) {
                if (g_PadEdge & 0x1000) {
                    GamePlaySoundCue(4);
                    temp_v1_4 = D_8007F950.value;
                    temp_v1 = g_TeamLogoClut + temp_v1_4;
                    temp_a1 = *temp_v1 | 0x8000;
                    *temp_v1 = temp_a1;
                    var_v0_5 = temp_a1;
                    switch (D_8007F958) {
                    case 0:
                        temp_v0 = temp_a1 & 0x1F;
                        if (temp_v0 == 0) {
                            var_a0_3 = 0x1F;
                        } else {
                            var_a0_3 = temp_v0 - 1;
                        }
                        g_TeamLogoClut[D_8007F950.value] =
                            var_a0_3 |
                            (g_TeamLogoClut[D_8007F950.value] & 0xFFE0);
                        break;
                    case 1:
                        temp_v0_2 = (var_v0_5 >> 5) & 0x1F;
                        if (temp_v0_2 != 0) {
                            var_a0_3 = (temp_v0_2 << 5) - 0x20;
                        } else {
                            var_a0_3 = 0x3E0;
                        }
                        g_TeamLogoClut[D_8007F950.value] =
                            var_a0_3 |
                            (g_TeamLogoClut[D_8007F950.value] & 0xFC1F);
                        break;
                    case 2:
                        temp_v0_3 = (var_v0_5 >> 0xA) & 0x1F;
                        if (temp_v0_3 != 0) {
                            var_a0_3 = (temp_v0_3 << 0xA) - 0x400;
                        } else {
                            var_a0_3 = 0x7C00;
                        }
                        g_TeamLogoClut[D_8007F950.value] =
                            var_a0_3 |
                            (g_TeamLogoClut[D_8007F950.value] & 0x83FF);
                        break;
                    default:
                        break;
                    }
                }
                if (g_PadEdge & 0x4000) {
                    GamePlaySoundCue(4);
                    temp_v1_4 = D_8007F950.value;
                    temp_v1 = g_TeamLogoClut + temp_v1_4;
                    temp_a1 = *temp_v1 | 0x8000;
                    *temp_v1 = temp_a1;
                    var_v0_5 = temp_a1;
                    switch (D_8007F958) {
                    case 0:
                        temp_v1_3 = temp_a1 & 0x1F;
                        if (temp_v1_3 >= 0x1FU) {
                            var_a0_4 = 0;
                        } else {
                            var_a0_4 = temp_v1_3 + 1;
                        }
                        g_TeamLogoClut[D_8007F950.value] =
                            var_a0_4 |
                            (g_TeamLogoClut[D_8007F950.value] & 0xFFE0);
                        return;
                    case 1:
                        temp_v1_4 = (var_v0_5 >> 5) & 0x1F;
                        if (temp_v1_4 < 0x1FU) {
                            var_a0_4 = (temp_v1_4 + 1) << 5;
                        } else {
                            var_a0_4 = 0;
                        }
                        g_TeamLogoClut[D_8007F950.value] =
                            var_a0_4 |
                            (g_TeamLogoClut[D_8007F950.value] & 0xFC1F);
                        return;
                    case 2:
                        temp_v1_5 = (var_v0_5 >> 0xA) & 0x1F;
                        if (temp_v1_5 < 0x1FU) {
                            var_a0_4 = (temp_v1_5 + 1) << 0xA;
                        } else {
                            var_a0_4 = 0;
                        }
                        g_TeamLogoClut[D_8007F950.value] =
                            var_a0_4 |
                            (g_TeamLogoClut[D_8007F950.value] & 0x83FF);
                        return;
                    default:
                        return;
                    }
                }
            } else {
                if (g_PadEdge2 & 0x1000) {
                    GamePlaySoundCue(1);
                    var_v0_3 = 2;
                    if (D_8007F958 > 0) {
                        var_v0_3 = D_8007F958 - 1;
                    }
                    D_8007F958 = var_v0_3;
                }
                if (g_PadEdge2 & 0x4000) {
                    GamePlaySoundCue(1);
                    var_v1_5 = 0;
                    if (D_8007F958 < 2) {
                        var_v1_5 = D_8007F958 + 1;
                    }
                    D_8007F958 = var_v1_5;
                }
            }
        }
    } else {
        if ((g_PadHeld & 0x20) && (D_8019CAB8 != 0)) {
            if (g_PadEdge2 & 0x20) {
                GamePlaySoundCue(4);
            }
            for (var_t0 = 0; var_t0 < D_8007F94C; var_t0++) {
                    for (var_a3 = 0; var_a3 < D_8007F94C; var_a3++) {
                            u16 *p;
                            s32 sum;
                            s32 adj;
                            s32 row;
                            s32 q;
                            s32 rem;

                            p = (u16 *)g_TeamLogoCanvas;
                            sum = D_8007F93C + D_8007F934 + var_a3;
                            adj = sum;
                            row = (D_8007F940 + D_8007F938 + var_t0) * 0x10;
                            if (sum < 0) {
                                adj = sum + 3;
                            }
                            q = adj >> 2;
                            p += row + q;
                            rem = sum - (q * 4);
                            switch (rem) {
                            case 0:
                                *p = (*p & 0xFFF0) | D_8007F950.lo;
                                break;
                            case 1:
                                *p = (*p & 0xFF0F) | (D_8007F950.lo << 4);
                                break;
                            case 2:
                                *p = (*p & 0xF0FF) | (D_8007F950.lo << 8);
                                break;
                            case 3:
                                *p = (*p & 0xFFF) | (D_8007F950.lo << 0xC);
                                break;
                            }
                    }
            }
        }
        if (g_PadHeld & 0x80) {
            if (g_PadEdge2 & 0x80) {
                GamePlaySoundCue(4);
            }
            for (var_t0_2 = 0; var_t0_2 < D_8007F94C; var_t0_2++) {
                    for (var_a3_2 = 0; var_a3_2 < D_8007F94C; var_a3_2++) {
                            u16 *p;
                            s32 sum;
                            s32 adj;
                            s32 row;
                            s32 q;
                            s32 rem;

                            sum = D_8007F93C + D_8007F934 + var_a3_2;
                            p = (u16 *)g_TeamLogoCanvas;
                            adj = sum;
                            row = D_8007F940 + D_8007F938 + var_t0_2;
                            row *= 0x10;
                            if (sum < 0) {
                                adj = sum + 3;
                            }
                            q = adj >> 2;
                            adj = row;
                            p += adj + q;
                            rem = sum - (q * 4);
                            switch (rem) {
                            case 0:
                                *p &= 0xFFF0;
                                break;
                            case 1:
                                *p &= 0xFF0F;
                                break;
                            case 2:
                                *p &= 0xF0FF;
                                break;
                            case 3:
                                *p &= 0xFFF;
                                break;
                            }
                    }
            }
        }
        {
            u16 *input = &g_PadEdge2;

        if (*input & 0x40) {
            GamePlaySoundCue(2);
            D_8007F954 = 1;
        }
        if (*input & 0x10) {
            GamePlaySoundCue(2);
            switch (D_8007F94C) {
            case 1:
                D_8007F94C = 2;
                break;
            case 2:
                D_8007F94C = 4;
                break;
            case 4:
                D_8007F94C = 1;
                break;
            }
            if ((D_8007F934 + D_8007F94C) >= 0x20) {
                D_8007F934 = 0x20 - D_8007F94C;
            }
            if ((D_8007F938 + D_8007F94C) >= 0x20) {
                D_8007F938 = 0x20 - D_8007F94C;
            }
        }
        }
        {
            u16 *held = &g_PadHeld;
            u16 heldValue = *held;

        if ((heldValue & 8) && (D_8007F930 != 0)) {
            if (heldValue & 4) {
                if (g_PadEdge2 & 0x1000) {
                    GameRotateTeamLogoCw();
                }
                if (g_PadEdge2 & 0x4000) {
                    func_8004BC68();
                }
                if (g_PadEdge2 & 0x8000) {
                    GameRotateTeamLogoCcw();
                }
                if (g_PadEdge2 & 0x2000) {
                    GameFlipTeamLogoHorizontal();
                }
            } else if ((D_8007FB14 == 0x14) || (D_8007FB14 == 1)) {
                if (heldValue & 0x1000) {
                    func_8004B9B8(D_8007FB14);
                }
                if (*held & 0x4000) {
                    func_8004BA50();
                }
                if (*held & 0x8000) {
                    func_8004BAE4();
                }
                if (*held & 0x2000) {
                    func_8004BBA8();
                }
            }
        } else {
            var_a1 = 0;
            if ((D_8007FB14 == 0x14) || (D_8007FB14 == 1) || (g_PadHeld & 5)) {
                var_a0_7 = 0;
                if (g_PadHeld & 0x1000) {
                    if (D_8007F938 > 0) {
                        D_8007F938 -= 1;
                        var_a0_7 = 1;
                    } else if (D_8007F940 > 0) {
                        D_8007F940 -= 1;
                        var_a0_7 = 1;
                    }
                }
                if (g_PadHeld & 0x4000) {
                    if ((D_8007F938 + D_8007F94C) < 0x20) {
                        D_8007F938 += 1;
                        var_a0_7 = 1;
                    } else if (D_8007F940 < 0x20) {
                        D_8007F940 += 1;
                        var_a0_7 = 1;
                    }
                }
                if (g_PadHeld & 0x8000) {
                    if (D_8007F934 > 0) {
                        D_8007F934 -= 1;
                        var_a1 = 1;
                    } else if (D_8007F93C > 0) {
                        D_8007F93C -= 1;
                        var_a1 = 1;
                    }
                }
                if (g_PadHeld & 0x2000) {
                    if ((D_8007F934 + D_8007F94C) < 0x20) {
                        D_8007F934 += 1;
                        var_a1 = 1;
                    } else if (D_8007F93C < 0x20) {
                        D_8007F93C += 1;
                        var_a1 = 1;
                    }
                }
                if ((g_PadHeld & 0xA0) && ((var_a1 != 0) || (var_a0_7 != 0))) {
                    GamePlaySoundCue(4);
                }
            }
        }
        }
        if ((g_PadEdge2 & 2) && (D_8007F930 != 0)) {
            GamePlaySoundCue(4);
            temp_a1_5 = (u16 *)g_TeamLogoCanvas;
            temp_a0 = D_8007F93C + D_8007F934;
            var_a2 = temp_a0;
            var_v1_3 = (D_8007F940 + D_8007F938) * 0x10;
            if (temp_a0 < 0) {
                var_a2 = temp_a0 + 3;
            }
            temp_v0_5 = var_a2 >> 2;
            temp_a1_5 += var_v1_3 + temp_v0_5;
            temp_v1_9 = temp_a0;
            temp_v1_9 = temp_v1_9 - (temp_v0_5 * 4);
            switch (temp_v1_9) {
            case 0:
                var_v0_7 = temp_a1_5[0] & 0xF;
                break;
            case 1:
                var_v0_7 = *(u8 *)temp_a1_5 >> 4;
                break;
            case 2:
                var_v0_7 = *((u8 *)temp_a1_5 + 1) & 0xF;
                break;
            case 3:
                var_v0_7 = temp_a1_5[0] >> 0xC;
                break;
            default:
                return;
            }
            if (var_v0_7 == 0) {
                var_v0_7 = (u32)D_8007F950.value;
            }
            D_8007F950.value = (s32)var_v0_7;
        }
    }
}

extern s32 g_TeamLogoClutRect asm("D_8007BEDC");
extern s32 D_8007F964;
void LoadImage(Rect *rect, void *data) asm("func_80065B24");

void func_8004CED0(void) { LoadImage((Rect *)&g_TeamLogoClutRect, &D_8007F964); }

void func_8004CF00(void) { LoadImage((Rect *)&g_TeamLogoClutRect, g_TeamLogoClut); }

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
            s32 cnt = D_8007FB20;
            /* These pins are load-bearing: removing any one changes .text. */
            register s32 m11 asm("$2") = cnt * 11;
            register u32 u0 asm("$8");
            u8 c1;
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
            /* These pins are load-bearing: removing any one changes .text. */
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
            /* These pins are load-bearing: removing any one changes .text. */
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
