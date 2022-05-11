/**
 * @file main.cpp
 * @author Wilho-Pekka Ilvesmäki (wilho.ilvesmaki@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <mbed.h>

#define MOSI_PIN  PA_7
#define MISO_PIN  PA_6
#define SCLK_PIN  PA_5
#define SS_PIN    PA_4

SPI lcd(MOSI_PIN, MISO_PIN, SCLK_PIN, SS_PIN);

int main() 
{

  // put your setup code here, to run once:

  while(1) {
    // put your main code here, to run repeatedly:
  }
}