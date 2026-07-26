#include "common.h"
#include "game/state.h"
#include "game/menu.h"
#include "psyq/gpu.h"

extern s32 D_8009B564;
extern s32 D_8009B568;
extern s32 D_8009B6E8;
extern s32 D_8009B6F0;
extern s32 D_8009B6F4;
extern s32 D_8009B6F8;
extern s32 D_8009B6FC;
extern s32 D_8009B700;
extern s32 D_8009B704;
extern s32 D_8009B718;
extern volatile s32 D_8009B720_v asm("D_8009B720");
extern volatile s32 D_8009B72C_v asm("D_8009B72C");
extern s32 D_8009B730;
extern s32 D_8009B734;
extern s32 D_8009B73C;
extern volatile s32 D_8009B740_v asm("D_8009B740");
extern s32 D_8009B744;
extern s32 D_8009B9A0;
extern s32 D_8009B9A4;
extern s32 D_80082F50;
extern s32 D_80082F54;
extern s32 D_80082F58;
extern s32 D_80082FA4;
extern s32 D_80082FA8;
extern s32 D_80082FAC;
extern volatile s32 D_80082FAC_v asm("D_80082FAC");
extern s32 D_80082FB0;
extern s32 D_80082FB4;
extern s32 D_80082FB8;
extern s32 D_80082FBC;
extern s32 D_80082FC0;
extern s32 D_80082FC4;
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
        s32 step = D_8009B9A0;
        if (step < 0) {
            D_8009B9A4 = D_8009B9A4 + D_8009B9A0;
            fadeBusy = 1;
            if (D_8009B9A4 > 0) goto L57;
            D_8009B9A0 = 0;
            D_8009B9A4 = 0;
            goto L56;
        } else if (step > 0) {
            D_80082FB8 = 1;
            D_8009B9A4 = D_8009B9A4 + D_8009B9A0;
            fadeBusy = 1;
            if (D_8009B9A4 < 0xFF) goto L57;
            D_8009B9A0 = 0;
            D_8009B9A4 = 0;
            D_80082FB8 = 0;
            g_SceneId = two;
        } else {
            goto L57;
        }
    L56:
        fadeBusy = 1;
    }
L57:
    func_8006136C(D_8009B9A4);

    {
    register s32 cur asm("$4") = g_SceneTimer;
    if ((u32) cur < 5) {
        s32 ns = cur + 1;
        g_SceneTimer = ns;
        g_McMenuPhase = 0xF;
        if (ns != 3) goto L_epilogue;
        D_8009B564 = 0;
        func_8005F65C(&D_8009B568);
        D_8009B6F4 = -1;
        g_McMenuPhase = 0;
        g_McMenuSelection = ns;
        g_McMenuState = ns;
        D_80082FA4 = 0;
        D_80082FA8 = 0;
        D_80082FAC = 0;
        D_80082FB0 = 0;
        D_80082FB4 = 0;
        D_80082FB8 = 0;
        D_80082FC8 = 1;
        goto L_epilogue;
    }

    /* state >= 5: active-menu entry */
    {
        register s32 nx asm("$2") = cur + 1;
        g_SceneTimer = nx;
    }
    }
    if (D_80082FB8 == 0) goto L_sw1;
    if (D_80082FC0 == 0) goto L_sw2;

L_sw1:
    {
        s32 st = func_8005ECE0(0, 0);
        s32 c;
        s32 sd;
        g_McMenuSlotData = st;
        switch (st) {
        case 0: {
            s32 b = D_8009B6E8;
            D_8009B6E8 = b + 1;
            if (b >= 6) {
                g_McMenuSelection = 3;
            }
            goto L_sw2;
        }
        case 1: sd = D_8009B720_v; c = 2; break;
        case 2: sd = D_8009B720_v; c = 1; break;
        case -1: sd = D_8009B720_v; c = 0xA; break;
        case -2: sd = D_8009B720_v; c = 0xB; break;
        case -3: sd = D_8009B720_v; c = 0x11; break;
        default: sd = D_8009B720_v; c = 0x11; break;
        }
        D_8009B6E8 = 0;
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
    D_80082FB8 = 0;
    if ((lpad & 0x90) && !fadeBusy) {
        GamePlaySoundCue(3);
        func_8006138C();
    }
    }
    switch (g_McMenuSelection) {
    case 1:
        if (g_McMenuSlotData == 1) {
            if (D_8009B6F4 != 2) {
                g_McMenuState = 2;
            } else {
                g_McMenuState = g_McMenuSlotData;
            }
        }
        goto L254;
    case 2:
        g_McMenuState = 2;
        goto L254;
    case -1:
    case -2:
        g_McMenuState = g_McMenuSelection;
        goto L254;
    case 3:
        goto L254;
    case -3:
    default: /* 0 */
        if (g_McMenuSlotData == -3) {
            s32 r = D_8009B6F0;
            D_8009B6F0 = r + 1;
            if (r >= 4) {
                g_McMenuState = g_McMenuSlotData;
            }
        }
        goto L254;
    }
L254:
    if (g_McMenuState != 3) {
        D_8009B6F0 = 0;
    }
    goto L_epilogue;

L_state1:
    if (D_80082F50 == 0) {
        goto L_copyselect;
    }
    if (D_80082F50 == 1) {
        goto L_sw4;
    }
    goto L_copyreset;

L_copyselect:
    {
        s32 *p = &D_80082F54;
        g_McMenuPhase = 0;
        func_800611C8(p, 0, D_8009B744 - 1);
        pad = g_PadEdge2;
        if ((pad & 0x860) == 0) goto L_cx3;
        if (*p < D_8009B744 - 1) {
            GamePlaySoundCue(2);
            D_80082F50 = 1;
            D_80082FA4 = 0;
            D_80082FA8 = 0;
            D_80082F58 = D_80082FC4;
            D_8009B734 = *p;
            goto L_sw5;
        }
        if (fadeBusy) goto L_sw5;
        GamePlaySoundCue(2);
        goto L_cxfade;
    L_cx3:
        if ((pad & 0x90) == 0) goto L_sw5;
        if (fadeBusy) goto L_sw5;
        GamePlaySoundCue(3);
    L_cxfade:
        D_80082FB8 = 0;
        func_8006138C();
    }
    goto L_sw5;

L_sw4:
    switch (D_80082FA4) {
    case 0x00: {
        s32 *s0 = &D_80082F58;
        s32 a0;
        s32 nv;
        func_800611C8(s0, 0, 2);
        if (D_8009B734 == 0) goto L_b391;
        a0 = D_8009B564;
        if ((a0 & 7) == 0) goto L_b381;
        g_McMenuPhase = 2;
        if ((g_PadEdge2 & 0x860) == 0) goto L_b477;
        if (((a0 >> *s0) & 1) == 0) goto L_b377;
        GamePlaySoundCue(2);
        D_80082FAC = 0;
        nv = 0x1E;
        goto L_b475;
    L_b377:
        GamePlaySoundCue(5);
        nv = 0x28;
        goto L_b475;
    L_b381:
        g_McMenuPhase = 5;
        if ((g_PadEdge2 & 0x860) == 0) goto L_b477;
        goto L_b433;
    L_b391:
        if (D_8009B73C != 0) goto L_b448;
        a0 = D_8009B564;
        if ((a0 & 7) == 0) goto L_b425;
        g_McMenuPhase = 1;
        if ((g_PadEdge2 & 0x860) == 0) goto L_b477;
        if (((a0 >> *s0) & 1) == 0) goto L_b421;
        GamePlaySoundCue(2);
        D_80082FAC = 0;
        nv = 0xA;
        goto L_b475;
    L_b421:
        GamePlaySoundCue(2);
        nv = 0x19;
        goto L_b475;
    L_b425:
        g_McMenuPhase = 4;
        if ((g_PadEdge2 & 0x860) == 0) goto L_b439;
    L_b433:
        GamePlaySoundCue(5);
        D_80082F50 = 0;
        goto L_b477;
    L_b439:
        if ((func_8006131C() & 0xFFFF) == 0) goto L_b477;
        D_80082F50 = 0;
        goto L_b477;
    L_b448:
        g_McMenuPhase = 1;
        if ((g_PadEdge2 & 0x860) == 0) goto L_b477;
        if (((D_8009B564 >> *s0) & 1) == 0) goto L_b469;
        GamePlaySoundCue(2);
        D_80082FAC_v = 0;
        nv = 0xA;
        goto L_b475;
    L_b469:
        GamePlaySoundCue(2);
        D_80082FB4 = 0x1E;
        nv = 0xB;
    L_b475:
        D_80082FA4 = nv;
    L_b477:
        if ((func_8006131C() & 0xFFFF) == 0) goto L_sw5;
        D_80082F50 = 0;
        goto L_sw5;
    }

    case 0x0A: {
        s32 *p = &D_80082FAC;
        s32 hi = D_80082F58 << 1;
        s32 lo = D_80082FAC + 9;
        g_McMenuPhase = hi + lo;
        func_8006124C(p);
        if (D_80082FAC == 0) goto L_b518;
        if ((func_800612CC() & 0xFFFF) == 0) goto L_b513;
        D_80082FA4 = 0xB;
        goto L_sw5;
    L_b513:
        if (D_80082FAC != 0) goto L_b523;
    L_b518:
        if ((func_800612CC() & 0xFFFF) != 0) goto L_b864;
    L_b523:
        if ((func_8006131C() & 0xFFFF) == 0) goto L_sw5;
        goto L_b864;
    }

    case 0x0B:
        g_McMenuPhase = 0xF;
        D_80082FB4 = 0xA;
        D_80082FA4 = 0xC;
        goto L_sw5;

    case 0x0C: {
        s32 t = D_80082FB4;
        D_80082FB8 = 1;
        D_80082FB4 = t - 1;
        if (D_80082FB4 != 0) goto L_sw5;
        D_80082FA4 = 0xD;
        goto L_sw5;
    }

    case 0x0D: {
        s32 a0 = D_80082F58;
        s32 x;
        s32 dp;
        g_McMenuSubState = 5;
        x = func_80060724(a0, (void *)((s32)&D_8009B568 + (a0 << 7)));
        D_80082FA8 = x;
        if (x == 0) goto L_b574;
        D_8009B6F8 = 1;
        x = 6;
        goto L_b577;
    L_b574:
        x = 0x10;
        D_8009B6F8 = 0;
    L_b577:
        D_8009B72C_v = x;
        dp = D_8009B740_v;
        D_80082FA4 = 0xF;
        D_8009B718 = dp;
        goto L_sw5;
    }

    case 0x0F:
        D_80082FA4 = 0x10;
        goto L_sw5;

    case 0x10: {
        s32 nv;
        if (D_80082FA8 == 0) goto L_b611;
        {
            s32 r = func_80060C3C(0, &D_8009B568);
            D_8009B564 = r;
            if (r == 0) goto L_b605;
            if ((r & 0xFFFF) != 0) goto L_b608;
            nv = 0xE;
            goto L_b606;
        L_b605:
            nv = 0xC;
        L_b606:
            g_McMenuSubState = nv;
        }
    L_b608:
        D_8009B718 = GameMenuLoadPhase;
    L_b611:
        nv = 0x11;
    L_b613:
        D_80082FA4 = nv;
        goto L_sw5;
    }

    case 0x11:
        D_80082FB4 = 5;
        D_80082FA4 = 0x12;
        goto L_sw5;

    case 0x12: {
        s32 t = D_80082FB4;
        D_80082FB4 = t - 1;
        if (D_80082FB4 != 0) goto L_sw5;
        D_8009B6FC = 0;
        D_80082FA4 = 0x13;
        goto L_sw5;
    }

    case 0x13: {
        s32 t;
        if (func_8005ECE0(0, 0) != 1) goto L_sw5;
        t = D_8009B6FC + 1;
        D_8009B6FC = t;
        if (t < 4) goto L_sw5;
        D_80082FA4 = 0x14;
        goto L_sw5;
    }

    case 0x14: {
        s32 x = D_8009B6F8;
        if (x != 0) {
            x = 0x12;
        } else {
            x = 0x10;
        }
        g_McMenuPhase = x;
        D_80082FB4 = 0x3C;
        D_80082FB8 = 0;
        D_80082FA4 = 0x15;
        goto L_sw5;
    }

    case 0x15: {
        s32 t = D_80082FB4;
        s32 cm1;
        D_80082FB4 = t - 1;
        if (D_80082FB4 != 0) goto L_sw5;
        cm1 = D_8009B744;
        D_80082F50 = 0;
        D_80082FA4 = 0;
        cm1 = cm1 - 1;
        D_80082F54 = cm1;
        goto L_sw5;
    }

    case 0x19:
        g_McMenuPhase = 4;
        goto L_b850;

    case 0x1E:
        g_McMenuSubState = 7;
        D_80082FB4 = 5;
        D_80082FA4 = 0x1F;
        goto L_sw5;

    case 0x1F: {
        s32 t = D_80082FB4;
        D_80082FB4 = t - 1;
        if (D_80082FB4 != 0) goto L_sw5;
        D_80082FA4 = 0x20;
        goto L_sw5;
    }

    case 0x20: {
        register s32 f asm("$2");
        register s32 one asm("$3");
        f = 0xF;
        one = 1;
        g_McMenuPhase = f;
        D_80082FB4 = f;
        D_80082FB8 = one;
        D_80082FA4 = 0x21;
        goto L_sw5;
    }

    case 0x21: {
        s32 t = D_80082FB4;
        D_80082FB4 = t - 1;
        if (D_80082FB4 != 0) goto L_sw5;
        D_80082FA4 = 0x22;
        goto L_sw5;
    }

    case 0x22: {
        s32 *s0 = &D_80082F58;
        s32 a0 = *s0;
        register s32 v1x asm("$3");
        s32 dp;
        D_80082FA8 = func_800609E4(a0, (void *)((s32)&D_8009B568 + (a0 << 7)));
        if (D_80082FA8 == 0) goto L_b757;
        v1x = *s0;
        D_8009B6F8 = 1;
        g_McMenuSubState = 8;
        D_80082FC4 = v1x;
        goto L_b762;
    L_b757:
        D_8009B6F8 = 1;
        g_McMenuSubState = 0xF;
    L_b762:
        dp = GameMenuLoadPhase;
        D_80082FB4 = 0x3C;
        D_80082FA4 = 0x23;
        D_8009B718 = dp;
        goto L_sw5;
    }

    case 0x23:
        D_80082FB4 = 5;
        D_80082FA4 = 0x24;
        goto L_sw5;

    case 0x24: {
        s32 t = D_80082FB4;
        D_80082FB4 = t - 1;
        if (D_80082FB4 != 0) goto L_sw5;
        D_8009B6FC = 0;
        D_80082FA4 = 0x25;
        goto L_sw5;
    }

    case 0x25: {
        s32 t;
        if (func_8005ECE0(0, 0) != 1) goto L_sw5;
        t = D_8009B6FC + 1;
        D_8009B6FC = t;
        if (t < 4) goto L_sw5;
        D_80082FA4 = 0x26;
        goto L_sw5;
    }

    case 0x26: {
        s32 x = D_8009B6F8;
        if (x != 0) {
            x = 0x11;
        } else {
            x = 0x10;
        }
        g_McMenuPhase = x;
        D_80082FB4 = 0x3C;
        D_80082FB8 = 0;
        D_80082FA4 = 0x27;
        goto L_sw5;
    }

    case 0x27: {
        s32 t = D_80082FB4;
        s32 cm1;
        D_80082FB4 = t - 1;
        if (D_80082FB4 != 0) goto L_sw5;
        cm1 = D_8009B744;
        D_80082F50 = 0;
        D_80082FA4 = 0;
        cm1 = cm1 - 1;
        D_80082F54 = cm1;
        goto L_sw5;
    }

    case 0x28:
        g_McMenuPhase = 0x14;
    L_b850:
        if ((func_800612CC() & 0xFFFF) != 0) goto L_b862;
        if ((func_8006131C() & 0xFFFF) == 0) goto L_sw5;
    L_b862:
        D_80082F50 = 0;
    L_b864:
        D_80082FA4 = 0;
        goto L_sw5;

    L_copyreset:
        {
            s32 cm1 = D_8009B744;
            D_80082F50 = 0;
            D_80082F58 = 0;
            D_80082FA4 = 0;
            D_80082FB8 = 0;
            D_80082FA8 = 0;
            D_80082FAC = 0;
            D_80082FB4 = 0;
            cm1 = cm1 - 1;
            D_80082F54 = cm1;
        }
        goto L_sw5;

    default:
        goto L_sw5;
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
        goto L_sw5tail;
    case 1:
        if (D_80082FC0 != 0) {
            D_80082FC0 = 0;
            D_80082FBC = 3;
        }
        goto L_sw5tail;
    case -3:
    default: /* 0 */
        {
            s32 sd = g_McMenuSlotData;
            D_80082FC0 = 1;
            if (sd == -3) {
                s32 r = D_80082FBC - 1;
                D_80082FBC = r;
                if (r == 0) {
                    g_McMenuState = sd;
                }
            }
        }
        goto L_sw5tail;
    }
L_sw5tail:
    if (g_McMenuState == 1) {
        goto L_epilogue;
    }
    D_80082FA4 = 0;
    D_80082FA8 = 0;
    D_80082FAC = 0;
    D_80082FB8 = 0;
    goto L_epilogue;

L_state2:
    g_McMenuSubState = 1;
    g_McMenuPhase = 0xF;
    switch (D_80082FA4) {
    case 0:
        if ((u32)g_SceneTimer < 0x1F) goto L_sw7;
        wtmp = 1;
        goto L_b1017;
    case 1:
        D_80082FB8 = 0;
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
        if (g_McMenuSlotData != 1) goto L_sw7;
        D_8009B700 += 1;
        if (D_8009B700 < 2) goto L_sw7;
        wtmp = 2;
    L_b1017:
        D_8009B700 = 0;
        D_8009B704 = 0;
        D_80082FA4 = wtmp;
        goto L_sw7;
    case 2:
        D_80082FB8 = 1;
        D_80082FB4 = 5;
        D_80082FA4 = 3;
        goto L_sw7;
    case 3:
        {
            s32 t = D_80082FB4;
            D_80082FB4 = t - 1;
            if (D_80082FB4 != 0) goto L_sw7;
        }
        D_80082FA4 = 5;
        goto L_sw7;
    case 5:
        {
            s32 x = func_80060C3C(1, &D_8009B568);
            s32 w;
            D_8009B564 = x;
            if (x == 0) goto L1060;
            x = x & 7;
            if (x != 0) { x = 2; goto L1061; }
            x = 0xE;
            goto L1061;
        L1060:
            x = 0xC;
        L1061:
            D_8009B72C_v = x;
            w = D_8009B740_v;
            D_80082FA4 = 6;
            D_8009B718 = w;
        }
        goto L_sw7;
    case 6:
        D_80082FB4 = 5;
        D_80082FA4 = 7;
        goto L_sw7;
    case 7:
        {
            s32 t = D_80082FB4;
            D_80082FB4 = t - 1;
            if (D_80082FB4 != 0) goto L_sw7;
        }
        D_80082FB4 = 5;
        D_80082FB8 = 0;
        D_80082FA4 = 8;
        goto L_sw7;
    case 8:
        {
            s32 t = D_80082FB4;
            D_80082FB4 = t - 1;
            if (D_80082FB4 != 0) goto L_sw7;
        }
        D_80082FA4 = 9;
        goto L_sw7;
    case 9:
        if (g_McMenuSelection != 1) goto L_sw7;
        g_McMenuState = g_McMenuSelection;
        goto L_sw7;
    default:
        goto L_sw7;
    }

L_sw7:
    switch (g_McMenuSelection) {
    case 3:
        D_8009B6F4 = g_McMenuState;
        /* fallthrough */
    case -2:
    case -1:
        g_McMenuState = g_McMenuSelection;
        goto L_sw7tail;
    case 2:
        if (D_80082FC0 != 0) {
            D_80082FC0 = 0;
            D_80082FBC = 3;
        }
        goto L_sw7tail;
    case 1:
        goto L_sw7tail;
    case -3:
    case 0:
    default:
        D_80082FC0 = 1;
        if (g_McMenuSlotData != -3) goto L_sw7tail;
        {
            s32 t = D_80082FBC;
            D_80082FBC = t - 1;
            if (D_80082FBC != 0) goto L_sw7tail;
        }
        g_McMenuState = g_McMenuSlotData;
        goto L_sw7tail;
    }

L_sw7tail:
    if (g_McMenuState == 2) goto L_epilogue;
    g_McMenuSubState = 1;
    g_McMenuPhase = 0xF;
    D_80082FA4 = 0;
    goto L_b1678;
L_stateM1:
    g_McMenuSubState = 0xA;
    g_McMenuPhase = 3;
    D_80082FB8 = 0;
    mst = D_80082FA4;
    if (mst == 1) goto L_h1219;
    if (mst < 2) {
        if (mst == 0) goto L_h1205;
        goto L_sw8;
    }
    if (mst == 3) goto L_m3;
    goto L_sw8;

L_h1205:
    D_80082FB4 = 5;
    D_8009B564 = 0;
    func_8005F65C(&D_8009B568);
    D_80082FC4 = 0;
    D_80082FA4 = 1;
    goto L_sw8;

L_h1219:
    D_80082FB4 -= 1;
    if (D_80082FB4 != 0) goto L_sw8;
    D_80082FA4 = 3;
    goto L_sw8;

L_m3:
    {
        s32 mph = D_80082F50;
        if (mph == 0) goto L_b1240;
        if (mph == 1) goto L_b1280;
    }
    goto L_sw8;

L_b1240:
    {
        s32 *mp = &D_80082F54;
        func_800611C8(mp, 0, D_8009B744 - 1);
        if (func_800612CC() == 0) goto L_b1268;
        if (*mp != D_8009B744 - 1) goto L_b1264;
    }
    if (fadeBusy != 0) goto L_sw8;
    D_80082FA4 = 0;
    GamePlaySoundCue(2);
    goto L_b1288;

L_b1264:
    GamePlaySoundCue(5);
    goto L_sw8;

L_b1268:
    if ((g_PadEdge2 & 0x90) == 0) goto L_sw8;
    if (fadeBusy != 0) goto L_sw8;
    D_80082FA4 = 0;
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
        goto L_sw8tail;
    case -2:
        g_McMenuState = -2;
        goto L_sw8tail;
    default:
    case -3:
    case 0:
        mslot = g_McMenuSlotData;
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
    D_80082FA4 = 0;
    goto L_epilogue;
L_stateM2:
    if (D_80082F50 == 0) goto L_m2sel;
    if (D_80082F50 == 1) goto L_sw9;
    goto L_sw10;

L_m2sel:
    {
        s32 *p = &D_80082F54;
        g_McMenuSubState = 0xB;
        g_McMenuPhase = 0;
        func_800611C8(p, 0, D_8009B744 - 1);
        pad = g_PadEdge2;
        if ((pad & 0x860) == 0) goto L1415;
        if (*p != 0) goto L1395;
        GamePlaySoundCue(2);
        D_80082F50 = 1;
        D_80082FAC = 0;
        D_8009B734 = *p;
        goto L_sw10;
    L1395:
        if (*p != D_8009B744 - 1) goto L1405;
        if (fadeBusy) goto L_sw10;
        GamePlaySoundCue(2);
        goto L_b1420;
    L1405:
        GamePlaySoundCue(5);
        D_80082F50 = 1;
        D_8009B734 = *p;
        goto L_sw10;
    L1415:
        if ((pad & 0x90) == 0) goto L_sw10;
        if (fadeBusy) goto L_sw10;
        GamePlaySoundCue(3);
        goto L_b1420;
    }

L_b1420:
    D_80082FB8 = 0;
    func_8006138C();
    goto L_sw10;

L_sw9:
    switch (D_80082FA4) {
    case 0:
        if (D_8009B734 == 0) goto L1461;
        g_McMenuPhase = 5;
    L1447:
        { u16 p = func_800612CC(); if (p) goto L1457; }
    L_b1452:
        { u16 p = func_8006131C(); if (p == 0) goto L_sw10; }
    L1457:
        D_80082F50 = 0;
        goto L_b1606;
    L1461:
        g_McMenuPhase = 6;
        { u16 p = func_800612CC(); if (p == 0) goto L_b1452; }
        D_80082FA4 = 1;
        goto L_sw10;
    case 1:
        g_McMenuPhase = D_80082FAC + 7;
        func_8006124C(&D_80082FAC);
        if (D_80082FAC == 0) goto L1447;
        { u16 p = func_800612CC(); if (p == 0) goto L1496; }
        D_80082FA4 = 2;
        goto L_sw10;
    L1496:
        if (D_80082FAC == 0) goto L1447;
        goto L_b1452;
    case 2:
        D_80082FB8 = 1;
        D_80082FB4 = 0x14;
        D_80082FA4 = 3;
        goto L_sw10;
    case 3:
        D_80082FB4 -= 1;
        if (D_80082FB4 == 0) {
            D_80082FA4 = 5;
        }
        goto L_sw10;
    case 5:
        D_80082FA8 = func_8005EF44(0, 0);
        if (D_80082FA8 == 1) {
            D_80082FA4 = 7;
            D_80082FB4 = 0x3C;
        } else {
            D_80082FA4 = 0xA;
        }
        goto L_sw10;
    case 7:
        g_McMenuPhase = 0x13;
        D_80082FB4 -= 1;
        if (D_80082FB4 == 0) {
            D_80082FB8 = 0;
            D_80082FA4 = 8;
        }
        goto L_sw10;
    case 8:
        {
        u16 lpad = g_PadEdge2;
        g_McMenuPhase = 0x13;
        if ((lpad & 0x90) == 0) goto L_sw10;
        }
        D_80082FB8 = 0;
        D_80082FA4 = 0;
        D_80082FA8 = 0;
        D_80082FAC = 0;
        D_80082FB4 = 0;
        if (fadeBusy) goto L_sw10;
        GamePlaySoundCue(3);
        func_8006138C();
        goto L_sw10;
    case 0xA:
        g_McMenuSubState = 0x12;
        g_McMenuPhase = 0x10;
        D_80082FB8 = 0;
        { u16 p = func_800612CC(); if (p) goto L_b1606; }
        { u16 p = func_8006131C(); if (p == 0) goto L_sw10; }
    L_b1606:
        D_80082FA4 = 0;
        /* fall through to L_sw10 */
    default:
        goto L_sw10;
    }

L_sw10:
    switch (g_McMenuSelection) {
    case 1:
    case 2:
        g_McMenuState = 2;
        goto L_sw10tail;
    case 3:
        D_8009B6F4 = g_McMenuState;
        g_McMenuState = 3;
        goto L_sw10tail;
    case -1:
        g_McMenuState = -1;
        goto L_sw10tail;
    case -2:
        if (D_80082FC0 != 0) {
            D_80082FC0 = 0;
            D_80082FBC = 3;
        }
        goto L_sw10tail;
    case -3:
    default:
        {
            s32 sd = g_McMenuSlotData;
            D_80082FC0 = 1;
            if (sd == -3) {
                s32 r = D_80082FBC - 1;
                D_80082FBC = r;
                if (r == 0) {
                    g_McMenuState = sd;
                }
            }
        }
        goto L_sw10tail;
    }

L_sw10tail:
    if (g_McMenuState == -2) {
        goto L_epilogue;
    }
    D_80082FA4 = 0;
    D_80082FB8 = 0;
L_b1678:
    D_80082FA8 = 0;
    D_80082FAC = 0;
    goto L_epilogue;
L_stateIdle:
    g_McMenuSubState = 0x11;
    {
    u16 lpad = g_PadEdge2;
    g_McMenuPhase = 0x10;
    if ((lpad & 0x90) && !fadeBusy) {
        GamePlaySoundCue(3);
        D_80082FB8 = 0;
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
    goto L_epilogue;

L_epilogue:
    if (D_80082FC8 != 0) {
        func_80027A84(D_80082F50, D_8009B730, D_80082F54, D_80082F58);
        if (g_McMenuPhase != 0) {
            func_80027D84(g_McMenuPhase - 1);
        }
        func_80060DF0(D_8009B564, &D_8009B568);
    }
}
