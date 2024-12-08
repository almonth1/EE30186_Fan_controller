#include "ButtonInput.h"

// Define the button input and a timer for tracking debounce
InterruptIn button(BUTTON1);  
Timer buttonTimer;
int Button_Mode = -1;
bool settingsModeFlag = false;
static Timer debounceTimer;          // Tracks time since last button press
bool buttonPressedFlag = false; // Records if button was pressed since last check
bool buttonHeldFlag = false;


// Initializes the button input
void InitializeButtonInput() {
    //debounceTimer.start();            // Start the timer to track time between presses
    button.rise(OnButtonPressRise);  
    button.fall(OnButtonPressFall);    
}

// Checks if the button was pressed since the last call. 
// If true, resets the flag so that it only returns once per press
bool WasButtonPressed() {
    if (buttonPressedFlag) {         
        // Reset the flag after detecting the press
        return true;                  // Return true to indicate a press was detected
    }
    return false;                     // Return false if no press was detected
}

// Checks if the button was pressed since the last call. 
// If true, resets the flag so that it only returns once per press
bool WasButtonHeld() {
    if (buttonHeldFlag) {         
        buttonHeldFlag = false;    // Reset the flag after detecting the press
        return true;                  // Return true to indicate a press was detected
    }
    return false;                     // Return false if no press was detected
}

// Function called when the button is pressed down
// Sets buttonPressedFlag to true only if debounce time has passed
void OnButtonPressRise() {

       if (std::chrono::duration_cast<std::chrono::milliseconds>(
            debounceTimer.elapsed_time()) > debounceDelay) {
                // Check if enough time has passed since the last press
            if (std::chrono::duration_cast<std::chrono::milliseconds>(
                    buttonTimer.elapsed_time()) >= buttonHoldTime) {
                
                buttonHeldFlag = true;     // Record that the button was pressed
                settingsModeFlag = !settingsModeFlag;
                buttonPressedFlag = false;
                buttonTimer.stop();
                buttonTimer.reset();        // Reset timer for the next debounce period
            }
            else {
                buttonPressedFlag = true;     // Record that the button was pressed
                buttonHeldFlag = false; 
                buttonTimer.stop();
                buttonTimer.reset();        // Reset timer for the next debounce period
            
            }
            debounceTimer.stop();
            debounceTimer.reset();        // Reset timer for the next debounce period
    }
    else {
        buttonTimer.stop();
        buttonTimer.reset();        // Reset timer for the next debounce period
    }
 
}

void OnButtonPressFall(){
    debounceTimer.start();
    buttonTimer.start();     
    debounceTimer.reset();        // Reset timer for the next debounce period
    
}