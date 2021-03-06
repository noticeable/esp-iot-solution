/*
  * ESPRESSIF MIT License
  *
  * Copyright (c) 2017 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
  *
  * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
  * it is free of charge, to any person obtaining a copy of this software and associated
  * documentation files (the "Software"), to deal in the Software without restriction, including
  * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
  * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
  * to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all copies or
  * substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  *
  */
  
#define LIS2DH12_TEST_CODE 1
#if LIS2DH12_TEST_CODE

#include <stdio.h>
#include "driver/i2c.h"
#include "lis2dh12.h"
#include "unity.h"

#define I2C_MASTER_SCL_IO    19        /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO    18        /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM     I2C_NUM_1   /*!< I2C port number for master dev */
#define I2C_MASTER_TX_BUF_DISABLE   0  /*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0  /*!< I2C master do not need buffer */
#define I2C_MASTER_FREQ_HZ    100000   /*!< I2C master clock frequency */

static i2c_bus_handle_t i2c_bus = NULL;
static lis2dh12_handle_t sens = NULL;

/**
 * @brief i2c master initialization
 */
void i2c_master_init()
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    i2c_bus = i2c_bus_create(I2C_MASTER_NUM, &conf);
    sens = sensor_lis2dh12_create(i2c_bus, LIS2DH12_I2C_ADDRESS);
}

void lis2dh12_test_task(void* pvParameters)
{
    uint8_t deviceid;
    uint16_t x_acc;
    uint16_t y_acc;
    uint16_t z_acc;
    lis2dh12_get_deviceid(sens, &deviceid);
    printf("LIS2DH12 device id is: %02x\n", deviceid);

    lis2dh12_config_t  lis2dh12_config;
    lis2dh12_get_config(sens, &lis2dh12_config);
    printf("temp_enable is: %02x\n", lis2dh12_config.temp_enable);
    printf("odr is: %02x\n", lis2dh12_config.odr);
    printf("option mode is: %02x\n", lis2dh12_config.opt_mode);
    printf("z_enable status is: %02x\n", lis2dh12_config.z_enable);
    printf("y_enable status is: %02x\n", lis2dh12_config.y_enable);
    printf("x_enable status is: %02x\n", lis2dh12_config.x_enable);
    printf("bdu_status status is: %02x\n", lis2dh12_config.bdu_status);
    printf("full scale is: %02x\n", lis2dh12_config.fs);

    lis2dh12_config.temp_enable = LIS2DH12_TEMP_DISABLE;
    lis2dh12_config.odr = LIS2DH12_ODR_1HZ;
    lis2dh12_config.opt_mode = LIS2DH12_OPT_NORMAL;
    lis2dh12_config.z_enable = LIS2DH12_ENABLE;
    lis2dh12_config.y_enable = LIS2DH12_ENABLE;
    lis2dh12_config.x_enable = LIS2DH12_ENABLE;
    lis2dh12_config.bdu_status = LIS2DH12_DISABLE;
    lis2dh12_config.fs = LIS2DH12_FS_16G;
    lis2dh12_set_config(sens, &lis2dh12_config);

    while(1){
        printf("\n******************************************\n");
        lis2dh12_get_x_acc(sens, &x_acc);
        printf("X-axis acceleration is: %08x\n", x_acc);
        lis2dh12_get_y_acc(sens, &y_acc);
        printf("Y-axis acceleration is: %08x\n", y_acc);
        lis2dh12_get_z_acc(sens, &z_acc);
        printf("Z-axis acceleration is: %08x\n", z_acc);
        printf("******************************************\n");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void lis2dh12_test()
{
    i2c_master_init();
    vTaskDelay(1000 / portTICK_RATE_MS);
    xTaskCreate(lis2dh12_test_task, "lis2dh12_test_task", 1024 * 2, NULL, 10, NULL);
}

TEST_CASE("Sensor lis2dh12 test", "[lis2dh12][iot][sensor]")
{
    lis2dh12_test();
}
#endif

