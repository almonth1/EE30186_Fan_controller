#include "mbed.h"
#include "taco_test.h"
#include "timer.h"
#include "pid_controller.h"
#include "pins_config.h"
#include "rotary_input.h"
#include "ButtonInput.h"
#include <chrono>
#include <cstdio>

#define TargetSpeed 1500



//** Tasks to think about
// priority to interrupts?
// rotary encoder input needs to be completed
// LCD Display
// hold to activate timer
// Digital Filtering of tachometer at low speeds (ignore pulse if occurs within 2ms of each other)
// Timer display on 7 seg

//**** ideal pwm parameters for low speed
// FanPWM.period(0.002);
// FanPWM.write(0.00111);

//global variables

int Button_Mode = 1;

Timer printTimer; 
PwmOut FanPWM(PB_0);

int main() {
    printTimer.start();

    InitializeButtonInput();
    // Runs Tacho mode when TACHO_DEBUG is defined in "pins_config.h" (only define one at a time)
    #ifdef TACHO_DEBUG
        // Tacho
        Init_Calculate_Fan_RPM();
        Init_PID_Controller(pid_speed_ptr, speed_controller_params);
    #endif

    // Runs PID mode when PID_DEBUG is defined in "pins_config.h" (only define one at a time)
    #ifdef PID_DEBUG
        // PID
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

        // Check if the button was pressed to change the mode
        if (WasButtonPressed()) {
            Button_Mode++;     
            if (Button_Mode > 3) {
                Button_Mode = 1;  // Wrap around to mode 1 after mode 3
            }
            printf("Switched to mode %d\n", Button_Mode);
        }
            switch (Button_Mode) {
                case 1:
                    // Mode 1: PID Speed Control
                    #ifdef TACHO_DEBUG
                        
                        Calculate_Fan_RPM();
                        Rotary_Input();  // Update encoder position

                    int rotaryPosition = RotaryInput_GetPosition();  // Get the current encoder position
                    if (rotaryPosition > 99) {
                        rotaryPosition = 99;  // Cap at maximum
                    }

                    float rotaryP = rotaryPosition / 100.0f;  // Convert to a percentage

                        PID_Control(pid_speed_ptr, TargetSpeed, fanrpm);
                        FanPWM.write(pid_output);

                        if ( std::chrono::duration_cast<std::chrono::milliseconds>(
                            printTimer.elapsed_time()) >= 3000ms) {
                            printf("Average RPM: %g\n", fanrpm);
                            printf("PWM duty %g\n", pid_output);
                            printTimer.reset();
                             }

                    #endif
                    break;

                case 2:
                    // Mode 2: PID Temperature Limiting  
                    #ifdef TACHO_DEBUG
                        // Call relevant PID control function
                        Calculate_Fan_RPM();
                        Rotary_Input();  // Update encoder position

                    int rotaryPosition = RotaryInput_GetPosition();  // Get the current encoder position
                    if (rotaryPosition > 99) {
                        rotaryPosition = 99;  // Cap at maximum
                    }

                    float rotaryP = rotaryPosition / 100.0f;  // Convert to a percentage
                    #endif
                    break;

                case 3:
                    // Mode 3: Timer display (e.g., TIMER mode)
                    #ifdef TIMER_DEBUG
                        Rotary_Input();  // Update encoder position

                        int rotaryPosition = RotaryInput_GetPosition();  // Get the current encoder position
                        if (rotaryPosition > 99) {
                            rotaryPosition = 99;  // Cap at maximum
                        }

                        float rotaryP = rotaryPosition / 100.0f;  // Convert to a percentage
                        printf("Timer Value: %d\n", timer_value); // Replace `timer_value` with actual variable
                    #endif
                    break;

                default:
                    Button_Mode = 1;
                    break;
            }
    }
}

