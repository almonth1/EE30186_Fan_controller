#ifndef PINS_CONFIG
#define PINS_CONFIG
#include "mbed.h"

// File for Setting Hardware Pins and Debug states

//*** TACHO and SPEED CONTROL
//*****************************
#define TACHO_DEBUG
extern DigitalIn TACHO;
extern std::chrono::milliseconds tacho_delay;
extern std::chrono::milliseconds tacho_period;

#ifdef TACHO_DEBUG
extern DigitalOut led;
#endif
//*****************************

//*** ROTARY_INPUT
//*****************************
#define ROTARY_DEBUG
#ifdef ROTARY_DEBUG
    extern DigitalIn aClock;
    extern DigitalIn bSignal;
    extern DigitalOut led_A;
    extern DigitalOut led_B;
#endif
//*****************************

//*** PID
//*****************************
//#define PID_DEBUG
extern std::chrono::microseconds pid_period;
//*****************************

//*** TIMER
//*****************************
//#define TIMER_DEBUG
#ifdef TIMER_DEBUG
    // extern InterruptIn Timer_Test;
    // extern DigitalOut led;
#endif
//*****************************

#endif