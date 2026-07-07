#include "psyq/snd.h"


asm(
    ".set noreorder\n"
    ".set noat\n"
    ".globl Snd_SetPlayMode\n"
    "Snd_SetPlayMode:\n"
    ".globl func_800724F0\n"
    "func_800724F0:\n"
    ".word 0x27BDFFE8\n"
    ".word 0x00805821\n"
    ".word 0x000B1400\n"
    ".word 0x3C03801E\n"
    ".word 0x246379CC\n"
    ".word 0x00021383\n"
    ".word 0x00435021\n"
    ".word 0x00051C00\n"
    ".word 0x00031C03\n"
    ".word 0x00031040\n"
    ".word 0x00431021\n"
    ".word 0x00021080\n"
    ".word 0x00431023\n"
    ".word 0x00021080\n"
    ".word 0x00431023\n"
    ".word 0xAFBF0010\n"
    ".word 0x8D430000\n"
    ".word 0x00024880\n"
    ".word 0x01234021\n"
    ".word 0x8D020090\n"
    ".word 0x2403FDFF\n"
    ".word 0x00431024\n"
    ".word 0xAD020090\n"
    ".word 0x8D430000\n"
    ".word 0x2404FFFB\n"
    ".word 0x01231821\n"
    ".word 0x8C620090\n"
    ".word 0x30C600FF\n"
    ".word 0x00441024\n"
    ".word 0xAC620090\n"
    ".word 0x34020001\n"
    ".word 0x14C20013\n"
    ".word 0xA5070046\n"
    ".word 0x00052200\n"
    ".word 0x01642025\n"
    ".word 0x00042400\n"
    ".word 0x8D430000\n"
    ".word 0x00042403\n"
    ".word 0x01231821\n"
    ".word 0x8C620090\n"
    ".word 0x00003821\n"
    ".word 0x34420001\n"
    ".word 0xAC620090\n"
    ".word 0x95050074\n"
    ".word 0x95060076\n"
    ".word 0x34020001\n"
    ".word 0xA5000048\n"
    ".word 0x0C01DB16\n"
    ".globl func_800725B0\n"
    "func_800725B0:\n"
    ".word 0xA102002B\n"
    ".word 0x0801C978\n"
    ".word 0x00000000\n"
    ".word 0x14C00008\n"
    ".word 0x00000000\n"
    ".word 0x8D430000\n"
    ".word 0x00000000\n"
    ".word 0x01231821\n"
    ".word 0x8C620090\n"
    ".word 0x00000000\n"
    ".word 0x34420002\n"
    ".word 0xAC620090\n"
    ".word 0x8FBF0010\n"
    ".word 0x27BD0018\n"
    ".word 0x03E00008\n"
    ".word 0x00000000\n");

void func_80076C58(s32 seq_sep, u16 left, u16 right, s32 arg3);
void func_80076DCC(s32 seq_sep, s32 value, s32 ticks);

void SsSeqPlay(s32 seq, s32 play_mode, s32 loop_count) {
    Snd_SetPlayMode((s16)seq, 0, (u8)play_mode, (s16)loop_count);
}

void SsSepPlay(s32 seq, s32 sep, s32 play_mode, s32 loop_count) {
    Snd_SetPlayMode((s16)seq, (s16)sep, (u8)play_mode, (s16)loop_count);
}

void _SsSndSetVol(s32 seq, s32 sep, s32 left, s32 right) {
    func_80076C58((s16)(seq | (sep << 8)), (u16)left, (u16)right, 0);
}

void SsSeqSetVol(s32 seq, s32 left, s32 right) {
    func_80076C58((s16)seq, (u16)left, (u16)right, 0);
}

void SsSepSetVol(s32 seq, s32 sep, s32 left, s32 right) {
    func_80076C58((s16)(seq | (sep << 8)), (u16)left, (u16)right, 0);
}

void SsSepSetCrescendo(s32 seq, s32 sep, s32 value, s32 ticks) {
    func_80076DCC((s16)(seq | (sep << 8)), value, ticks);
}

INCLUDE_ASM("asm/PAL/main/nonmatchings/lib/libsnd/Snd_SetPlayMode", _SsSndStop);
