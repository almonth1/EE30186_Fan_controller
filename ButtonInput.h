#ifndef BUTTONINPUT_H
#define BUTTONINPUT_H

#include "mbed.h"

// A small delay to ignore debounce button 
constexpr std::chrono::milliseconds debounceDelay = 100ms;

// Sets up the button to detect presses with debouncing
void InitializeButtonInput();


// Only return true once per press.
bool WasButtonPressed();

#endif // BUTTONINPUT_H
