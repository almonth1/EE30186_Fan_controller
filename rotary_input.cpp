#include "rotary_input.h"


//initiate rotary  values
void Init_Rotary_Input(){  
    #ifdef ROTARY_INPUT
        bool currentB;
        bool prevB = bSignal.read();
        bool prevClk = aClock.read();
        bool currentClk = aClock.read();
    #endif    
}

// measures rotation direction of rotary encoder        
void Rotary_Input(){
    #ifdef ROTARY_INPUT
        if(currentClk != prevClk)
        {
        currentB = bSignal.read();
            if(currentB == currentClk){
                led_ex = true;
            }
            else {
                led_ex = false;
            }
            prevB = currentB;
            prevClk = currentClk;
            wait_us(1000);
        }
    #endif
