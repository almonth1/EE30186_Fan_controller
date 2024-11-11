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
std::chrono::seconds tacho_period = 5s;
//*****************************

//*** ROTARY_INPUT
//*****************************
#ifdef ROTARY_DEBUG
    DigitalIn aClock(PA_1);
    DigitalIn bSignal(PA_4);
    DigitalOut led_ex(PC_0);
#endif
//*****************************

//****** PID
//*****************************
std::chrono::milliseconds pid_period = 100ms;
//*****************************

//*** TIMER
//*****************************
#ifdef TIMER_DEBUG
    extern InterruptIn Timer_Test(BUTTON1);
    extern DigitalOut led(LED1);
#endif
//*****************************