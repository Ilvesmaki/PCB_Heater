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
  float peak_temp;
  float peak_time_s;
  float cool_ramp;
} heater_settings_t;

#ifndef __CLI__H__
#define __CLI__H__

// Allowed arguments for each function. If many arguments are specified, seperate them with spaces.
#define SOAK_RAMP_PARAM "-sr"
#define SOAK_TEMP_PARAM "-st"
#define SOAK_TIME_S_PARAM "-stim"
#define PEAK_RAMP_PARAM "-pr"
#define PEAK_TEMP_PARAM "-pt"
#define PEAK_TIME_S_PARAM "-ptim"
#define COOLING_RAMP_PARAM "-cr"
#define HELP_PARAMS "-H, --help"

void cli(char *line);

void assign_stream_cb(FILE *stream);

void assign_callback(char* param, void (*callback_fn)(char*));

void print_help(FILE *stream);

#endif  //!__CLI__H__