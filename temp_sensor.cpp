#include "temp_sensor.h"

const int address = 0x4D << 1;
char data[2];
bool shdReadTemp = false;

int Read_Temperature(){
    
i2c.write(address, 0x00, 2);
i2c.read(address, data , 2);
wait_us(10000);
return data[0];
};

void StartTempRead(){
    shdReadTemp = true;
}