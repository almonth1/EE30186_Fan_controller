#ifndef PINS_CONFIG
#define PINS_CONFIG
#include "mbed.h"


//*****************************
//#define PID_DEBUG
//*****************************


//*****************************
//#define TIMER_DEBUG
#ifdef TIMER_DEBUG
    extern InterruptIn Timer_Test;
    extern DigitalOut led;
#endif
//*****************************


//*****************************
#define TACHO_DEBUG
extern DigitalIn TACHO;

#ifdef TACHO_DEBUG
extern InterruptIn Tacho_Test;
extern DigitalOut led;
#endif
//*****************************


#endif
