#include "pid_controller.h"
#include <chrono>
#include <cstdint>
#include <cstdio>


Timer pid_timer;
float pid_output = 0;
float duty_cycle;
float bias = 0.03;
float i_error = 0;

PID lowspeed_controller_params = {.error = 0, .Kp = 0.00058, .Ki = 0.00000, .Kd = 0.001, .d_error = 0, .prev_error = 0};
// PID values is good for low speed, not high speed
PID* pid_lowspeed_ptr = &lowspeed_controller_params;

//PID highspeed_controller_params = {.error = 0, .Kp = 0.00007, .Ki = 0.000015, .Kd = 0.000005, .d_error = 0, .prev_error = 0};

PID highspeed_controller_params = {.error = 0, .Kp = 0.001, .Ki = 0.000134, .Kd = 0.00000, .d_error = 0, .prev_error = 0};

// PID values is good for low speed, not high speed
PID* pid_highspeed_ptr = &highspeed_controller_params;

PID temp_controller_params = {.error = 0, .Kp = 0.02, .Ki = 0.0001, .Kd = 0, .d_error = 0, .prev_error = 0};
PID* pid_temp_ptr = &temp_controller_params;

// Initializes PID Speed controller parameters 
void Init_PID_Controller(PID *pid_params, PID param_init){
    *pid_params = param_init; 
    pid_output = 0;
    pid_timer.start();
}

void PID_Dummy(){
    printf("PID running! :)");
    wait_us(1000000);
};

float PID_Control( PID *pid_params, uint16_t target_value, uint16_t current_value, bool speed){

     if ( std::chrono::duration_cast<std::chrono::microseconds>(pid_timer.elapsed_time()) >= pid_period) {
        
        pid_params->error = target_value - current_value;
        i_error += pid_params->error;

        if (speed) {
            if (target_value >= 500 ) {
            pid_output = pid_params->error*pid_params->Kp + i_error*pid_params->Ki;
            }
            else{
            pid_output = pid_params->error*pid_params->Kp + i_error*pid_params->Ki + bias;
            }
        }
        else {
            pid_output = pid_params->error*pid_params->Kp + i_error*pid_params->Ki;
        }
        
        if (pid_output > 1) {
            pid_output = 1.0;
        }
        else if (pid_output < 0){
            pid_output = 0;
        } 
    
        pid_timer.reset();
        
                    }
    return pid_output;
};