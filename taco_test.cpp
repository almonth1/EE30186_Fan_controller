
#include "taco_test.h"
#include "low_speed_pulse.h"
#include <cstdio>
// measure width of pulse. multiply by 4 and then 60 to get rpm
//use pwm pulses to increase pulse count
Ticker tacho_tick;
Timer tacho_filter_timer;
Timer tacho_fall_timer;
Timer tacho_rise_timer;   
InterruptIn tacho_interrupt(PA_0);
// global variables
int pulse_count;
vector<float> pulse_width_vector = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0};
vector<float> weights = {0.125, 0.125, 0.25, 0.25, 0.25};  // Example weights: most recent pulse gets highest weight
int prevpulse = 0;
float fanrpm;
int shdprint = 0;
float pulse_weighted_average;
float pulse_width_average;
float pulse_width = 0.0;
float pulse_width_filter = 0.0;
float pulse_width_temp = 0.0;
float min_threshold = 0.0;
float max_threshold = 0.0;
float low_speed_bias = 0.0018;
float high_speed_bias =  0.000503;
float med_speed_bias = (low_speed_bias + high_speed_bias)/2;

// calculates rpm value and sets flag to print result
void CalculateRPM () {

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

    // If pulse_vector[0] is 0 (no data), use raw pulse count for calculation
    if (pulse_width_vector[0] == 0) {
        // pulse_width_vector.erase(pulse_width_vector.begin());
        // pulse_width_vector.push_back(pulse_width);
        fanrpm = 0;
    }
    else {
            // pulse_width_vector.erase(pulse_width_vector.begin());
            // pulse_width_vector.push_back(pulse_width);
            // pulse_width_average = std::accumulate(pulse_width_vector.begin(), pulse_width_vector.end(), 0.0) / pulse_width_vector.size();

        //     // Check if pulse_width is within ±10% of the previous average
        //     min_threshold = pulse_width_average * 0.90; // 90% of the previous average
        //     max_threshold = pulse_width_average * 1.10; // 110% of the previous average
        //     // If the new pulse width is outside the range of ±10%, we ignore it and skip the update
        // if (pulse_width < min_threshold || pulse_width > max_threshold) {
        //     // // Shift the old pulse counts in the vector to make room for the new one
        //     pulse_width_vector.erase(pulse_width_vector.begin());
        //     pulse_width_vector.push_back(pulse_width);
        //     pulse_width_average = std::accumulate(pulse_width_vector.begin(), pulse_width_vector.end(), 0.0) / pulse_width_vector.size();
        // }
        fanrpm = 60/((pulse_width_average)*4);
        if (fanrpm >= 1300){
            fanrpm = 60/((pulse_width_average  + high_speed_bias)*4);
        }
        else if(  600 <= fanrpm || fanrpm < 1300){
            fanrpm = 60/((pulse_width_average  + med_speed_bias)*4);
        }
        else {
            fanrpm = 60/((pulse_width_average  + low_speed_bias)*4);
        }
        
    }

    // // Reset pulse count after calculation
    // pulse_count = 0;
    //pulse_width_average = (round(pulse_width_average*100000)/100000)
}

// initiates tacho ticker interrupt that calls calcilate rpm every tacho_period
void Init_Calculate_Fan_RPM(){

        led.write(true);
        tacho_tick.attach(&CalculateRPM, tacho_period);
        tacho_interrupt.fall(&Falling_Edge_Pulse);
        tacho_interrupt.rise(&Rising_Edge_Pulse);
        tacho_filter_timer.start();
        tacho_fall_timer.start();
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

    pulse_width_filter = tacho_rise_timer.elapsed_time().count()/1000000.0;

    // if (std::chrono::duration_cast<std::chrono::milliseconds>(
    //         tacho_fall_timer.elapsed_time()) >= tacho_filter) {
        
    // }
    if (pulse_width_filter > (tacho_filter.count()/1000.0)) {
        tacho_fall_timer.reset();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(
                tacho_filter_timer.elapsed_time()) >= tacho_wait_timer) {
            pulse_width = pulse_width_temp;
            tacho_filter_timer.reset();
        }
            pulse_width_vector.erase(pulse_width_vector.begin());
            pulse_width_vector.push_back(pulse_width);
            pulse_width_average = std::accumulate(pulse_width_vector.begin(), pulse_width_vector.end(), 0.0) / pulse_width_vector.size();
    }
    

}
void Rising_Edge_Pulse(){
    tacho_rise_timer.reset();
    if ( std::chrono::duration_cast<std::chrono::milliseconds>(
                tacho_fall_timer.elapsed_time()) >= tacho_filter) {
                pulse_width_temp = tacho_fall_timer.elapsed_time().count()/1000000.0;
        }
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