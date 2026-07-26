#include "common.h"
#include "game/render.h"
#include "psyq/gte.h"

/* World position in full-precision components; the camera keeps one of these
 * in the scratchpad at 0x1F800008. */
typedef struct WorldPos {
    s32 vx;
    s32 vy;
    s32 vz;
} WorldPos;

/* The 56-byte scratch block callers hand in: the object-relative offset as a
 * short vector, the rotated result, then the matrix that is handed to the
 * GTE. */
typedef struct ObjectMatrixWork {
    s16 relative[3];
    s16 pad06;
    WorldPos view;
    s32 pad14;
    Matrix mtx;
} ObjectMatrixWork;

/* The per-frame scratchpad block: camera position at +8, view matrix at +0x28. */
#define SCRATCH_CAMERA_POS ((WorldPos *)0x1F800008)
#define SCRATCH_VIEW_MATRIX ((void *)0x1F800028)

/*
 * Per-object GTE setup: takes the object's offset from the camera through the
 * scratchpad view matrix, scales the rotated offset by 4 into the work
 * matrix's translation, and programs the GTE with the caller's rotation and
 * that translation.
 */
void GameSetGteObjectMatrix(void *work, void *objectPos, Matrix *rot) {
    ObjectMatrixWork *w = (ObjectMatrixWork *)work;
    WorldPos *pos = (WorldPos *)objectPos;

    w->relative[0] = pos->vx - SCRATCH_CAMERA_POS->vx;
    w->relative[1] = pos->vy - SCRATCH_CAMERA_POS->vy;
    w->relative[2] = pos->vz - SCRATCH_CAMERA_POS->vz;
    ApplyMatrix(SCRATCH_VIEW_MATRIX, w->relative, &w->view);
    w->mtx.t[0] = w->view.vx * 4;
    w->mtx.t[1] = w->view.vy * 4;
    w->mtx.t[2] = w->view.vz * 4;
    SetRotMatrix(rot);
    SetTransMatrix(&w->mtx);
}
