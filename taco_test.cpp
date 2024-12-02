
#include "taco_test.h"
#include "low_speed_pulse.h"
#include <cstdio>

Ticker tacho_tick;
Timer tacho_timer;  
InterruptIn tacho_interrupt(PA_0);
// global variables
int pulse_count;
vector<int> pulse_vector = {0,0,0,0,0};
int prevpulse = 0;
float fanrpm;
int shdprint = 0;
float pulse_average;

// calculates rpm value and sets flag to print result
void CalculateRPM () {

    pulse_vector.erase(pulse_vector.begin()+0);
    pulse_vector.push_back(pulse_count);

    pulse_average = std::accumulate(pulse_vector.begin(), pulse_vector.end(), 0.0) / pulse_vector.size();

    if (pulse_vector[0] == 0) {
    fanrpm = (pulse_count / (tacho_period.count()/1000.0)) * (60.0/4);
    }
    else {
    fanrpm = (pulse_average / (tacho_period.count()/1000.0)) * (60.0/4);
    }
    //fan rpm calculation returning a float from integers
    
    pulse_count = 0;
}

// initiates tacho ticker interrupt that calls calcilate rpm every tacho_period
void Init_Calculate_Fan_RPM(){
    #ifdef TACHO_DEBUG
        led.write(true);
        tacho_tick.attach(&CalculateRPM, tacho_period);
        tacho_interrupt.fall(&Calculate_Fan_RPM);
        //tacho_interrupt.rise(&Calculate_Fan_RPM);
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

