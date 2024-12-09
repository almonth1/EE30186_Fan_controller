#include "pid_controller.h"


Timer pid_timer;
float pid_output = 0;
float duty_cycle = 0;
float bias = 0.03;
float i_error = 0;
int low_speed_threshold = 300;
int med_speed_threshold = 700;
float dt = (pid_period.count()/1000000.0);
//.Kp = 0.000075 , .Kd = 0.00025
PID lowspeed_controller_params = {.error = 0, .Kp = 0.00006855, .Ki = 0.000024, .Kd = 0.00005, .d_error = 0, .prev_error = 0};
// PID values is good for low speed, not high speed
PID* pid_lowspeed_ptr = &lowspeed_controller_params;

PID medspeed_controller_params = {.error = 0, .Kp = 0.000324, .Ki = 0.00016, .Kd = 0.000025, .d_error = 0, .prev_error = 0};

// PID values is good for med speed, not high or low speed
PID* pid_medspeed_ptr = &medspeed_controller_params;

//PID highspeed_controller_params = {.error = 0, .Kp = 0.00007, .Ki = 0.000015, .Kd = 0.000005, .d_error = 0, .prev_error = 0};

PID highspeed_controller_params = {.error = 0, .Kp = 0.0015, .Ki = 0.0005, .Kd = 0.00000, .d_error = 0, .prev_error = 0};

// PID values is good for low speed, not high speed
PID* pid_highspeed_ptr = &highspeed_controller_params;
//.Ki = 0.000134
PID temp_controller_params = {.error = 0, .Kp = 0.02, .Ki = 0.001, .Kd = 0, .d_error = 0, .prev_error = 0};
PID* pid_temp_ptr = &temp_controller_params;

// Initializes PID Speed controller parameters 
void Init_PID_Controller(PID *pid_params, PID param_init){
    *pid_params = param_init; 
    pid_output = 0;
    pid_timer.start();
}


float PID_Control( PID *pid_params, uint16_t target_value, uint16_t current_value, bool speed){

     if ( std::chrono::duration_cast<std::chrono::microseconds>(pid_timer.elapsed_time()) >= pid_period) {
        
        pid_params->error = target_value - current_value;
        i_error += pid_params->error *dt;
        pid_params->d_error = (pid_params->error - pid_params->prev_error)/dt;

    
        if (target_value == 0) {
            i_error = 0;
            pid_output = 0;
            }
        else {
            pid_output = pid_params->error*pid_params->Kp + i_error*pid_params->Ki + pid_params->d_error*pid_params->Kd + bias;
        }
        
    
        if (pid_output > 1) {
            pid_output = 1.0;
        }
        else if (pid_output < 0){
            pid_output = 0;
        } 
        pid_params->prev_error = pid_params->error;
    
        pid_timer.reset();
        
                    }
    return pid_output;
};