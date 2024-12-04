#ifndef PINS_CONFIG
#define PINS_CONFIG
#include "mbed.h"

// File for Setting Hardware Pins and Debug states

//*** TACHO and SPEED CONTROL
//*****************************

extern DigitalIn TACHO;
extern std::chrono::milliseconds tacho_delay;
extern std::chrono::milliseconds tacho_filter;
extern std::chrono::milliseconds tacho_period;
extern std::chrono::milliseconds tacho_wait_timer;
extern PwmOut FanPWM;
extern DigitalOut led;

//*****************************

//*** ROTARY_INPUT
//*****************************
    extern InterruptIn aClock;
    extern DigitalIn bSignal;
    extern DigitalOut led_A;
    extern DigitalOut led_B;

//*****************************

//*** PID
//*****************************
extern std::chrono::microseconds pid_period;
//*****************************


//*** Temp
//*****************************
extern I2C i2c;
extern DigitalOut led_Ext;
//*****************************

#endif