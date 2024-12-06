#include "rotary_input.h"
#include "mbed.h"
#include "taco_test.h"
#include "pid_controller.h"

// Global variable to track the encoder position
float encoderPosition = 0;  // Initialize to zero or your desired starting value
float baseIncrement = 1;
int max_value = 100;
int min_value = 0;
bool currentB;
bool prevB;
bool prevClk;
bool currentClk;

// Speed-related variables for acceleration
float currentSpeed = 0.0;
float maxSpeedFactor = 0.5;  // Reduced for less sensitive acceleration
float speedScalingFactor = 0.1;  // Further dampening of speed effects
uint32_t lastPulseTime = 0;

// Stability-related variables
std::chrono::milliseconds debounceThreshold = 5ms;  // Debounce time threshold
Timer timeSincePinAChange, timeSincePinBChange;
bool isPinAHigh = false;
bool isPinBHigh = false;

std::chrono::microseconds rotaryWait_us = 0us;
Timer rotaryTimer;

// Prototypes for internal functions
float calculateEncoderSpeed(uint32_t currentTime);
float getEncoderIncrement(float baseIncrement, float speed);
float calculateSmoothedSpeed(float newSpeed);

// Smoothing-related variables
float smoothedSpeed = 0.0;
float alpha = 0.1;  // Smoothing factor for speed (0 < alpha < 1)

// Function to calculate the encoder speed
float calculateEncoderSpeed(uint32_t currentTime) {
    uint32_t timeDifference = currentTime - lastPulseTime;
    lastPulseTime = currentTime;

    if (timeDifference > 0) {
        return 1.0 / static_cast<float>(timeDifference / 10000.0); // Speed is inversely proportional to time difference
    } else {
        return 0.0;  // No movement detected
    }
}

// Function to smooth the speed (reduces sensitivity to sudden spikes)
float calculateSmoothedSpeed(float newSpeed) {
    smoothedSpeed = alpha * newSpeed + (1.0 - alpha) * smoothedSpeed;
    return smoothedSpeed;
}

// Function to get the increment based on speed
float getEncoderIncrement(float baseIncrement, float speed) {
    if (speed < 0.01) {  // Ignore very small speed values to reduce jitter
        return baseIncrement;  // No scaling for low speeds
    }
    float speedFactor = std::min(speed * speedScalingFactor, maxSpeedFactor);  // Use reduced scaling factor
    return baseIncrement * (1.0 + speedFactor);  // Scale increment based on speed
}

// Function to initialize the rotary encoder
void Init_Rotary_Input(int button_mode) {
    aClock.rise(Rotary_Input);
    prevB = bSignal.read();
    prevClk = aClock.read();
    currentClk = aClock.read();

    // Start timers for stability improvements
    timeSincePinAChange.start();
    timeSincePinBChange.start();

    switch (button_mode) {
        case 0:
            // Mode 0: Open Loop Speed Control
            baseIncrement = 0.1;
            max_value = 100;
            min_value = 0;
            encoderPosition = duty_cycle;
            break;

        case 1:
            // Mode 1: PID Speed Control
            baseIncrement = 50;
            max_value = 1750;
            min_value = 0;
            encoderPosition = fanrpm/10;
            break;

        case 2:
            // Mode 2: PID Temperature Limiting  
            baseIncrement = 1;
            max_value = 80;
            min_value = 20;
            encoderPosition = min_value;
            break;

        case 3:
            // Mode 3: Timer display (e.g., TIMER mode)
            baseIncrement = 1;
            max_value = 99;
            min_value = 0;
            encoderPosition = min_value;
            break;

        case 4:
            // Mode 3: Timer display (e.g., TIMER mode)
            baseIncrement = 1;
            max_value = 99;
            min_value = 0;
            encoderPosition = min_value;
            break;

        default:
            // Mode 0: Open Loop Speed Control
            baseIncrement = 5;
            max_value = 100;
            min_value = 0;
            encoderPosition = min_value;
            break;
    }
}

// Function to process the encoder input and update the position
void Rotary_Input() {
    rotaryTimer.start();
    if (std::chrono::duration_cast<std::chrono::microseconds>(rotaryTimer.elapsed_time()) >= rotaryWait_us) {
        currentClk = aClock.read();
        if (currentClk != prevClk) {
            uint32_t currentTime = rotaryTimer.elapsed_time().count();
            currentSpeed = calculateEncoderSpeed(currentTime);

            // Apply smoothing to speed and calculate dynamic increment
            float increment = getEncoderIncrement(baseIncrement, calculateSmoothedSpeed(currentSpeed));

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

            // Ensure encoderPosition stays within bounds
            if (encoderPosition > max_value) {
                encoderPosition = max_value;  // Cap at maximum
            } else if (encoderPosition < min_value) {
                encoderPosition = min_value;  // Cap at minimum
            }
        }
        rotaryTimer.reset();
    }
}


// Function to get the current encoder position
float RotaryInput_GetPosition() {
    return encoderPosition;
}