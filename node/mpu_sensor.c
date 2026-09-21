#include <stdio.h>
#include "driver/i2c.h"
#include "mpu6050.h"
#include "mpu_sensor.h"
#include "esp_system.h"
#include "esp_log.h"

static const char *TAG = "mpu6050";
static mpu6050_handle_t mpu6050 = NULL;


static void check(esp_err_t ret)
{
   if (ret!=ESP_OK) { ESP_LOGE(TAG, "erreur mpu6050: %d", ret);}
}

static void i2c_bus_init(void)
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = (gpio_num_t)I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;

    esp_err_t ret = i2c_param_config(I2C_MASTER_NUM, &conf);
    check(ret);

    ret = i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
    check(ret);
}


mpu6050_handle_t mpu6050_init(void)
{
    esp_err_t ret;

    i2c_bus_init();
    mpu6050 = mpu6050_create(I2C_MASTER_NUM, MPU6050_I2C_ADDRESS);

    ret = mpu6050_config(mpu6050, ACCE_FS_4G, GYRO_FS_500DPS);
    check(ret);

    ret = mpu6050_wake_up(mpu6050);
    check(ret);

    return mpu6050;
}

esp_err_t mpu6050_read(mpu6050_handle_t mpu6050,
                       mpu6050_acce_value_t *acce,
                       mpu6050_gyro_value_t *gyro,
                       mpu6050_temp_value_t *temp)
{
    esp_err_t ret;

    ret = mpu6050_get_acce(mpu6050, acce);
    check(ret);

    ret = mpu6050_get_gyro(mpu6050, gyro);
    check(ret);

    ret = mpu6050_get_temp(mpu6050, temp);
    check(ret);

    return ret;
}
