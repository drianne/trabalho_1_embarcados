#include "bme280_defs.h"
#include "../src/main.h"
#define I2C_DEVICE "/dev/i2c-1"

/* Structure that contains identifier details used in example */
struct identifier
{
    uint8_t dev_addr;
    int8_t fd;
};


int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev, struct Sensors *temps);
void *temperature_control(void *params);