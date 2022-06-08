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

#include "mbed.h"

#define MOSI_PIN  PA_7
#define MISO_PIN  PA_6
#define SCLK_PIN  PA_5
#define SS_PIN    PA_4
#define LED_PIN   PC_13

#define TX1_PIN PA_9
#define RX1_PIN PA_10
#define TX2_PIN PA_2
#define RX2_PIN PA_3
#define TX3_PIN PB_10
#define RX3_PIN PB_11

typedef struct
{
  float soak_ramp;
  float soak_temp;
  float soak_time_s;
  float peak_ramp;
} heater_settings_t;

DigitalOut led(LED_PIN);

SPI lcd(MOSI_PIN, MISO_PIN, SCLK_PIN);

USBSerial serial;

//static Serial usb(USBTX, USBRX); same as serial2
static Serial serial1(TX1_PIN, RX1_PIN);
static Serial serial2(TX2_PIN, RX2_PIN);
static Serial serial3(TX3_PIN, RX3_PIN);

int main() 
{
  // put your setup code here, to run once:
  serial.printf("Hello from usb\n");
  serial1.printf("Hello from Serial1\n");
  serial2.printf("Hello from Serial2\n");
  serial3.printf("Hello from Serial3\n");

  led = 0;
  uint16_t val = 0;
  while(1) {
    led = !led; ///toggle
    wait_ms(val); //
    val++;
    // put your main code here, to run repeatedly:
  }
}