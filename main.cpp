
#include "mbed.h"
#include "taco_test.h"
#include "timer.h"
#include "pid_controller.h"
#include "pins_config.h"
#include "rotary_input.h"
#include "mbed.h"
#include <chrono>
#include <cstdio>


#define pid_period 0.01

//** Tasks to think about
// priority to interrupts?
// parameters in callback?
// rotary encoder input needs to be completed
// LCD Display
// button modes
// hold to activate timer
// Digital Filtering of tachometer at low speeds (ignore pulse if occurs within 2ms of each other)

//**** ideal pwm parameters for low speed
// FanPWM.period(0.002);
// FanPWM.write(0.00111);


//global variables
int8_t pid_output;
int button_mode;


PwmOut FanPWM(PB_0);


int main(){
    // Runs Tacho mode when TACHO_DEBUG is defined in "pins_config.h" (only define one at a time)
    #ifdef TACHO_DEBUG
        //Tacho
        Init_Calculate_Fan_RPM();
    #endif

    // Runs PID mode when PID_DEBUG is defined in "pins_config.h" (only define one at a time)
    #ifdef PID_DEBUG
        // PID
        Init_PID_Interrupt(pid_period); 
    #endif

    // Runs Timer mode when TIMER_DEBUG is defined in "pins_config.h" (only define one at a time)
    #ifdef TIMER_DEBUG
        // Timer mode
        Init_Timer_Mode();
    #endif


    #ifdef ROTARY_INPUT
        Init_Rotary_Input();
    #endif

    FanPWM.period(0.002);
    FanPWM.write(0.00111);
    
    while(true){   

        #ifdef TIMER_DEBUG
            printf("Timer Value %d\n", timer_value);
        #endif

        #ifdef TACHO_DEBUG
            Calculate_Fan_RPM();   
        #endif

        //*** Button mode switch case implementation
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