#ifndef TIMER_MODE
#define TIMER_MODE

#include "mbed.h"
#include "pins_config.h"

extern uint8_t current_timer;
extern Ticker timer_tick;
extern bool init_timer;
extern uint8_t timer_value;

void Init_Timer_Mode();
void Start_Timer();
void Decrement_Timer();

#endif
