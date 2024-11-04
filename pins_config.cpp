#include "mbed.h"

/////////////////////////////////
DigitalIn TACHO(PA_0);

#ifdef TACHO_DEBUG
    InterruptIn Tacho_Test(BUTTON1);
    DigitalOut led(LED1);
#endif

////////////////////////////////
#ifdef TIMER_DEBUG
    extern InterruptIn Timer_Test(BUTTON1);
    extern DigitalOut led(LED1);
#endif

///////////////////////////////