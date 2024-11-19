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

//WORKING:  PID lowspeed_controller_params = {.error = 0, .Kp = 0.000007, .Ki = 0.00000008, .Kd = 0, .d_error = 0, .prev_error = 0};
//WORKING:  FanPWM.period(0.00242); 
//else if (pid_output <= 0.0004) {
//            pid_output = 0.0004;
//       } - speed 60rpm
//next steps: rotary poistion: 0, LOW SPEED MODE, 100, 150...
// pulse to kick in at LOW SPEED MODE, 100, 150(?)
// with current PI config, big overshoot at 500-1000rpm 
// high speed PID still to do

int Button_Mode = 0;
int target_value;
int current_temp;
int target_temp;
float pwm_period;
bool init_low_PID = true;
bool init_high_PID = true;

float rotaryP;
Timer printTimer; 
PwmOut FanPWM(PB_0);
bool start_timer = true;
int set_timer = 10;

//LCD::LCD(PinName rs, PinName e, PinName d4, PinName d5, PinName d6, PinName d7): _rs(rs), _e(e), _d4(d4), _d5(d5), _d6(d6), _d7(d7), _row(0), _column(0)
//LCD lcd(PB_15, PB_14, PB_10, PA_8, PA_2, PB_2, PB_1);

int main() {
    printTimer.start();

    InitializeButtonInput();
    // Runs Tacho mode when TACHO_DEBUG is defined in "pins_config.h" (only define one at a time)
    #ifdef TACHO_DEBUG
        // Tacho
        Init_Calculate_Fan_RPM();
        Init_PID_Controller(pid_lowspeed_ptr, lowspeed_controller_params);
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

    FanPWM.period(0.00242);
    FanPWM.write(0.00111);
   
    
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
                if (target_value >= 1000){
                    Init_PID_Controller(pid_highspeed_ptr, highspeed_controller_params);
                }
                else{
                    Init_PID_Controller(pid_lowspeed_ptr, lowspeed_controller_params);
                }
                printf("target value = %d\n", target_value);
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

                        if (target_value >= 1000){
                            init_low_PID = true;

                            if(init_high_PID){
                            Init_PID_Controller(pid_highspeed_ptr, highspeed_controller_params);
                            init_high_PID = false;
                            }

                            duty_cycle = PID_Control(pid_highspeed_ptr, target_value, fanrpm, false);
                            //FanPWM.period(0.002);
                            FanPWM.write(duty_cycle);
                            
                        }
                        else{
                            init_high_PID = true;
                            if (init_low_PID) {
                                Init_PID_Controller(pid_lowspeed_ptr, lowspeed_controller_params);
                                init_low_PID = false;
                            }

                            duty_cycle = PID_Control(pid_lowspeed_ptr, target_value, fanrpm, true);
                            FanPWM.write(duty_cycle);
                           
                        }
                        

                        if ( std::chrono::duration_cast<std::chrono::milliseconds>(
                            printTimer.elapsed_time()) >= 3000ms) {
                            printf("Average RPM: %g\n", fanrpm);
                            printf("Average Duty: %g\n", duty_cycle);
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
                    duty_cycle = PID_Control(pid_temp_ptr, current_temp , target_temp, false);
                
                    
                    FanPWM.write(duty_cycle);
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

