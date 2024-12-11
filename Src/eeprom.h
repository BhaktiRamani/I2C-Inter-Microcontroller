/*
 * eeprom.h
 *
 *  Created on: Dec 10, 2024
 *      Author: Bhakti Ramani
 */

#ifndef EEPROM_H_
#define EEPROM_H_

/* MCU Hardware Includes */
#include <stm32f091xc.h> /* MCU specific definitions */
#include <stm32f0xx.h>   /* STM32F0 common definitions */

/* Project-Specific Includes */
#include <utilities.h>    /* Utility functions */

#include <stdio.h>      /* Standard I/O operations */
#include<stdint.h>

#include"i2c1.h"
/* I2C Operation Mode Definitions */
#define READ 0x01           /* I2C Read operation bit */
#define WRITE 0x00          /* I2C Write operation bit */
#define EEPROM_ID 0xA0      /* Device identifier for X24C02 EEPROM */

void eeprom_read(uint8_t address);
void eeprom_write(uint8_t block, uint8_t address, uint8_t data);

#endif /* EEPROM_H_ */
