
#include "taco_test.h"
#include <chrono>






Ticker tacho_tick;

int pulse_count;

std::chrono::seconds tacho_period = 5s;
int prevpulse = 0;
float fanrpm;
bool shdprint = false;


void CalculateRPM () {
    //fan rpm calculation returning a float from integers
    fanrpm = ((float)pulse_count / tacho_period.count()) * 30;
    shdprint = true;
    
}

void Increment(){
    #ifdef TACHO_DEBUG
    pulse_count += 1;
    #endif  
    
}

void Init_Calculate_Fan_RPM(){
    #ifdef TACHO_DEBUG
        led.write(true);
        tacho_tick.attach(&CalculateRPM, tacho_period);
        Tacho_Test.mode(PullUp);
        Tacho_Test.fall(&Increment);
        pulse_count = 0 ;
        fanrpm = 0.0;
    #endif
}



void Calculate_Fan_RPM(){
    
        if ( !(TACHO.read() == 1) ) {

            if (prevpulse == 0) {
            pulse_count += 1;
            }
        
         prevpulse = 1;
        }

        else {
            prevpulse = 0;
        }
        

        // debug prints speed every taco_perios
        if(shdprint == true){
            printf("Average RPM: %g\n", fanrpm);
            printf(" Pulses = %d\n", pulse_count);
            pulse_count = 0;
            shdprint = false;
        }
       wait_us(5000);

}