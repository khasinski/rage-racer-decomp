#ifndef RAGE_PC_PSYQ_SND_H
#define RAGE_PC_PSYQ_SND_H

#include <sys/types.h>

#include "common.h"
#include "psyq/snd_types.h"


typedef SeqStruct SequenceState;




#define SS_SEQUENCE_CHANNEL_COUNT 0x10

#define SS_SEQ_FLAG_PLAYING 0x1
#define SS_SEQ_FLAG_PAUSED 0x2
#define SS_SEQ_FLAG_STOPPED 0x4
#define SS_SEQ_FLAG_8 0x8
#define SS_SEQ_FLAG_TEMPO_DEC 0x40
#define SS_SEQ_FLAG_TEMPO_INC 0x80

void _SsSndStop(short seq, short sep) asm("func_80072734");
void SsSeqStop(long seq);
void SsSepStop(long seq, long sep);
void _SsSndTempo(short seq, short sep);
void SsUtSetReverbDepth(long left, long right) asm("func_80073748");
long SsUtSetReverbType(long type) asm("func_80073614");
long SsUtGetReverbType(void);
void SsUtReverbOn(void);
void SsUtReverbOff(void);
void SsUtSetReverbFeedback(long feedback);
void SsUtSetReverbDelay(long delay);
short SsUtGetVVol(short voice, short *left, short *right);
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
    long volR);
void _SsVmInit(void) asm("func_8007865C");

void SsSetMVol(short left, short right) asm("func_8006EAFC");
void SsSetSerialVol(u_char source, short left, short right);
void SsSetSpuInputAttr(u_char source, u_char field, u_char value);
/* LibRef47 14-13 gives `short SsSeqOpen(u_long *addr, short vab_id)`. The body
 * here takes one argument, so this is the one-argument internal entry, not the
 * documented API. */
long SsSeqOpen(long seq_data) asm("func_8006F004");
void SsSeqAdvanceChannelDelta(long seq, long channel);
void SsSeqSetChannelPitchBend(long seq, long channel, long pitch, long amount);
void SsSeqApplyProgramChange(long seq, long channel);
long SsSeqReadDeltaTime(long seq, long channel) asm("func_80070D70");
void SsUnpackAdsr(u_long adsr1, u_long adsr2, u_short *out);
void SsPackAdsr(u_short *in, u_short *out0, u_short *out1);
void SsSeqRestartPlayback(short seq, short sep);
/* LibRef47 declares SsSeqPause with one argument (seq access number); this
 * entry takes seq and sep, so it is really the shared _SsSndPause body. */
void SsSeqPause(long seq, long sep);
void SsSeqAdvanceChannelTick(long seq, long sep);
void SsSeqResume(long seq, long sep);
void SsSeqClose(long seq);
void SsSeqCloseWrapper(short seq) asm("func_80071AC4");
void SsSepCloseWrapper(short seq);
void _SsInitTables(void);
void ssinit(void);
void SsQuit(void);
void SsStartSoundTick(long mode);
void SsStartSoundTickMode1(void);
void SsStartSoundTickMode0(void);
void SsSoundTickCallback(void);
void SsSoundTickVSyncCallback(void);
void SsSetTickMode(long mode);
void SsStopSoundTick(void);
void SsSetTableSize(u_char *table, long seq_count, long sep_count);
void Snd_SetPlayMode(long seq, long sep, long playMode, long loopCount);
void SsSeqPlay(long seq, long play_mode, long loop_count);
void SsSepPlay(long seq, long sep, long play_mode, long loop_count);
void _SsSndSetVol(long seq, long sep, long left, long right);
void SsSeqSetVol(long seq, long left, long right);
void SsSepSetVol(long seq, long sep, long left, long right);
/* Recorded here as SsSepSetCrescendo, which the callee rules out: SpuVmGetSeqVol
   (SpuVmGetSeqVol) reads the score's 0x74/0x76 volume pair through two
   out-pointers, and this is a one-line forwarder to it. */
long SsSepGetVol(long seq, long sep, short *voll, short *volr);
void SsSetReservedVoice(u_char voices);
void SsSetMono(void);
void SsSetStereo(void);
u_char SsSetVoiceCount(u_char voices) asm("func_80072B04");
void SsVabClose(short vab_id) asm("func_80072B3C");
short SsVabOpen(u_char *addr, VabHdr *vab_header);
short SsVabOpenHead(u_char *addr, short vab_id);
short SsVabOpenHeadSticky(u_char *addr, short vab_id, u_long spu_addr) asm("func_80072C4C");
short SsVabFakeHead(u_char *addr, short vab_id, u_long spu_addr);
short SsVabOpenHeadWithMode(u_char *addr, short vabid, short mode, u_long spuAddr);
short SsVabTransBody(u_char *addr, short vab_id) asm("func_800730BC");
short SsVabTransCompleted(short immediate_flag) asm("func_8007317C");
void SpuVmDamperOff(void);
void SpuVmDamperOn(void);
/* The tick entry point: interprets SEQ/SEP data and carries out playback
 * (LibRef47 14-32). It is the only caller of SsSeqAdvanceChannelTick.
 * Was bound to SpuVmDamperStep here; that was wrong (docs/names.md 17). */
void SsSeqCalledTbyT(void) asm("func_80071018");
long SsUtGetProgAtr(long vab_id, long program, ProgAtr *out);
long SpuVmVSetUp(long vab_id, long program);
long SsUtGetVagAtr(long vab_id, long program, long tone, VagAtr *out);
long SsUtSetVagAtr(long vab_id, long program, long tone, VagAtr *in);
u_short SpuVmCalculateCurrentPitch(void);
u_short SpuVmCalculateTonePitch(long center, long fine);
u_char SpuVmAlloc(long priority);
void SpuVmKeyOnCore(long voice, u_short note, u_short fine, u_short left, u_short right);
void SpuVmKeyOnWithVol(long note, long fine, long left, long right);
void SpuVmClearFinishedVoices(void);
void SpuVmKeyOnWithDefaultVol(long note, long fine);
long SpuVmApplyPitchBendToVoice(long voice, long note, long vab_id, long program, long bend);
long SpuVmApplyPitchBendByTone(long note, long vab_id, long program, long bend);
void SsUtFlush(void);
void SpuVmSeKeyOn(long seq, long vab_id, long program, long tone, long volume, long pan);
void SpuVmSeKeyOff(long seq, long vab_id, long program, long tone);
/* LibRef47 14-103/14-104. func_80076B30 (6 args, void) and func_80076C1C
 * (3 args) cannot be these; they are left raw (docs/names.md 17). */
short SsUtKeyOn(short vabId, short prog, short tone, short note, short fine, short volL, short volR);
long SsUtKeyOff(long voice, long vabId, long prog, long tone, long note);
void SpuVmSeqKeyOff(long seq_sep);
long SsUtSetProgVol(long vab_id, long program, long volume) asm("func_8007701C");
long SsUtGetProgVol(long vab_id, long program);
long SsUtSetProgPan(long vab_id, long program, long pan);
long SsUtGetProgPan(long vab_id, long program);
long SsUtKeyOffV(long voice);
long SsUtPitchBend(long voice, long vab_id, long program, long note, u_short pbend) asm("func_80078130");
long SsUtChangePitch(long voice, long vab_id, long program, long old_note, long old_fine, long new_note, long new_fine) asm("func_800781C0");
long SsUtChangeADSR(long voice, long vab_id, long program, long old_note, long adsr1, long adsr2);
long SsUtGetDetVVol(long voice, short *left, short *right);
long SsUtSetDetVVol(long voice, short left, short right);
long SsUtSetVVol(long voice, short left, short right) asm("func_80078528");
long SsUtAutoVol(long voice, long start_vol, long end_vol, long delta_time);
long SsUtAutoPan(long voice, long start_pan, long end_pan, long delta_time);
void SsSeqSetNoteParam2C(long seq, long sep, u_char value);
void SsSeqResetChannelNote(long seq, long sep) asm("func_8006FCE0");
void SsSeqApplyControlChange(long seq, long sep, u_char value) asm("func_8006FDA8");
void SsSeqSetChannelMode(long seq, long sep, u_char mode) asm("func_8006FED8");
void SsSeqSetChannelParam13(long seq, long sep, u_char value) asm("func_8007001C");
void SsSeqSetChannelParam14(long seq, long sep, u_char value) asm("func_80070094");

#endif
