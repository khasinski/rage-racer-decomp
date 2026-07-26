#ifndef GAME_SOUND_H
#define GAME_SOUND_H

#include "common.h"

/* Volume-scale table at D_801E6CA4. */
typedef struct SoundScale {
    s32 scale;
    s16 values[3];
} SoundScale;

/*
 * Shared sound work area at 0x801E6D00..0x801E6DA8, three contiguous regions:
 *   0x6D00  MusicChannel[2]  stride 0x18
 *   0x6D30  EffectVoice[4]   stride 0x14 (hardware voices 10..13)
 *   0x6D80  scalar control block (reverb depth / volume scale / flags)
 */

/* Music / sound-mode channel; `left` and `right` are also read as their low
 * halves. Reset to left=right=-1, mode=1, vols=0. */
typedef struct MusicChannel {
    s32 left;      /* +0x00 current left/tone value (D_801E6D00) */
    s32 right;     /* +0x04 current right value     (D_801E6D04) */
    s32 mode;      /* +0x08 state/mode 0/1/2/-1      (D_801E6D08) */
    s32 reserved;  /* +0x0C unused                              */
    s32 volLeft;   /* +0x10 scaled left volume       (D_801E6D10) */
    s32 volRight;  /* +0x14 scaled right volume      (D_801E6D14) */
} MusicChannel; /* sizeof 0x18 */

extern MusicChannel D_801E6D00[];

/* Effect voice, 4 elements for hardware voices 10..13. func_8005C914 walks it
 * with a pointer to `.state`. */
typedef struct EffectVoice {
    s16 note;      /* +0x00 note/detune base  (D_801E6D30) */
    s16 unk02;     /* +0x02                                */
    s16 tone;      /* +0x04 tone              (D_801E6D34) */
    s16 unk06;     /* +0x06                                */
    s32 state;     /* +0x08 state 0/1/2/-1    (D_801E6D38) */
    s32 pitch;     /* +0x0C pitch             (D_801E6D3C) */
    s32 volume;    /* +0x10 volume            (D_801E6D40) */
} EffectVoice; /* sizeof 0x14 */

extern EffectVoice D_801E6D30[];

/* Scalar control block at 0x6D80. Retail addresses these individually by
 * symbol, never base+index, so they stay independent externs. */
extern s32 D_801E6D80; /* +0x00 */
extern s32 D_801E6D84; /* reverb depth left  */
extern s32 D_801E6D88; /* reverb depth right */
extern s32 D_801E6D8C; /* sequence flag/state */
/* libsnd access number of the open SEQ, returned by SsSeqOpen in
 * GameOpenVabSequenceSlot; the `seq` handle for SsSeqPlay/Stop/SetVol. */
extern s16 g_SeqHandle asm("D_801E6D90");
extern s32 D_801E6D94; /* volume scale (also read as s16) */
extern s32 D_801E6D98; /* sequence volume scale */
extern s32 D_801E6D9C; /* active/enabled flag */
extern s32 D_801E6DA0; /* +0x20 */
extern s16 D_801E6DA4[]; /* +0x24 s16 table */

/*
 * Pre-race BGM picker (scene 0xA, left/right on the pad). Per-file types.
 *   g_BgmSelection    D_801E42CC  0 = shuffle, else track + 1; saved
 *   g_BgmShuffleOrder D_801E7734  the shuffle bag func_8001B488 refills
 *   g_BgmShuffleIndex D_8009E6CC  cursor into it, wraps at g_BgmTrackCount
 *   g_BgmTrack        D_801E40E0  the chosen track; GameRequestCdTrack(n + 3)
 */


#endif
