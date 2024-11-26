
#include "mbed.h"
#include "taco_test.h"
#include "timer.h"
#include "pid_controller.h"
#include "pins_config.h"
#include "rotary_input.h"
#include "ButtonInput.h"
#include "temp_sensor.h"
#include "LCDText.h"
#include "low_speed_pulse.h"
#include "sev_seg_display.h"
#include <chrono>
#include <cstdint>
#include <cstdio>

#define TargetSpeed 1500


//** Tasks to think about

// Improve speed measure to +/- 1rpm
// complete mode 3 to 5 transition
// reduce rpm period
// tune pi controller
//  - Fire pwm  every time tacho is measured, use pid to adjust pulse width (duty)
//  - 


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
bool start_timer = true;
int set_timer = 10;
int previousButtonMode = -1;

TextLCD lcd(PB_15, PB_14, PB_5, PB_4, PB_10, PA_8);


BusOut TensDigit(PinName c5, PinName c6, PinName c7, PinName c8,
                 PinName c10, PinName c11, PinName c12);

BusOut OnesDigit(PB_11, PB_12, PA_11, PA_12, PA_6, PA_7, PB_6);

PinName a = PA_6;
BusOut test(a,a);

//LCD::LCD(PinName rs, PinName e, PinName d4, PinName d5, PinName d6, PinName d7): _rs(rs), _e(e), _d4(d4), _d5(d5), _d6(d6), _d7(d7), _row(0), _column(0)
//LCD lcd(PB_15, PB_14, PB_10, PA_8, PA_2, PB_2, PB_1);

SevSeg sevseg(PB_11, PB_12, PA_11, PA_12, PA_6, PA_7, PB_6, PC_5, PC_6, PB_1,PC_8, PC_4, PA_10, PB_2);




int main() {
test.write(7);
    sevseg.InitSegDict();
    sevseg.SevSegWriteOnes(sev_seg_dict[Button_Mode]);
    sevseg.SevSegWriteTens(seg_zero);
    printTimer.start();


    lcd.cls();            // Clear screen
    lcd.locate(0, 0);     // Move cursor to (0,0)
    lcd.printf(" Hello"); 
    
   // lcd.cls();            // Clear screen
   // lcd.locate(-1, 0);     // Move cursor to (0,0)
    //lcd.printf(" Hello");  // Display "Hello" on the screen

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
            if (Button_Mode > 5) {
                Button_Mode = 0;  // Wrap around to mode 1 after mode 3
            }
            switch (Button_Mode){
            case 0:
                // Mode 0: Open Loop Speed Control
                Init_Calculate_Fan_RPM();
                Detach_Low_Speed_Pulses();
                sevseg.SevSegWriteOnes(seg_zero);
                sevseg.SevSegWriteTens(seg_zero);
                low_speed_mode = false;
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
                sevseg.SevSegWriteOnes(seg_one);
                sevseg.SevSegWriteTens(seg_zero);
                break;
            case 2:
                // Mode 2: PID Temperature Limiting  
                Init_PID_Controller(pid_temp_ptr, temp_controller_params);
                sevseg.SevSegWriteOnes(seg_two);
                sevseg.SevSegWriteTens(seg_zero);
                current_temp = Read_Temperature();
                break;
            case 3:
                // Mode 3: Timer display (e.g., TIMER mode)
                lcd.locate(0, 0);  // Move the cursor to the first row
                lcd.printf("Max Speed Timer Mode");  //
                sevseg.SevSegWriteOnes(seg_three);
                sevseg.SevSegWriteTens(seg_zero);
                break;
            case 4:
                break;
            case 5:
                FanPWM.write(0);
                Init_Low_Speed_Pulses();
                sevseg.SevSegWriteOnes(seg_five);
                sevseg.SevSegWriteTens(seg_zero);
                low_speed_mode = true;
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
                    
                    Rotary_Input();  // Update encoder position
                    target_value = RotaryInput_GetPosition();  // Get the current encoder position
                    duty_cycle = target_value/100.0;
                    if (target_value > 0 && fanrpm <= 10.0){
                        Kick_Start_pulse(fanrpm);
                    }
                    else {
                     FanPWM.write(duty_cycle);
                    }

                   
                    if ( std::chrono::duration_cast<std::chrono::milliseconds>(
                            printTimer.elapsed_time()) >= 1000ms) {
                            printf("Average RPM: %g\n", fanrpm);
                            //printf("PWM duty %g\n", pid_output);
                            //printf("rotary position: %d\n", target_value);
                            lcd.cls();
                            wait_us(2000);
                            lcd.locate(0, 0);  // Move the cursor to the first row
                            lcd.printf("RPM: %.1f", fanrpm);  // Print current fan RPM (add trailing spaces to clear leftovers)
                            lcd.locate(0, 1);  // Move the cursor to the first row
                            lcd.printf("Duty Cycle: %.1f", duty_cycle);  //
                            printTimer.reset();
                             }

                    break;

                case 1:
                    // Mode 1: PID Speed Control
                    #ifdef TACHO_DEBUG
                 
                        Rotary_Input();  // Update encoder position
                        target_value = RotaryInput_GetPosition();  // Get the current encoder position
                        if (target_value > 0 && fanrpm <= 10.0){
                        Kick_Start_pulse(fanrpm);
                        }
                        else {
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
                        }
                        
                        
                        if ( std::chrono::duration_cast<std::chrono::milliseconds>(
                            printTimer.elapsed_time()) >= 500ms) {
                            printf("Average RPM: %g\n", fanrpm);
                            printf("Average Duty: %g\n", duty_cycle);
                            //printf("PWM duty %g\n", pid_output);
                            //printf("rotary position: %d\n", target_value);
                            lcd.cls();
                            wait_us(2000);
                            lcd.locate(0, 0);  // Move the cursor to the first row
                            lcd.printf("Current RPM:%0.0f", fanrpm);  // Print current fan RPM (add trailing spaces to clear leftovers)

                            lcd.locate(0, 1);  // Move the cursor to the second row
                            lcd.printf("Desired RPM:%d", target_value);  // Print the PWM duty cycle (add trailing spaces to clear leftovers)
                            printTimer.reset();
                             }
                    #endif
                    break;

                case 2:
                    // Mode 2: PID Temperature Limiting
                    // Read Temperature
                    Rotary_Input();  // Update encoder position

                    target_temp = RotaryInput_GetPosition();


                    // PID Controller takes temp_pid parameters, current temperature, and input value
                    // if (current_temp > target_temp) {
                    // PID_Control(pid_temp_ptr, current_temp , target_temp);
                    // }
                    duty_cycle = PID_Control(pid_temp_ptr, current_temp , target_temp, false);
                
                    if (target_value < target_temp && fanrpm <= 10.0){
                        Kick_Start_pulse(fanrpm);
                    }
                    else {
                     FanPWM.write(duty_cycle);
                    }
                
                    if ( std::chrono::duration_cast<std::chrono::milliseconds>(
                            printTimer.elapsed_time()) >= 1000ms) {
                            lcd.cls();
                            wait_us(2000);
                            lcd.locate(0, 0);  // Move the cursor to the first row
                            lcd.printf("Target Temp:%d", target_temp);  // Print current fan RPM (add trailing spaces to clear leftovers)
                            lcd.locate(0, 1);  // Move the cursor to the second row
                            lcd.printf("Current Temp:%d", current_temp);  // Print the PWM duty cycle (add trailing spaces to clear leftovers)
                            current_temp = Read_Temperature();
                            printTimer.reset();
                            }


                    break;

                case 3:
                    // Mode 3: Timer display (e.g., TIMER mode)
                        Rotary_Input();  // Update encoder position
                        start_timer = true;
                        target_value = RotaryInput_GetPosition();  // Get the current encoder position
                        duty_cycle = 0.3;
                        FanPWM.write(duty_cycle);
                        if (target_value == 0) {
                        sevseg.SevSegWriteOnes(seg_three);
                        }
                        else {
                         sevseg.SevSegWriteOnes(sev_seg_dict[target_value % 10]);
                         sevseg.SevSegWriteTens(sev_seg_dict[(target_value / 10) % 10 ]);
                        }
                   
                    break;
                
                case 4:
                    if (start_timer == true) {
                        Start_Timer(target_value);
                        start_timer = false;
                    }
                    sevseg.SevSegWriteOnes(sev_seg_dict[timer_value % 10]);
                    sevseg.SevSegWriteTens(sev_seg_dict[(timer_value / 10) % 10]);
                    if (timer_value == 0) {
                        timer_tick.detach();
                        duty_cycle = 0.3;
                        // Button_Mode = 3;
                        // Init_Rotary_Input(Button_Mode);
                    }
                    else{
                        duty_cycle = 1.0;
                        FanPWM.write(duty_cycle);
                    }      
                    if ( std::chrono::duration_cast<std::chrono::milliseconds>(
                            printTimer.elapsed_time()) >= 1000ms) {
                            printf("timer_value =%d\n",timer_value);
                            printf("PWM duty %g\n", duty_cycle);
                            printf("Temp: %d\n" , data[0] );
                            //printf("rotary position: %d\n", target_value);
                            printTimer.reset();
                            }
                    break;
                case 5:
                     if (target_value > 0 && fanrpm <= 10.0){
                        Kick_Start_pulse(fanrpm);
                    }

                     if ( std::chrono::duration_cast<std::chrono::milliseconds>(
                            printTimer.elapsed_time()) >= 3000ms) {
                            printf("Average RPM: %g\n", fanrpm);
                            printf("Pulse Count = : %d\n", pulse_count);
                            printTimer.reset();
                             }
                    
                    break;


                default:
                    Button_Mode = 0;
                    break;
            }
    }
}


