/**
 * @file cli.c
 * @author Wilho-Pekka Ilvesmäki (wilho.ilvesmaki@gmail.com)
 * @brief This file provides implementations for the CLI functions.
 * @version 0.1
 * @date 2022-05-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <stdio.h>
#include <string.h>
#include "cli.h"

#define ERROR stderr
#define INFO stdout
#define TRACE(...) fprintf(__VA_ARGS__)

#define MAX_PARAMS 16

typedef struct
{
    char* param;
    void (*cb)(char*);
    char* description;
}cmd;

#define PARAM_UNKNOWN "Parameter(s) not recognized. See --help for usage."

#define PROMT ">"

#define CMD_AMOUNT 8

#define PARAM_IDENT "    "

const char* APP_DESCRIPTION = "This application controlls and monitors the heated bed for reflow soldering. The required parameters are supplied via the command line interface. ";

FILE *stream_ = NULL;

cmd COMMANDS[CMD_AMOUNT] = 
{
    {HELP_PARAMS, NULL, "Display this message"},
    {SOAK_RAMP_PARAM, NULL, "This will set the steepness of the soak ramp. Value will indicate temperature rise in seconds. Allowed values are 1.0-10.0"},
    {SOAK_TEMP_PARAM, NULL, "This will set the desired soak temperature in degrees."},
    {SOAK_TIME_S_PARAM, NULL, "This will set the time for soak. Unit: seconds."},
    {PEAK_RAMP_PARAM, NULL, "This will set the steepness of the peak ramp. Value will indicate temperature rise in seconds. Allowed values are 1.0-10.0"},
    {PEAK_TEMP_PARAM, NULL, "This will set the desired peak temperature in degrees"},
    {PEAK_TIME_S_PARAM, NULL, "This will set the time for peak (actual soldering time). Unit: seconds."},
    {COOLING_RAMP_PARAM, NULL, "This will set the steepness of the cooling ramp. Value will indicate temperature fall in seconds. Allowed values are 1.0-10.0"},
};

/** TODO: Consider strings between quotes "" or '' as a single param. this is for json params
 * @brief This parses given string in a substrings with specified delimiter
 * Resulting string will be "substr1\0substr2\0substr3\0" etc. The result 
 * will be stored in the char array pointer where each one of the substrings
 * can be accessed through the corresponding pointer as results[0], results[1], ... etc
 * total amount of substrings will be returned as int
 * 
 * @param line String to be parsed
 * @param delimiter null terminated string that contains delimiter characters to be parsed
 * @param result result char array containing enough space for the resulting substrings
 * @return int Total amount of substrings returned, -1 indicating failure
 */

int split(char* line, const char* delimiter, char *result[MAX_PARAMS])
{
    size_t len = 0;
    size_t argc = 0;

    char* token = NULL;
    token = strtok(line, delimiter);
    while (token != NULL && argc < MAX_PARAMS)
    {
        // calculate the new pointer value for a next argument
        //result[argc] = &result[0][len];
        //len += strlen(token) + 1; // remeber to add space for the null terminator
        //strcpy(result[argc], token);
        result[argc++] = token;
        token = strtok(NULL, delimiter); // find next substring
    }
    return argc;
}

int is_command(char* arg)
{
    int j;
    int command_found = -1;
    if(arg == NULL || strlen(arg) < 2 || arg[0] != '-')
    {
        return -1;
    }
    for(j = 0; j < CMD_AMOUNT; j++)
    {
        if(strstr(COMMANDS[j].param, arg) != NULL)
        {
            if(command_found >= 0)
            {
                return -1;
            }
            command_found = j;
        }
    }
    return command_found;
}

void cli(char* line)
{
    char* substrs[MAX_PARAMS];
    int parts = split(line, " \n", substrs);
    int i = 0;

    while(i < parts)
    {
        char* arg = substrs[i++];
        int found = is_command(arg);
        if(found < 0)
        {
            fprintf(stream_, "%s\n", PARAM_UNKNOWN);
        }
        else if(found == 0)
        {
            print_help(stream_);
        }
        else if(COMMANDS[found].cb == NULL)
        {
            i++; // Skip value
            TRACE(INFO, "Parameter %s doesnt't have a function.\n", COMMANDS[found].param);
        }
        else
        {
            if(i == parts)
            {
                break;
            }
            COMMANDS[found].cb(substrs[i++]);
        }
    }
}

void assign_stream_cb(FILE *stream)
{
    stream_ = stream;
}

void assign_callback(char* param, void (*callback_fn)(char*))
{
    int i = 0;
    for(i = 0; i < CMD_AMOUNT; i++)
        {
            if(strstr(COMMANDS[i].param, param) != NULL)
            {
                COMMANDS[i].cb = callback_fn;
                TRACE(INFO, "Callback assigned for command %s\n", COMMANDS[i].param);
            }
        }
}

void print_help(FILE *stream)
{
    int i = 0;
    fprintf(stream, "\n%s\n\n", APP_DESCRIPTION);
    fprintf(stream,"Usage: >param1 value1 ...\n");
    for (i = 0; i < CMD_AMOUNT;i++)
    {
        fprintf(stream, "%s%-15s%s\n", PARAM_IDENT,COMMANDS[i].param, COMMANDS[i].description);
    }
    fprintf(stream, "\n");
}