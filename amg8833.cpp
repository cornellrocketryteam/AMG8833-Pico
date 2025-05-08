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

bool AMG8833::read_register(const uint8_t reg, uint8_t *buf, uint8_t len) {

     // Write the register address first
    if (i2c_write_timeout_us(i2c, AMG8833_ADDR, &reg, 1, true, AMG8833_BYTE_TIMEOUT_US) < 1) {
        return false;
    }

    // Read the register value
    if (i2c_read_timeout_us(i2c, AMG8833_ADDR, buf, len, false, len * AMG8833_BYTE_TIMEOUT_US) != len) {
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

bool AMG8833::read_thermistor(float *data){
    uint8_t low, high;
    uint16_t raw;

    if (!read_register(AMG88xx_TTHL, &low, 2)) return false;
    if (!read_register(AMG88xx_TTHH, &high, 2)) return false;
    raw = ((uint16_t)high << 8) | low;

    *data = (float)(raw * AMG8833_THERMISTOR_CONVERSION);
    
    return true;
}

bool AMG8833::read_pixels(float *pixel_array){
    uint8_t byte_num = (uint8_t)AMG8833_PIXEL_NUM << 1;
    uint8_t raw_array[byte_num];
    uint16_t raw;
    if (!read_register(AMG88xx_PIXEL_OFFSET, raw_array, byte_num)) return false;

    for (int i = 0; i < AMG8833_PIXEL_NUM; i++) {
        uint8_t pos = i << 1;
        raw = ((uint16_t)raw_array[pos + 1] << 8) | ((uint16_t)raw_array[pos]);
    
        pixel_array[i] = (float)raw * AMG8833_PIXEL_TEMP_CONVERSION;
    }

    return true;
}

void AMG8833::convert_to_heatmap(float *temps, RGB *colors, float min_temp, float max_temp) {
    for (int i = 0; i < AMG8833_PIXEL_NUM; i++) {
        float t = (temps[i] - min_temp) / (max_temp - min_temp);
        t = t < 0 ? 0 : t > 1 ? 1 : t; // Clamp to [0, 1]

        RGB color;

        // Blue → Cyan → Green → Yellow → Red
        if (t < 0.25f) { // Blue to Cyan
            color.r = 0;
            color.g = (uint8_t)(255 * (t / 0.25f));
            color.b = 255;
        } else if (t < 0.5f) { // Cyan to Green
            color.r = 0;
            color.g = 255;
            color.b = (uint8_t)(255 * (1 - (t - 0.25f) / 0.25f));
        } else if (t < 0.75f) { // Green to Yellow
            color.r = (uint8_t)(255 * ((t - 0.5f) / 0.25f));
            color.g = 255;
            color.b = 0;
        } else { // Yellow to Red
            color.r = 255;
            color.g = (uint8_t)(255 * (1 - (t - 0.75f) / 0.25f));
            color.b = 0;
        }

        colors[i] = color;
    }
}

