#include "ButtonInput.h"

// Define the button input and a timer for tracking debounce
static InterruptIn button(BUTTON1);  
static Timer debounceTimer;          // Tracks time since last button press
static bool buttonPressedFlag = false; // Records if button was pressed since last check

// Function triggered when the button is pressed
static void OnButtonPress();

// Initializes the button input
void InitializeButtonInput() {
    debounceTimer.start();            // Start the timer to track time between presses
    button.rise(OnButtonPress);       
}

// Checks if the button was pressed since the last call. 
// If true, resets the flag so that it only returns once per press
bool WasButtonPressed() {
    if (buttonPressedFlag) {         
        buttonPressedFlag = false;    // Reset the flag after detecting the press
        return true;                  // Return true to indicate a press was detected
    }
    return false;                     // Return false if no press was detected
}

// Function called when the button is pressed down
// Sets buttonPressedFlag to true only if debounce time has passed
static void OnButtonPress() {
    // Check if enough time has passed since the last press
    if (std::chrono::duration_cast<std::chrono::milliseconds>(
            debounceTimer.elapsed_time()) > debounceDelay) {
        
        buttonPressedFlag = true;     // Record that the button was pressed
        debounceTimer.reset();        // Reset timer for the next debounce period
    }
}
