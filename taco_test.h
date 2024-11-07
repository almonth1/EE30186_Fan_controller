#ifndef TACO_TEST
#define TACO_TEST

#include "mbed.h"
#include "pins_config.h"
#include <chrono>

// Declaring header variables

extern Ticker tacho_tick;
extern std::chrono::milliseconds tacho_delay;

extern int pulse_count;
extern std::chrono::seconds tacho_period;
extern int prevpulse;
extern float fanrpm ;
extern bool shdprint;

// Declares header functions
void Init_Calculate_Fan_RPM();
void Calculate_Fan_RPM();

#endif