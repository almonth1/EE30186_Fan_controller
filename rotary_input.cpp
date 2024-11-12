#include "rotary_input.h"
#include "mbed.h"

// Global variable to track the encoder position
static int encoderPosition = 0;  // Initialize to zero or your desired starting value

// Function to initialize the rotary encoder
void Init_Rotary_Input() {
    #ifdef ROTARY_DEBUG
        prevB = bSignal.read();
        prevClk = aClock.read();
        currentClk = aClock.read();
    #endif
}

// Function to process the encoder input and update the position
void Rotary_Input() {
    #ifdef ROTARY_DEBUG

        currentClk = aClock.read();
        if (currentClk != prevClk) {
            currentB = bSignal.read();

            if (currentB == currentClk) {
                encoderPosition++;  // Increment for clockwise rotation
            } else {
                encoderPosition--;  // Decrement for counter-clockwise rotation
            }

            prevB = currentB;
            prevClk = currentClk;
            wait_us(10000);  // Basic debouncing delay
        }
    #endif
}

// Function to get the current encoder position
int RotaryInput_GetPosition() {
    return encoderPosition;
}
