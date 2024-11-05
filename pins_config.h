#ifndef PINS_CONFIG
#define PINS_CONFIG
#include "mbed.h"

// File for Setting Hardware Pins and Debug states

//*** PID
//*****************************
//#define PID_DEBUG
//*****************************

//*** TIMER
//*****************************
//#define TIMER_DEBUG
#ifdef TIMER_DEBUG
    extern InterruptIn Timer_Test;
    extern DigitalOut led;
#endif
//*****************************


//*****************************

//#define TACHO_DEBUG
extern DigitalIn TACHO;

#ifdef TACHO_DEBUG
extern InterruptIn Tacho_Test;
extern DigitalOut led;
#endif
//*****************************

//*****************************
//*** ROTARY_INPUT
#define ROTARY_INPUT

#ifdef ROTARY_INPUT
    extern DigitalIn aClock;
    extern DigitalIn bSignal;
    extern DigitalOut led_ex;
#endif
//*****************************

#endif
