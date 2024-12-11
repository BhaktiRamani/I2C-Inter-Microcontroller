/*
 * temp_sensor.c
 *
 *  Created on: Dec 10, 2024
 *      Author: Bhakti Ramani
 */

#include "temp_sensor.h"


void humidity_read() {

	    // Wait for I2C to be not busy
	    while(I2C1->ISR & I2C_ISR_BUSY);

	    // 1. Start bit + Slave Address with Write (0)
	    I2C1->CR2 = 0;
	    I2C1->CR2 = (3 << 16) | (0x40 << 1); // 1 byte, slave address 0x40

	    //i2c_start();

	    // 2. Send Measure Command (0xF5 for humidity measurement)
	    while (!(I2C1->ISR & I2C_ISR_TXE));
	    I2C1->TXDR = 0xF5;

//	     3. Start bit + Slave Address with Read (1)
//	    I2C1->CR2 = (1 << 16) | ((0x40 | 1) << 1) | I2C_CR2_RD_WRN | I2C_CR2_AUTOEND;
//	    I2C1->CR2 = (1 << 16) | ((0x40 | 1 ) << 1); // 1 byte, slave address 0x40
//	    i2c_start();
//
	    while (!(I2C1->ISR & I2C_ISR_TXE));
	    I2C1->TXDR = (0x40 | 1);

	    i2c_start();

	    while (!(I2C1->ISR & I2C_ISR_TXE));
	    I2C1->TXDR = (0x40 | 1);

	    // 4. Receive two bytes
	    uint16_t humidity_raw = 0;

	    // First byte (MSB)
	    while (!(I2C1->ISR & I2C_ISR_RXNE));
	    humidity_raw = I2C1->RXDR << 8;

	    // Second byte (LSB)
	    while (!(I2C1->ISR & I2C_ISR_RXNE));
	    humidity_raw |= I2C1->RXDR;


}
