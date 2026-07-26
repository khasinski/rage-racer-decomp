#ifndef GAME_CD_H
#define GAME_CD_H

#include "common.h"

void GameClearCdResultEvents(void) asm("func_8005F2AC");
void GameClearCdCompleteEvents(void) asm("func_8005F304");
s32 GameWaitCdResultEvent(void) asm("func_8005F4D8");
s32 GameWaitCdCompleteEvent(void) asm("func_8005F55C");
s32 GamePollCdResultEventWithTimeout(void) asm("func_8005F35C");
s32 GamePollCdResultEventLimit(s32 attempts) asm("func_8005F420");
void GameOpenCdEventGroup(void) asm("func_8005EFAC");
void GameEnableCdEventGroup(void) asm("func_8005F0D4");
void GameDisableCdEventGroup(void) asm("func_8005F16C");
void GameCloseCdEventGroup(void) asm("func_8005F204");
s32 GameIssueCdCommand3F(s32 param) asm("func_80063DEC");
void GameCdReadAndSetMode(s32 param) asm("func_8005EB20");
void GameCdSeekParam(s32 param) asm("func_8005EB78");
s32 GameCdReadStatusPair(s32 high, s32 low) asm("func_8005EBB0");
s32 GameSendFormattedCdCommand(s32 arg0, s32 arg1) asm("func_8005EF44");

/*
 * CD-DA (music) front end. Nothing here talks to the drive directly: each call
 * only posts a request into D_8007F600..D_8007F60C, which GameTickCdAudio pumps
 * one CdControl at a time (CdlPlay 0x03, CdlPause 0x09, CdlGetlocP 0x11).
 */
/* Queue track `track` from the D_8009AFD4 CdlLOC table. */
void GameRequestCdTrack(s32 track) asm("func_80042BC0");
/* Issue CdlPlay for whatever is queued / paused. */
void GameStartCdAudio(void) asm("func_80042BF0");
/* Capture the current position, then CdlPause. */
void GamePauseCdAudio(void) asm("func_80042C0C");
/* Undo the pause: replay the track when the pause crossed a track boundary. */
void GameResumeCdAudio(void) asm("func_80042C28");
/* Drop any pending track/command and reset the current track index to 2. */
void GameResetCdAudioState(void) asm("func_80042C94");

/*
 * CD-DA attenuator. GameSetCdVolume scales the four D_8007F5A8 mix values by
 * `volume` (0..0x7F) into both the current and target levels and pushes them
 * with CdMix; GameStartCdVolumeFade sets the remaining frame count of the
 * fade GameStepCdVolumeFade runs each frame (positive fades out, negative fades
 * back to the targets), clamped to +/-0xFFF.
 */
void GameSetCdVolume(s32 volume) asm("func_80042FA0");
void GameStartCdVolumeFade(s32 frames) asm("func_80042CCC");
void GameStepCdVolumeFade(void) asm("func_80042D10");
/* Re-push the current g_CdVolume (used after a mode change). */
void GameApplyCdVolume(void) asm("func_8004310C");
/* Map the 0..15 option-screen level onto the 0..0x7F attenuator. */
void GameSetCdVolumeSetting(s32 level) asm("func_80043134");
/* Select which 4-byte row of the D_8007F5A8 mix table GameSetCdVolume scales. */
void GameSetCdMixPreset(s32 preset) asm("func_8004318C");

/*
 * The CD-DA pump. GameTickCdAudio runs once per frame from GameMainLoop and
 * issues at most one CdControl: a pending track goes to GameStepCdTrackRequest,
 * otherwise g_CdCommandPending 1/2/3 selects play / pause / resume. Each step
 * function is a small state machine over g_CdTrackStep / g_CdCommandStep that
 * clears the pending value when it finishes. See docs/names.md 13.
 */
void GameTickCdAudio(void) asm("func_80043974");
void GameStepCdTrackRequest(void) asm("func_800432A8");   /* CdlSeekP 0x16 */
void GameStepCdPlayRequest(void) asm("func_80043494");    /* CdlPlay 0x03 */
void GameStepCdPauseRequest(void) asm("func_80043598");   /* CdlGetlocP + CdlPause */
void GameStepCdResumeRequest(void) asm("func_800437B8");  /* CdlPlay 0x03 */
/* Boot-time setup: SPU CD input on, drive into CD-DA mode, track table built,
 * every pending/step word cleared and the volume set to full. */
void GameInitCdAudio(void) asm("func_800438BC");
/* Build the D_8009AFD4 CdlLOC table: CdGetToc, each audio track pushed 0x3C
 * sectors in, then the file-backed entries found with DsSearchFile. */
void GameBuildCdTrackTable(void) asm("func_800431BC");

#endif
