#ifndef RAGE_PC_PSYQ_SND_H
#define RAGE_PC_PSYQ_SND_H

#include <sys/types.h>

#include "common.h"

typedef struct SeqStruct {
    u_char unk0;
    u_char pad1[3];
    u_char *read_pos;
    u_char *next_sep_pos;
    u_char *loop_pos;
    u_char unk10;
    u_char unk11;
    u_char channel;
    u_char unk13;
    u_char play_mode;
    u_char unk15;
    u_char unk16;
    u_char panpot[16];
    u_char unk27;
    u_char unk28;
    u_char unk29;
    u_char unk2a;
    u_char unk2b;
    u_char programs[16];
    u_char unk3C;
    u_char pad3D;
    short unk3E;
    short unk40;
    short unk42;
    short tempo_step;
    short unk46;
    u_short unk48;
    short tempo_multiplier;
    short unk4c;
    short vol[16];
    short unk6E;
    short tick_period;
    short unk72;
    u_short unk74;
    short unk76;
    short unk78;
    short unk7A;
    long base_delta_value;
    u_long unk80;
    long base_unk84;
    long delta_value;
    long tempo;
    long flags;
    long unk94;
    long unk98;
    long unk9C;
    long tempo_countdown;
    u_long target_tempo;
    short padA8;
    short padAA;
} SeqStruct;

typedef SeqStruct SequenceState;

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
    short vag;
    short reserved[4];
} VagAtr;

#define SS_SEQUENCE_CHANNEL_COUNT 0x10

#define SS_SEQ_FLAG_PLAYING 0x1
#define SS_SEQ_FLAG_PAUSED 0x2
#define SS_SEQ_FLAG_STOPPED 0x4
#define SS_SEQ_FLAG_8 0x8
#define SS_SEQ_FLAG_TEMPO_DEC 0x40
#define SS_SEQ_FLAG_TEMPO_INC 0x80

void _SsSndStop(short seq, short sep) asm("func_80072734");
void SsSeqStop(long seq) asm("func_800728A0");
void SsSepStop(long seq, long sep) asm("func_800728C8");
void _SsSndTempo(short seq, short sep) asm("func_800728F4");
void SsUtSetReverbDepth(long left, long right) asm("func_80073748");
long SsUtSetReverbType(long type) asm("func_80073614");
long SsUtGetReverbType(void) asm("func_800736B8");
void SsUtReverbOn(void) asm("func_800736C8");
void SsUtReverbOff(void) asm("func_800736E8");
void SsUtSetReverbFeedback(long feedback) asm("func_80073708");
void SsUtSetReverbDelay(long delay) asm("func_800737E0");
short SsUtGetVVol(short voice, short *left, short *right) asm("func_8007849C");
/*
 * Key-on a named hardware voice: rejects voice >= 24 or an unknown program,
 * stamps the utility sep number 0x21 into the current-voice record D_801E4BD0,
 * derives volume/pan from (volL, volR), copies the program and tone attributes
 * and starts the note; returns the voice number, or -1. Declared with long
 * parameters, not the SDK's short, because the game calls it unprototyped and
 * passes full words.
 */
long SsUtKeyOnV(
    long voice,
    long vabId,
    long prog,
    long tone,
    long note,
    long fine,
    long volL,
    long volR) asm("func_80077C7C");
void _SsVmInit(void) asm("func_8007865C");

void SsSetMVol(short left, short right) asm("func_8006EAFC");
void SsSetSerialVol(u_char source, short left, short right) asm("func_8006EBE0");
void SsSetSpuInputAttr(u_char source, u_char field, u_char value) asm("func_8006EB4C");
/* LibRef47 14-13 gives `short SsSeqOpen(u_long *addr, short vab_id)`. The body
 * here takes one argument, so this is the one-argument internal entry, not the
 * documented API. */
long SsSeqOpen(long seq_data) asm("func_8006F004");
void SsSeqAdvanceChannelDelta(long seq, long channel) asm("func_8006F0D0");
void SsSeqSetChannelPitchBend(long seq, long channel, long pitch, long amount) asm("func_8006F48C");
void SsSeqApplyProgramChange(long seq, long channel) asm("func_80070970");
long SsSeqReadDeltaTime(long seq, long channel) asm("func_80070D70");
void SsUnpackAdsr(u_long adsr1, u_long adsr2, u_short *out) asm("func_80070E28");
void SsPackAdsr(u_short *in, u_short *out0, u_short *out1) asm("func_80070E84");
void SsSeqRestartPlayback(short seq, short sep) asm("func_80070F10");
/* LibRef47 declares SsSeqPause with one argument (seq access number); this
 * entry takes seq and sep, so it is really the shared _SsSndPause body. */
void SsSeqPause(long seq, long sep) asm("func_80071814");
void SsSeqAdvanceChannelTick(long seq, long sep) asm("func_800718B4");
void SsSeqResume(long seq, long sep) asm("func_800718E0");
void SsSeqClose(long seq) asm("func_80071948");
void SsSeqCloseWrapper(short seq) asm("func_80071AC4");
void SsSepCloseWrapper(short seq) asm("func_80071AE8");
void _SsInitTables(void) asm("func_80071B0C");
void ssinit(void) asm("func_80071BF4");
void SsQuit(void) asm("func_80071C24");
void SsStartSoundTick(long mode) asm("func_80071C44");
void SsStartSoundTickMode1(void) asm("func_80071EAC");
void SsStartSoundTickMode0(void) asm("func_80071ECC");
void SsSoundTickCallback(void) asm("func_80071EEC");
void SsSoundTickVSyncCallback(void) asm("func_80071F2C");
void SsSetTickMode(long mode) asm("func_800720F4");
void SsStopSoundTick(void) asm("func_80072260");
void SsSetTableSize(u_char *table, long seq_count, long sep_count) asm("func_80072310");
void Snd_SetPlayMode(long seq, long sep, long playMode, long loopCount) asm("func_800724F0");
void SsSeqPlay(long seq, long play_mode, long loop_count) asm("func_800725F0");
void SsSepPlay(long seq, long sep, long play_mode, long loop_count) asm("func_80072628");
void _SsSndSetVol(long seq, long sep, long left, long right) asm("func_80072660");
void SsSeqSetVol(long seq, long left, long right) asm("func_80072698");
void SsSepSetVol(long seq, long sep, long left, long right) asm("func_800726C8");
/* Recorded here as SsSepSetCrescendo, which the callee rules out: func_80076DCC
   (SpuVmGetSeqVol) reads the score's 0x74/0x76 volume pair through two
   out-pointers, and this is a one-line forwarder to it. */
long SsSepGetVol(long seq, long sep, short *voll, short *volr) asm("func_80072700");
void SsSetReservedVoice(u_char voices) asm("func_80072AD0");
void SsSetMono(void) asm("func_80072AE0");
void SsSetStereo(void) asm("func_80072AF4");
u_char SsSetVoiceCount(u_char voices) asm("func_80072B04");
void SsVabClose(short vab_id) asm("func_80072B3C");
short SsVabOpen(u_char *addr, VabHdr *vab_header) asm("func_80072BC0");
short SsVabOpenHead(u_char *addr, short vab_id) asm("func_80072C18");
short SsVabOpenHeadSticky(u_char *addr, short vab_id, u_long spu_addr) asm("func_80072C4C");
short SsVabFakeHead(u_char *addr, short vab_id, u_long spu_addr) asm("func_80072C80");
short SsVabOpenHeadWithMode(u_char *addr, short vabid, short mode, u_long spuAddr) asm("func_80072CB4");
short SsVabTransBody(u_char *addr, short vab_id) asm("func_800730BC");
short SsVabTransCompleted(short immediate_flag) asm("func_8007317C");
void SpuVmDamperOff(void) asm("func_800731A8");
void SpuVmDamperOn(void) asm("func_800731B8");
/* The tick entry point: interprets SEQ/SEP data and carries out playback
 * (LibRef47 14-32). It is the only caller of SsSeqAdvanceChannelTick.
 * Was bound to func_800731CC here; that was wrong (docs/names.md 17). */
void SsSeqCalledTbyT(void) asm("func_80071018");
long SsUtGetProgAtr(long vab_id, long program, ProgAtr *out) asm("func_8007320C");
long SpuVmVSetUp(long vab_id, long program) asm("func_80073314");
long SsUtGetVagAtr(long vab_id, long program, long tone, VagAtr *out) asm("func_800733D8");
long SsUtSetVagAtr(long vab_id, long program, long tone, VagAtr *in) asm("func_80073820");
u_short SpuVmCalculateCurrentPitch(void) asm("func_800749B4");
u_short SpuVmCalculateTonePitch(long center, long fine) asm("func_80074A6C");
u_char SpuVmAlloc(long priority) asm("func_800739E8");
void SpuVmKeyOnCore(long voice, u_short note, u_short fine, u_short left, u_short right) asm("func_80074818");
void SpuVmKeyOnWithVol(long note, long fine, long left, long right) asm("func_80075AE0");
void SpuVmClearFinishedVoices(void) asm("func_80075B78");
void SpuVmKeyOnWithDefaultVol(long note, long fine) asm("func_80075C30");
long SpuVmApplyPitchBendToVoice(long voice, long note, long vab_id, long program, long bend) asm("func_80075CB0");
long SpuVmApplyPitchBendByTone(long note, long vab_id, long program, long bend) asm("func_80075EB4");
void SsUtFlush(void) asm("func_80075FA4");
void SpuVmSeKeyOn(long seq, long vab_id, long program, long tone, long volume, long pan) asm("func_80076350");
void SpuVmSeKeyOff(long seq, long vab_id, long program, long tone) asm("func_80076940");
/* LibRef47 14-103/14-104. func_80076B30 (6 args, void) and func_80076C1C
 * (3 args) cannot be these; they are left raw (docs/names.md 17). */
short SsUtKeyOn(short vabId, short prog, short tone, short note, short fine, short volL, short volR) asm("func_800776E4");
long SsUtKeyOff(long voice, long vabId, long prog, long tone, long note) asm("func_80077A88");
void SpuVmSeqKeyOff(long seq_sep) asm("func_80076ED8");
long SsUtSetProgVol(long vab_id, long program, long volume) asm("func_8007701C");
long SsUtGetProgVol(long vab_id, long program) asm("func_80077090");
long SsUtSetProgPan(long vab_id, long program, long pan) asm("func_800770E4");
long SsUtGetProgPan(long vab_id, long program) asm("func_80077158");
long SsUtKeyOffV(long voice) asm("func_80078018");
long SsUtPitchBend(long voice, long vab_id, long program, long note, u_short pbend) asm("func_80078130");
long SsUtChangePitch(long voice, long vab_id, long program, long old_note, long old_fine, long new_note, long new_fine) asm("func_800781C0");
long SsUtChangeADSR(long voice, long vab_id, long program, long old_note, long adsr1, long adsr2) asm("func_80078300");
long SsUtGetDetVVol(long voice, short *left, short *right) asm("func_800783D8");
long SsUtSetDetVVol(long voice, short left, short right) asm("func_80078430");
long SsUtSetVVol(long voice, short left, short right) asm("func_80078528");
long SsUtAutoVol(long voice, long start_vol, long end_vol, long delta_time) asm("func_800785B4");
long SsUtAutoPan(long voice, long start_pan, long end_pan, long delta_time) asm("func_80078608");
void SsSeqSetNoteParam2C(long seq, long sep, u_char value) asm("func_8006F57C");
void SsSeqResetChannelNote(long seq, long sep) asm("func_8006FCE0");
void SsSeqApplyControlChange(long seq, long sep, u_char value) asm("func_8006FDA8");
void SsSeqSetChannelMode(long seq, long sep, u_char mode) asm("func_8006FED8");
void SsSeqSetChannelParam13(long seq, long sep, u_char value) asm("func_8007001C");
void SsSeqSetChannelParam14(long seq, long sep, u_char value) asm("func_80070094");

#endif
