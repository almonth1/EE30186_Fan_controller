#ifndef ROTARY_INPUT
#define ROTARY_INPUT
#include "mbed.h"
#include "pins_config.h"
#include "taco_test.h"
#include "pid_controller.h"
#include "settings_page.h"

// Declares header variables
extern float encoderPosition;
//extern int increment;
extern int max_value;
extern int min_value;

extern bool currentB;
extern bool prevB;
extern bool prevClk;
extern bool currentClk;

extern Timer rotaryTimer;
extern std::chrono::microseconds rotaryWait_us;


// Declares header functions
void Init_Rotary_Input(int button_mode);
void Rotary_Input();
float RotaryInput_GetPosition();

#endif