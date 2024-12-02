#include "sev_seg_display.h"
#include <cstdio>



//  SevSeg::OnesDigit(PinName c5, PinName c6, PinName c7, 
//                 PinName c8, PinName c10, PinName c11, 
//                 PinName c12)
map<int, sev_seg_digits>  sev_seg_dict;

SevSeg::SevSeg(PinName c5, PinName c6, PinName c7, PinName c8,
                 PinName c10, PinName c11, PinName c12, PinName c1, PinName c2, PinName c17, PinName c3, PinName c15,
                 PinName c16,  PinName c18) : OnesDigit(c5, c6, c7, c8, c10, c11, c12), TensDigit(c1, c2, c17, c3, c15, c16, c18){ 
            }

void SevSeg::SevSegWriteOnes(sev_seg_digits ones_value){

    OnesDigit.write(ones_value);
    //printf("Ones Written!");
}

void SevSeg::SevSegWriteTens(sev_seg_digits tens_value){

    TensDigit.write(tens_value);

}

void SevSeg::InitSegDict(){
    sev_seg_dict[0] = seg_zero;
    sev_seg_dict[1] = seg_one;
    sev_seg_dict[2] = seg_two;
    sev_seg_dict[3] = seg_three;
    sev_seg_dict[4] = seg_four;
    sev_seg_dict[5] = seg_five;
    sev_seg_dict[6] = seg_six;
    sev_seg_dict[7] = seg_seven;
    sev_seg_dict[8] = seg_eight;
    sev_seg_dict[9] = seg_nine;
    sev_seg_dict[10] = seg_off;
}

