#include "rotary_input.h"


bool currentB;
bool prevB = bSignal.read();
bool prevClk = aClock.read();
bool currentClk = aClock.read();
        
        
void Rotary_Input(){
if(currentClk != prevClk)
{
currentB = bSignal.read();
    if(currentB == currentClk)   
    {
    led_ex = true;
    led = false;
    }
    else {
    led = true;
    led_ex = false;
    }
    prevB = currentB;
    prevClk = currentClk;
    wait_us(1000)
}