#include "common.h"

INCLUDE_ASM("asm/nonmatchings/PAL/main", Snd_SetPlayMode);
INCLUDE_ASM("asm/nonmatchings/PAL/main", SsSeqPlay);
INCLUDE_ASM("asm/nonmatchings/PAL/main", _SsSndStop);

void func_80072734(s32 seq, s32 sep);

void SsSeqStop(s32 seq) asm("func_800728A0");
void SsSeqStop(s32 seq) {
    func_80072734((s16)seq, 0);
}

void SsSepStop(s32 seq, s32 sep) asm("func_800728C8");
void SsSepStop(s32 seq, s32 sep) {
    func_80072734((s16)seq, (s16)sep);
}
