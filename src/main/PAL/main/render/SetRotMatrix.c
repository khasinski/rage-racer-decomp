#include "common.h"
#include "psyq/gte_macros.h"

/*
 * PSY-Q libgte low-level GTE (COP2) accessor routines.
 *
 * This translation unit is the set of small "set control register / load
 * data register / run one COP2 command" leaf routines from the SDK. Each
 * function body is expressed with the named GTE macros in
 * include/psyq/gte_macros.h; ordinary CPU work (the fixed-point shifts) stays
 * in C. The `.align 4` directives reproduce the 16-byte function alignment the
 * retail object uses from SetBackColor onward (the padding shows up as the
 * zero-word "functions" func_80069A70 / func_80069A84).
 */

/* --- SetRotMatrix.s --- */

void SetRotMatrix(void *m);
void SetRotMatrix(void *m) { gte_SetRotMatrix(m); }
void SetLightMatrix(void *m);
void SetLightMatrix(void *m) { gte_SetLightMatrix(m); }
void SetColorMatrix(void *m);
void SetColorMatrix(void *m) { gte_SetColorMatrix(m); }
void SetTransMatrix(void *m);
void SetTransMatrix(void *m) { gte_SetTransMatrix(m); }
