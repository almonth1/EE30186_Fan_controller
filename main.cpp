
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
#include "settings_page.h"
#include <chrono>
#include <cstdint>
#include <cstdio>

#define TargetSpeed 1500


//** Tasks to think about

// fix low speed measure
// Improve speed measure to +/- 1rpm
// tune pi controller
// Both rising and falling for tacho pulse?


//** Extensions
// voltage monitor?
// settings mode to change params (rotary sensitivity, OTW threshold, Abnormal speed threshold)
// help page for controller (on button hold and start up)
// OTW



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

int prev_button_mode = 0;
float target_value;
int current_temp;

float previous_target = -1;
float previous_fan_rpm = 0;
int previous_temp;
int previous_page;


float pwm_period;
bool init_low_PID = true;
bool init_high_PID = true;


float rotaryP;
Timer printTimer; 
bool start_timer = true;
int set_timer = 10;


//LCD::LCD(PinName rs, PinName e, PinName d4, PinName d5, PinName d6, PinName d7): _rs(rs), _e(e), _d4(d4), _d5(d5), _d6(d6), _d7(d7), _row(0), _column(0)
//LCD lcd(PB_15, PB_14, PB_10, PA_8, PA_2, PB_2, PB_1);

SevSeg sevseg(PB_11, PB_12, PA_11, PA_12, PA_6, PA_7, PB_6, PC_5, PC_6, PB_1,PC_8, PC_4, PA_10, PB_2);

void OnButtonHeldHandler(){
    if (WasButtonHeld()) {
        if (settingsModeFlag) {
            Button_Mode = 10;        
            lcd.cls();              // Clear screen
            wait_us(2000);          
            lcd.locate(0, 0);     // Move cursor to (0,0)
            lcd.printf("Loading Settings..."); 
            wait_us(2000000);
            Init_Rotary_Input(Button_Mode);
            target_value = RotaryInput_GetPosition();
            PageHandler(page_number,int(target_value));
        }
        else {        
            previous_target = -1;
            lcd.cls();              // Clear screen
            wait_us(2000);          
            lcd.locate(0, 0);     // Move cursor to (0,0)
            lcd.printf("Exiting Settings..."); 
            wait_us(1000000);
        }
    
    

    }
    
}
void OnButtonPressHandler(){
    // Check if the button was pressed to change the mode
   
    if (WasButtonPressed() || (Button_Mode != prev_button_mode && !settingsModeFlag)) {
                if (settingsModeFlag) {
                    switch (Button_Mode) {
                        case 10:
                            if (floorf(target_value) == 1) {
                                BackButton();
                            }
                            else if (floorf(target_value) == 2) {
                                previous_target = -1;
                                SelButton(page_number);
                                Init_Rotary_Input(Button_Mode);
                            }
                            else if (floorf(target_value) == 3) {
                                NextButton(); 
                            }
                            break;

                        case 11:
                            previous_target = -1;
                            Temperature_Warning = target_value;
                            Button_Mode -= page_number;
                            buttonPressedFlag = true;
                            Init_Rotary_Input(Button_Mode);
                            
                            break;

                        case 12:
                            previous_target = -1;
                            Abnormal_Speed = target_value;
                            Button_Mode -= page_number;
                            buttonPressedFlag = true;
                            Init_Rotary_Input(Button_Mode);
                            
                            break;

                        case 13:
                            previous_target = -1;
                            Button_Mode -= page_number;
                            buttonPressedFlag = true;
                            Init_Rotary_Input(Button_Mode);
                            break;

                        case 14:
                            previous_target = -1;
                            Button_Mode -= page_number;
                            buttonPressedFlag = true;
                            Init_Rotary_Input(Button_Mode);
                            break;
                            
                        default:
                            lcd.cls();              // Clear screen
                            wait_us(2000);          
                            lcd.locate(0, 0);     // Move cursor to (0,0)
                            lcd.printf("Error Unknown State..."); 
                    }
                    buttonPressedFlag = false;
                }
                else{
                    if (Button_Mode == prev_button_mode) {
                        Button_Mode++; 
                        buttonPressedFlag = false;
                    }
                    else {
                        Button_Mode = prev_button_mode;
                    }
                    if (Button_Mode > 4) {
                        Button_Mode = 0;  // Wrap around to mode 1 after mode 3
                    }
                    switch (Button_Mode){
                    case 0:
                        // Mode 0: Open Loop Speed Control
                        prev_button_mode = 0;
                        ResetTimer();
                        Init_Calculate_Fan_RPM();
                        sevseg.SevSegWriteOnes(seg_zero);
                        sevseg.SevSegWriteTens(seg_zero);
                        lcd.cls();
                        wait_us(2000);
                        lcd.locate(0, 0);  
                        lcd.printf("Open Loop Mode"); 
                        wait_us(1000000);
                        break;
                    case 1:
                        // Mode 1: PID Speed Control
                        previous_target = -1;
                        prev_button_mode = 1;
                        if (target_value >= 1000){
                            Init_PID_Controller(pid_highspeed_ptr, highspeed_controller_params);
                        }
                        else{
                            Init_PID_Controller(pid_lowspeed_ptr, lowspeed_controller_params);
                        }
                        sevseg.SevSegWriteOnes(seg_one);
                        sevseg.SevSegWriteTens(seg_zero);
                        lcd.cls();
                        wait_us(2000);
                        lcd.locate(0, 0);  
                        lcd.printf("Speed Control"); 
                        lcd.locate(0, 1);  
                        lcd.printf("Mode");
                        wait_us(1000000);
                        break;
                    case 2:
                        // Mode 2: PID Temperature Limiting  
                        prev_button_mode = 2;
                        i_error = 0;
                        previous_target = -1;     
                        Init_PID_Controller(pid_temp_ptr, temp_controller_params);
                        sevseg.SevSegWriteOnes(seg_two);
                        sevseg.SevSegWriteTens(seg_zero);
                        lcd.cls();
                        wait_us(2000);
                        lcd.locate(0, 0);  
                        lcd.printf("Temp Limit Mode"); 
                        wait_us(1000000);
                        break;
                    case 3:
                        // Mode 3: Timer display (e.g., TIMER mode)
                        i_error = 0;
                        prev_button_mode = 3;
                        previous_target = -1;
                        start_timer = true;
                        sevseg.SevSegWriteOnes(seg_three);
                        sevseg.SevSegWriteTens(seg_zero);
                        lcd.cls();
                        wait_us(2000);
                        lcd.locate(0, 0);  
                        lcd.printf("Timer Mode:");  //
                        lcd.locate(0, 1); 
                        lcd.printf("Set Time");
                        break;

                    case 4:
                        prev_button_mode = 4;
                        lcd.cls();
                        wait_us(10000);
                        lcd.locate(0, 0);  
                        lcd.printf("Fan Running...");  //
                        if (target_value == 0) {
                            prev_button_mode = 0;
                        }
                        break;

                    default:
                        // Error Message
                        lcd.cls();
                        wait_us(10000);
                        lcd.locate(0, 0);  
                        lcd.printf("Error Unkwon Mode");
                        break;
                }
                Init_Rotary_Input(Button_Mode);
                printf("Switched to mode %d\n", Button_Mode);
            }
    }
}
void ButtonModeHandler(){
                switch (Button_Mode) {
                    case 0:      
                        target_value = RotaryInput_GetPosition();  // Get the current encoder position
                        duty_cycle = target_value/100.0;
                        if (target_value > 0 && fanrpm <= 10.0){
                            Kick_Start_pulse(fanrpm);
                        }
                        else {
                            FanPWM.write(duty_cycle);
                        }

                        if (fanrpm != previous_fan_rpm || target_value != previous_target) {
                                lcd.cls();
                                wait_us(2000);
                                lcd.locate(0, 0);  // Move the cursor to the first row
                                lcd.printf("Current RPM:%0.0f", fanrpm);  // Print current fan RPM (add trailing spaces to clear leftovers)
                                lcd.locate(0, 1);  // Move the cursor to the second row
                                if (target_value < 100) {
                                lcd.printf("Duty Cycle:%0.1f%%", target_value);
                                }
                                else if (target_value == 100) {
                                lcd.printf("Duty Cycle:100%%");
                                }
                                


                                // Update the previous values
                                previous_fan_rpm = fanrpm;
                                previous_target = target_value; 
                        }               
                        break;

                    case 1:
                        // Mode 1: PID Speed Control

                            target_value = RotaryInput_GetPosition();  // Get the current encoder position
                            if (target_value > 0 && fanrpm <= 10.0){
                            Kick_Start_pulse(fanrpm);
                            i_error = 0;
                            }
                            else {

                                if (target_value >= low_speed_threshold){
                                    init_low_PID = true;

                                    if(init_high_PID){
                                    Init_PID_Controller(pid_highspeed_ptr, highspeed_controller_params);
                                    init_high_PID = false;
                                    }


                                    duty_cycle = PID_Control(pid_highspeed_ptr, target_value, fanrpm, true);
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

                                if (fanrpm != previous_fan_rpm || target_value != previous_target) {
                                    lcd.cls();
                                    wait_us(2000);
                                    lcd.locate(0, 0);  // Move the cursor to the first row
                                    lcd.printf("Current RPM:%0.0f", fanrpm);  // Print current fan RPM (add trailing spaces to clear leftovers)

                                    lcd.locate(0, 1);  // Move the cursor to the second row
                                    lcd.printf("Desired RPM:%0.0f", target_value);  // Print the target RPM (add trailing spaces to clear leftovers)

                                    // Update the previous values
                                    previous_fan_rpm = fanrpm;
                                    previous_target = target_value;
                                }          
                                if ( std::chrono::duration_cast<std::chrono::milliseconds>(
                                    printTimer.elapsed_time()) >= 1000ms) {
                                    //printf("Average RPM: %g\n", fanrpm);
                                    //printf("Pulse Width: %g\n", pulse_width);
                                    printf("error %d\n", pid_lowspeed_ptr ->error);
                                    printf("i error %f\n", i_error);
                                    printf("d error %d\n", pid_lowspeed_ptr->d_error);
                                    printf("duty:%0.5f\n", duty_cycle);
                                    // printf("pulse vector =\n 1:%g\n 2:%g\n 3:%g\n",pulse_width_vector[0], pulse_width_vector[1], pulse_width_vector[2]);
                                    printTimer.reset();
                                }    

                        break;
                    case 2:
                        // Mode 2: PID Temperature Limiting
                        // Read Temperature
                        target_value = RotaryInput_GetPosition();

                        // PID Controller takes temp_pid parameters, current temperature, and input value
                        // if (current_temp > target_temp) {
                        // PID_Control(pid_temp_ptr, current_temp , target_temp);
                        // }
                        duty_cycle = PID_Control(pid_temp_ptr, current_temp, int(target_value), false);
                    
                        if (current_temp > target_value && fanrpm <= 10.0){
                            Kick_Start_pulse(fanrpm);

                        }
                        else {
                        FanPWM.write(duty_cycle);
                        }
         
                        if (current_temp != previous_temp || target_value != previous_target) {
                                lcd.cls();
                                wait_us(2000);
                                lcd.locate(0, 0);  // Move the cursor to the first row
                                lcd.printf("Target Temp:%d", int(target_value));  // Print current fan RPM (add trailing spaces to clear leftovers)
                                lcd.locate(0, 1);  // Move the cursor to the second row
                                lcd.printf("Current Temp:%d", current_temp);  // Print the PWM duty cycle (add trailing spaces to clear leftovers)       
                                // Update the previous values
                                previous_temp = current_temp;
                                previous_target = target_value;
                            }     
                         if ( std::chrono::duration_cast<std::chrono::milliseconds>(
                                    printTimer.elapsed_time()) >= 1000ms) {
                                    //printf("Average RPM: %g\n", fanrpm);
                                    //printf("Pulse Width: %g\n", pulse_width);
                                    printf("error %d\n", pid_lowspeed_ptr ->error);
                                    printf("i error %f\n", i_error);
                                    
                                    printf("duty:%0.5f\n", duty_cycle);
                                    // printf("pulse vector =\n 1:%g\n 2:%g\n 3:%g\n",pulse_width_vector[0], pulse_width_vector[1], pulse_width_vector[2]);
                                    printTimer.reset();
                                }    
                        break;

                    case 3:
                        // Mode 3: Timer display (e.g., TIMER mode)
                        
                            target_value = RotaryInput_GetPosition();  // Get the current encoder position
                        
                            duty_cycle = 0.2;
                            FanPWM.write(duty_cycle);
                            if (target_value == 0) {
                            sevseg.SevSegWriteOnes(seg_three);
                            }
                            else {
                            sevseg.SevSegWriteOnes(sev_seg_dict[int(target_value) % 10]);
                            sevseg.SevSegWriteTens(sev_seg_dict[(int(target_value) / 10) % 10 ]);
                            }                    
                        break;
                    
                    case 4:   
                        sevseg.SevSegWriteOnes(sev_seg_dict[timer_value % 10]);
                        sevseg.SevSegWriteTens(sev_seg_dict[(timer_value / 10) % 10]);
                        if (target_value != 0) {
                            if (start_timer == true) {
                                Start_Timer(target_value);
                                start_timer = false;
                            }
                            if (timer_value == 0) {
                            timer_tick.detach();
                            wait_us(100000);                                   
                            prev_button_mode = 3;
                            }
                            else{
                                duty_cycle = 1.0;
                                FanPWM.write(duty_cycle);
                                lcd.cls();
                                wait_us(2000);
                                lcd.locate(0, 0);  
                                lcd.printf("Fan Running...");  //
                            }   
                        }
                        break;
                    case 10:
                        target_value = RotaryInput_GetPosition();  // Get the current encoder position
                        if (page_number != previous_page || target_value != previous_target) {
                                PageHandler(page_number, int(target_value));
                                previous_page = page_number;
                                previous_target = target_value;
                            } 
                        break;
                    case 11:
                    //OTW Settings page
                        target_value = RotaryInput_GetPosition();  // Get the current encoder position
                        if (target_value != previous_target) {
                            ShowOTWPageOne(int(target_value));
                            previous_target = target_value;
                        }

                        break;
                    case 12:
                    //Abnormal Speed Settings page
                        target_value = RotaryInput_GetPosition();  // Get the current encoder position
                        if (target_value != previous_target) {
                            ShowAbSpeedPageTwo(int(target_value));
                            previous_target = target_value;
                        }
    
                       
                        break;
                    case 13:
                    //Voltage Stats Settings page
                      
                        if (duty_cycle*100 != previous_target) {
                            ShowStatsPageThree(duty_cycle);
                            previous_target = duty_cycle*100;
                        }

                        break;
                    case 14:
                        // settings: help page 
                        if (0 != previous_target) {
                         ShowHelpPageFour();
                         previous_target = 0;
                        }

                        break;
                    default:
                        Button_Mode = 0;
                        break;
                }
    
}

int main() {
    
    wait_us(2000);          // Clear screen
    lcd.cls(); 
    wait_us(2000);          // Clear screen
    lcd.locate(0, 0);     // Move cursor to (0,0)
    lcd.printf("Welcome To Fan  Controller!"); 
    wait_us(2000000);

    lcd.cls(); 
    wait_us(2000);          // Clear screen
    lcd.locate(0, 0);     // Move cursor to (0,0)
    lcd.printf("BTN => Mode"); 
    lcd.locate(0, 1);     // Move cursor to (0,0)
    lcd.printf("ROT => Val"); 
    wait_us(2000000);
    
    
    sevseg.InitSegDict();
    sevseg.SevSegWriteOnes(sev_seg_dict[Button_Mode]);
    sevseg.SevSegWriteTens(seg_zero);

    Write_Temp_Command();

    led_Ext.write(0);

    InitializeButtonInput();
    Init_Calculate_Fan_RPM();
    Init_Timer_Mode();
    Init_Rotary_Input(Button_Mode);
    

    FanPWM.period(0.00242);
    //FanPWM.write(0.00111);

    printTimer.start();
   
    while (true) {   
        current_temp = Read_Temperature();  
        if (current_temp > Temperature_Warning && !settingsModeFlag) {
            FanPWM.write(1);
            OTW_tick.attach(&OTW_Blinky,300ms);
            lcd.cls();
            wait_us(2000);
            lcd.locate(0, 0);  
            lcd.printf("Over-Temperature"); 
            lcd.locate(2,1);
            lcd.printf("! Temp:%dC !", current_temp);
            wait_us(500000);
            lcd.cls();
            wait_us(500000);
            }
        else if (fanrpm > Abnormal_Speed && !settingsModeFlag) {
            FanPWM.write(0);
            OTW_tick.attach(&OTW_Blinky,300ms);
            lcd.cls();
            wait_us(2000);
            lcd.locate(0, 0);  
            lcd.printf("Abnormal Speed"); 
            lcd.locate(2,1);
            lcd.printf("! RPM:%f0.0 !", fanrpm);
            wait_us(500000);
            lcd.cls();
            wait_us(500000);
        }
        else {
            OTW_tick.detach();
            led_Ext.write(1);
            OnButtonHeldHandler();
            OnButtonPressHandler();
            ButtonModeHandler();
        }
    }
}


