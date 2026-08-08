#ifndef GAME_COURSE_SELECT_INTERNAL_H
#define GAME_COURSE_SELECT_INTERNAL_H

#include "common.h"

typedef struct CourseSelectPrizeTable {
    s32 values[4][6][3];
} CourseSelectPrizeTable;

typedef union CourseSelectScrollState {
    s32 value;
} CourseSelectScrollState;

extern CourseSelectScrollState g_CourseSelectScrollState;
extern s32 g_CourseSelectScrollValue asm("g_CourseSelectScrollState");

#endif
