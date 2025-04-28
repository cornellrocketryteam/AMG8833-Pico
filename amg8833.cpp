#include "amg8833.hpp"
#include <cstdio>

AMG8833::AMG8833(i2c_inst_t *i2c_type) {
    i2c = i2c_type;
}

bool AMG8833::write_register(const uint8_t reg, const uint16_t val) {
    uint8_t buf[3];
    buf[0] = reg;
    buf[1] = (val >> 8) & 0xFF;
    buf[2] = val & 0xFF;

    if (i2c_write_timeout_us(i2c, AMG8833_ADDR, buf, 3, true, AMG8833_BYTE_TIMEOUT_US) < 1) {
        return false;
    }

    return true;
}

bool AMG8833::read_register(const uint8_t reg, uint8_t *val) {

     // Write the register address first
    if (i2c_write_timeout_us(i2c, AMG8833_ADDR, &reg, 1, true, AMG8833_BYTE_TIMEOUT_US) < 1) {
        return false;
    }

    // Read the register value
    if (i2c_read_timeout_us(i2c, AMG8833_ADDR, val, 2, false, 6 * AMG8833_BYTE_TIMEOUT_US) != 2) {
        return false;
    }

    return true;
}

bool AMG8833::begin(){
    if((!write_register(AMG88xx_PCTL, AMG88xx_NORMAL_MODE)) || 
        (!write_register(AMG88xx_RST, AMG88xx_INITIAL_RESET)) ||
        (!write_register(AMG88xx_FPSC, AMG88xx_FPS_10)) ||
        (!write_register(AMG88xx_INTC, 0))){
        return false;
    }

    sleep_ms(100);

    return true;
}

bool AMG8833::read_thermistor(uint16_t *data){
    uint8_t low, high;
    if (!read_register(AMG88xx_TTHL, &low)) return false;
    if (!read_register(AMG88xx_TTHH, &high)) return false;
    
    *data = ((uint16_t)high << 8) | low;
    
    return true;
}