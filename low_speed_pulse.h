#ifndef LOW_SPEED_PULSE_H
#define LOW_SPEED_PULSE_H

#include "mbed.h"

extern Timer low_speed_timer;
extern Timeout low_speed_timeout;
extern Timer kick_start_timer;
extern std::chrono::microseconds low_speed_pulse_period;
extern bool low_speed_mode;

void Low_Speed_Start_Pulse();
void Low_Speed_Stop_Pulse();
void Init_Low_Speed_Pulses();
void Detach_Low_Speed_Pulses();
void Kick_Start_pulse(float current_speed);
#endif