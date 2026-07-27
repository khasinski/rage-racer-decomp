#include "common.h"

/* MIDI Control Change 6 (Data Entry MSB): applies the pending RPN/NRPN to the
 * channel's VAB program. Declaration only - the emitted symbol is unchanged. */
void SsSeqApplyDataEntry(void *seq, s32 channel) asm("func_8007010C");

/*
 * SsSeqApplyDataEntry - a libsnd internal static, named descriptively in the
 * style src/main/PAL/lib/libsnd/ already uses; there is no public Ss* symbol.
 * Reached from SsSeqDispatchControlChange's `case 6:`. Applies the pending
 * RPN (unk29 == 2) or NRPN (unk2a == 2) to the channel's VAB program by
 * rewriting the VagAtr of every tone. See docs/names.md 17.
 */
INCLUDE_ASM("asm/PAL/main/nonmatchings/main/sdk/SsSeqApplyDataEntry", func_8007010C);

