#ifndef SEV_SEG_DISPLAY_H
#define SEV_SEG_DISPLAY_H

#include "mbed.h"
#include<map>
// Ones Side (PB_11, PB_12, PA_11, PA_12, PA_6, PA_7, PB_6)
// Tens Side (  PC_5, PC_6, PB_1,PC_8, PC_4, PA_10, PB_2)
extern int sev_seg_ones_data;



enum sev_seg_digits{
    seg_zero = 0b0000100,
    seg_one = 0b1100111,
    seg_two = 0b1001000,
    seg_three = 0b1000001,
    seg_four = 0b0100011,
    seg_five = 0b0010001,
    seg_six = 0b0010000,
    seg_seven = 0b1000111,
    seg_eight = 0b000000,
    seg_nine = 0b0000001,
    seg_test = 0b0111011,
    seg_off = 0b1111111
};

extern map<int, sev_seg_digits>  sev_seg_dict;


class SevSeg{
    public: 

        SevSeg(PinName c5, PinName c6, PinName c7, PinName c8,
                 PinName c10, PinName c11, PinName c12, PinName c1, PinName c2, PinName c17, PinName c3, PinName c15,
                 PinName c16, PinName c18);
        void SevSegWriteOnes(sev_seg_digits ones_value);
        void SevSegWriteTens(sev_seg_digits tens_value);
        void InitSegDict();

        BusOut OnesDigit;
        BusOut TensDigit;

};

#endif