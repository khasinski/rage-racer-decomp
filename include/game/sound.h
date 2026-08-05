#ifndef GAME_SOUND_H
#define GAME_SOUND_H

#include "common.h"

/* Volume-scale table at D_801E6CA4. */
typedef struct SoundScale {
    s32 scale;
    s16 values[3];
} SoundScale;

/*
 * Volume-scale record at 0x801E6CA4. Its `.scale` word is the master scale
 * applied to every sound-effect voice volume, 0..0x80: SetEffectVolumeScale
 * clamps into that range and the voice code multiplies a cue's nominal volume
 * by it before writing the SPU. `.values` is the VAB id table at 0x801E6CA8.
 */
extern SoundScale g_SoundScale asm("D_801E6CA4");

/*
 * Shared sound work area at 0x801E6D00..0x801E6DA8, three contiguous regions:
 *   0x6D00  MusicChannel[2]  stride 0x18
 *   0x6D30  EffectVoice[4]   stride 0x14 (hardware voices 10..13)
 *   0x6D80  scalar control block (reverb depth / volume scale / flags)
 */

/* Music / sound-mode channel; `left` and `right` are also read as their low
 * halves. Reset to left=right=-1, mode=1, vols=0. */
typedef struct MusicChannel {
    s32 left;      /* +0x00 current left/tone value (g_MusicChannels) */
    s32 right;     /* +0x04 current right value     (D_801E6D04) */
    s32 mode;      /* +0x08 state/mode 0/1/2/-1      (D_801E6D08) */
    s32 reserved;  /* +0x0C unused                              */
    s32 volLeft;   /* +0x10 scaled left volume       (D_801E6D10) */
    s32 volRight;  /* +0x14 scaled right volume      (D_801E6D14) */
} MusicChannel; /* sizeof 0x18 */

extern MusicChannel g_MusicChannels[] asm("D_801E6D00");

/* Effect voice, 4 elements for hardware voices 10..13. SetPitchedSoundCue walks it
 * with a pointer to `.state`. */
typedef struct EffectVoice {
    s16 note;      /* +0x00 note/detune base  (g_EffectVoices) */
    s16 unk02;     /* +0x02                                */
    s16 tone;      /* +0x04 tone              (g_EffectVoiceTone) */
    s16 unk06;     /* +0x06                                */
    s32 state;     /* +0x08 state 0/1/2/-1    (g_EffectVoiceState) */
    s32 pitch;     /* +0x0C pitch             (D_801E6D3C) */
    s32 volume;    /* +0x10 volume            (g_EffectVoiceVolume) */
} EffectVoice; /* sizeof 0x14 */

extern EffectVoice g_EffectVoices[] asm("D_801E6D30");

/* Scalar control block at 0x6D80. Retail addresses these individually by
 * symbol, never base+index, so they stay independent externs. */
extern s32 D_801E6D80; /* +0x00 */
extern s32 g_ReverbDepthL asm("D_801E6D84"); /* reverb depth left  */
extern s32 g_ReverbDepthR asm("D_801E6D88"); /* reverb depth right */
/* Per-frame step added to g_ReverbDepthL/R by UpdateSequenceFadeOut; -3
 * while a BGM fade-out runs, 0 when it has finished. Kept on the raw spelling
 * because ForceBasicEffectVoicesEnabled also uses &D_801E6D8C as the end
 * address of g_EffectVoices (= &g_EffectVoices[4].pitch). */
extern s32 g_ReverbFadeStep asm("D_801E6D8C");
/* libsnd access number of the open SEQ, returned by SsSeqOpen in
 * OpenVabSequenceSlot; the `seq` handle for SsSeqPlay/Stop/SetVol. */
extern s16 g_SeqHandle asm("D_801E6D90");
extern s32 g_SeqVolume asm("D_801E6D94"); /* current SEQ volume, also read as s16 */
extern s32 g_SeqVolumeSetting asm("D_801E6D98"); /* 0..15 OPTIONS level; volume = n * 114 / 15 */
extern s32 g_SeqVolumeFadeStep asm("D_801E6D9C"); /* step added to g_SeqVolume each frame; -4 while fading out */
extern s32 g_PrizeCountStep; /* +0x20 */
extern s16 D_801E6DA4[]; /* +0x24 s16 table */

/* Per-slot engine tone, one entry per bank; a slot is re-cued when its two
 * banks disagree. The old g_SoundSlotToneBank1 symbol (D_80082F2A) is [i][1]
 * of this table. Six slots. */
extern s16 g_SoundSlotTone[][2] asm("D_80082F28");

/*
 * Indexed effect table in rodata at D_800126AC: three entries, twelve bytes
 * each, selected by SetIndexedEffectVoice (index clamped to 0..2). The old
 * g_IndexedEffectVolumes symbol (D_800126B4) is D_800126AC + 8, the third word
 * of the same element, which is why both were indexed by the same i * 12.
 * Retail data: { 14, 0, 64 }, { 14, 0, 64 }, { 16, 0, 90 }.
 */
typedef struct IndexedEffect {
    s32 tone;
    s32 unused;
    s32 volume;
} IndexedEffect; /* sizeof 0xC */

extern IndexedEffect g_IndexedEffects[] asm("D_800126AC");

/* Byte-offset view: the retail code keeps i * 12 in a register rather than
 * indexing, so the scaled offset is passed in directly. */
#define INDEXED_EFFECT(byteOffset) (*(IndexedEffect *)((s32)g_IndexedEffects + (byteOffset)))

/*
 * Pre-race BGM picker (scene 0xA, left/right on the pad). Per-file types.
 *   g_BgmSelection    D_801E42CC  0 = shuffle, else track + 1; saved
 *   g_BgmShuffleOrder D_801E7734  the shuffle bag ShuffleBgmOrder refills
 *   g_BgmShuffleIndex D_8009E6CC  cursor into it, wraps at g_BgmTrackCount
 *   g_BgmTrack        D_801E40E0  the chosen track; RequestCdTrack(n + 3)
 */


#endif
