#ifndef PID_CONTROLLER
#define PID_CONTROLLER

#include "mbed.h"
#include <cstdint>
#include "pins_config.h"

extern float pid_output;
extern Timer pid_timer;


// structure "PID" of PID Parameters
struct PID{
    int16_t error;

    float Kp;
    float Ki;
    float Kd;

    int i_error;
    int16_t d_error;

    int16_t prev_error;
};


extern PID speed_controller_params;
extern PID temp_controller_params;

extern PID* pid_speed_ptr;
extern PID* pid_temp_ptr;

// Declares header functions
void Init_PID_Controller(PID *pid_params, PID param_init);
void PID_Dummy();
void PID_Control(PID *pid_params, uint16_t target_value, uint16_t current_value);


#endif