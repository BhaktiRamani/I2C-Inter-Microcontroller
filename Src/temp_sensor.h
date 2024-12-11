/*
 * temp_sensor.h
 *
 *  Created on: Dec 10, 2024
 *      Author: Bhakti Ramani
 */

#ifndef TEMP_SENSOR_H_
#define TEMP_SENSOR_H_

/* MCU Hardware Includes */
#include <stm32f091xc.h> /* MCU specific definitions */
#include <stm32f0xx.h>   /* STM32F0 common definitions */

/* Project-Specific Includes */
#include <utilities.h>    /* Utility functions */

#include <stdio.h>      /* Standard I/O operations */
#include<stdint.h>

#include"i2c1.h"

void humidity_read();

#endif /* TEMP_SENSOR_H_ */
