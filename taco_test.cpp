
#include "taco_test.h"
#include "low_speed_pulse.h"
#include <cstdio>

Ticker tacho_tick;
Timer tacho_timer;  
InterruptIn tacho_interrupt(PA_0);
// global variables
int pulse_count;
int prevpulse = 0;
float fanrpm;
int shdprint = 0;

// calculates rpm value and sets flag to print result
void CalculateRPM () {
    //fan rpm calculation returning a float from integers
    fanrpm = ((float)pulse_count / (tacho_period.count()/1000.0)) * 30;
    pulse_count = 0;
    
}

// increments plulse count by 1
void Increment(){
    #ifdef TACHO_DEBUG
    pulse_count += 1;
    #endif  
}


// initiates tacho ticker interrupt that calls calcilate rpm every tacho_period
void Init_Calculate_Fan_RPM(){
    #ifdef TACHO_DEBUG
        led.write(true);
        tacho_tick.attach(&CalculateRPM, tacho_period);
        tacho_interrupt.fall(&Tacho_Fall);
        tacho_timer.start();
        pulse_count = 0 ;
        fanrpm = 0.0;
    #endif
}

// measures tacho for roation speed of fan and prints the results when data is available
void Calculate_Fan_RPM(){
    #ifdef TACHO_DEBUG

        if ( std::chrono::duration_cast<std::chrono::milliseconds>(
            tacho_timer.elapsed_time()) >= tacho_delay) {
            pulse_count += 1;
        }
        tacho_timer.reset();
 
    #endif
}

void Tacho_Fall(){
    Calculate_Fan_RPM();
    if (low_speed_mode) {
    Low_Speed_Start_Pulse();
    }
    
}