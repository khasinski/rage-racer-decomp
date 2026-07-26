#include "common.h"
#include "game/car.h"
#include "game/race.h"
#include "game/state.h"

extern s32 D_8009E73C;
extern s32 D_8009E740;
extern s16 D_801E6E74;
extern GameCarRuntime *D_801E40BC[];
extern s32 D_801E4BB4;
extern s32 D_801E7740;
extern s16 D_801E6F26;
extern s16 D_8009E6A0;

void func_8005D6EC(s32);

void func_8003A974(void) {
    s32 s6;
    s32 s5;
    s32 s4;
    s32 s3;
    s16 *s2;
    s32 s1;
    s32 s0;

    s6 = D_8009E73C + D_8009E740;
    if ((g_CourseIndex & 3) == 3) {
        s5 = 0xC00;
        s4 = 0x1400;
    } else {
        s5 = 0x600;
        s4 = 0xE00;
    }

    if (D_801E6E74 >= 4) {
        return;
    }

    s1 = 3;
    s3 = 0x20;
    s2 = &D_801E6F26;
    s0 = 4;

    do {
        s32 a0 = D_801E40BC[s1]->field_68 + D_801E40BC[s1]->field_6C - s6;

        if (a0 >= 0) {
            if (s1 == 0) {
                D_801E4BB4 &= ~1;
            }
            D_801E7740 = s1;
            if (s4 < a0) {
                D_801E4BB4 &= ~(0x200 >> s0);
                if (D_801E40BC[s1]->field_A4 >= 0x321) {
                    D_801E40BC[s1]->field_130 = D_801E40BC[s1]->field_130 * 90 / 100;
                }
                return;
            }
            if (s5 < a0) {
                s32 counter;

                if (D_801E40BC[s1]->field_A4 >= 0x3E9) {
                    D_801E40BC[s1]->field_130 = D_801E40BC[s1]->field_130 * 98 / 100;
                }
                counter = *s2;
                D_801E4BB4 |= (s3 >> s0);
                if (counter >= 0x12D) {
                    *s2 = 0;
                }
                return;
            }
            if (!((0x200 >> s0) & D_801E4BB4)) {
                s32 bit;
                s32 flags;

                switch ((u32)g_SceneTimer % 3) {
                case 1:
                    goto cue_33;
                case 0:
                    goto cue_32;
                case 2:
                    goto cue_34;
                }
cue_32:
                if (D_8009E6A0 != 0) {
                    func_8005D6EC(0x32);
                }
                bit = 0x200;
                goto cue_done;
cue_33:
                if (D_8009E6A0 != 0) {
                    func_8005D6EC(0x33);
                }
                bit = 0x200;
                goto cue_done;
cue_34:
                if (D_8009E6A0 != 0) {
                    func_8005D6EC(0x34);
                }
                bit = 0x200;
cue_done:
                flags = D_801E4BB4;
                *s2 = 0;
                D_801E4BB4 = (bit >> s0) | flags;
                return;
            }
            (*(u16 *)s2)++;
            return;
        } else {
            if (s1 == 0 && !(D_801E4BB4 & 1) && a0 < -0x1C00) {
                if (D_8009E6A0 != 0 && g_RacePosition == 1) {
                    func_8005D6EC(0x2D);
                }
                D_801E4BB4 = (D_801E4BB4 & ~0x10) | 1;
            } else if (a0 >= -0x7FF && !((s3 >> s0) & D_801E4BB4)) {
                if (g_SceneTimer & 1) {
                    if (D_8009E6A0 != 0) func_8005D6EC(0x2F);
                } else {
                    if (D_8009E6A0 != 0) func_8005D6EC(0x30);
                }
                D_801E4BB4 |= (s3 >> s0);
            } else {
                if (a0 < -0x1000) {
                    D_801E4BB4 &= ~(s3 >> s0);
                } else if (a0 < -0x800) {
                    if (*s2 >= 0x12D) {
                        if (g_SceneTimer & 1) {
                            if (D_8009E6A0 != 0) func_8005D6EC(0x37);
                        } else {
                            if (D_8009E6A0 != 0) func_8005D6EC(0x36);
                        }
                        *s2 = 0;
                    }
                }
            }
            s2--;
            s0--;
            s1--;
        }
    } while (s1 >= 0);
}
