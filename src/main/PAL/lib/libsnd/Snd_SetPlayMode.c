#include "psyq/snd.h"


INCLUDE_ASM("asm/PAL/main/nonmatchings/lib/libsnd/Snd_SetPlayMode", Snd_SetPlayMode);

void func_80076C58(s32 seq_sep, u16 left, u16 right, s32 arg3);
void func_80076DCC(s32 seq_sep, s32 value, s32 ticks);

void SsSeqPlay(s32 seq, s32 play_mode, s32 loop_count) {
    Snd_SetPlayMode((s16)seq, 0, (u8)play_mode, (s16)loop_count);
}

void SsSepPlay(s32 seq, s32 sep, s32 play_mode, s32 loop_count) {
    Snd_SetPlayMode((s16)seq, (s16)sep, (u8)play_mode, (s16)loop_count);
}

void _SsSndSetVol(s32 seq, s32 sep, s32 left, s32 right) {
    func_80076C58((s16)(seq | (sep << 8)), (u16)left, (u16)right, 0);
}

void SsSeqSetVol(s32 seq, s32 left, s32 right) {
    func_80076C58((s16)seq, (u16)left, (u16)right, 0);
}

void SsSepSetVol(s32 seq, s32 sep, s32 left, s32 right) {
    func_80076C58((s16)(seq | (sep << 8)), (u16)left, (u16)right, 0);
}

void SsSepSetCrescendo(s32 seq, s32 sep, s32 value, s32 ticks) {
    func_80076DCC((s16)(seq | (sep << 8)), value, ticks);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/lib/libsnd/Snd_SetPlayMode", _SsSndStop);
