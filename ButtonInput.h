#ifndef BUTTONINPUT_H
#define BUTTONINPUT_H

#include "mbed.h"


extern Timer buttonTimer;
extern int Button_Mode;
extern bool buttonHoldFlag; 
extern bool settingsModeFlag;
extern bool buttonPressedFlag;
// A small delay to ignore debounce button 
constexpr std::chrono::milliseconds debounceDelay = 50ms;

constexpr std::chrono::milliseconds buttonHoldTime = 1000ms;

// Sets up the button to detect presses with debouncing
void InitializeButtonInput();

void OnButtonPressRise();
void OnButtonPressFall();
// Only return true once per press.
bool WasButtonPressed();
bool WasButtonHeld();


#endif // BUTTONINPUT_H
