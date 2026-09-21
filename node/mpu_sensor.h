#include "driver/i2c.h"
#include "mpu6050.h"

#ifndef MPU6050_H
#define MPU6050_H


#define I2C_MASTER_SCL_IO 36
#define I2C_MASTER_SDA_IO 35
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000

mpu6050_handle_t mpu6050_init(void);

esp_err_t mpu6050_read(mpu6050_handle_t mpu6050,
                        mpu6050_acce_value_t *acce,
                        mpu6050_gyro_value_t *gyro,
                        mpu6050_temp_value_t *temp);

#endif




