#include "low_speed_pulse.h"
#include "taco_test.h"
#include "pins_config.h"
#include <chrono>

Timer low_speed_timer;
Timer kick_start_timer;
Timeout low_speed_timeout;
std::chrono::microseconds low_speed_pulse_period = 10000us;
bool low_speed_mode;

void Low_Speed_Start_Pulse(){
    // if (std::chrono::duration_cast<std::chrono::milliseconds>(
    //          low_speed_timer.elapsed_time()) >= 100ms){
    //         FanPWM.write(1);
    //         low_speed_timeout.attach(&Low_Speed_Stop_Pulse, 100ms);
    //         low_speed_timer.reset();
    // }

    //     if (current_speed <= 10.0) {
    //     if(std::chrono::duration_cast<std::chrono::milliseconds>(
    //         kick_start_timer.elapsed_time()) >= 20ms){
    //         FanPWM.write(1);
    //         low_speed_timeout.attach(&Low_Speed_Stop_Pulse, 10ms);
    //         kick_start_timer.reset();
    //         }        
    // }
    // else{
    //     if(std::chrono::duration_cast<std::chrono::milliseconds>(
    //         kick_start_timer.elapsed_time()) >= 2ms){
    //         FanPWM.write(1);
    //         low_speed_timeout.attach(&Low_Speed_Stop_Pulse, 170us);
    //         kick_start_timer.reset();

            
    //         }

}

void Low_Speed_Stop_Pulse(){
    FanPWM.write(0);

}

void Kick_Start_pulse(float current_speed){

    kick_start_timer.start(); 

        if(std::chrono::duration_cast<std::chrono::milliseconds>(
            kick_start_timer.elapsed_time()) >= 100ms){
            FanPWM.write(0.4);
            low_speed_timeout.attach(&Low_Speed_Stop_Pulse, 70ms);
            kick_start_timer.reset();
            }        
 
}

void Init_Low_Speed_Pulses(){
    low_speed_timer.start(); 
}

