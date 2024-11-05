
#include "mbed.h"
#include "taco_test.h"
#include "timer.h"
#include "pid_controller.h"
#include "pins_config.h"
#include "mbed.h"
#include <chrono>
#include <cstdio>


#define pid_period 0.01


// priority to interrupts?
// parameters in callback?
// rotary encoder input needs to be completed
// LCD Display
// button modes
// hold to activate timer







int8_t pid_output;

int button_mode;

Ticker test;

PwmOut FanPWM(PB_0);


int main(){
    
    #ifdef TACHO_DEBUG
        //Tacho
        Init_Calculate_Fan_RPM();
    #endif

    #ifdef PID_DEBUG
        // PID
        Init_PID_Interrupt(pid_period); 
    #endif

    #ifdef TIMER_DEBUG
        // Timer mode
        Init_Timer_Mode();
    #endif

    FanPWM.period(0.002);

    FanPWM.write(0.00111);

    
    while(true){ 
        Calculate_Fan_RPM();

        #ifdef TIMER_DEBUG
            printf("Timer Value %d\n", timer_value);
        #endif

        #ifdef TACHO_DEBUG
         
            // printf("Pulses: %d\nfanrpm = %g\n", pulse_count, fanrpm);
            
       
        #endif

        

        // switch (button_mode) {
        // case 1:
        //     Calculate_Fan_RPM();
        // case 2: 
        // // temp control
        //     Calculate_Fan_RPM();
        // case 3:
        // // timer mode
        //  }   

    }
       
}