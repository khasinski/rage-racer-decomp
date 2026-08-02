#include "common.h"
#include "game/state.h"
#include "game/menu.h"
#include "psyq/gpu.h"

extern s32 g_McSlotUsedMask asm("D_8009B564");
extern s32 g_McSaveHeaders asm("D_8009B568");
extern s32 g_McNoCardTicks asm("D_8009B6E8");
extern s32 g_McErrorTicks asm("D_8009B6F0");
extern s32 D_8009B6F4;
extern s32 g_McActionOk asm("D_8009B6F8");
extern s32 g_McSettleTicks asm("D_8009B6FC");
extern s32 D_8009B700;
extern s32 D_8009B704;
extern s32 g_McSavedLoadPhase asm("D_8009B718");
/* Volatile aliases of three game/menu.h globals, NOT extra objects: this file
 * reads each of them both ways, and only the volatile spelling forces the
 * reload the retail code has at those few sites. A redeclaration cannot add the
 * qualifier (gcc 2.6.3 keeps the first declaration's type), so the alias needs
 * its own identifier. */
extern volatile s32 g_McCardStatusV asm("D_8009B720");
extern volatile s32 g_McMenuSubStateV asm("D_8009B72C");
extern s32 g_McFromLoadMenu asm("D_8009B730");
extern s32 g_McSaveMode asm("D_8009B734");
extern s32 g_McFreeBlocks asm("D_8009B73C");
extern volatile s32 g_McMenuLoadPhaseV asm("D_8009B740");
extern s32 g_McMenuRowCount asm("D_8009B744");
extern s32 g_McFadeStep asm("D_8009B9A0");
extern s32 g_McFadeLevel asm("D_8009B9A4");
extern s32 g_McMenuPage asm("D_80082F50");
extern s32 g_McMenuRowCursor asm("D_80082F54");
extern s32 g_McSlotCursor asm("D_80082F58");
extern s32 g_McActionState asm("D_80082FA4");
extern s32 g_McActionResult asm("D_80082FA8");
extern s32 D_80082FAC;
extern volatile s32 D_80082FAC_v asm("D_80082FAC");
extern s32 D_80082FB0;
extern s32 g_McActionTimer asm("D_80082FB4");
extern s32 g_McActionBusy asm("D_80082FB8");
extern s32 D_80082FBC;
extern s32 D_80082FC0;
extern s32 g_McLastSlot asm("D_80082FC4");
extern s32 D_80082FC8;

void func_8005F65C(void *buf);
s32 func_80060724(s32 off, void *buf);
s32 func_800609E4(s32 off, void *buf);
s32 func_80060C3C(s32 arg0, void *buf);
s32 func_8005EF44(s32 a, s32 b);
s32 func_8005ECE0(s32 a, s32 b);
void func_80027A84(s32 a0, s32 a1, s32 a2, s32 a3);
void func_80027D84(s32 a0);
void func_80060DF0(s32 mask, void *buf);
void func_800611C8(s32 *value, s32 lo, s32 hi);
void func_8006124C(s32 *value);
u16 func_800612CC(void);
u16 func_8006131C(void);
void func_8006136C(s32 level);
void func_8006138C(void);
void GamePlaySoundCue(s32 cue) asm("func_8005D6EC");

void GameUpdateMemoryCardMenu(void) asm("func_80061520");
void GameUpdateMemoryCardMenu(void) {
    s32 fadeBusy = 0;
    s32 two = 2;
    s32 code;
    s32 tmp;
    u16 pad;
    s32 wtmp;
    s32 mst;
    s32 mcue;
    s32 mslot;

    if (g_SceneTimer == two) {
        SetDispMask(1);
    }

    if ((u32) g_SceneTimer >= 6) {
        s32 step = g_McFadeStep;
        if (step < 0) {
            g_McFadeLevel = g_McFadeLevel + g_McFadeStep;
            fadeBusy = 1;
            if (g_McFadeLevel > 0) goto L57;
            g_McFadeStep = 0;
            g_McFadeLevel = 0;

        } else if (step > 0) {
            g_McActionBusy = 1;
            g_McFadeLevel = g_McFadeLevel + g_McFadeStep;
            fadeBusy = 1;
            if (g_McFadeLevel < 0xFF) goto L57;
            g_McFadeStep = 0;
            g_McFadeLevel = 0;
            g_McActionBusy = 0;
            g_SceneId = two;
        } else {
            goto L57;
        }
        fadeBusy = 1;
    }
L57:
    func_8006136C(g_McFadeLevel);

    {
    s32 cur = g_SceneTimer;
    if ((u32) cur < 5) {
        s32 ns = cur + 1;
        g_SceneTimer = ns;
        g_McMenuPhase = 0xF;
        if (ns != 3) goto L_epilogue;
        g_McSlotUsedMask = 0;
        func_8005F65C(&g_McSaveHeaders);
        D_8009B6F4 = -1;
        g_McMenuPhase = 0;
        g_McMenuSelection = ns;
        g_McMenuState = ns;
        g_McActionState = 0;
        g_McActionResult = 0;
        D_80082FAC = 0;
        D_80082FB0 = 0;
        g_McActionTimer = 0;
        g_McActionBusy = 0;
        D_80082FC8 = 1;
        goto L_epilogue;
    }

    /* state >= 5: active-menu entry */
    {
        s32 nx = cur + 1;
        g_SceneTimer = nx;
    }
    }
    if (!(g_McActionBusy == 0)) {
    if (D_80082FC0 == 0) goto L_sw2;

    }
    {
        s32 st = func_8005ECE0(0, 0);
        s32 c;
        s32 sd;
        g_McCardStatus = st;
        switch (st) {
        case 0: {
            s32 b = g_McNoCardTicks;
            g_McNoCardTicks = b + 1;
            if (b >= 6) {
                g_McMenuSelection = 3;
            }
            goto L_sw2;
        }
        case 1: sd = g_McCardStatusV; c = 2; break;
        case 2: sd = g_McCardStatusV; c = 1; break;
        case -1: sd = g_McCardStatusV; c = 0xA; break;
        case -2: sd = g_McCardStatusV; c = 0xB; break;
        case -3: sd = g_McCardStatusV; c = 0x11; break;
        default: sd = g_McCardStatusV; c = 0x11; break;
        }
        g_McNoCardTicks = 0;
        g_McMenuSubState = c;
        g_McMenuSelection = sd;
    }

L_sw2:
    switch (g_McMenuState) {
    case 3: goto L_state3;
    case 1: goto L_state1;
    case 2: goto L_state2;
    case -1: goto L_stateM1;
    case -2: goto L_stateM2;
    case -3: goto L_stateIdle;
    default: goto L_stateIdle; /* 0 */
    }

L_state3:
    {
    u16 lpad = g_PadEdge2;
    g_McMenuPhase = 0xF;
    g_McActionBusy = 0;
    if ((lpad & 0x90) && !fadeBusy) {
        GamePlaySoundCue(3);
        func_8006138C();
    }
    }
    switch (g_McMenuSelection) {
    case 1:
        if (g_McCardStatus == 1) {
            if (D_8009B6F4 != 2) {
                g_McMenuState = 2;
            } else {
                g_McMenuState = g_McCardStatus;
            }
        }
        break;
    case 2:
        g_McMenuState = 2;
        break;
    case -1:
    case -2:
        g_McMenuState = g_McMenuSelection;
        break;
    case 3:
        break;
    case -3:
    default: /* 0 */
        if (g_McCardStatus == -3) {
            s32 r = g_McErrorTicks;
            g_McErrorTicks = r + 1;
            if (r >= 4) {
                g_McMenuState = g_McCardStatus;
            }
        }
    }
    if (g_McMenuState != 3) {
        g_McErrorTicks = 0;
    }
    goto L_epilogue;

L_state1:
    if (g_McMenuPage == 0) {
        goto L_copyselect;
    }
    if (g_McMenuPage == 1) {
        goto L_sw4;
    }
    goto L_copyreset;

L_copyselect:
    {
        s32 *p = &g_McMenuRowCursor;
        g_McMenuPhase = 0;
        func_800611C8(p, 0, g_McMenuRowCount - 1);
        pad = g_PadEdge2;
        if (!((pad & 0x860) == 0)) {
        if (*p < g_McMenuRowCount - 1) {
            GamePlaySoundCue(2);
            g_McMenuPage = 1;
            g_McActionState = 0;
            g_McActionResult = 0;
            g_McSlotCursor = g_McLastSlot;
            g_McSaveMode = *p;
            goto L_sw5;
        }
        if (fadeBusy) goto L_sw5;
        GamePlaySoundCue(2);
        goto L_cxfade;
        }
        if ((pad & 0x90) == 0) goto L_sw5;
        if (fadeBusy) goto L_sw5;
        GamePlaySoundCue(3);
    L_cxfade:
        g_McActionBusy = 0;
        func_8006138C();
    }
    goto L_sw5;

L_sw4:
    switch (g_McActionState) {
    case 0x00: {
        s32 *s0 = &g_McSlotCursor;
        s32 a0;
        s32 nv;
        func_800611C8(s0, 0, 2);
        if (!(g_McSaveMode == 0)) {
        a0 = g_McSlotUsedMask;
        if (!((a0 & 7) == 0)) {
        g_McMenuPhase = 2;
        if ((g_PadEdge2 & 0x860) == 0) goto L_b477;
        if (!(((a0 >> *s0) & 1) == 0)) {
        GamePlaySoundCue(2);
        D_80082FAC = 0;
        nv = 0x1E;
        goto L_b475;
        }
        GamePlaySoundCue(5);
        nv = 0x28;
        goto L_b475;
        }
        g_McMenuPhase = 5;
        if ((g_PadEdge2 & 0x860) == 0) goto L_b477;
        goto L_b433;
        }
        if (g_McFreeBlocks != 0) goto L_b448;
        a0 = g_McSlotUsedMask;
        if (!((a0 & 7) == 0)) {
        g_McMenuPhase = 1;
        if ((g_PadEdge2 & 0x860) == 0) goto L_b477;
        if (!(((a0 >> *s0) & 1) == 0)) {
        GamePlaySoundCue(2);
        D_80082FAC = 0;
        nv = 0xA;
        goto L_b475;
        }
        GamePlaySoundCue(2);
        nv = 0x19;
        goto L_b475;
        }
        g_McMenuPhase = 4;
        if ((g_PadEdge2 & 0x860) == 0) goto L_b439;
    L_b433:
        GamePlaySoundCue(5);
        g_McMenuPage = 0;
        goto L_b477;
    L_b439:
        if ((func_8006131C() & 0xFFFF) == 0) goto L_b477;
        g_McMenuPage = 0;
        goto L_b477;
    L_b448:
        g_McMenuPhase = 1;
        if ((g_PadEdge2 & 0x860) == 0) goto L_b477;
        if (!(((g_McSlotUsedMask >> *s0) & 1) == 0)) {
        GamePlaySoundCue(2);
        D_80082FAC_v = 0;
        nv = 0xA;
        goto L_b475;
        }
        GamePlaySoundCue(2);
        g_McActionTimer = 0x1E;
        nv = 0xB;
    L_b475:
        g_McActionState = nv;
    L_b477:
        if ((func_8006131C() & 0xFFFF) == 0) goto L_sw5;
        g_McMenuPage = 0;
        break;
    }

    case 0x0A: {
        s32 *p = &D_80082FAC;
        s32 hi = g_McSlotCursor << 1;
        s32 lo = D_80082FAC + 9;
        g_McMenuPhase = hi + lo;
        func_8006124C(p);
        if (!(D_80082FAC == 0)) {
        if (!((func_800612CC() & 0xFFFF) == 0)) {
        g_McActionState = 0xB;
        break;
        }
        if (D_80082FAC != 0) goto L_b523;
        }
        if ((func_800612CC() & 0xFFFF) != 0) goto L_b864;
    L_b523:
        if ((func_8006131C() & 0xFFFF) == 0) goto L_sw5;
        goto L_b864;
    }

    case 0x0B:
        g_McMenuPhase = 0xF;
        g_McActionTimer = 0xA;
        g_McActionState = 0xC;
        break;

    case 0x0C: {
        s32 t = g_McActionTimer;
        g_McActionBusy = 1;
        g_McActionTimer = t - 1;
        if (g_McActionTimer != 0) goto L_sw5;
        g_McActionState = 0xD;
        break;
    }

    case 0x0D: {
        s32 a0 = g_McSlotCursor;
        s32 x;
        s32 dp;
        g_McMenuSubState = 5;
        x = func_80060724(a0, (void *)((s32)&g_McSaveHeaders + (a0 << 7)));
        g_McActionResult = x;
        if (!(x == 0)) {
        g_McActionOk = 1;
        x = 6;
        goto L_b577;
        }
        x = 0x10;
        g_McActionOk = 0;
    L_b577:
        g_McMenuSubStateV = x;
        dp = g_McMenuLoadPhaseV;
        g_McActionState = 0xF;
        g_McSavedLoadPhase = dp;
        break;
    }

    case 0x0F:
        g_McActionState = 0x10;
        break;

    case 0x10: {
        s32 nv;
        if (g_McActionResult == 0) goto L_b611;
        {
            s32 r = func_80060C3C(0, &g_McSaveHeaders);
            g_McSlotUsedMask = r;
            if (!(r == 0)) {
            if ((r & 0xFFFF) != 0) goto L_b608;
            nv = 0xE;
            goto L_b606;
            }
            nv = 0xC;
        L_b606:
            g_McMenuSubState = nv;
        }
    L_b608:
        g_McSavedLoadPhase = GameMenuLoadPhase;
    L_b611:
        nv = 0x11;
    L_b613:
        g_McActionState = nv;
        break;
    }

    case 0x11:
        g_McActionTimer = 5;
        g_McActionState = 0x12;
        break;

    case 0x12: {
        s32 t = g_McActionTimer;
        g_McActionTimer = t - 1;
        if (g_McActionTimer != 0) goto L_sw5;
        g_McSettleTicks = 0;
        g_McActionState = 0x13;
        break;
    }

    case 0x13: {
        s32 t;
        if (func_8005ECE0(0, 0) != 1) goto L_sw5;
        t = g_McSettleTicks + 1;
        g_McSettleTicks = t;
        if (t < 4) goto L_sw5;
        g_McActionState = 0x14;
        break;
    }

    case 0x14: {
        s32 x = g_McActionOk;
        if (x != 0) {
            x = 0x12;
        } else {
            x = 0x10;
        }
        g_McMenuPhase = x;
        g_McActionTimer = 0x3C;
        g_McActionBusy = 0;
        g_McActionState = 0x15;
        break;
    }

    case 0x15: {
        s32 t = g_McActionTimer;
        s32 cm1;
        g_McActionTimer = t - 1;
        if (g_McActionTimer != 0) goto L_sw5;
        cm1 = g_McMenuRowCount;
        g_McMenuPage = 0;
        g_McActionState = 0;
        cm1 = cm1 - 1;
        g_McMenuRowCursor = cm1;
        break;
    }

    case 0x19:
        g_McMenuPhase = 4;
        goto L_b850;

    case 0x1E:
        g_McMenuSubState = 7;
        g_McActionTimer = 5;
        g_McActionState = 0x1F;
        break;

    case 0x1F: {
        s32 t = g_McActionTimer;
        g_McActionTimer = t - 1;
        if (g_McActionTimer != 0) goto L_sw5;
        g_McActionState = 0x20;
        break;
    }

    case 0x20: {
        /* These pins are load-bearing: removing any one changes .text. */
        register s32 f asm("$2");
        register s32 one asm("$3");
        f = 0xF;
        one = 1;
        g_McMenuPhase = f;
        g_McActionTimer = f;
        g_McActionBusy = one;
        g_McActionState = 0x21;
        break;
    }

    case 0x21: {
        s32 t = g_McActionTimer;
        g_McActionTimer = t - 1;
        if (g_McActionTimer != 0) goto L_sw5;
        g_McActionState = 0x22;
        break;
    }

    case 0x22: {
        s32 *s0 = &g_McSlotCursor;
        s32 a0 = *s0;
        /* This pin is load-bearing: removing it changes .text. */
        register s32 v1x asm("$3");
        s32 dp;
        g_McActionResult = func_800609E4(a0, (void *)((s32)&g_McSaveHeaders + (a0 << 7)));
        if (!(g_McActionResult == 0)) {
        v1x = *s0;
        g_McActionOk = 1;
        g_McMenuSubState = 8;
        g_McLastSlot = v1x;
        goto L_b762;
        }
        g_McActionOk = 1;
        g_McMenuSubState = 0xF;
    L_b762:
        dp = GameMenuLoadPhase;
        g_McActionTimer = 0x3C;
        g_McActionState = 0x23;
        g_McSavedLoadPhase = dp;
        break;
    }

    case 0x23:
        g_McActionTimer = 5;
        g_McActionState = 0x24;
        break;

    case 0x24: {
        s32 t = g_McActionTimer;
        g_McActionTimer = t - 1;
        if (g_McActionTimer != 0) goto L_sw5;
        g_McSettleTicks = 0;
        g_McActionState = 0x25;
        break;
    }

    case 0x25: {
        s32 t;
        if (func_8005ECE0(0, 0) != 1) goto L_sw5;
        t = g_McSettleTicks + 1;
        g_McSettleTicks = t;
        if (t < 4) goto L_sw5;
        g_McActionState = 0x26;
        break;
    }

    case 0x26: {
        s32 x = g_McActionOk;
        if (x != 0) {
            x = 0x11;
        } else {
            x = 0x10;
        }
        g_McMenuPhase = x;
        g_McActionTimer = 0x3C;
        g_McActionBusy = 0;
        g_McActionState = 0x27;
        break;
    }

    case 0x27: {
        s32 t = g_McActionTimer;
        s32 cm1;
        g_McActionTimer = t - 1;
        if (g_McActionTimer != 0) goto L_sw5;
        cm1 = g_McMenuRowCount;
        g_McMenuPage = 0;
        g_McActionState = 0;
        cm1 = cm1 - 1;
        g_McMenuRowCursor = cm1;
        break;
    }

    case 0x28:
        g_McMenuPhase = 0x14;
    L_b850:
        if (!((func_800612CC() & 0xFFFF) != 0)) {
        if ((func_8006131C() & 0xFFFF) == 0) goto L_sw5;
        }
        g_McMenuPage = 0;
    L_b864:
        g_McActionState = 0;
        break;

    L_copyreset:
        {
            s32 cm1 = g_McMenuRowCount;
            g_McMenuPage = 0;
            g_McSlotCursor = 0;
            g_McActionState = 0;
            g_McActionBusy = 0;
            g_McActionResult = 0;
            D_80082FAC = 0;
            g_McActionTimer = 0;
            cm1 = cm1 - 1;
            g_McMenuRowCursor = cm1;
        }
        break;

    default:
    }
L_sw5:
    switch (g_McMenuSelection) {
    case 3:
        D_8009B6F4 = g_McMenuState;
        /* fallthrough */
    case -2:
    case -1:
    case 2:
        g_McMenuState = g_McMenuSelection;
        break;
    case 1:
        if (D_80082FC0 != 0) {
            D_80082FC0 = 0;
            D_80082FBC = 3;
        }
        break;
    case -3:
    default: /* 0 */
        {
            s32 sd = g_McCardStatus;
            D_80082FC0 = 1;
            if (sd == -3) {
                s32 r = D_80082FBC - 1;
                D_80082FBC = r;
                if (r == 0) {
                    g_McMenuState = sd;
                }
            }
        }
    }
    if (g_McMenuState == 1) {
        goto L_epilogue;
    }
    g_McActionState = 0;
    g_McActionResult = 0;
    D_80082FAC = 0;
    g_McActionBusy = 0;
    goto L_epilogue;

L_state2:
    g_McMenuSubState = 1;
    g_McMenuPhase = 0xF;
    switch (g_McActionState) {
    case 0:
        if ((u32)g_SceneTimer < 0x1F) goto L_sw7;
        wtmp = 1;
        goto L_b1017;
    case 1:
        g_McActionBusy = 0;
        {
            s32 t = D_8009B704 + 1;
            D_8009B704 = t;
            if ((g_PadEdge2 & 0x90) == 0) goto L1003;
            if (t < 0x79) goto L1003;
        }
        D_8009B700 = 0;
        D_8009B704 = 0;
        if (fadeBusy != 0) goto L1003;
        GamePlaySoundCue(3);
        func_8006138C();
    L1003:
        if (g_McCardStatus != 1) goto L_sw7;
        D_8009B700 += 1;
        if (D_8009B700 < 2) goto L_sw7;
        wtmp = 2;
    L_b1017:
        D_8009B700 = 0;
        D_8009B704 = 0;
        g_McActionState = wtmp;
        break;
    case 2:
        g_McActionBusy = 1;
        g_McActionTimer = 5;
        g_McActionState = 3;
        break;
    case 3:
        {
            s32 t = g_McActionTimer;
            g_McActionTimer = t - 1;
            if (g_McActionTimer != 0) goto L_sw7;
        }
        g_McActionState = 5;
        break;
    case 5:
        {
            s32 x = func_80060C3C(1, &g_McSaveHeaders);
            s32 w;
            g_McSlotUsedMask = x;
            if (!(x == 0)) {
            x = x & 7;
            if (x != 0) { x = 2; goto L1061; }
            x = 0xE;
            goto L1061;
            }
            x = 0xC;
        L1061:
            g_McMenuSubStateV = x;
            w = g_McMenuLoadPhaseV;
            g_McActionState = 6;
            g_McSavedLoadPhase = w;
        }
        break;
    case 6:
        g_McActionTimer = 5;
        g_McActionState = 7;
        break;
    case 7:
        {
            s32 t = g_McActionTimer;
            g_McActionTimer = t - 1;
            if (g_McActionTimer != 0) goto L_sw7;
        }
        g_McActionTimer = 5;
        g_McActionBusy = 0;
        g_McActionState = 8;
        break;
    case 8:
        {
            s32 t = g_McActionTimer;
            g_McActionTimer = t - 1;
            if (g_McActionTimer != 0) goto L_sw7;
        }
        g_McActionState = 9;
        break;
    case 9:
        if (g_McMenuSelection != 1) goto L_sw7;
        g_McMenuState = g_McMenuSelection;
        break;
    default:
    }

L_sw7:
    switch (g_McMenuSelection) {
    case 3:
        D_8009B6F4 = g_McMenuState;
        /* fallthrough */
    case -2:
    case -1:
        g_McMenuState = g_McMenuSelection;
        break;
    case 2:
        if (D_80082FC0 != 0) {
            D_80082FC0 = 0;
            D_80082FBC = 3;
        }
        break;
    case 1:
        break;
    case -3:
    case 0:
    default:
        D_80082FC0 = 1;
        if (g_McCardStatus != -3) goto L_sw7tail;
        {
            s32 t = D_80082FBC;
            D_80082FBC = t - 1;
            if (D_80082FBC != 0) goto L_sw7tail;
        }
        g_McMenuState = g_McCardStatus;
    }

L_sw7tail:
    if (g_McMenuState == 2) goto L_epilogue;
    g_McMenuSubState = 1;
    g_McMenuPhase = 0xF;
    g_McActionState = 0;
    goto L_b1678;
L_stateM1:
    g_McMenuSubState = 0xA;
    g_McMenuPhase = 3;
    g_McActionBusy = 0;
    mst = g_McActionState;
    if (mst == 1) goto L_h1219;
    if (mst < 2) {
        if (mst == 0) goto L_h1205;
        goto L_sw8;
    }
    if (mst == 3) goto L_m3;
    goto L_sw8;

L_h1205:
    g_McActionTimer = 5;
    g_McSlotUsedMask = 0;
    func_8005F65C(&g_McSaveHeaders);
    g_McLastSlot = 0;
    g_McActionState = 1;
    goto L_sw8;

L_h1219:
    g_McActionTimer -= 1;
    if (g_McActionTimer != 0) goto L_sw8;
    g_McActionState = 3;
    goto L_sw8;

L_m3:
    {
        s32 mph = g_McMenuPage;
        if (mph == 0) goto L_b1240;
        if (mph == 1) goto L_b1280;
    }
    goto L_sw8;

L_b1240:
    {
        s32 *mp = &g_McMenuRowCursor;
        func_800611C8(mp, 0, g_McMenuRowCount - 1);
        if (func_800612CC() == 0) goto L_b1268;
        if (*mp != g_McMenuRowCount - 1) goto L_b1264;
    }
    if (fadeBusy != 0) goto L_sw8;
    g_McActionState = 0;
    GamePlaySoundCue(2);
    goto L_b1288;

L_b1264:
    GamePlaySoundCue(5);
    goto L_sw8;

L_b1268:
    if ((g_PadEdge2 & 0x90) == 0) goto L_sw8;
    if (fadeBusy != 0) goto L_sw8;
    g_McActionState = 0;
    GamePlaySoundCue(3);
    goto L_b1288;

L_b1280:
    if ((g_PadEdge2 & 0x90) == 0) goto L_sw8;
    if (fadeBusy != 0) goto L_sw8;
    GamePlaySoundCue(3);
    /* fall through */

L_b1288:
    func_8006138C();
    /* fall through */

L_sw8:
    switch (g_McMenuSelection) {
    case 1:
    case 2:
        g_McMenuState = 2;
        /* fall through */
    case -1:
        if (D_80082FC0 == 0) goto L_sw8tail;
        D_80082FC0 = 0;
        D_80082FBC = 3;
        break;
    case -2:
        g_McMenuState = -2;
        break;
    default:
    case -3:
    case 0:
        mslot = g_McCardStatus;
        D_80082FC0 = 1;
        if (mslot != -3) goto L_sw8tail;
        D_80082FBC -= 1;
        if (D_80082FBC != 0) goto L_sw8tail;
        g_McMenuState = mslot;
        /* fall through */
    case 3:
        ;
    }

L_sw8tail:
    if (g_McMenuState == -1) goto L_epilogue;
    g_McActionState = 0;
    goto L_epilogue;
L_stateM2:
    if (!(g_McMenuPage == 0)) {
    if (g_McMenuPage == 1) goto L_sw9;
    goto L_sw10;

    }
    {
        s32 *p = &g_McMenuRowCursor;
        g_McMenuSubState = 0xB;
        g_McMenuPhase = 0;
        func_800611C8(p, 0, g_McMenuRowCount - 1);
        pad = g_PadEdge2;
        if (!((pad & 0x860) == 0)) {
        if (!(*p != 0)) {
        GamePlaySoundCue(2);
        g_McMenuPage = 1;
        D_80082FAC = 0;
        g_McSaveMode = *p;
        goto L_sw10;
        }
        if (!(*p != g_McMenuRowCount - 1)) {
        if (fadeBusy) goto L_sw10;
        GamePlaySoundCue(2);
        goto L_b1420;
        }
        GamePlaySoundCue(5);
        g_McMenuPage = 1;
        g_McSaveMode = *p;
        goto L_sw10;
        }
        if ((pad & 0x90) == 0) goto L_sw10;
        if (fadeBusy) goto L_sw10;
        GamePlaySoundCue(3);
    }

L_b1420:
    g_McActionBusy = 0;
    func_8006138C();
    goto L_sw10;

L_sw9:
    switch (g_McActionState) {
    case 0:
        if (g_McSaveMode == 0) goto L1461;
        g_McMenuPhase = 5;
    L1447:
        { u16 p = func_800612CC(); if (p) goto L1457; }
    L_b1452:
        { u16 p = func_8006131C(); if (p == 0) goto L_sw10; }
    L1457:
        g_McMenuPage = 0;
        goto L_b1606;
    L1461:
        g_McMenuPhase = 6;
        { u16 p = func_800612CC(); if (p == 0) goto L_b1452; }
        g_McActionState = 1;
        break;
    case 1:
        g_McMenuPhase = D_80082FAC + 7;
        func_8006124C(&D_80082FAC);
        if (D_80082FAC == 0) goto L1447;
        { u16 p = func_800612CC(); if (p == 0) goto L1496; }
        g_McActionState = 2;
        break;
    L1496:
        if (D_80082FAC == 0) goto L1447;
        goto L_b1452;
    case 2:
        g_McActionBusy = 1;
        g_McActionTimer = 0x14;
        g_McActionState = 3;
        break;
    case 3:
        g_McActionTimer -= 1;
        if (g_McActionTimer == 0) {
            g_McActionState = 5;
        }
        break;
    case 5:
        g_McActionResult = func_8005EF44(0, 0);
        if (g_McActionResult == 1) {
            g_McActionState = 7;
            g_McActionTimer = 0x3C;
        } else {
            g_McActionState = 0xA;
        }
        break;
    case 7:
        g_McMenuPhase = 0x13;
        g_McActionTimer -= 1;
        if (g_McActionTimer == 0) {
            g_McActionBusy = 0;
            g_McActionState = 8;
        }
        break;
    case 8:
        {
        u16 lpad = g_PadEdge2;
        g_McMenuPhase = 0x13;
        if ((lpad & 0x90) == 0) goto L_sw10;
        }
        g_McActionBusy = 0;
        g_McActionState = 0;
        g_McActionResult = 0;
        D_80082FAC = 0;
        g_McActionTimer = 0;
        if (fadeBusy) goto L_sw10;
        GamePlaySoundCue(3);
        func_8006138C();
        break;
    case 0xA:
        g_McMenuSubState = 0x12;
        g_McMenuPhase = 0x10;
        g_McActionBusy = 0;
        { u16 p = func_800612CC(); if (p) goto L_b1606; }
        { u16 p = func_8006131C(); if (p == 0) goto L_sw10; }
    L_b1606:
        g_McActionState = 0;
        /* fall through to L_sw10 */
    default:
    }

L_sw10:
    switch (g_McMenuSelection) {
    case 1:
    case 2:
        g_McMenuState = 2;
        break;
    case 3:
        D_8009B6F4 = g_McMenuState;
        g_McMenuState = 3;
        break;
    case -1:
        g_McMenuState = -1;
        break;
    case -2:
        if (D_80082FC0 != 0) {
            D_80082FC0 = 0;
            D_80082FBC = 3;
        }
        break;
    case -3:
    default:
        {
            s32 sd = g_McCardStatus;
            D_80082FC0 = 1;
            if (sd == -3) {
                s32 r = D_80082FBC - 1;
                D_80082FBC = r;
                if (r == 0) {
                    g_McMenuState = sd;
                }
            }
        }
    }

    if (g_McMenuState == -2) {
        goto L_epilogue;
    }
    g_McActionState = 0;
    g_McActionBusy = 0;
L_b1678:
    g_McActionResult = 0;
    D_80082FAC = 0;
    goto L_epilogue;
L_stateIdle:
    g_McMenuSubState = 0x11;
    {
    u16 lpad = g_PadEdge2;
    g_McMenuPhase = 0x10;
    if ((lpad & 0x90) && !fadeBusy) {
        GamePlaySoundCue(3);
        g_McActionBusy = 0;
        func_8006138C();
    }
    }
    {
        s32 sel = g_McMenuSelection;
        s32 three = 3;
        if (sel == -3) goto L_epilogue;
        if (sel == three) {
            D_8009B6F4 = g_McMenuState;
        }
        tmp = D_80082FB0;
        g_McMenuState = sel;
        D_80082FB0 = tmp + 1;
        if (D_80082FC0 != 0) {
            D_80082FC0 = 0;
            D_80082FBC = three;
        }
    }

L_epilogue:
    if (D_80082FC8 != 0) {
        func_80027A84(g_McMenuPage, g_McFromLoadMenu, g_McMenuRowCursor, g_McSlotCursor);
        if (g_McMenuPhase != 0) {
            func_80027D84(g_McMenuPhase - 1);
        }
        func_80060DF0(g_McSlotUsedMask, &g_McSaveHeaders);
    }
}
