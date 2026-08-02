#include "common.h"
#include "game/audio.h"
#include "game/cd.h"
#include "game/memcard.h"

extern s16 g_SeqHandle asm("D_801E6D90");
extern s16 g_SeqVolume asm("D_801E6D94");
void SsSeqSetVol(s32 arg0, s32 arg1, s32 arg2) asm("func_80072698");
void func_8005B190(s32 arg0, s32 arg1);
void ApplyCurrentSequenceAudio(void) asm("func_8005EA6C");
void ApplyCurrentSequenceAudio(void) {
    SsSeqSetVol(g_SeqHandle, g_SeqVolume, g_SeqVolume);
    func_8005B190(0x28, 0x28);
}

void SsSetMVol(s32 arg0, s32 arg1) asm("func_8006EAFC");
void SetMasterVolumeMono(s16 arg0) asm("func_8005EAA8");
void SetMasterVolumeMono(s16 arg0) { SsSetMVol(arg0, arg0); }

void StartMemoryCardEvents(void) {
    OpenMemoryCardEvents();
    EnableMemoryCardEvents();
}

void StopMemoryCardEvents(void) {
    DisableMemoryCardEvents();
    CloseMemoryCardEvents();
}

void func_8005F304(s32 arg0);
s32 _card_clear(s32 arg0) asm("func_80063DEC");
void func_8005F55C(void);
void func_8005F2AC(void);
void _card_load(s32 arg0) asm("func_80063DBC");
void func_8005F4D8(void);
void CardReadAndSetMode(s32 arg0) asm("func_8005EB20");
void CardReadAndSetMode(s32 arg0) {
    func_8005F304(arg0);
    while (_card_clear(arg0 & 0xFF) == 0) {}
    func_8005F55C();
    func_8005F2AC();
    _card_load(arg0 & 0xFF);
    func_8005F4D8();
}
