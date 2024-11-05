#ifndef PID_CONTROLLER
#define PID_CONTROLLER

#include "mbed.h"
#include <cstdint>
#include "pins_config.h"

extern int8_t pid_output;
extern Ticker pid_tick;

// structure "PID" of PID Parameters
typedef struct{
    int8_t error;

    int8_t Kp;
    int8_t Ki;
    int8_t Kd;


    int8_t i_error;
    int8_t d_error;

    int8_t prev_error;
} PID;

// Declares header functions
void Init_PID_Controller();
void PID_Dummy();
int8_t PID_Control(PID pid_params, int8_t target_value, int8_t current_value, int8_t time_step );
void Init_PID_Interrupt(float pid_period);

#endif