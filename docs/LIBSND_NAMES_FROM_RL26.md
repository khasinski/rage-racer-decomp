# libsnd internal names, recovered from Runtime Library 2.6

Source: `Programmer Tool - Runtime Library Version 2.6 (Japan) (En,Ja)_DTL-S2170_redump.zip`
on the Internet Archive `ps1_sdks` item, disc dated 1995-01-24, `PSX/LIB/LIBSND.LIB`.

**Why this exists.** Runtime Library 2.x was built with a compiler that did not inline
static helpers, so every internal libsnd function is a separate symbol in the library's
export table with its real name. By PsyQ 3.5, which is what Rage Racer links, those same
helpers are inlined and their names are gone. This file recovers them.

The embedded source paths are `C:\PSX.NEW\SRC\SND\<FILE>.C`; 3.5's objects say
`C:\PSX\SRC\SND\<FILE>.C`, so the file layout is unchanged between the two.

| source member | identifiers |
|---|---|
| `VOL.C` | `Snd_setvol_data` |
| `TEMPO.C` | `Snd_setvol_data`, `Snd_tempo`, `VqR`, `_ss_score` |
| `CRES.C` | `Snd_crescendo`, `Snd_tempo`, `VqR`, `_ss_score` |
| `DECRE.C` | `Snd_crescendo`, `Snd_decrescendo`, `SpuVmGetSeqVol`, `SpuVmSetSeqVol`, `VqR`, `_ss_score` |
| `NEXT.C` | `Snd_decrescendo`, `Snd_nextseq`, `SpuVmGetSeqVol`, `SpuVmSetSeqVol`, `VqR`, `_ss_score` |
| `PAUSE.C` | `Snd_nextpause`, `Snd_nextseq`, `Snd_pause`, `SpuVmSetSeqVol`, `VqR`, `_ss_score` |
| `PLAY.C` | `SeqPlay`, `Snd_nextpause`, `Snd_pause`, `Snd_play`, `Snd_replay`, `SpuVmSeqKeyOff`, `VqR`, `_ss_score` |
| `REPLAY.C` | `SeqPlay`, `Snd_play`, `Snd_replay`, `VqR`, `_ss_score` |
| `ADSR.C` | `Snd_replay`, `SsUtBuildADSR`, `SsUtResolveADSR`, `VqR`, `_ss_score` |
| `STOP.C` | `Snd_stop`, `SsSepStop`, `SsSeqStop`, `SsUtBuildADSR`, `SsUtResolveADSR`, `VqR` |
| `SEPINIT.C` | `InitSoundSep`, `Snd_stop`, `SpuVmSeqKeyOff`, `SsSepStop`, `SsSeqStop`, `VqR`, `VqRP`, `_ss_score` |
| `SEQINIT.C` | `InitSoundSep`, `InitSoundSeq`, `ReadDeltaValue`, `VqR`, `_ss_score`, `not`, `old` |
| `SSACCELE.C` | `InitSoundSeq`, `ReadDeltaValue`, `Snd_SetAccele`, `SsSepSetAccelerando`, `SsSeqSetAccelerando`, `VqR`, `_ss_score`, `not`, `old` |
| `SSCALL.C` | `Snd_SetAccele`, `SsSepSetAccelerando`, `SsSeqCalledTbyT`, `SsSeqSetAccelerando`, `VqR`, `VqR4`, `_ss_score` |
| `SSCLOSE.C` | `Snd_crescendo`, `Snd_decrescendo`, `Snd_pause`, `Snd_play`, `Snd_replay`, `Snd_stop`, `Snd_tempo`, `SpuVmFlush`, `SsSepClose`, `SsSeqCalledTbyT`, `SsSeqClose`, `VqR`, `_snd_ev_flag`, `_snd_openflag`, `_snd_seq_s_max`, `_snd_seq_t_max`, `_ss_score` |
| `SSCRES.C` | `SpuVmSeqKeyOff`, `SpuVmSetSeqVol`, `SsSepClose`, `SsSepSetCrescendo`, `SsSeqClose`, `SsSeqSetCrescendo`, `VqR`, `VqRh`, `_snd_openflag`, `_snd_seq_t_max`, `_ss_score` |
| `SSDECRES.C` | `Snd_setvol_data`, `SsSepSetCrescendo`, `SsSepSetDecrescendo`, `SsSeqSetCrescendo`, `SsSeqSetDecrescendo`, `VqR`, `_ss_score` |
| `SSINIT.C` | `MarkCallback`, `Snd_setvol_data`, `SsEnd`, `SsInit`, `SsQuit`, `SsSepSetDecrescendo`, `SsSeqSetDecrescendo`, `SsSetTableSize`, `SsSetTickMode`, `SsStart`, `SsStart2`, `VqR`, `_snd_ev_flag`, `_snd_openflag`, `_snd_seq_no_tick`, `_snd_seq_s_max`, `_snd_seq_t_max`, `_snd_seq_tick_mode`, `_ss_score` |
| `SSNEXT.C` | `CloseEvent`, `DisableEvent`, `EnableEvent0`, `EnterCriticalSection`, `ExitCriticalSection`, `MarkCallback`, `OpenEvent0`, `ResetCallback`, `ResetRCnt`, `SetRCnt0`, `SpuInit`, `SpuQuit`, `SpuVmInit0`, `SsEnd`, `SsInit0`, `SsQuit`, `SsSeqCalledTbyT0`, `SsSeqSetNext`, `SsSetTableSize`, `SsSetTickMode`, `SsStart0`, `SsStart2`, `StartRCnt`, `VSyncCallback`, `VqR`, `VuR`, `WqR`, `WqR0`, `WqRP`, `_SsStart`, `_snd_ev_flag`, `_snd_openflag`, `_snd_seq_no_tick`, `_snd_seq_s_max0`, `_snd_seq_t_max`, `_snd_seq_tick_mode`, `_ss_score`, `_ss_score0`, `_wait`, `qRx` |
| `SSOPEN.C` | `SsSepOpen`, `SsSeqOpen`, `SsSeqSetNext`, `VqR`, `_ss_score`, `any`, `more` |
| `SSPAUSE.C` | `InitSoundSep`, `InitSoundSeq`, `SsSepOpen`, `SsSepPause`, `SsSeqOpen`, `SsSeqPause`, `VqR`, `_snd_openflag`, `any`, `more` |
| `SSPLAY.C` | `Snd_SetPlayMode`, `SpuVmGetSeqVol`, `SsPlayBack`, `SsSepPause`, `SsSepPlay`, `SsSeqPause`, `SsSeqPlay`, `VqR`, `_ss_score` |
| `SSREPLAY.C` | `Snd_SetPlayMode`, `SpuVmSetSeqVol`, `SsPlayBack`, `SsSepPlay`, `SsSepReplay`, `SsSeqPlay`, `SsSeqReplay`, `VqR`, `VqR8`, `VqRp`, `_ss_score` |
| `SSRIT.C` | `Snd_SetRit`, `SsSepReplay`, `SsSepSetRitardando`, `SsSeqReplay`, `SsSeqSetRitardando`, `VqR`, `VqRx`, `_ss_score` |
| `SSVOL.C` | `Snd_SetRit`, `Snd_SetVol`, `SsSepSetRitardando`, `SsSepSetVol`, `SsSeqSetRitardando`, `SsSeqSetVol`, `VqR`, `VqR4`, `_ss_score` |
| `SEQREAD.C` | `ContDataEntry`, `ContModulation`, `ContNrpn1`, `ContNrpn2`, `ContPortaTime`, `ContPortamento`, `ContResetAll`, `ContRpn1`, `ContRpn2`, `GetMetaEvent`, `GetSeqData`, `NoteOn`, `ReadDeltaValue`, `SeqPlay`, `SetControlChange`, `SetPitchBend`, `SetProgramChange`, `Snd_SetVol`, `Snd_setVabAttr`, `SpuVmSetSeqVol`, `SsSepSetVol`, `SsSeqSetVol`, `VqR`, `VqR8`, `VqRt`, `_ss_score` |
| `SCGMVOL.C` | `ContDataEntry`, `ContModulation`, `ContNrpn1`, `ContNrpn2`, `ContPortaTime`, `ContPortamento`, `ContResetAll`, `ContRpn1`, `ContRpn2`, `GetMetaEvent`, `GetSeqData`, `MarkCallback`, `NoteOn`, `ReadDeltaValue`, `SeqPlay`, `SetControlChange`, `SetPitchBend`, `SetProgramChange`, `Snd_nextseq`, `Snd_setVabAttr`, `SpuVmDamperOff`, `SpuVmDamperOn`, `SpuVmGetProgVol`, `SpuVmKeyOff`, `SpuVmKeyOn`, `SpuVmPitchBend`, `SpuVmSeqKeyOff`, `SsGetMVol`, `SsUtBuildADSR`, `SsUtGetProgAtr`, `SsUtGetVagAtr`, `SsUtResolveADSR`, `SsUtReverbOff`, `SsUtReverbOn`, `SsUtSetReverbDelay`, `SsUtSetReverbDepth`, `SsUtSetReverbFeedback`, `SsUtSetReverbType`, `SsUtSetVagAtr`, `VqR`, `WqR`, `WqRx`, `_ss_score`, `cWqR` |
| `SCGNCK.C` | `SsGetMVol`, `SsGetNck`, `SsGetRVol`, `VqR`, `_spu_ioctl` |
| `SCGRVOL.C` | `SsGetNck`, `SsGetRVol`, `VqR`, `_spu_ioctl` |
| `SCGSATTR.C` | `SsGetRVol`, `SsGetSerialAttr`, `VqR`, `_spu_ioctl` |
| `SCGSVOL.C` | `SsGetSerialAttr`, `SsGetSerialVol`, `VqR`, `_spu_ioctl` |
| `SCSMVOL.C` | `SsGetSerialVol`, `SsSetMVol`, `SsSetMute`, `VqR`, `_spu_ioctl` |
| `SCSMUTE.C` | `SsSetMVol`, `SsSetMute`, `SsSetNck`, `VqR`, `_spu_ioctl` |
| `SCSNCK.C` | `SsSetMute`, `SsSetNck`, `SsSetRVol`, `VqR`, `_spu_ioctl` |
| `SCSRVOL.C` | `SsSetNck`, `SsSetRVol`, `SsSetSeria`, `VqR`, `_spu_ioctl` |
| `SCSSATTR.C` | `SsSetRVol`, `SsSetSerialAttr`, `VqR`, `_spu_ioctl` |
| `SCSSVOL.C` | `SsSetSerialAttr`, `SsSetSerialVol`, `VqR`, `_spu_ioctl` |
| `SCNON.C` | `SpuVmNoiseOn`, `SsSetNoiseOff`, `SsSetNoiseOn`, `SsSetSerialVol`, `VqR`, `_spu_ioctl` |
| `SCNOFF.C` | `SpuVmNoiseOff`, `SpuVmNoiseOn`, `SsIsEos`, `SsSetLoop`, `SsSetMarkCallback`, `SsSetNoiseOff`, `SsSetNoiseOn`, `SsSetTempo`, `VqR` |
| `SSADD.C` | `SpuVmNoiseOff`, `SsIsEos`, `SsSetLoop`, `SsSetMarkCallback`, `SsSetNoiseOff`, `SsSetTempo`, `VqR` |
| `SESKON.C` | `MarkCallback`, `SpuVmSeKeyOn`, `SsIsEos`, `SsSetLoop`, `SsSetMarkCallback`, `SsSetTempo`, `SsVoKeyOff`, `SsVoKeyOn`, `VqR`, `VqR4`, `VqRx`, `_ss_score` |
| `SESKOFF.C` | `SpuVmSeKeyOff`, `SpuVmSeKeyOn`, `SsUtPitchBend`, `SsVoKeyOff`, `SsVoKeyOn`, `VqR`, `_svm_orev`, `_svm_orev1` |
