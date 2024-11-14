#include "mbed.h"
#include "taco_test.h"
#include "timer.h"
#include "pid_controller.h"
#include "pins_config.h"
#include "rotary_input.h"
#include "ButtonInput.h"
#include "temp_sensor.h"
#include <chrono>
#include <cstdio>

#define TargetSpeed 1500



//** Tasks to think about
// LCD Display
// hold to activate timer
// Timer display on 7 seg
// Temperature readings
// low speed pi controller

//**** ideal pwm parameters for low speed
// FanPWM.period(0.002);
// FanPWM.write(0.00111);

//global variables

int Button_Mode = 0;
int target_value;
int current_temp;
int target_temp;

float rotaryP;
float duty_cycle;
Timer printTimer; 
PwmOut FanPWM(PB_0);
bool start_timer = true;
int set_timer = 10;

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
        Init_Rotary_Input(Button_Mode);
    #endif

    FanPWM.period(0.002);
    //FanPWM.write(0.00111);
   
    
    while (true) {   
         //FanPWM.write(1);
        #ifdef TIMER_DEBUG
            printf("Timer Value %d\n", timer_value);
        #endif

        // Check if the button was pressed to change the mode
        if (WasButtonPressed()) {
            Button_Mode++;     
            if (Button_Mode > 4) {
                Button_Mode = 0;  // Wrap around to mode 1 after mode 3
            }
            switch (Button_Mode){
            case 0:
                // Mode 0: Open Loop Speed Control
                break;
            case 1:
                // Mode 1: PID Speed Control
                Init_PID_Controller(pid_speed_ptr, speed_controller_params);
                break;
            case 2:
                // Mode 2: PID Temperature Limiting  
                Init_PID_Controller(pid_temp_ptr, temp_controller_params);
                break;
            case 3:
                // Mode 3: Timer display (e.g., TIMER mode)
                break;

            default:
                // Mode 0: Open Loop Speed Control
                
                break;
        }
            #ifdef ROTARY_DEBUG
                Init_Rotary_Input(Button_Mode);
            #endif
            

            printf("Switched to mode %d\n", Button_Mode);
        }
            switch (Button_Mode) {
                case 0:
                    Calculate_Fan_RPM();
                    Rotary_Input();  // Update encoder position
                    target_value = RotaryInput_GetPosition();  // Get the current encoder position
                    duty_cycle = target_value/100.0;
                    FanPWM.write(duty_cycle);
                    if ( std::chrono::duration_cast<std::chrono::milliseconds>(
                            printTimer.elapsed_time()) >= 3000ms) {
                            printf("Average RPM: %g\n", fanrpm);
                            //printf("PWM duty %g\n", pid_output);
                            //printf("rotary position: %d\n", target_value);
                            printTimer.reset();
                             }
                    break;

                case 1:
                    // Mode 1: PID Speed Control
                    #ifdef TACHO_DEBUG
                        
                        Calculate_Fan_RPM();
                        
                        Rotary_Input();  // Update encoder position
                        target_value = RotaryInput_GetPosition();  // Get the current encoder position

                        PID_Control(pid_speed_ptr, target_value, fanrpm);
                        FanPWM.write(pid_output);

                        if ( std::chrono::duration_cast<std::chrono::milliseconds>(
                            printTimer.elapsed_time()) >= 3000ms) {
                            printf("Average RPM: %g\n", fanrpm);
                            //printf("PWM duty %g\n", pid_output);
                            //printf("rotary position: %d\n", target_value);
                            printTimer.reset();
                             }
                    #endif
                    break;

                case 2:
                    // Mode 2: PID Temperature Limiting
                    // Read Temperature
                    Rotary_Input();  // Update encoder position
                    current_temp = Read_Temperature();
                    target_temp = RotaryInput_GetPosition();

                    // PID Controller takes temp_pid parameters, current temperature, and input value
                    // if (current_temp > target_temp) {
                    // PID_Control(pid_temp_ptr, current_temp , target_temp);
                    // }
                    PID_Control(pid_temp_ptr, current_temp , target_temp);
                
                    
                    FanPWM.write(pid_output);
                    break;

                case 3:
                    // Mode 3: Timer display (e.g., TIMER mode)
                        Rotary_Input();  // Update encoder position
                        start_timer = true;
                        target_value = RotaryInput_GetPosition();  // Get the current encoder position
                   
                    break;
                
                case 4:
                    if (start_timer == true) {
                        Start_Timer(target_value);
                        start_timer = false;
                    }
                    if (timer_value == 0) {
                        duty_cycle = 0.3;
                        FanPWM.write(duty_cycle);
                    }
                    else{
                        duty_cycle = 1.0;
                        FanPWM.write(duty_cycle);
                    }
                    if ( std::chrono::duration_cast<std::chrono::milliseconds>(
                            printTimer.elapsed_time()) >= 1000ms) {
                            printf("timer_value =%d\n",timer_value);
                            printf("PWM duty %g\n", duty_cycle);
                            //printf("rotary position: %d\n", target_value);
                            printTimer.reset();
                            }
                    break;

                default:
                    Button_Mode = 0;
                    break;
            }
    }
}

