#include "../amg8833.hpp"
#include "tusb.h"
#include <cstdio>

#define I2C_PORT i2c0
#define I2C_SDA 0
#define I2C_SCL 1

AMG8833 thermal_cam(I2C_PORT);

int main() {
    stdio_init_all();

    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    while (!tud_cdc_connected()) {
        sleep_ms(500);
    }
    printf("Connected\n");

    while (!thermal_cam.begin()) {
        printf("Error: Thermal camera failed to initialize\n");
        sleep_ms(1000);
    }

    printf("Thermal camera initialized");

    uint16_t data;

    while (true) {
        if (!thermal_cam.read_thermistor(&data)) {
            printf("Error: Thermal camera failed to read thermisotr\n");
        }

        printf("Thermistor: %u\n", data);
        sleep_ms(50);
    }

    return 0;
}