#include "temp_sensor.h"

Ticker OTW_tick;

const int address = 0x4D << 1;
char data[2];
bool shdReadTemp = false;
bool OTW_flag = false;

void Write_Temp_Command(){
i2c.write(address, 0x00, 2);
};

int Read_Temperature(){
i2c.read(address, data , 2);
wait_us(10000);
return data[0];
};

void StartTempRead(){
    shdReadTemp = true;
}

void OTW_Blinky(){
    led_Ext = !led_Ext;
}