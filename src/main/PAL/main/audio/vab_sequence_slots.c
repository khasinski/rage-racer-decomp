#include "common.h"
#include <stdio.h>
#include "game/audio.h"
#include "game/audio_internal.h"
#include "game/sound.h"
#include "psyq/kernel.h"
#include "psyq/snd.h"


s32 OpenVabSequenceSlot(s32 slot, s32 header, s32 body, s32 seq) {
    s16 vabId;
    /* $5 only matters for this second read. The first one is a cross-block
       value whose only use is SsVabTransBody's second argument, so it lands in
       a1 on its own; this one lives and dies inside one block, has no argument
       use to prefer a1, and local-alloc hands it v0. Retail keeps both in a1. */
    register s32 vabIdAgain asm("$5");

    g_AudioLoadSlot = slot;
    g_VabIds[slot] = SsVabOpenHeadSticky((u_char *)header, -1, g_VabSpuAddress[slot]);
    vabId = g_VabIds[slot];
    if (vabId == -1) {
        printf(g_MsgSeqVabOpenHeadError);
        BiosExit(1);
    }

    g_VabIds[slot] = SsVabTransBody((u_char *)body, vabId);
    vabIdAgain = g_VabIds[slot];
    if (vabIdAgain == -1) {
        printf(g_MsgSeqVabTransBodyError);
        BiosExit(1);
    }

    /* g_SeqHandle is a word (sym.bss size 0x4); the header calls it s16
       because every other reader wants the low half. */
    *(s32 *)&g_SeqHandle = (s16)SsSeqOpen(seq);
    g_SeqVolumeFadeStep = 0;
    g_VabTransferDone = SsVabTransCompleted(0);
    return g_VabTransferDone;
}

s32 CloseAudioSlot(s32 slot) {
    AudioBankRuntime *banks = (AudioBankRuntime *)&g_AudioSlotMask;
    s32 *flagsPtr = &banks->loadedMask;
    s32 bit = 1;
    s32 flags = *flagsPtr;
    s32 ret;
    s16 *ids;

    bit <<= slot;
    if ((bit & flags) == 0) {
        ret = 0;
    } else {
        *flagsPtr = bit ^ flags;
        SsUtSetReverbDepth(0, 0);
        _SsVmInit(0);
        SsSeqCloseWrapper(g_SeqHandle);
        ids = banks->vabIds;
        SsVabClose(ids[slot]);
        ret = 1;
    }
    return ret;
}

void StartVabSlotVoice(s32 voice, s32 unused, s16 vabSlot) {
    VabSlotVoice *slotVoice = &g_VabSlotVoices[voice];

    SsUtKeyOnV((s16)voice, g_VabIds[(s16)vabSlot], slotVoice->tone, 0, 0x3C, 0,
               0, 0);
}

void StopDirectVoice(s32 voice) {
    SsUtKeyOffV((s16)voice);
}
