#include "common.h"
#include "game/render.h"
#include "psyq/cd.h"
#include "game/race.h"

typedef struct FmvDisplayState {
    u8 pad0[0x18];
    u16 field_18;
    u16 field_1A;
    u8 pad1[0xC];
    s32 field_28;
    u16 field_2C;
    u16 field_2E;
    u8 pad2[0x4];
    s32 field_34;
} FmvDisplayState;

void *GetFmvFrame(s32 *arg0) asm("func_8001EDC4");
void MdecUnpackStatus(void *arg0, s32 arg1) asm("func_80064588");
s32 func_8006CFF0(void *arg0);
extern s32 g_FmvFrameWidth asm("D_8009AF6C");
extern s32 g_FmvFrameHeight asm("D_8009AF70");
extern s32 g_FmvStreamEnded asm("D_8009AF74");
extern s32 g_StreamSectorCount asm("D_8019CA1C");
extern u16 g_DispEnv0Y asm("D_8019CE96");
extern volatile u16 g_DispEnv0W asm("D_8019CE98");
extern volatile u16 g_DispEnv0H asm("D_8019CE9A");
extern u16 g_DispEnv1Y asm("D_801C067E");
extern volatile u16 g_DispEnv1W asm("D_801C0680");
extern volatile u16 g_DispEnv1H asm("D_801C0682");
long StGetNext(StRingEventRecord **arg0, StRingEventRecord **arg1);
void ClearImage(void *arg0, u32 arg1, u32 arg2, u32 arg3);
extern char g_MsgFmvDecodeTimeout[] asm("D_80010D34");
s32 func_8006A534(s32 arg0, s32 arg1);
long CdControl(long com, void *param, long result) asm("func_8006A5A4");
s32 VSync(s32 mode);
s32 CdRead2(s32 arg0);
extern u8 D_801E8AFC;
extern u8 *g_ReplayFramesGp asm("D_8009F0A4");
extern u8 *g_ReplayFramesTimeAttack asm("D_8019C7A4");
extern u32 g_ReplayWriteCursor asm("D_801E4BB0");
extern u32 g_ReplayFrameCount asm("D_8019CB6C");
extern u32 g_ReplayBufferWrapped asm("D_8009EC8C");
extern s16 D_8009E782;
extern s32 g_ReplayPlayerModel asm("D_801E4D8C");
extern s32 g_ReplayRivalModel asm("D_801E4BC0");

s32 PresentFmvFrame(s32 *arg0);
s32 PresentFmvFrame(s32 *arg0) {
    void *p;
    s32 retry;
    for (retry = 1; retry != 0; retry--) {
        p = GetFmvFrame(arg0);
        if (p != 0) {
            arg0[2] = (arg0[2] == 0);
            MdecUnpackStatus(p, arg0[arg0[2]]);
            return func_8006CFF0(p);
        }
    }
    return -1;
}

void * GetFmvFrame(s32 *arg0) asm("func_8001EDC4");
void *GetFmvFrame(s32 *arg0) {
    StRingEventRecord *slot[2];
    u16 rect[4];
    s32 count;
    u16 *dst;
    StRingEventRecord *entry;
    s32 w;
    s32 h;
    s32 h32;
    s32 w32;
    register u16 hgt16 asm("$4");
    u16 wid16;
    u16 wdraw;
    s32 half;
    s32 c067e;
    register void *ret asm("$2");

    for (count = 0x800000; count != 0; count--) {
        if (StGetNext(&slot[0], &slot[1]) == 0) {
            goto process;
        }
    }
    return 0;

process:
    entry = slot[1];
    if (*(u32 *)((char *)entry + 8) >= (u32)g_StreamSectorCount) {
        g_FmvStreamEnded = 1;
    }
    w = *(u16 *)((char *)entry + 0x10);
    if ((g_FmvFrameWidth != w) || (g_FmvFrameHeight != *(u16 *)((char *)entry + 0x12))) {
        h = *(u16 *)((char *)entry + 0x12);
        rect[0] = 0;
        rect[1] = 0;
        rect[2] = w * 3 / 2;
        rect[3] = 0x1E0;
        g_FmvFrameWidth = w;
        g_FmvFrameHeight = h;
        ClearImage(rect, 0, 0, 0);
    }

    dst = (u16 *)arg0;
    ret = slot[0];
    h32 = g_FmvFrameHeight;
    w32 = g_FmvFrameWidth;
    half = (0xF0 - h32) / 2;
    dst[0xD] = g_DispEnv0Y + half;
    __asm__ __volatile__("" ::: "memory");
    {
        register u32 wsgn asm("$4");
        u32 wm = (u32)w32 * 3;
        wsgn = wm >> 31;
        wdraw = (u16)((wm + wsgn) >> 1);
    }
    dst[0x12] = wdraw;
    dst[0xE] = wdraw;
    wid16 = g_FmvFrameWidth;
    c067e = g_DispEnv1Y;
    hgt16 = g_FmvFrameHeight;
    dst[0x13] = hgt16;
    dst[0xF] = hgt16;
    dst[0x19] = hgt16;
    c067e += half;
    dst[0x11] = c067e;
    g_DispEnv1W = wid16;
    g_DispEnv0W = wid16;
    g_DispEnv1H = hgt16;
    g_DispEnv0H = hgt16;
    return ret;
}

void WaitFmvDecode(FmvDisplayState *arg0);
void WaitFmvDecode(FmvDisplayState *arg0) {
    volatile s32 timeout = 0x800000;
    s32 one;
    u16 x;

    if (arg0->field_34 == 0) {
        one = 1;
        do {
            timeout = timeout - 1;
            if (timeout == 0) {
                DebugPrintf(g_MsgFmvDecodeTimeout);
                arg0->field_34 = one;
                arg0->field_28 = arg0->field_28 < 1U;
                x = ((FmvDisplayState *)((u8 *)arg0 + (arg0->field_28 << 3)))->field_18;
                arg0->field_2C = x;
                arg0->field_2E = ((FmvDisplayState *)((u8 *)arg0 + (arg0->field_28 << 3)))->field_1A;
            }
        } while (arg0->field_34 == 0);
    }

    arg0->field_34 = 0;
}

void StartStreamRead(void *arg0);
void StartStreamRead(void *arg0) {
    u8 byte;

outer:
    while (func_8006A534(1, 0) == 0) {
    }

    byte = 0x80;
    while (CdControl(0xE, &byte, 0) == 0) {
    }

pollFirst:
    switch (func_8006A534(1, 0)) {
    case 0:
        goto pollFirst;
    case 2:
        VSync(3);
        break;
    }

send:
    while (CdControl(0x15, arg0, 0) == 0) {
    }

pollNext:
    switch (func_8006A534(1, 0)) {
    case 0:
        goto pollNext;
    case 2:
        break;
    default:
        goto send;
    }

    if (CdRead2(0x1E0) == 0) {
        goto outer;
    }
}

void ResetReplayFrameCounts(void);
void ResetReplayFrameCounts(void) {
    g_ReplayFramesGp = &D_801E8AFC;
    g_ReplayFramesTimeAttack = &D_801E8AFC;
}

void ResetReplayWriteCursor(void);
void ResetReplayWriteCursor(void) {
    u32 value;

    value = g_GrandPrixMode;
    g_ReplayWriteCursor = 0;
    if (value != 0) {
        value = 0x5DC;
    } else {
        value = 0xA0A;
    }
    g_ReplayFrameCount = value;
    g_ReplayBufferWrapped = 0;
}

/*
 * Packs a GameRenderPairPoint (billboard/edge pair) from two
 * GameRenderSourcePoint records (srcA=first, srcB=second) into the pair-point
 * output ring g_ReplayFramesGp, keyed by pairIndex>>1. Only even indices hold a pair
 * (odd indices are skipped). Stride is ((n<<1)+n)<<4 == n*0x30 (sizeof pair).
 */
void StoreReplayCarFrame(s32 pairIndex, u8 *srcA, u8 *srcB);
void StoreReplayCarFrame(s32 pairIndex, u8 *srcA, u8 *srcB) {
    GameRenderPairPoint *dst;
    u8 *base;
    GameRenderSourcePoint *src1;
    GameRenderSourcePoint *src2;
    s32 sourceField_AE;
    s32 current;
    s32 odd;
    u32 first;

    current = D_8009E782;
    src2 = (GameRenderSourcePoint *)srcB;
    sourceField_AE = src2->field_AE;
    g_ReplayPlayerModel = current;
    odd = pairIndex & 1;
    g_ReplayRivalModel = sourceField_AE;
    if (odd) {
        return;
    }

    pairIndex >>= 1;
    dst = (GameRenderPairPoint *)((pairIndex * 3) << 4);
    base = g_ReplayFramesGp;
    src1 = (GameRenderSourcePoint *)srcA;
    first = src1->field_0;
    dst = (GameRenderPairPoint *)((u8 *)dst + (s32)base);
    dst->first_0 = first;
    dst->first_2 = src1->field_4;
    dst->first_4 = src1->field_8;
    dst->first_6 = src1->field_60;
    dst->first_8 = src1->field_20;
    dst->first_A = src1->field_24;
    dst->first_C = src1->field_28;
    dst->first_E = src1->field_48;
    dst->first_2C = src1->field_44;
    dst->second_10 = src2->field_0;
    dst->second_12 = src2->field_4;
    dst->second_14 = src2->field_8;
    dst->second_16 = src2->field_60;
    dst->second_18 = src2->field_20;
    dst->second_1A = src2->field_24;
    dst->second_1C = src2->field_28;
    dst->second_1E = src2->field_48;
    dst->second_2E = src2->field_44;
    dst->first_24 = src1->field_30;
    dst->second_28 = src2->field_30;
    dst->field_20 = src1->field_8C;
}

/*
 * Single-point variant of StoreReplayCarFrame: packs a GameRenderSinglePoint from a
 * GameRenderSourcePoint into the single-point output buffer g_ReplayFramesTimeAttack, keyed
 * by pointIndex>>1 (odd indices skipped). Stride ((n<<3)-n)<<2 == n*0x1C
 * (sizeof GameRenderSinglePoint).
 */
void StoreReplayTimeAttackFrame(s32 pointIndex, u8 *srcPtr);
void StoreReplayTimeAttackFrame(s32 pointIndex, u8 *srcPtr) {
    GameRenderSinglePoint *dst;
    u8 *base;
    GameRenderSourcePoint *src;
    u32 first;

    g_ReplayPlayerModel = D_8009E782;
    if (pointIndex % 2) {
        return;
    }

    pointIndex >>= 1;
    dst = (GameRenderSinglePoint *)((pointIndex * 7) << 2);
    base = g_ReplayFramesTimeAttack;
    src = (GameRenderSourcePoint *)srcPtr;
    first = src->field_0;
    dst = (GameRenderSinglePoint *)((u8 *)dst + (s32)base);
    dst->field_0 = first;
    dst->field_2 = src->field_4;
    dst->field_4 = src->field_8;
    dst->field_6 = src->field_60;
    dst->field_8 = src->field_20;
    dst->field_A = src->field_24;
    dst->field_C = src->field_28;
    dst->field_E = src->field_48;
    dst->field_18 = src->field_44;
    dst->field_14 = src->field_30;
    dst->field_10 = src->field_8C;
}
