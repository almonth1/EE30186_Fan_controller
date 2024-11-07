#ifndef PID_CONTROLLER
#define PID_CONTROLLER

#include "mbed.h"
#include <cstdint>
#include "pins_config.h"

extern float pid_output;
extern Timer pid_timer;
extern std::chrono::milliseconds pid_period;

// structure "PID" of PID Parameters
typedef struct{
    int16_t error;

    float Kp;
    float Ki;
    float Kd;


    int16_t i_error;
    int16_t d_error;

    int16_t prev_error;
} PID;

extern PID speed_controller_params;

// Declares header functions
void Init_Speed_PID_Controller();
void PID_Dummy();
void PID_Control(PID pid_params, uint16_t target_value, uint16_t current_value);


#endif