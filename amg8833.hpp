#ifndef AMG8833_HPP
#define AMG8833_HPP

#include "hardware/i2c.h"
#include "pico/stdlib.h"

#define AMG8833_ADDR (0x69)
#define AMG8833_BYTE_TIMEOUT_US (1500)

#define AMG8833_PIXEL_NUM 64
#define AMG8833_PIXEL_TEMP_CONVERSION .25
#define AMG8833_THERMISTOR_CONVERSION .0625

enum {
    AMG88xx_PCTL = 0x00,
    AMG88xx_RST = 0x01,
    AMG88xx_FPSC = 0x02,
    AMG88xx_INTC = 0x03,
    AMG88xx_STAT = 0x04,
    AMG88xx_SCLR = 0x05,
    // 0x06 reserved
    AMG88xx_AVE = 0x07,
    AMG88xx_INTHL = 0x08,
    AMG88xx_INTHH = 0x09,
    AMG88xx_INTLL = 0x0A,
    AMG88xx_INTLH = 0x0B,
    AMG88xx_IHYSL = 0x0C,
    AMG88xx_IHYSH = 0x0D,
    AMG88xx_TTHL = 0x0E,
    AMG88xx_TTHH = 0x0F,
    AMG88xx_INT_OFFSET = 0x010,
    AMG88xx_PIXEL_OFFSET = 0x80
  };
  
  enum power_modes {
    AMG88xx_NORMAL_MODE = 0x00,
    AMG88xx_SLEEP_MODE = 0x01,
    AMG88xx_STAND_BY_60 = 0x20,
    AMG88xx_STAND_BY_10 = 0x21
  };
  
  enum sw_resets { AMG88xx_FLAG_RESET = 0x30, AMG88xx_INITIAL_RESET = 0x3F };
  
  enum frame_rates { AMG88xx_FPS_10 = 0x00, AMG88xx_FPS_1 = 0x01 };
  
  enum int_enables { AMG88xx_INT_DISABLED = 0x00, AMG88xx_INT_ENABLED = 0x01 };
  
  enum int_modes { AMG88xx_DIFFERENCE = 0x00, AMG88xx_ABSOLUTE_VALUE = 0x01 };

  struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};


/**
 * Representation of the AMG8833 sensor.
 */
class AMG8833 {
public:
    /**
     * Initalizes an AMG8833 object on an I2C bus. 
     * @param i2c_type The I2C bus that the sensor is on
     */
    AMG8833(i2c_inst_t *i2c_type);

    /**
     * TODO: write spec
     */
    bool begin();

    /**
     * Reads a value from the thermistor.
     * @param data The array to store the
     * thermistor data
     */
    bool read_thermistor(float *data);

    /**
     * TODO: write spec
     */
    bool read_pixels(float *pixel_array);

    /**
     * TODO: write spec
     */
    static void convert_to_heatmap(float *temps, RGB *colors, float min_temp, float max_temp);

private:
    /**
     * Write a value to a specified register.
     * @param reg The register to write to
     * @param val The value to write
     */
    bool write_register(const uint8_t reg, const uint16_t val);

    /**
     * Read the value from a specified register.
     * @param reg The register to read from
     * @param buf The array to store the read value
     * @param len The number of bytes beimg read
     */
    bool read_register(const uint8_t reg, uint8_t *buf, uint8_t len);

    /**
     * The I2C bus.
     */
    i2c_inst_t *i2c;
};

#endif // AMG8833_HPP
