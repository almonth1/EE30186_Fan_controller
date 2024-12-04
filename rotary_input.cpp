#include "rotary_input.h"
#include "mbed.h"
#include "taco_test.h"
#include "pid_controller.h"

// Global variable to track the encoder position
int encoderPosition = 0;  // Initialize to zero or your desired starting value
int increment = 1;
int max_value = 100;
int min_value = 0;
bool currentB;
bool prevB;
bool prevClk;
bool currentClk;

std::chrono::microseconds rotaryWait_us = 0us;

Timer rotaryTimer;

// Function to initialize the rotary encoder
void Init_Rotary_Input(int button_mode) {

        aClock.rise(Rotary_Input);
        prevB = bSignal.read();
        prevClk = aClock.read();
        currentClk = aClock.read();

        switch (button_mode){
            case 0:
                // Mode 0: Open Loop Speed Control
                increment = 5;
                max_value = 100;
                min_value = 0;
                encoderPosition = duty_cycle;
                break;

            case 1:
                // Mode 1: PID Speed Control
                increment = 50;
                max_value = 2000;
                min_value = 0;
                encoderPosition = fanrpm;
                break;

            case 2:
                // Mode 2: PID Temperature Limiting  
                increment = 1;
                max_value = 40;
                min_value = 20;
                encoderPosition = min_value;
                break;
            case 3:
                // Mode 3: Timer display (e.g., TIMER mode)
                increment = 1;
                max_value = 99;
                min_value = 0;
                encoderPosition = min_value;
                break;
            case 4:
                break;
                
            default:
                // Mode 0: Open Loop Speed Control
                increment = 5;
                max_value = 100;
                min_value = 0;
                encoderPosition = min_value;
                break;
        }

}

// Function to process the encoder input and update the position
void Rotary_Input() {
    rotaryTimer.start();
    if ( std::chrono::duration_cast<std::chrono::microseconds>(rotaryTimer.elapsed_time()) >= rotaryWait_us) {
        currentClk = aClock.read();
        if (currentClk != prevClk) {
            currentB = bSignal.read();
        
            if (currentB == currentClk) {
                // Decrement for counter-clockwise rotation
                encoderPosition -= increment; 
                led_A.write(false);
                led_B.write(true);
                
            } else {
                // Increment for clockwise rotation
                encoderPosition += increment; 
                led_A.write(true);
                led_B.write(false); 
            }
            prevB = currentB;
            prevClk = currentClk;

            if (encoderPosition > max_value) {
                    encoderPosition = max_value;  // Cap at maximum
                }
            else if (encoderPosition < min_value) {
                    encoderPosition = min_value;  // Cap at maximum
                }
            //printf("rotary position: %d\n", encoderPosition);
            //wait_us(1000);  // Basic debouncing delay
        }
        rotaryTimer.reset();
    }

}

// Function to get the current encoder position
int RotaryInput_GetPosition() {
    return encoderPosition;
}
