#ifndef PID_CONTROLLER
#define PID_CONTROLLER

#include "mbed.h"
#include <cstdint>
#include "pins_config.h"
#include "taco_test.h"
#include <chrono>
#include <cstdint>
#include <cstdio>



extern float pid_output;
extern float duty_cycle;
extern float i_error;
extern Timer pid_timer;
extern float bias;
extern int low_speed_threshold;
extern int med_speed_threshold;
extern float dt;

// structure "PID" of PID Parameters
struct PID{
    int16_t error;

    float Kp;
    float Ki;
    float Kd;

    int16_t d_error;

    int16_t prev_error;
};


extern PID highspeed_controller_params;
extern PID medspeed_controller_params;
extern PID lowspeed_controller_params;
extern PID temp_controller_params;

extern PID* pid_highspeed_ptr;
extern PID* pid_medspeed_ptr;
extern PID* pid_lowspeed_ptr;
extern PID* pid_temp_ptr;

// Declares header functions
void Init_PID_Controller(PID *pid_params, PID param_init);
void PID_Dummy();
float PID_Control(PID *pid_params, uint16_t target_value, uint16_t current_value, bool low_speed);


#endif