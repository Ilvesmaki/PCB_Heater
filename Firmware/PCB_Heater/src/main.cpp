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
#define LED_PIN   PC_13

typedef struct
{
  float soak_ramp;
  float soak_temp;
  float soak_time_s;
  float peak_ramp;
  float peak_time_s;
  float cool_ramp;
} heater_settings_t;

DigitalOut led(LED_PIN);

SPI lcd(MOSI_PIN, MISO_PIN, SCLK_PIN);

int main() 
{
  // put your setup code here, to run once:
  led = 0;
  uint16_t val = 0;
  while(1) {
    led = !led; ///toggle
    wait_ms(val); //
    val++;
    // put your main code here, to run repeatedly:
  }
}