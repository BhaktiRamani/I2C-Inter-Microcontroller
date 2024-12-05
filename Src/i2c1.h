/*
 * i2c1.h
 *
 *  Created on: Dec 4, 2024
 *      Author: bhakti
 */

#ifndef I2C1_H_
#define I2C1_H_

/* MCU Hardware Includes */
#include <stm32f091xc.h> /* MCU specific definitions */
#include <stm32f0xx.h>   /* STM32F0 common definitions */

/* Project-Specific Includes */
#include <utilities.h>    /* Utility functions */

#include <stdio.h>      /* Standard I/O operations */

void i2c1__init__(uint8_t slave_address);
void i2c_start(void);
void i2c_write(uint8_t data);
void i2c_slave_address(uint8_t slave_address);
void i2c_stop(void);
#endif /* I2C1_H_ */
