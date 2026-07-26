#include "common.h"

s32 func_8006A0AC(s32 arg0, s32 arg1);

/* CdGetToc: reads the disc table of contents into `toc` (thin wrapper over
 * CdGetToc2 / func_8006A0AC with track count 1). */
s32 func_8006A088(s32 toc) {
    return func_8006A0AC(1, toc);
}
