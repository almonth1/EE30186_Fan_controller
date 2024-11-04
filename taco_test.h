#ifndef TACO_TEST
#define TACO_TEST

#include "mbed.h"
#include "pins_config.h"

extern Ticker tacho_tick;

extern int pulse_count;
extern std::chrono::seconds tacho_period;
extern int prevpulse;
extern float fanrpm ;
extern bool shdprint;

void Init_Calculate_Fan_RPM();
void Calculate_Fan_RPM();

#endif