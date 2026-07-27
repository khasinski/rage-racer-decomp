#include "common.h"
#include "psyq/cd.h"
#include "game/cd.h"

extern s32 g_CdTrackPending asm("D_8007F600");
extern s32 g_CdCommandPending asm("D_8007F604");
extern s32 g_CdTrackStep asm("D_8007F608");
extern s32 g_CdCommandStep asm("D_8007F60C");
extern u8 g_CdVolume asm("D_8009B194");
extern u8 g_CdCurrentTrack asm("D_8009B1B0");
extern s32 g_CdFadeFrames asm("D_8009B1B4");
extern CdlLOC g_CdTrackLocs[] asm("D_8009AFD4");

s32 func_8006A534(s32 arg0, s32 arg1);
long CdControl(long com, void *param, long result) asm("func_8006A5A4");

void GameStepCdTrackRequest(void) asm("func_800432A8");
void GameStepCdTrackRequest(void) {
    s32 state;
    s32 status;
    u8 track;

    switch (g_CdTrackStep) {
    case 0:
        if (func_8006A534(1, 0) == 0) {
            goto done;
        }
        g_CdFadeFrames = 0;
        g_CdTrackStep = 1;
    case 1:
        if (CdControl(0x16, &g_CdTrackLocs[g_CdTrackPending], 0) == 0) {
            goto done;
        }
        g_CdTrackStep = 2;
        goto done;
    case 2:
        state = func_8006A534(1, 0);
        if (state == 2) {
            g_CdTrackStep = 3;
            goto done;
        }
        if (state == 5) {
            g_CdTrackStep = 1;
            goto done;
        }
        goto done;
    case 3:
        status = g_CdVolume;
        track = (g_CdCurrentTrack = *(u8 *)&g_CdTrackPending);
        g_CdTrackPending = -1;
        g_CdTrackStep = 0;
        GameSetCdVolume(status);
        goto done;
    case 4:
        if (func_8006A534(1, 0) == 0) {
            goto done;
        }
        g_CdTrackStep = 5;
    case 5:
        if (CdControl(0x16, &g_CdTrackLocs[g_CdTrackPending], 0) == 0) {
            goto done;
        }
        g_CdTrackStep = 6;
        goto done;
    case 6:
        state = func_8006A534(1, 0);
        if (state == 2) {
            g_CdTrackStep = 7;
            goto done;
        }
        if (state == 5) {
            g_CdTrackStep = state;
            goto done;
        }
        goto done;
    case 7:
        track = *(u8 *)&g_CdTrackPending;
        g_CdTrackPending = -1;
        g_CdTrackStep = 0;
        g_CdCurrentTrack = track;
        break;
    }

done:
    return;
}

void GameStepCdPlayRequest(void) asm("func_80043494");
void GameStepCdPlayRequest(void) {
    s32 state;
    s32 status;

    state = g_CdCommandStep;
    if (state == 1) {
        goto state_1;
    }
    if (state >= 2) {
        goto state_ge_2;
    }
    if (state == 0) {
        goto state_0;
    }
    goto done;

state_ge_2:
    if (state == 2) {
        goto state_2;
    }
    if (state == 3) {
        goto state_3;
    }
    goto done;

state_0:
    if (func_8006A534(1, 0) == 0) {
        goto done;
    }
    g_CdCommandStep = 1;
state_1:
    if (CdControl(3, 0, 0) == 0) {
        goto done;
    }
    g_CdCommandStep = 2;
    goto done;

state_2:
    status = func_8006A534(1, 0);
    if (status == state) {
        g_CdCommandStep = 3;
        goto done;
    }
    if (status == 5) {
        g_CdCommandStep = 1;
        goto done;
    }
    goto done;

state_3:
    g_CdCommandPending = -1;
    g_CdCommandStep = 0;

done:
    return;
}
