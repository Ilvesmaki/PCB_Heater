/**
 * @file cli.h
 * @author Wilho-Pekka Ilvesmäki (wilho.ilvesmaki@gmail.com)
 * @brief Simple Command Line Interface attached to the device UART port #1?.
 * CLI can be used to update new values for the heater and the user can get 
 * status updates from the device.
 * This file provides function declarations.
 * @version 0.1
 * @date 2022-05-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
typedef struct
{
  float soak_ramp;
  float soak_temp;
  float soak_time_s;
  float peak_ramp;
  float peak_time_s;
  float cool_ramp;
} heater_settings_t;

#ifndef __CLI__H__
#define __CLI__H__

void cli(char *line, heater_settings_t *settings )
{
        
}

#endif  //!__CLI__H__