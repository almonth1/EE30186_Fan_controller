#include "mbed.h"
#include "pins_config.h"

/////////////////////////////////


#ifdef TACHO_DEBUG
    DigitalIn TACHO(PA_0);
    InterruptIn Tacho_Test(BUTTON1);
    DigitalOut led(LED1);
#endif

////////////////////////////////
#ifdef TIMER_DEBUG
    extern InterruptIn Timer_Test(BUTTON1);
    extern DigitalOut led(LED1);
#endif

///////////////////////////////

#ifdef ROTARY_DEBUG
    DigitalIn aClock(PA_1);
    DigitalIn bSignal(PA_4);
    DigitalOut led_ex(PC_0);
#endif
