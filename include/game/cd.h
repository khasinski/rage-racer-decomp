#ifndef GAME_CD_H
#define GAME_CD_H

#include "common.h"

void GameClearCdResultEvents(void) asm("func_8005F2AC");
void GameClearCdCompleteEvents(void) asm("func_8005F304");
s32 GameWaitCdResultEvent(void) asm("func_8005F4D8");
s32 GameWaitCdCompleteEvent(void) asm("func_8005F55C");
s32 GamePollCdResultEventWithTimeout(void) asm("func_8005F35C");
s32 GamePollCdResultEventLimit(s32 attempts) asm("func_8005F420");
void GameOpenCdEventGroup(void) asm("func_8005EFAC");
void GameEnableCdEventGroup(void) asm("func_8005F0D4");
void GameDisableCdEventGroup(void) asm("func_8005F16C");
void GameCloseCdEventGroup(void) asm("func_8005F204");
s32 GameIssueCdCommand3F(s32 param) asm("func_80063DEC");
void GameCdReadAndSetMode(s32 param) asm("func_8005EB20");
void GameCdSeekParam(s32 param) asm("func_8005EB78");
s32 GameCdReadStatusPair(s32 high, s32 low) asm("func_8005EBB0");
s32 GameSendFormattedCdCommand(s32 arg0, s32 arg1) asm("func_8005EF44");

#endif
