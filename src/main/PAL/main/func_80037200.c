#include "common.h"
#include "game/audio.h"

extern s16 D_8019C750;
extern s16 D_8019C78C;
extern s32 D_8009E694;
extern u8 D_8009E6D4[];
extern s32 D_8009E744;
extern s16 D_8009E74C;
extern s32 D_801E3E7C;
extern s32 D_801E4030;
extern s32 D_801E40B8;
extern s16 D_801E414C;
extern s32 D_801E428C;
extern u16 D_801E436E;
extern s16 D_801E43FC;
extern s32 D_801E4BAC;
extern s16 D_801E4DAC;
extern s32 D_801E40D8;
extern s16 D_801E6E74;

void func_8001C974(void);
void func_80019EFC(s32 arg0);
void func_80033AA0(s32 arg0, s32 arg1);
void func_8003479C(s32 arg0);
void func_800350B4(s32 arg0);
void func_80035258(s32 arg0);
void func_8003CF14(u8 *arg0, s32 arg1);
void func_8003E1A4(s32 arg0, s32 arg1, s32 arg2);
void func_8003E590(void);
void func_8003EAF4(void);
void func_8003EC98(void);
void func_8003F02C(void);
void func_8003F9C4(void);
void func_80040730(void);
void func_8004087C(s32 arg0);
void func_80040ADC(s32 arg0);
void func_80040DB4(s32 arg0);
void func_8004123C(void);
void func_80041840(void);
void func_800418D4(void);
void func_80042BF0(void);
void func_80042C0C(void);
void func_80042C28(void);
void func_80042CCC(s32 arg0);
void func_80045CD4(void);

void func_80037200(void) {
    s32 value;
    s32 option;

    option = 0;
    D_801E40B8++;
    if ((u32)D_801E40B8 < 0x3D) {
        func_8001C974();
        value = D_801E40B8 - 6;
        func_80033AA0(0xFF - (((value * 3) * 4) - value), 0x49);
    }

    if (D_8019C750 > 0) {
        D_8019C750--;
    }

    if ((u32)((u16)D_801E6E74 - 1) < 2 && (D_801E436E & 0x800) && D_8019C750 <= 0) {
        D_8019C750 = 0;
        D_801E4BAC = (u32)D_801E4BAC < 1;

        if (D_801E4BAC != 0) {
            func_80042C0C();
            GameForceAllEffectVoicesEnabled(0);
            D_801E414C = 0;
            GamePlaySoundCue(2);
        } else if (D_801E414C == 2) {
            D_801E43FC = 0;
            D_801E6E74 = 7;
            func_80042CCC(0x1E);
        } else if (D_801E414C == 1) {
            func_80035258(0xB);
        } else {
            GameForceAllEffectVoicesEnabled(1);
            if (D_801E6E74 >= 2) {
                func_80042C28();
            }
        }
    }

    if (D_801E6E74 == 7) {
        value = D_801E43FC;
        if (value > 0) {
            func_80033AA0(value * 3, 0x49);
            option = 6;
            value = D_801E43FC;
        }
        if (value >= 0x3D) {
            func_80035258(option);
        }
        D_801E43FC++;
    }

    if (D_801E4BAC != 0) {
        if ((D_801E436E & 0x1000) && D_801E414C > 0) {
            D_801E414C--;
            GamePlaySoundCue(1);
        }

        if ((D_801E436E & 0x4000) && D_801E414C < (2 - D_801E4DAC)) {
            D_801E414C++;
            GamePlaySoundCue(1);
        }

        D_801E40B8--;
        func_8003479C(D_801E414C);
        func_8003CF14(D_8009E6D4, 0);
        func_800350B4(D_8009E744);
        func_80019EFC(D_8009E74C);
        func_800418D4();
        *(s32 *)0x1F800084 = D_801E4030;
        func_80041840();
        func_8004123C();
        func_8003EAF4();
        func_8003F02C();
        func_80040730();
        func_8003E1A4(D_801E428C & 3, D_801E40B8, 0);
        return;
    }

    D_8009E694++;

    if ((u32)D_801E40B8 >= 0x1F && D_801E6E74 == 0) {
        func_80042BF0();
        D_801E6E74 = 2;
        D_8019C750 = 0xA;
    }

    if (D_801E3E7C >= D_801E40D8 && D_801E6E74 < 3) {
        D_801E43FC = 0;
        D_801E6E74 = 7;
        func_80042CCC(0x1E);
        GameForceAllEffectVoicesEnabled(0);
    }

    GameUpdateLoadedAudioVoices(0, 1);
    func_8003CF14(D_8009E6D4, 1);
    func_80019EFC(D_8009E74C);
    func_80045CD4();
    func_800418D4();
    *(s32 *)0x1F800084 = D_801E4030;
    func_80041840();
    func_8004123C();
    func_8003E590();
    func_8003EAF4();
    func_8003EC98();
    func_8003F02C();
    func_8003F9C4();
    func_80040730();
    func_8003E1A4(D_801E428C & 3, D_801E40B8, 1);
    func_800350B4(D_8009E744);
    GameSetReverbDepth(D_8019C78C, D_8019C78C);
    func_80040DB4(D_8009E744);
    func_80040ADC(D_8009E744);
    func_8004087C(D_8009E74C);
}
