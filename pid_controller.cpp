
#include "pid_controller.h"
#include <chrono>
#include <cstdint>
#include <cstdio>


Timer pid_timer;
std::chrono::milliseconds pid_period = 100ms;
float pid_output = 0;
PID speed_controller_params = {.error = 0, .Kp = 0, .Ki = 0, .Kd = 0, .i_error = 0, .d_error = 0, .prev_error = 0};


// Initializes PID Speed controller parameters 
void Init_Speed_PID_Controller(){
    PID speed_controller_params = {.error = 0, .Kp = 0, .Ki = 0, .Kd = 0, .i_error = 0, .d_error = 0, .prev_error = 0};   
    pid_output = 0;
    pid_timer.start();
}

void PID_Dummy(){
    printf("PID running! :)");
    wait_us(1000000);
};

void PID_Control( PID pid_params, uint16_t target_value, uint16_t current_value){
     if ( std::chrono::duration_cast<std::chrono::milliseconds>( pid_timer.elapsed_time()) >= pid_period) {
                
        pid_params.error = target_value - current_value;
        pid_params.i_error += pid_params.error;
        pid_params.d_error = (pid_params.error-pid_params.prev_error)/pid_period.count();

        pid_output = pid_params.error*pid_params.Kp + pid_params.i_error*pid_params.Ki + pid_params.d_error*pid_params.d_error; 

        pid_params.prev_error = pid_params.error;

        // caps the duty cycle between 1 and 0
        if (pid_output > 1) {
            pid_output = 1.0;
        }
        else if (pid_output < 0) {
            pid_output = 0.0;
        }
        pid_timer.reset();
                    }
};



