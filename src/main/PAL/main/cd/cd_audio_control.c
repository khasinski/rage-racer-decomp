#include "common.h"
#include "game/cd.h"
void RequestCdTrack(s32 track) {
    g_CdTrackPending = (u8)track;
    g_CdTrackStep = 0;
    g_CdCommandPending = -1;
    g_CdCommandStep = 0;
}

void StartCdAudio(void) {
    g_CdCommandPending = 1;
    g_CdCommandStep = 0;
}

void PauseCdAudio(void) {
    g_CdCommandPending = 2;
    g_CdCommandStep = 0;
}

void ResumeCdAudio(void) {
    if (g_CdRestartOnResume != 0) {
        u8 value;

        value = g_CdCurrentTrack;
        g_CdTrackStep = 4;
        g_CdRestartOnResume = 0;
        g_CdCommandPending = 1;
        g_CdCommandStep = 0;
        g_CdTrackPending = value;
    } else {
        g_CdCommandPending = 3;
        g_CdCommandStep = 0;
    }
}

void ResetCdAudioState(void) {
    g_CdTrackPending = -1;
    g_CdCommandPending = -1;
    g_CdTrackStep = 0;
    g_CdCommandStep = 0;
    g_CdCurrentTrack = 2;
}
