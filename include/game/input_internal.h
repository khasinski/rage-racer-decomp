#ifndef GAME_INPUT_INTERNAL_H
#define GAME_INPUT_INTERNAL_H

#include "common.h"

#ifndef GAME_INPUT_BUTTON_TYPE
#define GAME_INPUT_BUTTON_TYPE u16
#endif
#ifndef GAME_INPUT_CALIBRATION_TYPE
#define GAME_INPUT_CALIBRATION_TYPE u16
#endif

extern GAME_INPUT_BUTTON_TYPE g_PadButtonMapping[];
typedef s16 ControllerMappingIndex;
extern ControllerMappingIndex g_PadMappingIndex;
extern ControllerMappingIndex g_NegconMappingIndex;
extern GAME_INPUT_CALIBRATION_TYPE g_NegconMaxTwist;
extern GAME_INPUT_CALIBRATION_TYPE g_NegconSteerPlay;
extern GAME_INPUT_CALIBRATION_TYPE g_NegconSteerNeutral;
extern GAME_INPUT_CALIBRATION_TYPE g_NegconNeutralI;
extern GAME_INPUT_CALIBRATION_TYPE g_NegconNeutralII;
extern GAME_INPUT_CALIBRATION_TYPE g_NegconNeutralL;

#endif
