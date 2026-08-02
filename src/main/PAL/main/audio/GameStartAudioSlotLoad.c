#include "common.h"
#include "game/audio.h"
#include "psyq/snd.h"

s32 func_800731CC(void);
s32 func_8007317C(s32 arg0);
s32 func_800730BC(s32 arg0, s32 arg1);
s32 func_80072C4C(s32 arg0, s32 arg1, s32 arg2);
s32 func_8005E4EC(s32 slot, s32 header, s32 body, s32 seq);
s32 GameStartVabTransferWithTable(s32 header, s32 body, u16 *table) asm("func_8005BA20");
s32 func_8005E600(s32 arg0);
s32 GameCloseVabOnlyAudioSlot(s32 arg0) asm("func_8005B948");
void func_80063D9C(s32 arg0);
void func_800736E8(void);
void func_80073614(s32 arg0);
void func_80073748(s32 arg0, s32 arg1);
void func_8007865C(s32 arg0);
long SsUtKeyOffV(long voice) asm("func_80078018");
s32 VSync(s32 mode) asm("func_8006DD30");
void func_80072B3C(s32 arg0);
void func_80072260(void);
void func_80071C24(void);

extern s32 g_AudioSlotMask asm("D_801E6C9C");
extern s32 g_SoundCueBank asm("D_801E6CA0");
extern s16 g_VabIds4 asm("D_801E6CB0");
extern s16 g_VabIds5 asm("D_801E6CB2");
extern s16 g_VabIds3 asm("D_801E6CAE");
extern s32 g_ExtraVabLoaded asm("D_801E6CC0");
extern s32 g_AudioLoadSlot asm("D_8009E68C");
extern s32 g_VabTransferDone asm("D_801F17B4");
extern s32 g_VabSpuAddress[] asm("D_800125EC");
extern s32 g_VabSpuAddressExtra asm("D_800125F8");
extern char g_MsgVabOpenHeadError[] asm("D_8001267C");
extern char g_MsgVabTransBodyError[] asm("D_80012694");

s32 GameStartAudioSlotLoad(s32 slot, s32 header, s32 body, s32 table) asm("func_8005B768");
s32 GameStartAudioSlotLoad(s32 slot, s32 header, s32 body, s32 table) {
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 slotReg asm("$16");
    register s32 bodyReg asm("$17");
    s16 *vabIdPtr;
    register s32 currentVabId asm("$5");
    register s32 ret asm("$2");

    /* This barrier is load-bearing: removing it changes .text. */
    asm("" : "=r"(slotReg) : "0"(slot));
    bodyReg = body;

    if (slotReg == 3) {
        ret = GameStartVabTransferWithTable(header, bodyReg, (u16 *)table);
        return (s16)ret;
    }

    {
        /* This pin is load-bearing: removing it changes .text. */
        register s32 seqSlotArg asm("$4") = slotReg;

        if (slotReg == 1) {
        } else {
        if (slotReg != 6) {
            goto loadVab;
        }

        }
        {
            /* This barrier is load-bearing: removing it changes .text. */
            asm volatile("" ::: "$6");
            ret = func_8005E4EC(seqSlotArg, header, bodyReg, table);
            return (s16)ret;
        }
    }

loadVab:
    g_AudioLoadSlot = slotReg;
    ret = func_80072C4C(header, -1, g_VabSpuAddress[slotReg]);
    {
        s16 *vabIdBase = g_VabIds;
        /* This pin is load-bearing: removing it changes .text. */
        register s32 offset asm("$3") = slotReg * 2;
        vabIdPtr = (s16 *)((s32)vabIdBase + offset);
    }
    *vabIdPtr = ret;
    /* This barrier is load-bearing: removing it changes .text. */
    asm volatile("" : "=r"(ret) : "0"(ret));

    {
        s32 fail;

        currentVabId = (s16)ret;
        fail = -1;
        if (currentVabId == fail) {
            GameDebugPrintf(g_MsgVabOpenHeadError);
            func_80063D9C(1);
        }

        ret = func_800730BC(bodyReg, currentVabId);
        *vabIdPtr = ret;
        if ((s16)ret == fail) {
            GameDebugPrintf(g_MsgVabTransBodyError);
            func_80063D9C(1);
        }
    }

    ret = func_8007317C(0);
    g_VabTransferDone = (s16)ret;
    return (s16)ret;
}

s32 GamePollAudioSlotLoad(void) asm("func_8005B89C");
s32 GamePollAudioSlotLoad(void) {
    s32 completed;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 *flagsPtr asm("$4");
    register s32 slot asm("$5");
    s32 one;
    s32 value;
    s32 bit;

    completed = func_8007317C(0);
    g_VabTransferDone = (s16)completed;

    if ((s16)completed != 0) {
        flagsPtr = &g_AudioSlotMask;
        one = 1;
        slot = g_AudioLoadSlot;
        value = *flagsPtr;
        bit = (s16)(one << slot);
        *flagsPtr = bit | value;

        if (slot == 0) {
            g_SoundCueBank = one;
        } else if (slot == one) {
            g_SoundCueBank = slot;
        } else {
            value = 2;
            if ((slot == value) || (slot == 3)) {
                g_SoundCueBank = value;
            }
        }
    }

    return (s16)g_VabTransferDone;
}

s32 GameCloseVabOnlyAudioSlot(s32 slot) {
    s32 *flagsPtr = &g_AudioSlotMask;
    s32 bit = 1;
    s32 flags = *flagsPtr;
    s32 zeroArg = 0;
    s32 ret;
    s16 *ids;

    bit <<= slot;

    if (bit & flags) {
    } else {

    ret = 0;
    goto done;

    }
    *flagsPtr = bit ^ flags;
    func_80073748(zeroArg, 0);
    func_8007865C(0);
    /* g_VabIds sits 0xC bytes past the slot mask; deriving it from flagsPtr
       (rather than naming the symbol) is what the retail code does. */
    ids = (s16 *)((s32)flagsPtr + 0xC);
    func_80072B3C(ids[slot]);
    ret = 1;
done:
    return ret;
}

s32 GameCloseLoadedAudioSlots(void) asm("func_8005B9CC");
s32 GameCloseLoadedAudioSlots(void) {
    func_800731CC();
    if (func_8005E600(1) == 0) {
        return 0;
    }
    if (GameCloseVabOnlyAudioSlot(2) == 0) {
        return 0;
    }
    if (GameCloseVabOnlyAudioSlot(3) == 0) {
        return 0;
    }
}

s32 GameStartVabTransferWithTable(s32 header, s32 body, u16 *table) {
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 ret asm("$2");
    register s32 currentVabId asm("$5");
    s16 *vabIdPtr;
    s32 fail;
    s32 tableReg = (s32)table;

    g_AudioLoadSlot = 3;
    ret = func_80072C4C(header, -1, g_VabSpuAddressExtra);
    vabIdPtr = &g_VabIds3;
    *vabIdPtr = ret;
    /* This barrier is load-bearing: removing it changes .text. */
    asm volatile("" : "=r"(ret) : "0"(ret));

    currentVabId = (s16)ret;
    fail = -1;
    if (currentVabId == fail) {
        GameDebugPrintf(g_MsgVabOpenHeadError);
        func_80063D9C(1);
    }

    ret = func_800730BC(body, currentVabId);
    *vabIdPtr = ret;
    if ((s16)ret == fail) {
        GameDebugPrintf(g_MsgVabTransBodyError);
        func_80063D9C(1);
    }

    if (tableReg != 0) {
        GameLoadAudioParameterTable((u16 *)tableReg);
    }

    g_ExtraVabLoaded = 1;
    ret = func_8007317C(0);
    g_VabTransferDone = (s16)ret;
    return g_VabTransferDone;
}

s32 GameLoadExtraVabSlotWithTable(s32 header, s32 body, s32 table) asm("func_8005BB1C");
s32 GameLoadExtraVabSlotWithTable(s32 header, s32 body, s32 table) {
    s32 bodyReg = body;
    s32 tableReg = table;
    s16 *vabIdPtr;
    /* These pins are load-bearing: removing any one changes .text. */
    register s32 currentVabId asm("$5");
    s32 fail;
    register s32 ret asm("$2");
    s32 flags;

    ret = func_80072C4C(header, -1, 0x6A000);
    vabIdPtr = &g_VabIds3;
    *vabIdPtr = ret;

    currentVabId = (s16)ret;
    fail = -1;
    if (currentVabId == fail) {
        GameDebugPrintf(g_MsgVabOpenHeadError);
        func_80063D9C(1);
    }

    ret = func_800730BC(bodyReg, currentVabId);
    *vabIdPtr = ret;
    if ((s16)ret == fail) {
        GameDebugPrintf(g_MsgVabTransBodyError);
        func_80063D9C(1);
    }

    func_8007317C(1);
    if (tableReg != 0) {
        GameLoadAudioParameterTable((u16 *)tableReg);
    }

    flags = g_AudioSlotMask;
    g_ExtraVabLoaded = 1;
    g_AudioSlotMask = flags | 0x20;
    return 0;
}

void GameCloseExtraVabSlot(void) asm("func_8005BC14");
void GameCloseExtraVabSlot(void) {
    s32 liveSlot;
    s32 *flagsPtr = &g_AudioSlotMask;
    s32 flags = *flagsPtr;
    s32 newFlags;

    if (flags & 0x20) {
        newFlags = flags ^ 0x20;
        *flagsPtr = newFlags;
        func_800736E8();
        func_80073748(0x28, 0x28);
        SsUtKeyOffV((s16)liveSlot);
        func_80072B3C(g_VabIds5);
    }
}

void GameShutdownSoundSystem(void) asm("func_8005BC80");
void GameShutdownSoundSystem(void) {
    s32 i;
    s32 *flag = &g_AudioSlotMask;

    if (*flag != 0) {
        *flag = 0;
        func_800736E8();
        func_80073614(0);
        func_80073748(0, 0);
        i = 0;
        while (i < 24) {
            SsUtKeyOffV((s16)i);
            i++;
        }
        VSync(2);
        func_80072B3C(g_VabIds4);
        func_80072B3C(g_VabIds5);
        func_80072260();
        func_80071C24();
    }
}
