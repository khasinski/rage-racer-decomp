#include "common.h"
#include "game/state.h"

/* The two live 0..7 selections: standard pad and NeGcon. */
extern u16 g_PadMappingIndex asm("D_8019CE08");
extern u16 g_NegconMappingIndex asm("D_8019CB08");
/* Their backups, restored if the screen is cancelled. */
extern u16 g_PadMappingIndexSaved asm("D_8019C7A8");
extern u16 g_NegconMappingIndexSaved asm("D_8019C76C");

extern s32 g_ControllerSceneAngleY asm("D_801E8AA4");
extern s32 g_ControllerSceneAngleX asm("D_801E8A9C");
extern s32 g_PadConfigFlipTimer asm("D_801E7A4C");
extern s32 g_PadConfigFlipPhase asm("D_801E6C7C");

/* Entry hook for the controller-configuration screen: clears the four screen
 * animation counters and snapshots both button-mapping selections so a cancel
 * can put them back. */
void BeginControllerConfig(void) {
    g_ControllerSceneAngleY = 0;
    g_ControllerSceneAngleX = 0;
    g_PadConfigFlipTimer = 0;
    g_PadConfigFlipPhase = 0;
    g_PadMappingIndexSaved = g_PadMappingIndex;
    g_NegconMappingIndexSaved = g_NegconMappingIndex;
}
