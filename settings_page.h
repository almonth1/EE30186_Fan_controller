#ifndef SETTINGS_PAGE_H
#define SETTINGS_PAGE_H
#include "mbed.h"
#include "pins_config.h"
#include "ButtonInput.h"
#include "LCDText.h"
#include "pid_controller.h"

//** Safety
extern int Temperature_Warning;
extern int Abnormal_Speed;
 
extern int page_number;
extern int number_of_pages;
void ShowOTWPageOne(int OTW_val);
void ShowAbSpeedPageTwo(int Abnormal_Speed);
void ShowStatsPageThree(float duty);
void ShowHelpPageFour();
void PageHandler(int page_num, int select);
void BackButton();
void SelButton(int page_num);
void NextButton();







#endif