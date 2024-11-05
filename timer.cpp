#include "timer.h"
#include <cstdint>
#include <stdint.h>

Ticker timer_tick;

uint8_t timer_value;

// initiates hardware interrupt which starts timer every time the Timer_Test button is clicked. Also intializes the timer value
void Init_Timer_Mode(){
    #ifdef TIMER_DEBUG
        led.write(true);
        Timer_Test.mode(PullUp); 
        Timer_Test.fall(&Start_Timer);
        timer_value = 5;
    #endif  
}

// sets timer_value and attaches interrupt to decrement every 1 second
void Start_Timer(){
    #ifdef TIMER_DEBUG
    // initiat timer when button is held down
    timer_value = 5;
    timer_tick.attach(callback(&Decrement_Timer) , 1s ); 
    #endif  
}

// Decrements timer value every 2 second. Detaches timer interrupt when timer reaches 0
void Decrement_Timer(){
    timer_value -= 1;
    if (timer_value <= 0){
        timer_tick.detach();
    }
}

