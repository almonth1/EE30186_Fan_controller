
#include "taco_test.h"
#include "low_speed_pulse.h"
#include <cstdio>
// measure width of pulse. multiply by 4 and then 60 to get rpm
//use pwm pulses to increase pulse count
Ticker tacho_tick;
Timer tacho_timer;
Timer tacho_fall_timer;
Timer tacho_rise_timer;   
InterruptIn tacho_interrupt(PA_0);
// global variables
int pulse_count;
vector<int> pulse_vector = {0, 0, 0, 0, 0};
vector<float> weights = {0.125, 0.125, 0.25, 0.25, 0.25};  // Example weights: most recent pulse gets highest weight
int prevpulse = 0;
float fanrpm;
int shdprint = 0;
float pulse_weighted_average;
float pulse_average;
float pulse_width = 0.0;

// calculates rpm value and sets flag to print result
void CalculateRPM () {
    // // Shift the old pulse counts in the vector to make room for the new one
    // pulse_vector.erase(pulse_vector.begin());
    // pulse_vector.push_back(pulse_count);

    // // Calculate weighted average of pulse counts
    // float weighted_sum = 0.0;
    // float weight_sum = 0.0;

    // // Loop through pulse_vector and corresponding weights to compute the weighted sum
    // // for (size_t i = 0; i < pulse_vector.size(); ++i) {
    // //     weighted_sum += pulse_vector[i] * weights[i];  // Multiply pulse count by weight
    // //     weight_sum += weights[i];  // Sum of weights
    // // }

    // // Calculate the weighted average
    // //pulse_weighted_average = weighted_sum / weight_sum;

    // pulse_average = std::accumulate(pulse_vector.begin(), pulse_vector.end(), 0.0) / pulse_vector.size();

    // // If pulse_vector[0] is 0 (no data), use raw pulse count for calculation
    // if (pulse_vector[0] == 0) {
    //     fanrpm = (pulse_count / (tacho_period.count()/1000.0)) * (60.0 / 4);  // Calculation assuming 4 pulses per revolution
    // }
    // else {
    //     fanrpm = (pulse_average / (tacho_period.count()/1000.0)) * (60.0 / 4);
    // }

    // // Reset pulse count after calculation
    // pulse_count = 0;

    fanrpm = pulse_width*4/60;
}

// initiates tacho ticker interrupt that calls calcilate rpm every tacho_period
void Init_Calculate_Fan_RPM(){

        led.write(true);
        tacho_tick.attach(&CalculateRPM, tacho_period);
        tacho_interrupt.fall(&Falling_Edge_Pulse);
        tacho_interrupt.rise(&Rising_Edge_Pulse);
        // tacho_timer.start();
        // tacho_fall_timer.start();
        tacho_rise_timer.start();
        pulse_count = 0 ;
        fanrpm = 0.0;
 
}

// measures tacho for roation speed of fan and prints the results when data is available
// void Calculate_Fan_RPM(){

//          if ( std::chrono::duration_cast<std::chrono::milliseconds>(
//             tacho_timer.elapsed_time()) >= tacho_delay) {
//             pulse_count += 1;
//             }
        
//         tacho_timer.reset();

// }

void Falling_Edge_Pulse(){
   if ( std::chrono::duration_cast<std::chrono::milliseconds>(
            tacho_rise_timer.elapsed_time()) >= tacho_rise_filter) {
            pulse_width = tacho_rise_timer.elapsed_time().count()/1000000.0;
            }
}
void Rising_Edge_Pulse(){

    tacho_rise_timer.reset();

}


// void Falling_Edge_Pulse(){

//     if ( std::chrono::duration_cast<std::chrono::milliseconds>(
//                 tacho_rise_timer.elapsed_time()) >= tacho_delay) {
//                 pulse_count += 1;
//                 }

//     tacho_fall_timer.reset();
        

// }
// void Rising_Edge_Pulse(){
//    if ( std::chrono::duration_cast<std::chrono::milliseconds>(
//             tacho_rise_timer.elapsed_time()) >= tacho_delay) {
//             pulse_count += 1;
//             }

//     tacho_rise_timer.reset();

// }