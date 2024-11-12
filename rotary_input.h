#ifndef ROTARY_INPUT
#define ROTARY_INPUT
#include "mbed.h"
#include "pins_config.h"

// Declares header variables
extern int encoderPosition;
extern int increment;
extern int max_value;
extern int min_value;

extern bool currentB;
extern bool prevB;
extern bool prevClk;
extern bool currentClk;


// Declares header functions
void Init_Rotary_Input(int button_mode);
void Rotary_Input();
int RotaryInput_GetPosition();

#endif