/**
 * @file max31865.cpp
 * @author Wilho-Pekka Ilvesmäki (wilho.ilvesmaki@gmail.com)
 * @brief This contains actual implementation for the functions in the class
 * @version 0.1
 * @date 2022-05-22
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "max31865.h"


max31865::max31865(SPI *spi, DigitalOut *cs): _spi_handler(spi), _cs_pin_ptr(cs)
{
}

max31865::~max31865()
{
}

void max31865::set_config(const max31865_config_t &config)
{
    uint8_t new_config = 0;
    new_config = (config.v_bias << 7) | (config.auto_conversion_mode << 6) |
                (config.one_shot_mode << 5) | (config.three_wire_mode << 4) |
                (config.fault_detection_cycle & 0xC) | (config.fault_status_clear << 1) |
                (config.fifty_hz_filter);
                
    write_reg(MAX31865_CONFIG_REG_WRITE_ADDR, new_config);
}

void max31865::set_resistors(const uint16_t ref)
{
    _ref_val = ref;
}

uint16_t max31865::get_raw_RTD(void)
{
    uint16_t rtd_val = 0;
    rtd_val = ((uint16_t)read_reg(MAX31865_RTD_MSB_REG_READ_ADDR) << 8);
    rtd_val |= (uint16_t)read_reg(MAX31865_RTD_LSB_REG_READ_ADDR);
    if(1 && rtd_val)
    {
        // Fault
        return 0;
    }
    return (rtd_val >> 1);
}

float max31865::get_temp()
{
    float temp_c = -273.15; // absolute zero as indicating failure
    if(_ref_val == 0)
    {
        return temp_c;
    }
    switch (_rtd_type)
    {
    case PT100:
        temp_c = calculate_temp_c_pt100();
        break;
    
    case PT1000:
        temp_c = calculate_temp_c_pt1000();
        break;
    
    default:
        break;
    }
    return temp_c;
}

/*---------------------------PRIVATE FUNCTIONS--------------------------------*/
float max31865::calculate_temp_c_pt100(void)
{
    const float RTD_ZERO_C = 100;
    const float A = 3.90830E-3;
    const float B = -5.77500E-7;
    /* this can be dismissed since we are operating allways over 0 deg
    const float C = -4.18301E-12; */
    // R(T) = R0*(1+aT+bT^2+c(T-100)T^3)

    uint32_t rtd_val = (uint32_t)get_raw_RTD();
    rtd_val *= _ref_val;
    float rtd = (float)rtd_val / 32768;
    float temp_c = (-A + sqrtf(A*A-4*B*(1-rtd/RTD_ZERO_C))) / (2*B);
    return temp_c;
}

float max31865::calculate_temp_c_pt1000(void)
{

}

uint8_t max31865::read_reg(const uint8_t &addr)
{
    int reg_val = 0;
    if(_cs_pin_ptr)
    {
        *_cs_pin_ptr = 0;
    }
    else
    {
        _spi_handler->select();
    }

    _spi_handler->write((int)addr);
    reg_val = _spi_handler->write(0xFF);

    if(_cs_pin_ptr)
    {
        *_cs_pin_ptr = 1;
    }
    else
    {
        _spi_handler->deselect();
    }
    return (uint8_t)reg_val;
}

void max31865::write_reg(const uint8_t &addr, const uint8_t &data)
{
    if(_cs_pin_ptr)
    {
        *_cs_pin_ptr = 0;
    }
    else
    {
        _spi_handler->select();
    }

    _spi_handler->write((int)addr);
    _spi_handler->write((int)data);

    if(_cs_pin_ptr)
    {
        *_cs_pin_ptr = 1;
    }
    else
    {
        _spi_handler->deselect();
    }
}