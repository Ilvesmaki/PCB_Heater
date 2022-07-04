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
#include <max31865.hpp>
#include <tgmath.h>


max31865::max31865(SPI_HandleTypeDef *spi, GPIO_TypeDef * cs_port, uint16_t cs_pin ):
					_spi_handler(spi), _cs_port(cs_port), _cs_pin(cs_pin)
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

void max31865::set_resistors(const uint16_t ref, max31865_rtd_type_t rtd)
{
    _ref_val = ref;
    _rtd_type = rtd;
}

uint16_t max31865::get_raw_RTD(void)
{
    uint16_t rtd_val = 0;
    rtd_val = ((uint16_t)read_reg(MAX31865_RTD_MSB_REG_READ_ADDR) << 8);
    rtd_val |= (uint16_t)read_reg(MAX31865_RTD_LSB_REG_READ_ADDR);
    if(1 & rtd_val)
    {
        // Fault
        return 0x8000;
    }
    return ((rtd_val >> 1) & 0x7FFF);
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
        temp_c = calculate_temp_c(100);
        break;
    
    case PT1000:
        temp_c = calculate_temp_c(1000);
        break;
    
    default:
        break;
    }
    return temp_c;
}

/*---------------------------PRIVATE FUNCTIONS--------------------------------*/
float max31865::calculate_temp_c(float rtd_zero_c)
{
    const float A = 3.90830E-3;
    const float B = -5.77500E-7;
    /* this can be dismissed since we are operating allways over 0 deg
    const float C = -4.18301E-12; */
    // R(T) = R0*(1+aT+bT^2+c(T-100)T^3)

    uint32_t rtd_val = (uint32_t)get_raw_RTD();
    rtd_val *= _ref_val;
    float rtd = (float)rtd_val / 32768;
    float temp_c = (-A + sqrtf(A*A-4*B*(1-rtd/rtd_zero_c))) / (2*B);
    return temp_c;
}

uint8_t max31865::read_reg(const uint8_t &addr)
{
	uint8_t reg_addr = addr;
    uint8_t reg_val = 0;
    if(!_spi_handler || !_cs_port) return 0;
    HAL_GPIO_WritePin(_cs_port, _cs_pin, GPIO_PIN_RESET);
    if(HAL_SPI_Transmit(_spi_handler, &reg_addr, 1, 100) != HAL_OK) return 0;
    if(HAL_SPI_Receive(_spi_handler, &reg_val, 1, 100) != HAL_OK) return 0;
    HAL_GPIO_WritePin(_cs_port, _cs_pin, GPIO_PIN_SET);
    return reg_val;
}

void max31865::write_reg(const uint8_t &addr, const uint8_t &data)
{
    uint8_t databuf[2] = {addr, data};
    if(!_spi_handler || !_cs_port) return;
    HAL_GPIO_WritePin(_cs_port, _cs_pin, GPIO_PIN_RESET);
    if(HAL_SPI_Transmit(_spi_handler, &databuf[0], 2, 100) != HAL_OK) return;
    HAL_GPIO_WritePin(_cs_port, _cs_pin, GPIO_PIN_SET);
    return;
}
