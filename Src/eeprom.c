/*
 * eeprom.c
 *
 *  Created on: Dec 10, 2024
 *      Author: Bhakti Ramani
 */

#include "eeprom.h"


void eeprom_write(uint8_t block, uint8_t address, uint8_t data)
{
	uint8_t cmd_buffer[] = {(EEPROM_ID | block | WRITE), address, data};

	printf("write started \n\r");
	while((I2C1 -> ISR & I2C_ISR_BUSY));

	I2C1 -> CR2 = 0;

	I2C1 -> CR2 = I2C_CR2_AUTOEND | (2<<16) | ( (EEPROM_ID | block | WRITE) << 1 );

	i2c_start();

    for(int i = 1; i < 3; i++ )
    {
		//while (!(I2C1->ISR & I2C_ISR_TXE)){;}
    	for(int i = 0; i < 100000; i++){;}
        I2C1 -> TXDR = cmd_buffer[i];
    }

   	//waiting for I2C to generate the stop bit
     while(!((I2C1 -> ISR & (1 << 5))));
     I2C1 -> ICR |= I2C_ICR_STOPCF;

     printf("write complete \n\r");
}

void eeprom_read(uint8_t address)
{
	printf("read started \n\r");
	uint8_t cmd_buffer[] = {(EEPROM_ID | WRITE), address, (EEPROM_ID | READ)};
	while((I2C1 -> ISR & I2C_ISR_BUSY));

	I2C1 -> CR2 = 0;

	I2C1 -> CR2 = I2C_CR2_AUTOEND | (2<<16) |( ( EEPROM_ID | WRITE) << 1 );

	i2c_start();

    for(int i = 1; i < 3; i++ )
    {
//		while (!(I2C1->ISR & I2C_ISR_TXE)){;}
    	for(int i = 0; i < 100000; i++){;}
        I2C1 -> TXDR = cmd_buffer[i];
    }

   	//waiting for I2C to generate the stop bit
     while(!((I2C1 -> ISR & (1 << 5))));
     I2C1 -> ICR |= I2C_ICR_STOPCF;

     printf("read completed\n\r");
}
