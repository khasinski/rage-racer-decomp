#ifndef GAME_CD_H
#define GAME_CD_H

#include "common.h"
/*
 * CD-DA (music) front end. Nothing here talks to the drive directly: each call
 * only posts a request into D_8007F600..D_8007F60C, which TickCdAudio pumps
 * one CdControl at a time (CdlPlay 0x03, CdlPause 0x09, CdlGetlocP 0x11).
 */
/* Queue track `track` from the D_8009AFD4 CdlLOC table. */
void RequestCdTrack(s32 track) asm("func_80042BC0");
/* Issue CdlPlay for whatever is queued / paused. */
void StartCdAudio(void) asm("func_80042BF0");
/* Capture the current position, then CdlPause. */
void PauseCdAudio(void) asm("func_80042C0C");
/* Undo the pause: replay the track when the pause crossed a track boundary. */
void ResumeCdAudio(void) asm("func_80042C28");
/* Drop any pending track/command and reset the current track index to 2. */
void ResetCdAudioState(void) asm("func_80042C94");

/*
 * CD-DA attenuator. SetCdVolume scales the four D_8007F5A8 mix values by
 * `volume` (0..0x7F) into both the current and target levels and pushes them
 * with CdMix; StartCdVolumeFade sets the remaining frame count of the
 * fade StepCdVolumeFade runs each frame (positive fades out, negative fades
 * back to the targets), clamped to +/-0xFFF.
 */
void SetCdVolume(s32 volume) asm("func_80042FA0");
void StartCdVolumeFade(s32 frames) asm("func_80042CCC");
void StepCdVolumeFade(void) asm("func_80042D10");
/* Re-push the current g_CdVolume (used after a mode change). */
void ApplyCdVolume(void) asm("func_8004310C");
/* Map the 0..15 option-screen level onto the 0..0x7F attenuator. */
void SetCdVolumeSetting(s32 level) asm("func_80043134");
/* Select which 4-byte row of the D_8007F5A8 mix table SetCdVolume scales. */
void SetCdMixPreset(s32 preset) asm("func_8004318C");

/*
 * The CD-DA pump. TickCdAudio runs once per frame from MainLoop and
 * issues at most one CdControl: a pending track goes to StepCdTrackRequest,
 * otherwise g_CdCommandPending 1/2/3 selects play / pause / resume. Each step
 * function is a small state machine over g_CdTrackStep / g_CdCommandStep that
 * clears the pending value when it finishes. See docs/names.md 13.
 */
void TickCdAudio(void) asm("func_80043974");
void StepCdTrackRequest(void) asm("func_800432A8");   /* CdlSeekP 0x16 */
void StepCdPlayRequest(void) asm("func_80043494");    /* CdlPlay 0x03 */
void StepCdPauseRequest(void) asm("func_80043598");   /* CdlGetlocP + CdlPause */
void StepCdResumeRequest(void) asm("func_800437B8");  /* CdlPlay 0x03 */
/* Boot-time setup: SPU CD input on, drive into CD-DA mode, track table built,
 * every pending/step word cleared and the volume set to full. */
void InitCdAudio(void) asm("func_800438BC");
/* Build the D_8009AFD4 CdlLOC table: CdGetToc, each audio track pushed 0x3C
 * sectors in, then the file-backed entries found with DsSearchFile. */
void BuildCdTrackTable(void) asm("func_800431BC");

#endif
