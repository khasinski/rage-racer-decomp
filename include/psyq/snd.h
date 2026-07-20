#ifndef RAGE_PC_PSYQ_SND_H
#define RAGE_PC_PSYQ_SND_H

#include "common.h"

typedef struct SeqStruct {
    u8 unk0;
    u8 pad1[3];
    u8 *read_pos;
    u8 *next_sep_pos;
    u8 *loop_pos;
    u8 unk10;
    u8 unk11;
    u8 channel;
    u8 unk13;
    u8 play_mode;
    u8 unk15;
    u8 unk16;
    u8 panpot[16];
    u8 unk27;
    u8 unk28;
    u8 unk29;
    u8 unk2a;
    u8 unk2b;
    u8 programs[16];
    u8 unk3C;
    u8 pad3D;
    s16 unk3E;
    s16 unk40;
    s16 unk42;
    s16 tempo_step;
    s16 unk46;
    u16 unk48;
    s16 tempo_multiplier;
    s16 unk4c;
    s16 vol[16];
    s16 unk6E;
    s16 tick_period;
    s16 unk72;
    u16 unk74;
    s16 unk76;
    s16 unk78;
    s16 unk7A;
    s32 base_delta_value;
    u32 unk80;
    s32 base_unk84;
    s32 delta_value;
    s32 tempo;
    s32 flags;
    s32 unk94;
    s32 unk98;
    s32 unk9C;
    s32 tempo_countdown;
    u32 target_tempo;
    s16 padA8;
    s16 padAA;
} SeqStruct;

typedef SeqStruct SequenceState;

typedef struct VabHdr {
    s32 form;
    s32 ver;
    s32 id;
    u32 fsize;
    u16 reserved0;
    u16 ps;
    u16 ts;
    u16 vs;
    u8 mvol;
    u8 pan;
    u8 attr1;
    u8 attr2;
    u32 reserved1;
} VabHdr;

typedef struct ProgAtr {
    u8 tones;
    u8 mvol;
    u8 prior;
    u8 mode;
    u8 mpan;
    s8 reserved0;
    s16 attr;
    u32 reserved1;
    u32 reserved2;
} ProgAtr;

typedef struct VagAtr {
    u8 prior;
    u8 mode;
    u8 vol;
    u8 pan;
    u8 center;
    u8 shift;
    u8 min;
    u8 max;
    u8 vibW;
    u8 vibT;
    u8 porW;
    u8 porT;
    u8 pbmin;
    u8 pbmax;
    u8 reserved1;
    u8 reserved2;
    u16 adsr1;
    u16 adsr2;
    s16 prog;
    s16 vag;
    s16 reserved[4];
} VagAtr;

#define SS_SEQUENCE_CHANNEL_COUNT 0x10

#define SS_SEQ_FLAG_PLAYING 0x1
#define SS_SEQ_FLAG_PAUSED 0x2
#define SS_SEQ_FLAG_STOPPED 0x4
#define SS_SEQ_FLAG_8 0x8
#define SS_SEQ_FLAG_TEMPO_DEC 0x40
#define SS_SEQ_FLAG_TEMPO_INC 0x80

void _SsSndStop(s16 seq, s16 sep) asm("func_80072734");
void SsSeqStop(s16 seq) asm("func_800728A0");
void SsSepStop(s16 seq, s16 sep) asm("func_800728C8");
void _SsSndTempo(s16 seq, s16 sep) asm("func_800728F4");
void SsUtSetReverbDepth(s32 left, s32 right) asm("func_80073748");
s32 SsUtSetReverbType(s32 type) asm("func_80073614");
s32 SsUtGetReverbType(void) asm("func_800736B8");
void SsUtReverbOn(void) asm("func_800736C8");
void SsUtReverbOff(void) asm("func_800736E8");
void SsUtSetReverbFeedback(s32 feedback) asm("func_80073708");
void SsUtSetReverbDelay(s32 delay) asm("func_800737E0");
s16 SsUtGetVVol(s16 voice, s16 *left, s16 *right) asm("func_8007849C");
void _SsVmInit(void) asm("func_8007865C");

void SsSetMVol(s16 left, s16 right) asm("func_8006EAFC");
void SsSetSerialVol(u8 source, s16 left, s16 right) asm("func_8006EBE0");
void SsSetSpuInputAttr(u8 source, u8 field, u8 value) asm("func_8006EB4C");
s32 SsSeqOpen(s32 seq_data) asm("func_8006F004");
void SsSeqAdvanceChannelDelta(s32 seq, s32 channel) asm("func_8006F0D0");
void SsSeqSetChannelPitchBend(s32 seq, s32 channel, s32 pitch, s32 amount) asm("func_8006F48C");
void SsSeqApplyProgramChange(s32 seq, s32 channel) asm("func_80070970");
s32 SsSeqReadDeltaTime(s32 seq, s32 channel) asm("func_80070D70");
void SsUnpackAdsr(u32 adsr1, u32 adsr2, u16 *out) asm("func_80070E28");
void SsPackAdsr(u16 *in, u16 *out0, u16 *out1) asm("func_80070E84");
void SsSeqRestartPlayback(s16 seq, s16 sep) asm("func_80070F10");
void SsSeqPause(s32 seq, s32 sep) asm("func_80071814");
void SsSeqAdvanceChannelTick(s32 seq, s32 sep) asm("func_800718B4");
void SsSeqResume(s32 seq, s32 sep) asm("func_800718E0");
void SsSeqClose(s32 seq) asm("func_80071948");
void SsSeqCloseWrapper(s16 seq) asm("func_80071AC4");
void SsSepCloseWrapper(s16 seq) asm("func_80071AE8");
void _SsInitTables(void) asm("func_80071B0C");
void ssinit(void) asm("func_80071BF4");
void SsQuit(void) asm("func_80071C24");
void SsStartSoundTick(s32 mode) asm("func_80071C44");
void SsStartSoundTickMode1(void) asm("func_80071EAC");
void SsStartSoundTickMode0(void) asm("func_80071ECC");
void SsSoundTickCallback(void) asm("func_80071EEC");
void SsSoundTickVSyncCallback(void) asm("func_80071F2C");
void SsSetTickMode(s32 mode) asm("func_800720F4");
void SsStopSoundTick(void) asm("func_80072260");
void SsSetTableSize(u8 *table, s32 seq_count, s32 sep_count) asm("func_80072310");
void Snd_SetPlayMode(s16 seq, s16 sep, u8 play_mode, s16 loop_count) asm("func_800724F0");
void SsSeqPlay(s32 seq, s32 play_mode, s32 loop_count) asm("func_800725F0");
void SsSepPlay(s32 seq, s32 sep, s32 play_mode, s32 loop_count) asm("func_80072628");
void _SsSndSetVol(s32 seq, s32 sep, s32 left, s32 right) asm("func_80072660");
void SsSeqSetVol(s32 seq, s32 left, s32 right) asm("func_80072698");
void SsSepSetVol(s32 seq, s32 sep, s32 left, s32 right) asm("func_800726C8");
void SsSepSetCrescendo(s32 seq, s32 sep, s32 value, s32 ticks) asm("func_80072700");
void SsSetReservedVoice(u8 voices) asm("func_80072AD0");
void SsSetMono(void) asm("func_80072AE0");
void SsSetStereo(void) asm("func_80072AF4");
u8 SsSetVoiceCount(u8 voices) asm("func_80072B04");
void SsVabClose(s16 vab_id) asm("func_80072B3C");
s16 SsVabOpen(u8 *addr, VabHdr *vab_header) asm("func_80072BC0");
s16 SsVabOpenHead(u8 *addr, s16 vab_id) asm("func_80072C18");
s16 SsVabOpenHeadSticky(u8 *addr, s16 vab_id, u32 spu_addr) asm("func_80072C4C");
s16 SsVabFakeHead(u8 *addr, s16 vab_id, u32 spu_addr) asm("func_80072C80");
s16 SsVabOpenHeadWithMode(u8 *addr, s16 vab_id, s32 mode, u32 spu_addr) asm("func_80072CB4");
s16 SsVabTransBody(u8 *addr, s16 vab_id) asm("func_800730BC");
s16 SsVabTransCompleted(s16 immediate_flag) asm("func_8007317C");
void SpuVmDamperOff(void) asm("func_800731A8");
void SpuVmDamperOn(void) asm("func_800731B8");
void SsSeqCalledTbyT(void) asm("func_800731CC");
s32 SsUtGetProgAtr(s32 vab_id, s32 program, ProgAtr *out) asm("func_8007320C");
s32 SpuVmVSetUp(s32 vab_id, s32 program) asm("func_80073314");
s32 SsUtGetVagAtr(s32 vab_id, s32 program, s32 tone, VagAtr *out) asm("func_800733D8");
s32 SsUtSetVagAtr(s32 vab_id, s32 program, s32 tone, VagAtr *in) asm("func_80073820");
u16 SpuVmCalculateCurrentPitch(void) asm("func_800749B4");
u16 SpuVmCalculateTonePitch(s32 center, s32 fine) asm("func_80074A6C");
u8 SpuVmAlloc(s32 priority) asm("func_800739E8");
void SpuVmKeyOnCore(s32 voice, u16 note, u16 fine, u16 left, u16 right) asm("func_80074818");
void SpuVmKeyOnWithVol(s32 note, s32 fine, s32 left, s32 right) asm("func_80075AE0");
void SpuVmClearFinishedVoices(void) asm("func_80075B78");
void SpuVmKeyOnWithDefaultVol(s32 note, s32 fine) asm("func_80075C30");
s32 SpuVmApplyPitchBendToVoice(s32 voice, s32 note, s32 vab_id, s32 program, s32 bend) asm("func_80075CB0");
s32 SpuVmApplyPitchBendByTone(s32 note, s32 vab_id, s32 program, s32 bend) asm("func_80075EB4");
void SsUtFlush(void) asm("func_80075FA4");
void SpuVmSeKeyOn(s32 seq, s32 vab_id, s32 program, s32 tone, s32 volume, s32 pan) asm("func_80076350");
void SpuVmSeKeyOff(s32 seq, s32 vab_id, s32 program, s32 tone) asm("func_80076940");
void SsUtKeyOn(s32 vab_id, s32 program, s32 tone, s32 unused, u16 left, u16 right) asm("func_80076B30");
void SsUtKeyOff(s32 vab_id, s32 program, s32 tone) asm("func_80076C1C");
void SpuVmSeqKeyOff(s32 seq_sep) asm("func_80076ED8");
s32 SsUtSetProgVol(s32 vab_id, s32 program, s32 volume) asm("func_8007701C");
s32 SsUtGetProgVol(s32 vab_id, s32 program) asm("func_80077090");
s32 SsUtSetProgPan(s32 vab_id, s32 program, s32 pan) asm("func_800770E4");
s32 SsUtGetProgPan(s32 vab_id, s32 program) asm("func_80077158");
s32 SsUtKeyOffV(s32 voice) asm("func_80078018");
s32 SsUtPitchBend(s32 voice, s32 vab_id, s32 program, s32 tone, u16 bend) asm("func_80078130");
s32 SsUtChangePitch(s32 voice, s32 vab_id, s32 program, s32 tone, s32 unused, s32 center, s32 fine) asm("func_800781C0");
s32 SsUtChangeADSR(s32 voice, s32 vab_id, s32 program, s32 tone, s32 adsr1, s32 adsr2) asm("func_80078300");
s32 SsUtGetDetVVol(s32 voice, s16 *left, s16 *right) asm("func_800783D8");
s32 SsUtSetDetVVol(s32 voice, s16 left, s16 right) asm("func_80078430");
s32 SsUtSetVVol(s32 voice, s16 left, s16 right) asm("func_80078528");
s32 SsUtVibrateOn(s32 voice, s32 width, s32 time, s32 unknown) asm("func_800785B4");
s32 SsUtAutoVol(s32 voice, s32 start, s32 end, s32 step) asm("func_80078608");
void SsSeqSetNoteParam2C(s32 seq, s32 sep, u8 value) asm("func_8006F57C");
void SsSeqResetChannelNote(s32 seq, s32 sep) asm("func_8006FCE0");
void SsSeqApplyControlChange(s32 seq, s32 sep, u8 value) asm("func_8006FDA8");
void SsSeqSetChannelMode(s32 seq, s32 sep, u8 mode) asm("func_8006FED8");
void SsSeqSetChannelParam13(s32 seq, s32 sep, u8 value) asm("func_8007001C");
void SsSeqSetChannelParam14(s32 seq, s32 sep, u8 value) asm("func_80070094");

#endif
