#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include "mbed.h"
#include "pins_config.h"

extern Ticker OTW_tick;
extern bool OTW_flag;
extern const int address;
extern char data[2];
extern bool shdReadTemp;
int Read_Temperature();
void Write_Temp_Command();
void StartTempRead();
void OTW_Blinky();

#endif