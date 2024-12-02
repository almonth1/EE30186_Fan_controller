#ifndef TACO_TEST
#define TACO_TEST

#include "mbed.h"
#include "pins_config.h"
#include "pid_controller.h"
#include <chrono>
#include <vector>
#include <numeric>

// Declaring header variables

extern Ticker tacho_tick;
extern std::chrono::milliseconds tacho_delay;
extern InterruptIn tacho_interrupt;

extern int pulse_count;
extern vector<int> pulse_vector;

extern int prevpulse;
extern float fanrpm ;
extern int shdprint;

extern float pulse_average;

// Declares header functions
void Init_Calculate_Fan_RPM();
void Calculate_Fan_RPM();
void Tacho_Fall();
void Falling_Edge_Pulse();
void Rising_Edge_Pulse();
#endif