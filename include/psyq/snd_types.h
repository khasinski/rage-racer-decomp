#ifndef PSYQ_SND_TYPES_H
#define PSYQ_SND_TYPES_H

#include "common.h"

/*
 * The libsnd table layouts on their own, without the function prototypes.
 * Several libsnd translation units need the types but declare the surrounding
 * functions with narrower parameters than psyq/snd.h does -- retail's callers
 * and callees disagree, and both spellings are load-bearing -- so those files
 * include this header instead of the full one.
 */
/* One sequencer volume, written as a short and read back unsigned. */
typedef union SeqVolume {
    short output;
    u_short value;
} SeqVolume;

/*
 * One SEQ score in g_SndSeqTable[seq][sep]: the parsed MIDI stream plus the
 * sequencer state the tick callback advances. Field names come from what the
 * libsnd sources here do with them; see docs/names.md 17.
 *
 * Loop markers. The SEQ format carries loops as controller events, which
 * SsSeqSetChannelMode dispatches by number: 0x14 opens a loop and parks
 * loop_pos at the current read position, 0x1E closes it, and the data byte in
 * between is the repeat count. loop_count == 0x7F means loop forever.
 *
 * RPN and NRPN. Both arrive as two selection bytes followed by a data entry.
 * rpn_pending and nrpn_pending count the selection bytes seen, so ContDataEntry
 * acts when either reaches 2 and then clears it.
 *
 * Volume fades. _SsSndCrescendo and _SsSndDecrescendo run while flags bit 0x10
 * or 0x20 is set, stepping the SPU sequence volume once every fade_step ticks
 * (a negative fade_step instead adds that many units per tick) and clearing
 * their flag when either counter reaches zero.
 */
typedef struct SeqStruct {
    /* Score to hand to SsSeqRestartPlayback when this one ends; restart_seq
     * stays 0xFF, meaning unchained, everywhere in this image. */
    u_char restart_sep;
    u_char pad1[3];
    u_char *read_pos;
    u_char *next_sep_pos;
    u_char *loop_pos;      /* start of the open loop, parked by marker 0x14 */
    u_char loop_count_set; /* the repeat count has been taken from the stream */
    u_char running_status; /* last MIDI status byte: 0x90, 0xB0, 0xC0, 0xE0, 0xFF */
    u_char channel;
    u_char rpn_param;      /* picks the VagAtr field data entry rewrites:
                            * 0 pbmin/pbmax, 1 shift, 2 center */
    u_char play_mode;
    u_char nrpn_lsb;
    u_char nrpn_msb;       /* also the tone index, or 0x10 for every tone */
    u_char panpot[16];
    u_char loop_marked;    /* marker 0x14 has opened a loop */
    u_char loop_count;     /* repeats left; 0x7F loops forever */
    u_char rpn_pending;    /* selection bytes seen, acted on at 2 */
    u_char nrpn_pending;
    u_char playing;        /* set by play and resume, cleared by pause; nothing
                            * in this image reads it back */
    u_char programs[16];
    u_char restart_seq;
    u_char pad3D;
    short fade_volume_range;
    short fade_steps_left;
    short fade_step;       /* > 0 ticks per unit, < 0 units per tick */
    short tempo_step;
    short play_count;      /* repeats requested by SsSeqPlay; 0 is endless */
    u_short plays_started;
    short tempo_multiplier;
    short vab_id;
    short vol[16];
    short tick_countdown;  /* fractional-tick divider; -1 when unneeded */
    short tick_period;
    short tick_period_initial;
    u_short left_volume;
    u_short right_volume;
    short cur_vol_left;    /* SPU volume read back after every fade step */
    short cur_vol_right;
    long base_delta_value;
    u_long elapsed_ticks;
    long base_tempo;
    long delta_value;
    long tempo;
    long flags;            /* 0x10 crescendo running, 0x20 decrescendo running */
    u_long fade_ticks_total;
    u_long fade_ticks_left;
    long reserved9C;
    long tempo_countdown;
    u_long target_tempo;
    short padA8;
    short padAA;
} SeqStruct;

typedef char SeqStructSizeCheck[sizeof(SeqStruct) == 0xAC ? 1 : -1];

typedef struct VabHdr {
    long form;
    long ver;
    long id;
    u_long fsize;
    u_short reserved0;
    u_short ps;
    u_short ts;
    u_short vs;
    u_char mvol;
    u_char pan;
    u_char attr1;
    u_char attr2;
    u_long reserved1;
} VabHdr;

typedef struct ProgAtr {
    u_char tones;
    u_char mvol;
    u_char prior;
    u_char mode;
    u_char mpan;
    signed char reserved0;
    short attr;
    u_long reserved1;
    u_long reserved2;
} ProgAtr;

typedef struct VagAtr {
    u_char prior;
    u_char mode;
    u_char vol;
    u_char pan;
    u_char center;
    u_char shift;
    u_char min;
    u_char max;
    u_char vibW;
    u_char vibT;
    u_char porW;
    u_char porT;
    u_char pbmin;
    u_char pbmax;
    u_char reserved1;
    u_char reserved2;
    u_short adsr1;
    u_short adsr2;
    short prog;
    u_short vag;
    short reserved[4];
} VagAtr;

typedef struct SndAdsr {
    u_short attack_mode;
    u_short attack_rate;
    u_short decay_rate;
    u_short sustain_level;
    u_short sustain_mode;
    short sustain_direction;
    short sustain_rate;
    u_short release_mode;
    short release_rate;
} SndAdsr;

/*
 * One libsnd voice slot (0x34 bytes). Seven translation units each carried
 * their own copy of this layout under a name ending in the address of the
 * function that introduced it; the offsets always agreed. SpuVmAutoVol's copy
 * named 0x1E..0x26 volumeStep / volumeCounter / volumeCounterReload /
 * currentVolume / targetVolume, which may be the better reading of the last
 * two -- the pan fields at 0x30/0x32 are what suggested start/end here.
 */
typedef struct SpuVoice {
    short vag;
    short age;
    short pitch;
    u_short env;
    short base_volume;
    signed char pan;
    signed char padB;
    short note;
    short seq_sep;
    short program_index;
    short program;
    short tone;
    short vab_id;
    short priority;
    u_char pad1A;
    u_char active;
    short auto_volume;
    short volume_step;
    short volume_counter;
    short volume_counter_reload;
    short start_volume;
    short end_volume;
    short auto_pan;
    short pan_step;
    short pan_counter;
    short pan_counter_reload;
    short start_pan;
    short end_pan;
} SpuVoice;

/* The libsnd "current attributes" block the key-on path fills in. */
typedef struct SvmCurrentAttr {
    u_char tones;
    u_char vab_id;
    u_char note;
    u_char fine;
    u_char volume;
    u_char pan;
    u_char program;
    u_char program_index;
    u_char pad8;
    u_char pad9;
    u_char master_volume;
    u_char master_pan;
    u_char tone;
    u_char tone_volume;
    u_char tone_pan;
    u_char priority;
    u_char center;
    u_char shift;
    u_char min;
    u_char max;
    u_char mode;
    u_char pad15;
    short seq_sep;
    short vag;
    short voice;
    short register_offset;
    short tone_index;
} SvmCurrentAttr;


#endif
