#include "common.h"
#include "game/render.h"
#include "game/audio.h"
#include "game/state.h"
#include "psyq/gpu.h"
#include "psyq/kernel.h"
#include "psyq/snd.h"

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
