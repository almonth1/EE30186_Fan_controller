#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include "mbed.h"
#include "pins_config.h"


extern const int address;
extern char data[2];

int Read_Temperature();

#endif