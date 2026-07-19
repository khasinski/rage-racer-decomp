#include "psyq/snd.h"

extern volatile s32 D_801E40AC;
extern SeqStruct *D_801E79CC[];
extern s32 D_801E4B80;
extern s16 D_801E826C;
extern s16 D_801E8270;

void func_8007128C(s16 seq, s16 sep);
void func_80071568(s16 seq, s16 sep);
void func_80072734(s32 seq, s32 sep);

void func_80071018(void) {
    s32 i;
    s32 j;

    if (D_801E40AC == 1) {
        return;
    }
    D_801E40AC = 1;
    SsUtFlush();

    for (i = 0; i < D_801E826C; i++) {
        if (((1 << i) & D_801E4B80) == 0) {
            continue;
        }
        for (j = 0; j < D_801E8270; j++) {
            if (D_801E79CC[i][j].flags & 0x1) {
                SsSeqAdvanceChannelTick((s16)i, (s16)j);
                if (D_801E79CC[i][j].flags & 0x10) {
                    func_8007128C((s16)i, (s16)j);
                }
                if (D_801E79CC[i][j].flags & 0x20) {
                    func_80071568((s16)i, (s16)j);
                }
                if (D_801E79CC[i][j].flags & 0x40) {
                    _SsSndTempo((s16)i, (s16)j);
                }
                if (D_801E79CC[i][j].flags & 0x80) {
                    _SsSndTempo((s16)i, (s16)j);
                }
            }
            if (D_801E79CC[i][j].flags & 0x2) {
                SsSeqPause((s16)i, (s16)j);
            }
            if (D_801E79CC[i][j].flags & 0x8) {
                SsSeqResume((s16)i, (s16)j);
            }
            if (D_801E79CC[i][j].flags & 0x4) {
                func_80072734(i, j);
                D_801E79CC[i][j].flags = 0;
            }
        }
    }
    D_801E40AC = 0;
}
