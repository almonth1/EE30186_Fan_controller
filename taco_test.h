#ifndef TACO_TEST
#define TACO_TEST

#include "mbed.h"
#include "pins_config.h"
#include "pid_controller.h"
#include <chrono>

// Declaring header variables

extern Ticker tacho_tick;
extern std::chrono::milliseconds tacho_delay;
extern InterruptIn tacho_interrupt;

extern int pulse_count;

extern int prevpulse;
extern float fanrpm ;
extern int shdprint;

// Declares header functions
void Init_Calculate_Fan_RPM();
void Calculate_Fan_RPM();
void Tacho_Fall();

#endif