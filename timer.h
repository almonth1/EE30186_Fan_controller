#ifndef TIMER_MODE
#define TIMER_MODE

#include "mbed.h"
#include "pins_config.h"
#include "sev_seg_display.h"
#include <cstdint>
#include <cstdio>
#include <stdint.h>

extern uint8_t current_timer;
extern Ticker timer_tick;
extern bool init_timer;
extern uint8_t timer_value;

extern int set_timer;

void Init_Timer_Mode();
void Start_Timer(int set_timer);
void Decrement_Timer();

#endif
