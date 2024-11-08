#ifndef ROTARY_INPUT
#define ROTARY_INPUT
#include "mbed.h"
#include "pins_config.h"

// Declares header variables
extern bool currentB;
extern bool prevB;
extern bool prevClk;
extern bool currentClk;


// Declares header functions
void Init_Rotary_Input();
void Rotary_Input();
int RotaryInput_GetPosition();

#endif