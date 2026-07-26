#include "common.h"
#include "game/render.h"
#include "game/audio.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"
#include "psyq/snd.h"
#include "game/memcard.h"
#include "psyq/cd.h"

void GameMainLoop(void);

void func_80065460(s32 arg0);
void func_800656CC(s32 arg0);
void func_80068928(void);
void func_8005F5E0(void);
void func_8001F0E0(void);
void func_80021A08(void);
void func_80017884(s32 arg0);
void func_80021338(void);

extern s32 D_801E4B9C;
extern s32 D_801E4B8C;
extern s32 D_801E79C8;
extern s32 D_801E4D14;
extern s32 D_8019CB10;
extern s32 D_8019CACC;

extern u16 D_8019CAD0;
extern u16 D_8019CE08;
extern u16 D_8019CB08;
extern u16 D_801E4BF0;
extern u16 D_8019CA08;
extern u16 D_8019CA0A;
extern u16 D_8019CA0C;
extern u16 D_801E418C;
extern u16 D_8019CAC0;

/*
 * One-shot boot chain called from GameMainLoop: sequencer, sound runtime, GPU
 * and DMA, the pad, then the persistent settings block reset to its defaults
 * (NeGcon uncalibrated, both button mappings on preset 0) and the scratchpad
 * camera block primed before the first frame.
 */
void GameInitSubsystems(void) {
    ssinit();
    GameInitSoundRuntime();
    func_80065460(0);
    func_800656CC(0);
    SetDispMask(0);
    D_801E4B9C = 0;
    D_801E4B8C = 0;
    SetDMAInterruptState(1);
    func_80068928();
    GameInitPad();
    func_8005F5E0();
    D_8019CAD0 = 1;
    D_8019CE08 = 0;
    D_8019CB08 = 0;
    D_801E4BF0 = 0;
    D_8019CA08 = 0;
    D_8019CA0A = 0;
    D_8019CA0C = 0;
    D_801E418C = 0;
    D_801E79C8 = 0;
    D_801E4D14 = 0x21;
    D_8019CB10 = 0;
    D_8019CACC = 0;
    func_8001F0E0();
    GameApplyPadButtonMapping();
    func_80021A08();
    func_80017884(5);
    func_80021338();
    *(s32 *)0x1F80000C = -64;
    *(s32 *)0x1F800010 = -256;
    D_8019CAC0 = 0;
    *(s32 *)0x1F800008 = 0;
    *(s32 *)0x1F800018 = 0x100;
    *(s32 *)0x1F80001C = 0;
    *(s32 *)0x1F800020 = 0;
    GameSetCameraRotMatrix();
}

void func_800630AC(void);
void func_80063190(s32 arg0);
void func_80018038(void);
void func_800438BC(void);
void func_80018078(void);
void func_80043974(void);
void func_8005AF78(void);
void func_80019C04(void);
void func_8001A030(void);
void func_80065ED4(u8 *env);
void func_800660AC(u8 *arg0);
void func_80065E60(u8 *ot);

/* The two 0x237E8-byte frame contexts the loop ping-pongs between. */
extern u8 D_8019CE38[];
/* Which of them is current, mirrored for everyone else to read. */
extern s32 D_801E4B34;
/* Minimum VSync count per frame; 0x80 means "never wait", i.e. free-running. */
extern s32 D_8019C768;
/* Running VSync-derived clock and the raw frame counter. */
extern s32 D_8009E678;
extern s32 D_8009E690;
/* Scene handlers, indexed by g_SceneId. */
extern void (*D_8007C268[])(void);

/*
 * The PS-EXE `main`. Boots the subsystems, then never returns: each pass picks
 * the frame context, resets its two ordering tables and the scratchpad packet
 * cursor, runs the CD / sequencer / loader services and the current scene
 * handler, waits for the frame deadline, swaps the display and refreshes the
 * pad.
 */
void GameMainLoop(void) {
    s32 frameLimit;
    s32 elapsed;
    s32 ticks;

    func_800630AC();
    KernelCallbackSlot3();
    func_80063190(2);
    CdInit();
    GameInitSubsystems();
    func_80018038();
    func_80065460(3);
    func_800438BC();
    D_8019C768 = 0x80;
    SetDispMask(0);
    GameSetupDisplay240(0, 0, 0);
    g_SceneTimer = 0;
    g_SceneId = 1;
    func_80018078();
    D_8009E678 = 0;
    D_8009E690 = 0;
    for (;;) {
        s32 parity = D_8009E690 & 1;
        u8 *frame = D_8019CE38 + parity * 0x237E8;

        g_DrawBuffer = frame;
        D_801E4B34 = parity;
        *(s32 *)0x1F800004 = (s32)(frame + 0xCC);
        *(s32 *)0x1F800000 = (s32)(frame + 0x16CC);
        ClearOTagR((u32 *)(frame + 0xCC), 0x2C0);
        ClearOTagR((u32 *)(g_DrawBuffer + 0xBCC), 0x2C0);
        func_80043974();
        func_8005AF78();
        func_80019C04();
        GameAdvanceSaveHeaderCounter();
        D_8007C268[g_SceneId]();
        DrawSync(0);
        func_8001A030();
        frameLimit = D_8019C768;
        while (VSync(1) < frameLimit) {
        }
        elapsed = VSync(1);
        ticks = D_8009E678 + 1;
        D_8009E678 = ticks + elapsed / 256;
        VSync(0);
        func_80065ED4(g_DrawBuffer);
        func_800660AC(g_DrawBuffer + 0x5C);
        func_80065E60(g_DrawBuffer + 0xBC8);
        func_80065E60(g_DrawBuffer + 0x16C8);
        GameUpdatePadState();
        D_8009E690 = D_8009E690 + 1;
    }
}

/*
 * Stubbed-out debug printf: returns 1 and does nothing. Left un-prototyped
 * because callers pass anything from one to seven arguments. The format
 * strings they pass survive in the binary and were the key to naming much of
 * the game (see docs/names.md).
 */
s32 GameDebugPrintf() {
    return 1;
}
