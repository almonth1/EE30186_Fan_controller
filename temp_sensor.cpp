#include "temp_sensor.h"

const int address = 0x4D << 1;
char data[2];


int Read_Temperature(){
    
i2c.write(address, 0x00, 2);
i2c.read(77, data , 2);

printf("Temp: %d\n" , data[0] );

return data[0]
};