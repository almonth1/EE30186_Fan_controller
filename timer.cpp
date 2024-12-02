#include "timer.h"


Ticker timer_tick;

uint8_t timer_value;


// initiates hardware interrupt which starts timer every time the Timer_Test button is clicked. Also intializes the timer value
void Init_Timer_Mode(){
    #ifdef TIMER_DEBUG
        timer_value = 0;
    #endif  
}

// sets timer_value and attaches interrupt to decrement every 1 second
void Start_Timer(int set_timer){
    if(timer_value == 0){
    timer_value = set_timer;
    timer_tick.attach(callback(&Decrement_Timer) , 1s );
    }

    // initiate timer when button is held down  
} 


// Decrements timer value every 2 second. Detaches timer interrupt when timer reaches 0
void Decrement_Timer(){
    if (timer_value >99) {
    timer_value = 99;
    }
    timer_value -= 1;
    

}

