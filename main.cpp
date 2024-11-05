#include "mbed.h"
#include "taco_test.h"
#include "timer.h"
#include "pid_controller.h"
#include "pins_config.h"
#include "rotary_input.h"
#include "ButtonInput.h"
#include <chrono>
#include <cstdio>

#define pid_period 10ms

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
int Button_Mode = 1;

PwmOut FanPWM(PB_0);

int main() {
    // Runs Tacho mode when TACHO_DEBUG is defined in "pins_config.h" (only define one at a time)
    #ifdef TACHO_DEBUG
        // Tacho
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

    #ifdef ROTARY_DEBUG
        Init_Rotary_Input();
    #endif

    FanPWM.period(0.002);
    FanPWM.write(0.00111);
    
    while (true) {   
        #ifdef TIMER_DEBUG
            printf("Timer Value %d\n", timer_value);
        #endif

        #ifdef TACHO_DEBUG
            Calculate_Fan_RPM();   
        #endif

        // Check if the button was pressed to change the mode
        if (WasButtonPressed()) {
            Button_Mode++;     
            if (Button_Mode > 3) {
                Button_Mode = 1;  // Wrap around to mode 1 after mode 3
            }
            printf("Switched to mode %d\n", Button_Mode);

            switch (Button_Mode) {
                case 1:
                    // Mode 1: Fan RPM calculation (e.g., TACHO mode)
                    #ifdef TACHO_DEBUG
                        Calculate_Fan_RPM();
                    #endif
                    break;

                case 2:
                    // Mode 2: PID control (e.g., temperature control mode)
                    #ifdef PID_DEBUG
                        // Call relevant PID control function
                        Calculate_Fan_RPM();
                    #endif
                    break;

                case 3:
                    // Mode 3: Timer display (e.g., TIMER mode)
                    #ifdef TIMER_DEBUG
                        printf("Timer Value: %d\n", timer_value); // Replace `timer_value` with actual variable
                    #endif
                    break;

                default:
                    Button_Mode = 1;
                    break;
            }
        }
    }
}
