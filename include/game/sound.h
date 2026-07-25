#ifndef GAME_SOUND_H
#define GAME_SOUND_H

#include "common.h"

/*
 * Shared sound-engine work area at 0x801E6D00..0x801E6DA8, touched by ~21
 * functions in src/main/PAL/main. Layout reconstructed from the retail
 * disassembly (base loaded as `lui rX,0x801e` + decimal offset 27904..) and
 * cross-checked against every C file that already casts these symbols.
 *
 *   0x6D00  MusicChannel   D_801E6D00[2]  stride 0x18 (music/mode channels)
 *   0x6D30  EffectVoice    D_801E6D30[4]  stride 0x14 (effect voices 10..13)
 *   0x6D80  scalar control block (reverb depth / volume scale / flags)
 *
 * The three regions are contiguous:
 *   MusicChannel[2] = 0x30 -> ends at 0x6D30
 *   EffectVoice[4]  = 0x50 -> ends at 0x6D80
 */

/*
 * Music / sound-mode channel. Stride 0x18 (24 bytes), 2 elements.
 * Indexed as D_801E6D00[i]; field +0x00/+0x04 also read as *(s16*) (low half)
 * by func_8005C6C0. +0x0C is never referenced (reserved padding word).
 * Reset (func_8005C31C): left=right=-1, mode=1, volLeft=volRight=0.
 * The old symbols D_801E6D08 == D_801E6D00[0].mode and
 * D_801E6D18 == D_801E6D00[1].left.
 */
typedef struct MusicChannel {
    s32 left;      /* +0x00 current left/tone value (D_801E6D00) */
    s32 right;     /* +0x04 current right value     (D_801E6D04) */
    s32 mode;      /* +0x08 state/mode 0/1/2/-1      (D_801E6D08) */
    s32 reserved;  /* +0x0C unused                              */
    s32 volLeft;   /* +0x10 scaled left volume       (D_801E6D10) */
    s32 volRight;  /* +0x14 scaled right volume      (D_801E6D14) */
} MusicChannel; /* sizeof 0x18 */

extern MusicChannel D_801E6D00[];

/*
 * Effect voice. Stride 0x14 (20 bytes), 4 elements (hardware voices 10..13).
 * Base D_801E6D30. func_8005C914 walks it with statePtr = &voice.state
 * (0x6D38) and while (statePtr < &D_801E6D38[20]) i.e. 4 * 0x14 = 0x50.
 *   note (+0x00) read as *(s16*) via (s16*)&state - 4
 *   tone (+0x04) read as *(s16*) (D_801E6D34)
 * The old symbols D_801E6D38/3C/40 == D_801E6D30[0].state/pitch/volume;
 * D_801E6D44/58/6C == D_801E6D30[1..3].note.
 */
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

/*
 * Scalar control block at 0x6D80. These are addressed individually by symbol
 * (never base+index), so they are kept as independent externs rather than a
 * struct - typing them as a struct would give no register-allocation benefit
 * and would break the ~10 functions that reference them by name.
 */
extern s32 D_801E6D80; /* +0x00 */
extern s32 D_801E6D84; /* reverb depth left  */
extern s32 D_801E6D88; /* reverb depth right */
extern s32 D_801E6D8C; /* sequence flag/state */
extern s16 D_801E6D90; /* sequence volume    */
extern s32 D_801E6D94; /* volume scale (also read as s16) */
extern s32 D_801E6D98; /* sequence volume scale */
extern s32 D_801E6D9C; /* active/enabled flag */
extern s32 D_801E6DA0; /* +0x20 */
extern s16 D_801E6DA4[]; /* +0x24 s16 table */

#endif
