#include "settings_page.h"


int page_number = 1;
int number_of_pages = 4;
int Temperature_Warning = 80;
int Abnormal_Speed = 2000;

void ShowOTWPageOne(int OTW_val){
    lcd.cls(); 
    wait_us(2000);          // Clear screen
    lcd.locate(0,0);     // Move cursor to (0,0)
    lcd.printf("  OTW Threshold"); 
    lcd.locate(0,1);
    lcd.printf("     Temp:%d", OTW_val);    
    wait_us(2000);
};
void ShowAbSpeedPageTwo(int Abnormal_Speed){
    lcd.cls(); 
    wait_us(2000);          // Clear screen
    lcd.locate(0,0);     // Move cursor to (0,0)
    lcd.printf("Abnormal Speed"); 
    lcd.locate(0,1);
    lcd.printf("Abn Speed:%d", Abnormal_Speed);    
    wait_us(2000);
}

void ShowStatsPageThree(float duty){
    lcd.cls(); 
    wait_us(2000);          // Clear screen
    lcd.locate(0,0);     // Move cursor to (0,0)
    lcd.printf("Duty Cycle:%0.1f%%", duty*100); 
    lcd.locate(0,1);
    lcd.printf("AVG Volt:%0.1fV", duty*12);    
    wait_us(2000);
}

void ShowHelpPageFour(){
    lcd.cls(); 
    wait_us(2000);          // Clear screen
    lcd.locate(0, 0);     // Move cursor to (0,0)
    lcd.printf("BTN => Mode"); 
    lcd.locate(0, 1);     // Move cursor to (0,0)
    lcd.printf("ROT => Val"); 

}
void PageHandler(int page_num, int select){

    switch (page_number) {
        case 1:
            lcd.cls(); 
            wait_us(2000);          // Clear screen
            lcd.locate(0,0);     // Move cursor to (0,0)
            lcd.printf("OTW Threshold"); 
            wait_us(2000);
            break;
        case 2:
            lcd.cls(); 
            wait_us(2000);          // Clear screen
            lcd.locate(0,0);     // Move cursor to (0,0)
            lcd.printf("Abnormal Speed"); 
            wait_us(2000);
            break;
        case 3:
            lcd.cls(); 
            wait_us(2000);          // Clear screen
            lcd.locate(0,0);     // Move cursor to (0,0)
            lcd.printf("Voltage Stats"); 
            wait_us(2000);
            break;
        case 4:
            lcd.cls(); 
            wait_us(2000);          // Clear screen
            lcd.locate(0,0);     // Move cursor to (0,0)
            lcd.printf("User Guide"); 
            wait_us(2000);
            break;
        default:
            lcd.cls(); 
            wait_us(2000);          // Clear screen
            lcd.locate(0,0);     // Move cursor to (0,0)
            lcd.printf("Error Unknown Page"); 
            wait_us(2000);
            break;

    }
    
    switch (select) {
        case 1:
            wait_us(2000);          // Clear screen
            lcd.locate(0, 1);     // Move cursor to (0,0)
            lcd.printf(">Back  Sel  Next"); 
            wait_us(2000);
            break;
        case 2:
            wait_us(2000);          // Clear screen
            lcd.locate(0, 1);     // Move cursor to (0,0)
            lcd.printf("Back  >Sel  Next"); 
            wait_us(2000);
            break;
        case 3:
            wait_us(2000);          // Clear screen
            lcd.locate(0, 1);     // Move cursor to (0,0)
            lcd.printf("Back  Sel  >Next"); 
            wait_us(2000);
            break;
        default:
            wait_us(2000);          // Clear screen
            lcd.locate(0,0);     // Move cursor to (0,0)
            lcd.printf("Error Unknown selection"); 
            wait_us(2000);
            break;
    }

};

void BackButton(){
    page_number--;
    if(page_number > number_of_pages){
        page_number = 1;
    }
    else if (page_number < 1) {
        page_number = number_of_pages;
    }
};
void SelButton(int page_num){
    Button_Mode = 10 + page_num;

};
void NextButton(){
    page_number++;
    if(page_number > number_of_pages){
        page_number = 1;
    }
    else if (page_number < 1) {
        page_number = number_of_pages;
    }
};