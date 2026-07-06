#include "psyq/spu.h"


void SsSetMVol(s16 left, s16 right) {
    s32 left_s = left;
    s32 right_s = right;
    SpuCommonAttr attr;

    attr.mask = 3;
    attr.mvol.volume.left = (left_s << 7) + left_s;
    attr.mvol.volume.right = (right_s << 7) + right_s;
    SpuSetCommonAttr(&attr);
}
