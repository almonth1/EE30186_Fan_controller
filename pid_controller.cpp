
#include "pid_controller.h"
#include <chrono>
#include <cstdint>


Ticker pid_tick;

// // PID period in ms
// void Init_PID_Interrupt(float pid_period){
//      pid_tick.attach(&PID_Control, pid_period);
// }



void Init_Speed_PID_Controller(){
    PID speed_controller_params = {.error = 0, .Kp = 0, .Ki = 0, .Kd = 0, .i_error = 0, .d_error = 0, .prev_error = 0};   
}

void PID_Control( PID pid_params, int target_value, int8_t current_value, int8_t time_step ){

    pid_params.error = target_value - current_value;
    pid_params.i_error += pid_params.error;
    pid_params.d_error = (pid_params.error-pid_params.prev_error)/time_step;

    pid_output = pid_params.error*pid_params.Kp + pid_params.i_error*pid_params.Ki + pid_params.d_error*pid_params.d_error; 

    pid_params.prev_error = pid_output;


    if (pid_output > 1) {
    pid_output = 1;
    }
    else if (pid_output < 0) {
    pid_output = 0;
    }
};



