#include "mbed.h"
#include "pins_config.h"

/////////////////////////////////

//*** TACHO and SPEED CONTROL
//*****************************

    DigitalIn TACHO(PA_0);
    DigitalOut led(LED1);
    PwmOut FanPWM(PB_0);

std::chrono::milliseconds tacho_delay = 13ms;
std::chrono::milliseconds tacho_filter = 7ms ;
std::chrono::milliseconds tacho_wait_timer = 0ms ;
std::chrono::milliseconds tacho_period = 300ms;
//*****************************

//*** ROTARY_INPUT
//*****************************

    InterruptIn aClock(PA_1);
    DigitalIn bSignal(PA_4);
    DigitalOut led_A(PB_7);
    DigitalOut led_B(PA_15);

//*****************************

//****** PID
//*****************************
std::chrono::microseconds pid_period = 300000us;
//*****************************


//*** Temp
//*****************************
I2C i2c(PB_9,PB_8);
DigitalOut led_Ext(PC_0);
//*****************************
