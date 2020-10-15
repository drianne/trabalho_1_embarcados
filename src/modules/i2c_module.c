/**\
 * Copyright (c) 2020 Bosch Sensortec GmbH. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 **/

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

/******************************************************************************/
/*!                         System header files                               */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>   


/******************************************************************************/
/*!                         Own header files                                  */
#include "../../inc/bme/bme280.h"

/******************************************************************************/
/*!                               Structures                                  */

struct identifier
{
    /* Variable to hold device address */
    uint8_t dev_addr;
    /* Variable that contains file descriptor */
    int8_t fd;
};

/****************************************************************************/
/*!                         Functions                                       */

/*! Function that creates a mandatory delay required in some of the APIs.   */ 
/* period: Delay in microseconds - intf_ptr: Void pointer that can enable 
 * the linking of descriptors for interface related call backs */

void user_delay_us(uint32_t period, void *intf_ptr);

/*! Function for print the temperature */
/* comp_data:   Structure instance of bme280_data*/

void print_sensor_data(struct bme280_data *comp_data);

/*! Function for reading the sensor's registers through I2C bus. */
/* reg_addr: Register address 
 * data: Pointer to the data buffer to store the read data. 
 * len: No of bytes to read.
 * intf_ptr  : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs. */
/*  ret 0 -> Success; ret > 0 -> Failure Info */

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);

/*! Function for writing the sensor's registers through I2C bus. */
/* reg_addr: Register address. 
 *  data: Pointer to the data buffer whose value is to be written.
 *  len: No of bytes to write.
 *  intf_ptr  : Void pointer that can enable the linking of descriptors
 *                                  for interface related call backs
 *  ret BME280_OK -> Success
 *  ret BME280_E_COMM_FAIL -> Communication failure. */

int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);

/*! Function reads temperature, humidity and pressure data in forced mode.
 * dev: Structure instance of bme280_dev. *
 * ret BME280_OK - Success.
 * ret BME280_E_NULL_PTR - Error: Null pointer error
 * ret BME280_E_COMM_FAIL - Error: Communication fail error
 * ret BME280_E_NVM_COPY_FAILED - Error: NVM copy failed */

int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev);

int get_external_temperature(int argc, char* argv[])
{   
    float value;
    struct bme280_dev dev;
    struct identifier id;

    /* Variable to define the result */
    int8_t rslt = BME280_OK;

    if (argc < 2)
    {
        fprintf(stderr, "Missing argument for i2c bus.\n");
        exit(1);
    }

    if ((id.fd = open(argv[1], O_RDWR)) < 0)
    {
        fprintf(stderr, "Failed to open the i2c bus %s\n", argv[1]);
        exit(1);
    }

    /* Make sure to select BME280_I2C_ADDR_PRIM or BME280_I2C_ADDR_SEC as needed */
    id.dev_addr = BME280_I2C_ADDR_PRIM;
    
    if (ioctl(id.fd, I2C_SLAVE, id.dev_addr) < 0)
    {
        fprintf(stderr, "Failed to acquire bus access and/or talk to slave.\n");
        exit(1);
    }

    dev.intf = BME280_I2C_INTF;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_us = user_delay_us;

    /* Update interface pointer with the structure that contains both device address and file descriptor */
    dev.intf_ptr = &id;

    /* Initialize the bme280 */
    rslt = bme280_init(&dev);
    if (rslt != BME280_OK)
    {
        fprintf(stderr, "Failed to initialize the device (code %+d).\n", rslt);
        exit(1);
    }

    value = stream_sensor_data_forced_mode(&dev);

    return value;
}

/*!
 * @brief This function reading the sensor's registers through I2C bus.
 */
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr)
{
    struct identifier id;

    id = *((struct identifier *)intf_ptr);

    write(id.fd, &reg_addr, 1);
    read(id.fd, data, len);

    return 0;
}

/*!
 * @brief This function provides the delay for required time (Microseconds) as per the input provided in some of the
 * APIs
 */
void user_delay_us(uint32_t period, void *intf_ptr)
{
    usleep(period);
}

/*!
 * @brief This function for writing the sensor's registers through I2C bus.
 */
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr)
{
    uint8_t *buf;
    struct identifier id;

    id = *((struct identifier *)intf_ptr);

    buf = malloc(len + 1);
    buf[0] = reg_addr;
    memcpy(buf + 1, data, len);

    if (write(id.fd, buf, len + 1) < (uint16_t)len)
    {
        return BME280_E_COMM_FAIL;
    }

    free(buf);

    return BME280_OK;
}

/*!
 * @brief This API used to print the sensor temperature, pressure and humidity data.
 */
void print_sensor_data(struct bme280_data *comp_data)
{
    float temp, press, hum;

#ifdef BME280_FLOAT_ENABLE
    temp = comp_data->temperature;
    press = 0.01 * comp_data->pressure;
    hum = comp_data->humidity;
#else
#ifdef BME280_64BIT_ENABLE
    temp = 0.01f * comp_data->temperature;
    press = 0.0001f * comp_data->pressure;
    hum = 1.0f / 1024.0f * comp_data->humidity;
#else
    temp = 0.01f * comp_data->temperature;
    press = 0.01f * comp_data->pressure;
    hum = 1.0f / 1024.0f * comp_data->humidity;
#endif
#endif
    printf("%0.2lf deg C, %0.2lf hPa, %0.2lf%%\n", temp, press, hum);
}

void save_into_csv(float temperatura_interna, float temperatura_externa, float temperatura_recebida){
    time_t rawtime;
    FILE *fp;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );  

    fp=fopen("temps.csv","a+");
    fprintf(fp,"%s,%0.2lf,%0.2lf,%0.2lf",asctime (timeinfo), temperatura_interna, temperatura_externa ,temperatura_recebida);
    fclose(fp);
    printf("\nGravado!");
}

/*!
 * @brief This API reads the sensor temperature, pressure and humidity data in forced mode.
 */
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev)
{   
    FILE *fp;
    time_t rawtime;
    struct tm * timeinfo;

    /* Variable to define the result */
    int8_t rslt = BME280_OK;

    /* Variable to define the selecting sensors */
    uint8_t settings_sel = 0;

    /* Variable to store minimum wait time between consecutive measurement in force mode */
    uint32_t req_delay;

    /* Structure to get the external temperature */
    struct bme280_data comp_data;

    /* Recommended mode of operation: Indoor navigation */
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

    /* Set the sensor settings */
    rslt = bme280_set_sensor_settings(settings_sel, dev);
    
    if (rslt != BME280_OK)
    {
        fprintf(stderr, "Failed to set sensor settings (code %+d).", rslt);

        return rslt;
    }

    /*Calculate the minimum delay required between consecutive measurement based upon the sensor enabled
     *  and the oversampling configuration. */
    req_delay = bme280_cal_meas_delay(&dev->settings);

    // float total_temperature = 0;
    // int contador = 0;

    // Start CSV
    // if( access( "temps.csv", F_OK ) == -1 ) {
    //     fp=fopen("temps.csv","a+");
    //     fprintf(fp,"Data/hora, Temp Interna, Temp Externa, Temp Usuário");
    //     fclose(fp);
    // }
          
    /* Set the sensor to forced mode */
    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);

    if (rslt != BME280_OK)
    {
        fprintf(stderr, "Failed to set sensor mode (code %+d).", rslt);
        
    }

    /* Wait for the measurement to complete and print data */
    dev->delay_us(req_delay, dev->intf_ptr);

    rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
    
    if (rslt != BME280_OK)
    {
        fprintf(stderr, "Failed to get sensor data (code %+d).", rslt);
        exit(1);
    }
    
    // print_sensor_data(&comp_data);
    // total_temperature += comp_data.temperature;
    // total_temperature = 0;
    // printf("%f\n", comp_data.temperature); // <<<<<<<<<<<<<< AQUI
    // if(contador == 10){
    //     time ( &rawtime );
    //     timeinfo = localtime ( &rawtime );           
        
    //     save_into_csv(&fp, timeinfo, total_temperature/10 , total_humidity/10 , total_pressure/10);
        
    //     contador = 0;    
    // }

    return comp_data.temperature;
}