/**
 * @file max31865.h
 * @author Wilho-Pekka Ilvesmäki (wilho.ilvesmaki@gmail.com)
 * @brief This file provides a class to control MAX31865 chip with mbed framework
 * @version 0.1
 * @date 2022-05-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __MAX31865__H__
#define __MAX31865__H__

#include <stdint.h> // for uint8_t
#include <mbed.h>

#define MAX31865_CONFIG_REG_READ_ADDR           0x00
#define MAX31865_CONFIG_REG_WRITE_ADDR          0x80
#define MAX31865_RTD_MSB_REG_READ_ADDR          0x01
#define MAX31865_RTD_LSB_REG_READ_ADDR          0x02
#define MAX31865_HIGH_FAULT_MSB_REG_READ_ADDR   0x03
#define MAX31865_HIGH_FAULT_MSB_REG_WRITE_ADDR  0x83
#define MAX31865_HIGH_FAULT_LSB_REG_READ_ADDR   0x04
#define MAX31865_HIGH_FAULT_LSB_REG_WRITE_ADDR  0x84
#define MAX31865_LOW_FAULT_MSB_REG_READ_ADDR    0x05
#define MAX31865_LOW_FAULT_MSB_REG_WRITE_ADDR   0x85
#define MAX31865_LOW_FAULT_LSB_REG_READ_ADDR    0x06
#define MAX31865_LOW_FAULT_LSB_REG_WRITE_ADDR   0x86
#define MAX31865_FAULT_STATUS_REG_READ_ADDR     0x07

enum max31865_rtd_type_t
{
    PT100,
    PT1000,
    UNKNOWN
};


typedef struct
{
    bool v_bias;
    bool auto_conversion_mode;
    bool one_shot_mode;
    bool three_wire_mode;
    uint8_t fault_detection_cycle;
    bool fault_status_clear;
    bool fifty_hz_filter;
} max31865_config_t;

const max31865_config_t MAX31865_DEFAULT_CONFIG = {
    .v_bias = false,
    .auto_conversion_mode = false,
    .one_shot_mode = false,
    .three_wire_mode = false,
    .fault_detection_cycle = 0,
    .fault_status_clear = false,
    .fifty_hz_filter = false
};

class max31865
{
private:
    /* private data */
    max31865_config_t _current_config = MAX31865_DEFAULT_CONFIG;
    max31865_rtd_type_t _rtd_type = UNKNOWN;
    uint16_t _ref_val = 0;

    SPI *_spi_handler = nullptr;
    DigitalOut *_cs_pin_ptr = nullptr;
    
    /* private functions */
    float calculate_temp_c_pt100(void);
    float calculate_temp_c_pt1000(void);

    uint8_t read_reg(const uint8_t &addr);
    void write_reg(const uint8_t &addr, const uint8_t &data);
public:
    max31865(SPI *spi, DigitalOut *cs);
    ~max31865();
    
    void set_config(const max31865_config_t &config);
    void set_resistors(const uint16_t ref);

    uint16_t get_raw_RTD(void);
    float get_temp();
};




#endif  //!__MAX31865__H__