#include "mbed.h"
#include "pins_config.h"

/////////////////////////////////

//*** TACHO and SPEED CONTROL
//*****************************
#ifdef TACHO_DEBUG
    DigitalIn TACHO(PA_0);
    DigitalOut led(LED1);
#endif
std::chrono::milliseconds tacho_delay = 13ms;
std::chrono::milliseconds tacho_period = 300ms;
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
std::chrono::milliseconds pid_period = 1ms;
//*****************************

//*** TIMER
//*****************************
#ifdef TIMER_DEBUG
    extern InterruptIn Timer_Test(BUTTON1);
    extern DigitalOut led(LED1);
#endif
//*****************************