#include <stdio.h>
#include <stdlib.h>
#include "cli.h"

void updateparam(char *value)
{
    printf("new value assigned: %f\n", atof(value));
}

int main(int argc, char const *argv[])
{
    heater_settings_t settings = {
        .soak_ramp = 0,
        .soak_temp = 0,
        .soak_time_s = 0,
        .peak_ramp = 0,
        .peak_temp = 0,
        .peak_time_s = 0,
        .cool_ramp = 0
    };
    char input[512];
    assign_callback(SOAK_TIME_S_PARAM, updateparam);
    assign_stream_cb(stdout);

    while(1)
    {
        printf(">");
        fgets(input, sizeof(input), stdin);
        cli(input);
    }
    return 0;
}
