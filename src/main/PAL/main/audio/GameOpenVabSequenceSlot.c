#include "common.h"
#include "psyq/snd.h"

extern s16 g_VabSlotVoiceTone[] asm("D_8009B510");
extern s32 g_VabSpuAddress[] asm("D_800125EC");
extern s32 g_AudioLoadSlot asm("D_8009E68C");
extern s32 g_AudioSlotMask asm("D_801E6C9C");
extern s16 g_SeqHandle asm("D_801E6D90");
extern s32 g_SeqVolumeFadeStep asm("D_801E6D9C");
extern s32 g_VabTransferDone asm("D_801F17B4");
extern char g_MsgSeqVabOpenHeadError[] asm("D_80012788");
extern char g_MsgSeqVabTransBodyError[] asm("D_800127A0");

void func_80063D9C(s32 arg0);
s32 func_8006F004(s32 arg0);
s32 func_8007317C(s32 arg0);
s32 func_800730BC(s32 arg0, s32 arg1);
s32 func_80072C4C(s32 arg0, s32 arg1, s32 arg2);
void func_80073748(s32 arg0, s32 arg1);
void func_8007865C(s32 arg0);
void func_80071AC4(s32 arg0);
void func_80072B3C(s32 arg0);

extern s16 g_VabIds[] asm("D_801E6CA8");

s32 GameOpenVabSequenceSlot(s32 slot, s32 header, s32 body, s32 seq) asm("func_8005E4EC");
s32 GameOpenVabSequenceSlot(s32 slot, s32 header, s32 body, s32 seq) {
    s32 slotReg = slot;
    s32 bodyReg = body;
    s32 seqReg = seq;
    s16 *vabIdPtr;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 currentVabId asm("$5");
    s32 fail;
    register s32 ret asm("$2");

    g_AudioLoadSlot = slotReg;
    ret = func_80072C4C(header, -1, g_VabSpuAddress[slotReg]);
    {
        s16 *vabIdBase = g_VabIds;
        slotReg *= 2;
        vabIdPtr = (s16 *)((s32)vabIdBase + slotReg);
    }
    *vabIdPtr = ret;
    /* This barrier is load-bearing: removing it changes .text. */
    asm volatile("" : "=r"(ret) : "0"(ret));

    currentVabId = (s16)ret;
    fail = -1;
    if (currentVabId == fail) {
        GameDebugPrintf(g_MsgSeqVabOpenHeadError);
        func_80063D9C(1);
    }

    ret = func_800730BC(bodyReg, currentVabId);
    *vabIdPtr = ret;
    currentVabId = (s16)ret;
    if (currentVabId == fail) {
        GameDebugPrintf(g_MsgSeqVabTransBodyError);
        func_80063D9C(1);
    }

    *(s32 *)&g_SeqHandle = (s16)func_8006F004(seqReg);
    g_SeqVolumeFadeStep = 0;
    ret = func_8007317C(0);
    g_VabTransferDone = (s16)ret;
    return g_VabTransferDone;
}

s32 GameCloseAudioSlot(s32 slot) asm("func_8005E600");
s32 GameCloseAudioSlot(s32 slot) {
    s32 *flagsPtr = &g_AudioSlotMask;
    s32 bit = 1;
    s32 flags = *flagsPtr;
    s32 zeroArg = 0;
    s32 ret;
    s16 *ids;

    bit <<= slot;
    if (bit & flags) {
        goto loaded;
    }
    ret = 0;
    goto done;

loaded:
    *flagsPtr = bit ^ flags;
    func_80073748(zeroArg, 0);
    func_8007865C(0);
    func_80071AC4(g_SeqHandle);
    /* g_VabIds sits 0xC bytes past the slot mask; deriving it from flagsPtr
       (rather than naming the symbol) is what the retail code does. */
    ids = (s16 *)((s32)flagsPtr + 0xC);
    func_80072B3C(ids[slot]);
    ret = 1;
done:
    return ret;
}

void GameStartVabSlotVoice(s32 voice, s32 unused, s16 vabSlot) asm("func_8005E694");
void GameStartVabSlotVoice(s32 voice, s32 unused, s16 vabSlot) {
    s32 voiceOffset = voice * 4;
    s32 vabOffset = (s16)vabSlot * 2;

    SsUtKeyOnV((s16)voice, *(s16 *)((s32)g_VabIds + vabOffset), *(s16 *)((s32)g_VabSlotVoiceTone + voiceOffset), 0, 0x3C, 0, 0, 0);
}

long SsUtKeyOffV(long voice) asm("func_80078018");

void GameStopDirectVoice(s16 voice) asm("func_8005E6F4");
void GameStopDirectVoice(s16 voice) {
    SsUtKeyOffV(voice);
}
