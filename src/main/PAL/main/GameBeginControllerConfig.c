#include "common.h"
#include "game/state.h"

/* The two live 0..7 selections: standard pad and NeGcon. */
extern u16 D_8019CE08;
extern u16 D_8019CB08;
/* Their backups, restored if the screen is cancelled. */
extern u16 D_8019C7A8;
extern u16 D_8019C76C;

extern s32 D_801E8AA4;
extern s32 D_801E8A9C;
extern s32 D_801E7A4C;
extern s32 D_801E6C7C;

/* Entry hook for the controller-configuration screen: clears the four screen
 * animation counters and snapshots both button-mapping selections so a cancel
 * can put them back. */
void GameBeginControllerConfig(void) {
    D_801E8AA4 = 0;
    D_801E8A9C = 0;
    D_801E7A4C = 0;
    D_801E6C7C = 0;
    D_8019C7A8 = D_8019CE08;
    D_8019C76C = D_8019CB08;
}
