#include "common.h"
#include "psyq/cd.h"
#include "game/state.h"
#include "game/cd.h"
#include "game/race.h"

extern s32 g_CdRestartOnResume asm("D_8007F5F8");
extern s32 g_CdMixPreset asm("D_8007F5FC");
extern s32 g_CdTrackPending asm("D_8007F600");
extern s32 g_CdCommandPending asm("D_8007F604");
extern s32 g_CdTrackStep asm("D_8007F608");
extern s32 g_CdCommandStep asm("D_8007F60C");
extern u8 g_CdTrackElapsedLoc[] asm("D_8009AFD0");
extern u8 D_8009B168;
extern u8 g_CdLocResult asm("D_8009B16C");
extern u8 g_CdLocMinute asm("D_8009B16E");
extern u8 g_CdLocSecond asm("D_8009B16F");
extern u8 g_CdVolume asm("D_8009B194");
extern u8 g_CdCurrentTrack asm("D_8009B1B0");
extern s32 g_CdFadeFrames asm("D_8009B1B4");
extern s32 g_CdTrackEnded asm("D_8019C7BC");
extern CdlLOC g_CdTrackLoopPoint[] asm("D_8007F5B0");

long CdControl(long com, void *param, long result) asm("func_8006A5A4");
s32 func_8006A534(s32 arg0, s32 arg1);
s32 func_8006A554(s32 arg0, void *arg1);
long CdPosToInt_Local(CdlLOC *loc) asm("func_8006AADC");
void GameStepCdTrackRequest(void) asm("func_800432A8");
void GameStepCdPlayRequest(void) asm("func_80043494");
void GameStepCdResumeRequest(void) asm("func_800437B8");
void GameBuildCdTrackTable(void) asm("func_800431BC");
void SsSetSpuInputAttr_Link(u8 source, u8 field, u8 value) asm("SsSetSpuInputAttr");

void GameStepCdPauseRequest(void) asm("func_80043598");
void GameStepCdPauseRequest(void) {
    s32 state;
    s32 result;
    s32 currentTime;
    s32 bestTime;
    s32 enteredTime;

    state = g_CdCommandStep;

    switch (state) {
    case 0:
        if (func_8006A534(1, 0) == 0) {
            break;
        }
        g_CdCommandStep = 1;
        /* fallthrough */

    case 1:
        if (CdControl(0x11, 0, (s32)&g_CdLocResult) != 0) {
            g_CdCommandStep = 2;
        }
        break;

    case 2:
        result = func_8006A534(1, 0);
        if (result == 2) {
            g_CdCommandStep = 3;
        } else if (result == 5) {
            g_CdCommandStep = 1;
        }
        break;

    case 3:
        g_CdTrackElapsedLoc[0] = g_CdLocMinute;
        g_CdTrackElapsedLoc[2] = 0;
        g_CdTrackElapsedLoc[1] = g_CdLocSecond;

        currentTime = CdPosToInt_Local(&g_CdTrackLoopPoint[g_CdCurrentTrack]);
        bestTime = CdPosToInt_Local(&g_CdTrackLoopPoint[0]);
        if (bestTime < currentTime) {
            enteredTime = CdPosToInt_Local((CdlLOC *)g_CdTrackElapsedLoc);
            currentTime = CdPosToInt_Local(&g_CdTrackLoopPoint[g_CdCurrentTrack]);
            if (enteredTime >= currentTime) {
                g_CdRestartOnResume = 1;
            } else {
                g_CdRestartOnResume = 0;
            }
        } else {
            g_CdRestartOnResume = 0;
        }

        g_CdCommandStep = 4;
        /* fallthrough */

    case 4:
        if (CdControl(9, 0, 0) != 0) {
            g_CdCommandStep = 5;
        }
        break;

    case 5:
        result = func_8006A534(1, 0);
        if (result == 2) {
            g_CdCommandStep = 6;
        } else if (result == 5) {
            g_CdCommandStep = 4;
        }
        break;

    case 6:
        g_CdCommandPending = -1;
        g_CdCommandStep = 0;
        break;
    }
}
void GameStepCdResumeRequest(void) {
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

void GameInitCdAudio(void) asm("func_800438BC");
void GameInitCdAudio(void) {
    SsSetSpuInputAttr_Link(0, 0, 1);
    asm(".set noreorder\n"
        "move $4,$0\n"
        "li $5,0x7FFF\n"
        ".word 0x0C01BAF8\n"
        ".globl func_800438E0\n"
        "func_800438E0:\n"
        ".word 0x34067FFF\n"
        ".set noreorder");
    asm(".set noreorder\n"
        "li $4,0xE\n"
        "lui $5,%hi(D_8009B168)\n"
        "addiu $5,$5,%lo(D_8009B168)\n"
        "move $6,$0\n"
        "li $2,0x7\n"
        ".word 0x0C01A969\n"
        "sb $2,0($5)\n"
        ".set noreorder");
    GameBuildCdTrackTable();

    g_CdTrackPending = -1;
    g_CdCommandPending = -1;
    g_CdCurrentTrack = 2;
    g_CdTrackStep = 0;
    g_CdCommandStep = 0;
    g_CdMixPreset = 0;
    g_CdRestartOnResume = 0;
    g_CdVolume = 0x7F;
    g_CdFadeFrames = 0;
    GameSetCdVolume(0x7F);
}

void GameTickCdAudio(void) asm("func_80043974");
void GameTickCdAudio(void) {
    s32 temp;
    s32 status;
    s32 state;
    s32 value;

    if (g_CdTrackPending < 0) {
        state = g_CdCommandPending;
        if (state == 2) {
            goto state_2;
        }
        if (state < 3) {
            if (state == 1) {
                goto state_1;
            }
            goto check_cd;
        }
        if (state == 3) {
            goto state_3;
        }
        goto check_cd;
state_1:
        GameStepCdPlayRequest();
        goto check_cd;
state_2:
        GameStepCdPauseRequest();
        goto check_cd;
state_3:
        GameStepCdResumeRequest();
        goto check_cd;
    } else {
        GameStepCdTrackRequest();
check_cd:
        status = func_8006A554(1, &g_CdLocResult);
        if (status == 4) {
            if (g_SceneId == 0x1C) {
                g_CdTrackEnded = 1;
            } else {
                temp = CdPosToInt_Local(&g_CdTrackLoopPoint[g_CdCurrentTrack]);
                value = CdPosToInt_Local(&g_CdTrackLoopPoint[0]);
                if (value < temp) {
                    g_CdTrackStep = status;
                    g_CdCommandPending = 1;
                    g_CdCommandStep = 0;
                    g_CdTrackPending = g_CdCurrentTrack;
                }
            }
        }
    }
done:
    GameStepCdVolumeFade();
}

extern void *g_TrackCameras asm("D_8019C7CC");

void func_80043AC8(u8 *arg0, s32 arg1) {
    void *ptr;

    if (arg1 != 0) {
        if (g_GrandPrixSeries != 0) {
            ptr = arg0 + *(s32 *)(arg0 + 4);
        } else {
            ptr = arg0 + *(s32 *)(arg0 + 0);
        }
    } else {
        ptr = arg0 + *(s32 *)(arg0 + 8);
    }

    g_TrackCameras = ptr;
}
