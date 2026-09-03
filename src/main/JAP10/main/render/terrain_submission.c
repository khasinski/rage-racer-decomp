#include "common.h"
#include "game/render.h"
#include "psyq/gte.h"

/*
 * The terrain/model submission engine is original hand-written R3000/GTE
 * assembly with a private register ABI and shared internal entry points.
 * Keep it as regional source rather than presenting it as compiler-generated
 * C.  The included source uses the named GTE instruction macros from
 * macro.inc (rtps, rtpt, nclip, avsz4 and dpcs).
 */
HANDWRITTEN_ASM("src/main/JAP10/main/render", terrain_submission);
