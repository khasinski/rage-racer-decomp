#ifndef RAGE_PC_PSYQ_SPU_INTERNAL_H
#define RAGE_PC_PSYQ_SPU_INTERNAL_H

#include "common.h"
#include "psyq/spu_internal_types.h"

extern long _spu_mem_mode_plus;
extern u_long g_SpuKeyStatus;
extern long g_SpuMemModeUnit;
extern u_char g_SpuMallocArea[];
extern volatile SpuRegisterMap *g_SpuRegBase;
extern SpuRevAttrState g_SpuRevAttr;
extern long g_SpuRevState;
extern long g_SpuRevWorkAreaStartAddr[];
extern u_char g_SpuTimeoutFmt[];
extern u_char g_SpuTimeoutMsgDmaf[];
extern u_char g_SpuTimeoutMsgWrdy[];
extern u_short g_SpuTransferStartAddr;
extern long g_SpuWaitCount;

#endif
