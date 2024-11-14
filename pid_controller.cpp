
#include "pid_controller.h"
#include <chrono>
#include <cstdint>
#include <cstdio>


Timer pid_timer;
float pid_output = 0;

PID speed_controller_params = {.error = 0, .Kp = 0.0003, .Ki = 0.00000001, .Kd = 0, .d_error = 0, .prev_error = 0};
// PID values is good for low speed, not high speed
PID* pid_speed_ptr = &speed_controller_params;

PID temp_controller_params = {.error = 0, .Kp = 0.0003, .Ki = 0.000002, .Kd = 0, .d_error = 0, .prev_error = 0};
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

void PID_Control( PID *pid_params, uint16_t target_value, uint16_t current_value){

     if ( std::chrono::duration_cast<std::chrono::microseconds>(pid_timer.elapsed_time()) >= pid_period) {
        
        pid_params->error = target_value - current_value;
        pid_params->i_error += pid_params->error;
        //pid_params.d_error = (pid_params.error-pid_params.prev_error)/pid_period.count();

        pid_output = pid_params->error*pid_params->Kp + pid_params->i_error*pid_params->Ki;
        // + pid_params.d_error*pid_params.d_error; 

        //pid_params.prev_error = pid_params.error;

        // caps the duty cycle between 1 and 0
        if (pid_output > 1) {
            pid_output = 1.0;
        }
        else if (pid_output < 0) {
            pid_output = 0.0;
        }
        
        // printf(" error = %d\n", pid_params.error);
        // printf(" target = %d\n", target_value);
        // printf(" current = %d\n", current_value);
        // printf(" pid output = %f\n", pid_output);
        pid_timer.reset();
                    }
};


