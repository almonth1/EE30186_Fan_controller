#include "mbed.h"
#include "pins_config.h"

/////////////////////////////////

//*** TACHO and SPEED CONTROL
//*****************************
#ifdef TACHO_DEBUG
    DigitalIn TACHO(PA_0);
    DigitalOut led(LED1);
    PwmOut FanPWM(PB_0);
#endif
std::chrono::milliseconds tacho_delay = 13ms;
std::chrono::milliseconds tacho_period = 1000ms;
//*****************************

//*** ROTARY_INPUT
//*****************************
#ifdef ROTARY_DEBUG
    DigitalIn aClock(PA_1);
    DigitalIn bSignal(PA_4);
    DigitalOut led_A(PB_7);
    DigitalOut led_B(PA_15);
#endif
//*****************************

//****** PID
//*****************************
std::chrono::microseconds pid_period = 1000us;
//*****************************

//*** TIMER
//*****************************
#ifdef TIMER_DEBUG
    // extern InterruptIn Timer_Test(BUTTON1);
    // extern DigitalOut led(LED1);
#endif
//*****************************


//*** Temp
//*****************************
I2C i2c(PB_9,PB_8);
//*****************************
