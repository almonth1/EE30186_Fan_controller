#include "timer.h"
#include <cstdint>
#include <stdint.h>

Ticker timer_tick;

uint8_t timer_value;

void Init_Timer_Mode(){
    #ifdef TIMER_DEBUG
        led.write(true);
        Timer_Test.mode(PullUp); 
        Timer_Test.fall(&Start_Timer);
        timer_value = 5;
    #endif  
  
}

void Start_Timer(){
    #ifdef TIMER_DEBUG
    // initiat timer when button is held down
    timer_value = 5;
    timer_tick.attach(callback(&Decrement_Timer) , 1s ); 
    #endif  
     
}

void Decrement_Timer(){

    timer_value -= 1;

    if (timer_value <= 0){
        timer_tick.detach();
    }
    
}

