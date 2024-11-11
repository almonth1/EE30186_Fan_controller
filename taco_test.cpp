
#include "taco_test.h"

Ticker tacho_tick;
Timer tacho_timer;  
// global variables
int pulse_count;
int prevpulse = 0;
float fanrpm;
bool shdprint = false;

// calculates rpm value and sets flag to print result
void CalculateRPM () {
    //fan rpm calculation returning a float from integers
    fanrpm = ((float)pulse_count / tacho_period.count()) * 30;
    shdprint = true;
    
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
        tacho_timer.start();
        pulse_count = 0 ;
        fanrpm = 0.0;
    #endif
}

// measures tacho for roation speed of fan and prints the results when data is available
void Calculate_Fan_RPM(){
    #ifdef TACHO_DEBUG
        if ( TACHO.read() == 0)  {
            if (prevpulse == 1) {

                if ( std::chrono::duration_cast<std::chrono::milliseconds>(
                    tacho_timer.elapsed_time()) > tacho_delay) {
                    pulse_count += 1;
                }
                tacho_timer.reset();
            } 
            prevpulse = 0;
        }
        else {
            prevpulse = 1;
        }

        // debug prints speed every taco_perios
        if(shdprint == true){
            printf("Average RPM: %g\n", fanrpm);
            printf(" Pulses = %d\n", pulse_count);
            pulse_count = 0;
            shdprint = false;
        }
        
    #endif
}