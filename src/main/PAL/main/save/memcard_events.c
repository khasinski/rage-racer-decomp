#include "common.h"
#include "game/audio.h"
#include "game/cd.h"
#include "game/memcard.h"

extern s16 g_SeqHandle asm("D_801E6D90");
extern s16 g_SeqVolume asm("D_801E6D94");

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
void CardReadAndSetMode(s32 arg0);
void CardReadAndSetMode(s32 arg0) {
    func_8005F304(arg0);
    while (_card_clear(arg0 & 0xFF) == 0) {}
    func_8005F55C();
    func_8005F2AC();
    _card_load(arg0 & 0xFF);
    func_8005F4D8();
}
