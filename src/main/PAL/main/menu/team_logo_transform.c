#include "common.h"
#include "game/menu_types.h"
#include "psyq/gpu.h"
#include "game/audio.h"
#include "game/race.h"
#include "game/screens.h"

/* Identical to the declaration in game/menu.h; this unit cannot include that
 * header yet because four of its own definitions still disagree with it
 * (ScrollTeamLogoUp, GameDrawMenuButton, g_RankingRecords, g_TimeRecords). */
extern u16 g_TeamLogoClut[16];
extern u32 g_TeamLogoCanvas[];

/* The logo canvas is 64 rows of eight words, and each word packs eight
 * four-bit pixels. Flipping a row therefore swaps word j with word 7 - j and
 * reverses the nibble order inside both. */
void FlipTeamLogoHorizontal(void) {
    u32 *base;
    s32 row;
    s32 lastWord;
    s32 rowOffset;
    u32 *lowWordPtr;
    s32 colOffset;
    s32 highIndex;
    s32 nibble;
    u32 lowPacked;
    u32 highPacked;
    u32 lowWord;
    u32 highWord;
    u32 shift;

    PlaySoundCue(8);

    base = g_TeamLogoCanvas;
    row = 0;
    lastWord = 7;
    do {
        rowOffset = row * 32;
        lowWordPtr = base;
        colOffset = 0;
        highIndex = lastWord;
        do {
            s32 addr;
            s32 highOffset;

            nibble = 0;
            lowPacked = 0;
            highPacked = 0;
            addr = rowOffset + (s32)lowWordPtr;
            lowWord = *(u32 *)addr;
            addr = highIndex << 2;
            highOffset = addr + (s32)base;
            addr = rowOffset + highOffset;
            highWord = *(u32 *)addr;
            do {
                lowPacked <<= 4;
                shift = nibble * 4;
                lowPacked |= (lowWord >> shift) & 0xF;
                highPacked <<= 4;
                highPacked |= (highWord >> shift) & 0xF;
                nibble++;
            } while (nibble < 8);
            {
                s32 lowAddr;
                s32 highAddr;

                lowAddr = rowOffset + (s32)lowWordPtr;
                lowWordPtr++;
                colOffset += 4;
                highAddr = highIndex << 2;
                highAddr += (s32)base;
                highOffset = highAddr;
                highAddr = rowOffset + highOffset;
                *(u32 *)highAddr = lowPacked;
                *(u32 *)lowAddr = highPacked;
            }
            highIndex--;
        } while (colOffset < 0x10);
        row++;
    } while (row < 0x40);
}

void RotateTeamLogoCcw(void) {
    s32 i;
    s32 j;
    s32 k;
    s32 limit;
    u32 *base;
    u32 *srcStart;
    register u32 *src asm("$6");
    register u32 *stackBase asm("$11");
    register u32 *dst asm("$4");
    s32 shift;
    register u32 value1 asm("$3");
    u32 value2;
    u32 saved[512];

    asm(".globl func_8004BDEC\nfunc_8004BDEC = RotateTeamLogoCcw + 0x38");

    PlaySoundCue(8);

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

void RotateTeamLogoCw(void) {
    s32 i;
    s32 j;
    s32 k;
    u32 *rowBase;
    u32 *base;
    u32 *srcStart;
    register u32 *src asm("$6");
    register u32 *stackBase asm("$11");
    register u32 *dst asm("$4");
    s32 shift;
    register u32 value1 asm("$3");
    register u32 value2 asm("$2");
    u32 saved[512];

    PlaySoundCue(8);

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
                dst = (u32 *)((u8 *)dst + 7);
                dst = (u32 *)((s32)dst - j);
                dst = (u32 *)(((s32)dst * 4) + (s32)(stackBase = saved));
                *dst = 0;
                value1 = src[0x00];
                shift = k * 4;
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

void ScrollTeamLogoUp(s32 repeat);
void ScrollTeamLogoDown(void);
void ScrollTeamLogoLeft(void);
void ScrollTeamLogoRight(void);
void FlipTeamLogoVertical(void);
extern u16 g_PadHeld;
extern u16 g_PadEdge;
extern u16 g_PadEdge2;
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

void UpdateTeamLogoCanvas(void) {
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
            PlaySoundCue(2);
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

                PlaySoundCue(1);
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

                PlaySoundCue(1);
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
                    PlaySoundCue(4);
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
                            var_a0_3 = (temp_v0_2 * 32) - 0x20;
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
                    PlaySoundCue(4);
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
                    PlaySoundCue(1);
                    var_v0_3 = 2;
                    if (D_8007F958 > 0) {
                        var_v0_3 = D_8007F958 - 1;
                    }
                    D_8007F958 = var_v0_3;
                }
                if (g_PadEdge2 & 0x4000) {
                    PlaySoundCue(1);
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
                PlaySoundCue(4);
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
                PlaySoundCue(4);
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
            PlaySoundCue(2);
            D_8007F954 = 1;
        }
        if (*input & 0x10) {
            PlaySoundCue(2);
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
                    RotateTeamLogoCw();
                }
                if (g_PadEdge2 & 0x4000) {
                    FlipTeamLogoVertical();
                }
                if (g_PadEdge2 & 0x8000) {
                    RotateTeamLogoCcw();
                }
                if (g_PadEdge2 & 0x2000) {
                    FlipTeamLogoHorizontal();
                }
            } else if ((D_8007FB14 == 0x14) || (D_8007FB14 == 1)) {
                if (heldValue & 0x1000) {
                    ScrollTeamLogoUp(D_8007FB14);
                }
                if (*held & 0x4000) {
                    ScrollTeamLogoDown();
                }
                if (*held & 0x8000) {
                    ScrollTeamLogoLeft();
                }
                if (*held & 0x2000) {
                    ScrollTeamLogoRight();
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
                    PlaySoundCue(4);
                }
            }
        }
        }
        if ((g_PadEdge2 & 2) && (D_8007F930 != 0)) {
            PlaySoundCue(4);
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
                var_v0_7 = *(u8 *)temp_a1_5 / 16;
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

extern s32 g_TeamLogoClutRect;
extern s32 D_8007F964;

void RestoreTeamLogoClut(void);
void RestoreTeamLogoClut(void) { LoadImage((Rect *)&g_TeamLogoClutRect, &D_8007F964); }

void UploadTeamLogoClut(void);
void UploadTeamLogoClut(void) { LoadImage((Rect *)&g_TeamLogoClutRect, g_TeamLogoClut); }

extern s32 D_8007FB20;

void SetDrawClipRect(void *ot, s32 x, s32 y, s32 w, s32 h);
void DrawGradientLine(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u32 r0, u32 g0, u32 b0, u8 r1, u8 g1, u8 b1, u8 alpha);
void DrawLogoRect(void *ot, s32 x0, s32 y0, s32 x1, u16 y1, u32 r, u32 g, u32 b, u8 alpha) asm("DrawSolidRect");

typedef struct Blob {
    s16 b[33];
} Blob;

extern const Blob D_80011898;
extern const Blob D_800118DC;
extern const char D_80011920[4];
extern const char D_80011924[4];
extern const char D_80011928[4];
extern const char D_8001192C[8];

void DrawMenuLightBurst(s32 arg) {
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
        s32 value;
        register u32 scaled asm("$8");

        SetDrawClipRect(s3, 0, 0, 0x140, 0x1E0);

        s0 = 0;
        s2 = 0;
        s1 = 0x00300000;
        do {
            s32 cnt = D_8007FB20;
            u8 c1;
            value = cnt * 11;
            scaled = (u32)value / 256;
            c1 = (u32)(cnt * 75) / 256;
            value = (u8)scaled;
            DrawGradientLine(s3, s1 >> 16, 0xAA, s2 >> 16, 0x1E0, value, value, value, c1, c1, c1, 0x60);
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
            value = v;
            scaled = (u32)value / 512;
            value = (u8)scaled;
            DrawLogoRect(s3, x0, y0, x1, 2, value, value, value, 0x60);
            s0++;
        } while (s0 < 0x21);

        prim = *(u8 **)0x1F800000;
        SetPolyG4(prim);
        SetSemiTrans(prim, 0);
        {
            register s32 x asm("$3") = D_8007FB20;
            value = x / 5 + (x >> 31);
            scaled = value - (x >> 31);
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
            prim[0x1C] = scaled;
            prim[0x14] = scaled;
            prim[0x1D] = scaled;
            prim[0x15] = scaled;
            prim[0x1E] = scaled;
            prim[0x16] = scaled;
        }
        {
            u8 *oldPrim = prim;
            prim += 0x24;
            AddPrim(s3, oldPrim);
        }
        *(u8 **)0x1F800000 = prim;
        SetDrawClipRect(s3, 0x48, 0, 0x140, 0x1E0);
    }
    if (arg > 0) {
        D_8007FB20 += arg;
        if (D_8007FB20 >= 0x201) {
            D_8007FB20 = 0x200;
        }
    }
}

typedef union {
    s32 value;
    struct {
        u16 fraction;
        u16 integer;
    } parts;
} PackedCoordinate;

/* GCC 2.6.3 needs the concrete outer bound for the indexed view below. */
extern RaceRecord g_RankingRecords[2][4][5];
extern RaceRecord g_TimeRecords[2][4][5];

void DrawSprite(void *ot, s32 x, s16 y, s32 w, s32 h, s32 u, s32 v,
                    s32 r, s32 g, s32 b, s32 clut, s32 shadeTex,
                    s32 semiTrans, s32 flags);
void DrawLargeText(s32 x, s16 y, u8 *text, s32 r, s32 g, s32 b,
                       s32 clut, s32 flags);
void GameDrawMenuButton(s32 x, s16 y, s32 w, s32 h, s32 r, s32 g, s32 b,
                        s32 flags, s32 textX, s32 textY,
                        s32 caption);
void DrawRectOutline(void *ot, s32 x, s16 y, s32 w, s32 h, s32 r,
                         s32 g, s32 b, s32 alpha);
void DrawSolidRect(void *ot, s32 x, s16 y, s32 w, s32 h, s32 r,
                       s32 g, s32 b, s32 alpha);

/* The animated five-row ranking/time-record panel. */
s32 DrawRankingTable(s32 *progress, s32 step, s32 ranking)
;
asm(".globl func_8004E07C\nfunc_8004E07C = DrawRankingTable + 0xCF8");

s32 DrawRankingTable(s32 *progress, s32 step, s32 ranking) {
    u8 text[16];
    void *ot;
    s32 phase;
    u32 slide;
    s32 headerTextureU;
    s16 panelY;
    s16 contentY;
    s16 rowY;
    s32 row;
    s16 rowYStep;
    s32 loopClut;
    s32 spriteHeight;
    s32 spriteOne;
    s32 badgeMask;
    PackedCoordinate badgeX;
    s16 car;
    s32 badgeXWord;
    s16 rectLeft;

    ot = *(void **)0x1F800004;
    if (step == 0) {
        *progress = 0;
        /* Initialization-only call; its caller ignores the return value. */
        return;
    }

    if (step < 0) {
        s32 currentValue;
        s32 value;

        currentValue = *progress;
        value = step + currentValue;
        *progress = value;
        if (value < 0) {
            *progress = 0;
        }
    }

    phase = *progress;
    if (phase >= 0) {
        if (phase >= 13) {
            phase = 12;
        }
        slide = ((u32)(phase * -1120)) >> 5;
        panelY = slide + 0x21A;

        if (g_CourseIndex >= 4) {
            DrawSprite(ot, 0xA4, (s16)(slide + 0x1EA), 0x30, 0x18,
                             0xCC, 0x38, 0, 0, 0, 0x20F, 1, 0, 0x3C);
        }
        DrawSprite(ot, 0xC8, (s16)(slide + 0x218), 0x20, 0x28,
                         0x48, 0xD8, 0, 0, 0, 0x220, 1, 0, 0x19);

        {
            s32 headerClut;
            s32 headerFlags;
            s32 contentYWide;

            headerClut = 0x244;
            headerFlags = 0x3B;
            headerTextureU = 0xA4;
            contentYWide = slide + 0x26C;
            contentY = contentYWide;
            DrawSprite(ot, 0xA4, contentY, 0x48, 0x10, 0x48, 0xAC,
                             0, 0, 0, headerClut, 1, 1, headerFlags);
            switch (g_CourseIndex & 3) {
            case 0:
                DrawSprite(ot, 0xA4, (s16)(slide + 0x25C), 0x54, 0x10,
                                 0, 0x9C, 0, 0, 0, headerClut, 1, 1, headerFlags);
                DrawSprite(ot, 0xEC, contentY, 0x20, 0x10, 0x44, 0xB4,
                                 0, 0, 0, headerClut, 1, 1, 0x3A);
                break;
            case 1:
                DrawSprite(ot, 0xA4, (s16)(slide + 0x25C), 0x4C, 0x10,
                                 0x54, 0x9C, 0, 0, 0, headerClut, 1, 1,
                                 headerFlags);
                DrawSprite(ot, 0xEC, contentY, 0x20, 0x10, 0x64, 0xB4,
                                 0, 0, 0, headerClut, 1, 1, 0x3A);
                break;
            case 2:
                DrawSprite(ot, 0xA4, (s16)(slide + 0x25C), 0x48, 0x10,
                                 0, 0xAC, 0, 0, 0, headerClut, 1, 1,
                                 headerFlags);
                DrawSprite(ot, 0xEC, contentY, 0x20, 0x10, 0x84, 0xB4,
                                 0, 0, 0, headerClut, 1, 1, 0x3A);
                break;
            case 3: {
                DrawSprite(ot, 0xA4, (s16)(slide + 0x25C), 0x5C, 0x10,
                                 headerTextureU, 0x9C, 0, 0, 0,
                                 headerClut, 1, 1,
                                 headerFlags);
                DrawSprite(ot, 0xEC, contentY, 0x20, 0x10,
                                 headerTextureU, 0xB4, 0, 0, 0,
                                 headerClut, 1, 1, 0x3A);
                break;
            }
            }
        }

        if (ranking != 0) {
            rowY = panelY + 0xA;
            DrawSprite(ot, 0x18, rowY, 0x12, 0x10, 0, 0x7C,
                             0, 0, 0, 0x244, 1, 1, 0x3B);
            DrawSprite(ot, 0x2B, rowY, 0x14, 0x10, 0xC0, 0x8C,
                             0, 0, 0, 0x244, 1, 1, 0x3B);
            DrawSprite(ot, 0x41, rowY, 0x2C, 0x10, 0xD4, 0x8C,
                             0, 0, 0, 0x244, 1, 1, 0x3B);
        } else {
            rowY = panelY + 0xA;
            DrawSprite(ot, 0x18, rowY, 0x1C, 0x10, 0x44, 0x7C,
                             0, 0, 0, 0x244, 1, 1, 0x3B);
            DrawSprite(ot, 0x34, rowY, 0x14, 0x10, 0xC0, 0x8C,
                             0, 0, 0, 0x244, 1, 1, 0x3B);
            DrawSprite(ot, 0x4A, rowY, 0x2C, 0x10, 0xD4, 0x8C,
                             0, 0, 0, 0x244, 1, 1, 0x3B);
        }
        GameDrawMenuButton(0, panelY, 0x99, 0x23, 0, 0, 0, 0, 0, 0, 0);

        rectLeft = 0;
        loopClut = 0x244;
        spriteHeight = 0x10;
        spriteOne = 1;
        badgeX.value = 0;
        /* Keeps GCC 2.6.3's packed-coordinate allocation deterministic. */
        asm("" : "=r"(badgeX.value) : "0"(badgeX.value));
        badgeMask = -65536;
        badgeX.value &= badgeMask;
        row = 0;
        badgeX.parts.integer = 0xE8;
        rowYStep = 0x82;
        do {
            if (ranking != 0) {
                FormatLapTime(
                    text,
                    g_RankingRecords[((g_CourseIndex & 4) >> 2)]
                                    [(g_CourseIndex & 3)][row].raceTime);
                rowY = panelY + rowYStep;
                DrawLargeText(0x36, rowY, text, 0x7F, 0x7F, 0x7F,
                                  loopClut, 0x20);
                DrawLargeText(
                    0x77, rowY,
                    g_RankingRecords[((g_CourseIndex & 4) >> 2)]
                                    [(g_CourseIndex & 3)][row].driverName,
                    0x7F, 0x7F, 0x7F, loopClut, 0xA0);
            } else {
                FormatLapTime(
                    text,
                    g_TimeRecords[((g_CourseIndex & 4) >> 2)]
                                 [(g_CourseIndex & 3)][row].raceTime);
                rowY = panelY + rowYStep;
                DrawLargeText(0x36, rowY, text, 0x7F, 0x7F, 0x7F,
                                  loopClut, 0x20);
                DrawLargeText(
                    0x77, rowY,
                    g_TimeRecords[((g_CourseIndex & 4) >> 2)]
                                 [(g_CourseIndex & 3)][row].driverName,
                    0x7F, 0x7F, 0x7F, loopClut, 0xA0);
            }

            DrawSprite(ot, 0x17, (s16)(panelY + rowYStep), 8,
                             spriteHeight, (s16)(row * 8 + 8), 0x18,
                             0, 0, 0, loopClut, spriteOne, spriteOne, 0x3B);
            DrawSprite(ot, 0xA7, (s16)(panelY + rowYStep), 8,
                             spriteHeight, 0x58, 0x28, 0, 0, 0, loopClut,
                             spriteOne, spriteOne, 0x3B);
            DrawSprite(ot, 0xDF, (s16)(panelY + rowYStep), 8,
                             spriteHeight, 0x58, 0x28, 0, 0, 0, loopClut,
                             spriteOne, spriteOne, 0x3B);

            if (ranking != 0) {
                car = (u16)g_RankingRecords[((g_CourseIndex & 4) >> 2)]
                                             [(g_CourseIndex & 3)][row].carIndex;
            } else {
                car = (u16)g_TimeRecords[((g_CourseIndex & 4) >> 2)]
                                          [(g_CourseIndex & 3)][row].carIndex;
            }
            switch (car) {
                case 0:
                case 1:
                case 2:
                case 10:
                    DrawSprite(ot, 0xAE, (s16)(panelY + rowYStep),
                                     0x14, spriteHeight, 0x50,
                                     0xBC, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3B);
                    break;
                case 3:
                    DrawSprite(ot, 0xAF, (s16)(panelY + rowYStep),
                                     0x20, spriteHeight, 0,
                                     0xBC, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3B);
                    break;
                case 4:
                case 5:
                case 6:
                case 11:
                    DrawSprite(ot, 0xAF, (s16)(panelY + rowYStep),
                                     0x20, spriteHeight, 0x64,
                                     0xBC, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3B);
                    break;
                case 7:
                case 8:
                case 9:
                case 12:
                    DrawSprite(ot, 0xAF, (s16)(panelY + rowYStep),
                                     0x30, spriteHeight, 0x22,
                                     0xBC, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3B);
                    break;
            }

            switch (car) {
                case 0:
                    badgeXWord = badgeX.value;
                    DrawSprite(ot, badgeXWord >> 16,
                                     (s16)(panelY + rowYStep),
                                     0x2A, spriteHeight, 0x16,
                                     0x30, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3E);
                    break;
                case 1:
                    badgeXWord = badgeX.value;
                    DrawSprite(ot, badgeXWord >> 16,
                                     (s16)(panelY + rowYStep),
                                     0x20, spriteHeight, 0x48,
                                     0x30, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3E);
                    break;
                case 2:
                    DrawSprite(ot, 0xE9, (s16)(panelY + rowYStep),
                                     0x20, spriteHeight, 0x7C,
                                     0x30, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3E);
                    break;
                case 10:
                    DrawSprite(ot, 0xE9, (s16)(panelY + rowYStep),
                                     0x2C, spriteHeight, 0xA4,
                                     0x30, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3E);
                    break;
                case 3:
                    DrawSprite(ot, 0xE7, (s16)(panelY + rowYStep),
                                     0x34, spriteHeight, 0,
                                     0x40, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3E);
                    break;
                case 4:
                    DrawSprite(ot, 0xE9, (s16)(panelY + rowYStep),
                                     0x28, spriteHeight, 0x74,
                                     0x50, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3E);
                    break;
                case 5:
                    DrawSprite(ot, badgeX.value >> 16,
                                     (s16)(panelY + rowYStep),
                                     0x2A, spriteHeight, 0x3E,
                                     0x50, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3E);
                    break;
                case 6:
                    badgeXWord = badgeX.value;
                    DrawSprite(ot, badgeXWord >> 16,
                                     (s16)(panelY + rowYStep),
                                     0x20, spriteHeight, 0xB0,
                                     0x50, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3E);
                    break;
                case 11:
                    badgeXWord = badgeX.value;
                    DrawSprite(ot, badgeXWord >> 16,
                                     (s16)(panelY + rowYStep),
                                     0x2A, spriteHeight, 0x0A,
                                     0x60, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3E);
                    break;
                case 7:
                    badgeXWord = badgeX.value;
                    DrawSprite(ot, badgeXWord >> 16,
                                     (s16)(panelY + rowYStep),
                                     0x28, spriteHeight, 0x40,
                                     0x40, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3E);
                    break;
                case 8:
                    DrawSprite(ot, 0xE9, (s16)(panelY + rowYStep),
                                     0x22, spriteHeight, 0x7A,
                                     0x40, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3E);
                    break;
                case 9:
                    DrawSprite(ot, 0xE9, (s16)(panelY + rowYStep),
                                     0x30, spriteHeight, 0xA0,
                                     0x40, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3E);
                    break;
                case 12:
                    badgeXWord = badgeX.value;
                    DrawSprite(ot, badgeXWord >> 16,
                                     (s16)(panelY + rowYStep),
                                     0x30, spriteHeight, 0x04,
                                     0x50, 0, 0, 0, loopClut,
                                     spriteOne, spriteOne, 0x3E);
                    break;
            }

            rowYStep += 0x20;
            row++;
        } while (row < 5);

        {
            register s32 suffixX asm("$20");
            s32 textShade;
            s32 textClut;
            s32 textFlags;
            u8 *lastSuffix;

            suffixX = 0x1E;
            asm("" : "=r"(suffixX) : "0"(suffixX));
            textShade = 0x7F;
            textClut = 0x244;
            textFlags = 0x20;
            DrawLargeText(suffixX, (s16)(panelY + 0x82),
                              (u8 *)D_80011920, textShade, textShade,
                              textShade, textClut, textFlags);
            DrawLargeText(suffixX, (s16)(panelY + 0xA2),
                              (u8 *)D_80011924, textShade, textShade,
                              textShade, textClut, textFlags);
            DrawLargeText(0x1F, (s16)(panelY + 0xC2),
                              (u8 *)D_80011928, textShade, textShade,
                              textShade, textClut, textFlags);
            lastSuffix = (u8 *)D_8001192C;
            DrawLargeText(suffixX, (s16)(panelY + 0xE2), lastSuffix,
                              textShade, textShade, textShade, textClut,
                              textFlags);
            DrawLargeText(suffixX, (s16)(panelY + 0x102), lastSuffix,
                              textShade, textShade, textShade, textClut,
                              textFlags);
        }

        {
            void *rectOt;
            void *rectCallOt;
            s32 rectX;
            s16 rectY;
            s32 rectHeight;
            s32 rectAlpha;

            rectOt = (u8 *)ot + 4;
            rectCallOt = rectOt;
            rectX = rectLeft;
            rectY = panelY + 0x7A;
            rectHeight = 0xA0;
            rectAlpha = 0xFF;
            DrawRectOutline(rectCallOt, rectX, rectY, 0x124,
                                rectHeight, 0xB4, 0xB4, 0xB4, rectAlpha);
            rectCallOt = rectOt;
            DrawSolidRect(rectCallOt, rectX, rectY, 0x124, rectHeight,
                              0, 0, 0, rectAlpha);
        }
    }

    if (step >= 0) {
        s32 nextProgress;

        nextProgress = step + *progress;
        if (nextProgress >= 15) {
            *progress = 15;
            return 1;
        }
        *progress = nextProgress;
    }
    return 0;
}
