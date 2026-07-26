#include "common.h"
#include "game/audio.h"
#include "game/cd.h"

extern s16 g_SeqHandle asm("D_801E6D90");
extern s16 D_801E6D94;
void func_80072698(s32 arg0, s32 arg1, s32 arg2);
void func_8005B190(s32 arg0, s32 arg1);
void GameApplyCurrentSequenceAudio(void) asm("func_8005EA6C");
void GameApplyCurrentSequenceAudio(void) {
    func_80072698(g_SeqHandle, D_801E6D94, D_801E6D94);
    func_8005B190(0x28, 0x28);
}

void func_8006EAFC(s32 arg0, s32 arg1);
void GameSetMasterVolumeMono(s16 arg0) asm("func_8005EAA8");
void GameSetMasterVolumeMono(s16 arg0) { func_8006EAFC(arg0, arg0); }

void GameStartCdEvents(void) {
    GameOpenCdEventGroup();
    GameEnableCdEventGroup();
}

void GameStopCdEvents(void) {
    GameDisableCdEventGroup();
    GameCloseCdEventGroup();
}

void func_8005F304(s32 arg0);
s32 func_80063DEC(s32 arg0);
void func_8005F55C(void);
void func_8005F2AC(void);
void func_80063DBC(s32 arg0);
void func_8005F4D8(void);
void GameCdReadAndSetMode(s32 arg0) asm("func_8005EB20");
void GameCdReadAndSetMode(s32 arg0) {
    func_8005F304(arg0);
    while (func_80063DEC(arg0 & 0xFF) == 0) {}
    func_8005F55C();
    func_8005F2AC();
    func_80063DBC(arg0 & 0xFF);
    func_8005F4D8();
}
