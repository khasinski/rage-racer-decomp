#include "common.h"
#include "game/state.h"

/* The two live 0..7 selections: standard pad and NeGcon. */
extern u16 g_PadMappingIndex;
extern u16 g_NegconMappingIndex;

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
