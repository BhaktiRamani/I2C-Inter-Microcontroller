/*
 * i2c1.c
 *
 *  Created on: Dec 4, 2024
 *      Author: bhakti
 */

#include "i2c1.h"

#define OLED_I2C_ADDR 0x3C

void i2c1__init__(uint8_t slave_address)
{
	//Enable GPIOB clock
	RCC -> AHBENR |= RCC_AHBENR_GPIOBEN_Msk;

	//Enable i2c1 clock
	RCC -> APB1ENR |= RCC_APB1ENR_I2C1EN;

	//Define pins PB8 and PB9 as alternate function for i2c1
	GPIOB -> MODER |= GPIO_MODER_MODER8_1;
	GPIOB -> MODER |= GPIO_MODER_MODER9_1;

	//Define PB8 and PB9 as I2c alternate functions
    GPIOB->AFR[1] &= ~(0xF0F);
    GPIOB->AFR[1] |= (1 << 0) | (1 << 4);

	//Defining PB8 and PB9 as Open drain output
	GPIOB -> OTYPER |= GPIO_OTYPER_OT_8;
	GPIOB -> OTYPER |= GPIO_OTYPER_OT_9;

	//Defining PB8 and PB9 as PULL up mode
    // Pull-up
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
	GPIOB -> PUPDR |= GPIO_PUPDR_PUPDR8_0;
	GPIOB -> PUPDR |= GPIO_PUPDR_PUPDR9_0;

	//Define High speed output mode
	GPIOB -> OSPEEDR |= GPIO_OSPEEDR_OSPEEDR8_0 | GPIO_OSPEEDR_OSPEEDR8_1;
	GPIOB -> OSPEEDR |= GPIO_OSPEEDR_OSPEEDR9_0 | GPIO_OSPEEDR_OSPEEDR9_1;

	//Peripheral input clock- TIMEOUT register
    I2C1->CR1 &= ~I2C_CR1_PE;
//	I2C1->TIMINGR |= (uint32_t)0x2000090E;
	I2C1->TIMINGR |= (uint32_t)0x00B01A4B;
	I2C1 -> CR1 |= I2C_CR1_NOSTRETCH;
	I2C1->CR1 |= I2C_CR1_PE;
	//I2C1->CR2 |= I2C_CR2_AUTOEND | (1 << 16) | (slave_address << 1);

}

void i2c_start(void)
{
	//Generating the start bit
	I2C1->CR2 |= I2C_CR2_START;

	//waiting while TC flag is cleared
	while((I2C1 -> ISR & (1<< 6)) != 0);

}

int I2C_WriteCommand(uint8_t RegisterAddress, uint8_t data)
{

	while((I2C1 -> ISR & I2C_ISR_BUSY)){;}

//	I2C1 -> CR2 = 0;

	   // Configure transfer
	    I2C1->CR2 = 0;
	    I2C1->CR2 |= (1 << 13);
	    I2C1->CR2 |= (OLED_I2C_ADDR << 1);  // Slave address
	    I2C1->CR2 |= (2 << I2C_CR2_NBYTES_Pos);          // 2 bytes (control + command)

	    //I2C1->CR2 |= I2C_CR2_AUTOEND;                    // Automatic END



//	I2C1 -> CR2 = I2C_CR2_AUTOEND | (3<<16) | (OLED_I2C_ADDR << 1 );

	//while (!(I2C1->ISR & I2C_ISR_TXE)){;}
    //i2c_start();
    //check if txdr is empty (means if bit is set)
	//sending the slave address
    while (!(I2C1->ISR & I2C_ISR_TXE)){;}
    I2C1 -> TXDR = OLED_I2C_ADDR;
   // while (!(I2C1->ISR & I2C_ISR_TXE)){;}

//	//sending the register address, basically to determine read or write
//    while (!(I2C1->ISR & I2C_ISR_TXE)){;}
//    I2C1 -> TXDR = RegisterAddress;
    //while (!(I2C1->ISR & I2C_ISR_TXE)){;}
//
//	//sending the actual data
    while (!(I2C1->ISR & I2C_ISR_TXE)){;}
    I2C1 -> TXDR = data;
    //while (!(I2C1->ISR & I2C_ISR_TXE)){;}
//
//
//
//	//waiting for I2C to generate the stop bit
//     while(!((I2C1 -> ISR & (1 << 5))));
//     I2C1 -> ICR |= I2C_ICR_STOPCF;

	//generating the stop bit mannually 
	i2c_stop();
	delay(50);
	return 0;
}

uint8_t i2c_WaitForFlag(uint32_t flag, uint32_t timeout) {
    while (!(I2C1->ISR & flag)) {
        if (--timeout == 0) return 0;
    }
    return 1;
}
void i2c_multi_write(uint8_t RegisterAddress, uint8_t *data, uint8_t n_data)
{
	while((I2C1 -> ISR & I2C_ISR_BUSY));

	I2C1 -> CR2 = 0;

	I2C1 -> CR2 |= (OLED_I2C_ADDR << 1);


    i2c_start();
    //check if txdr is empty (means if bit is set)
	//sending the slave address
    while (!(I2C1->ISR & I2C_ISR_TXE)){;}
    I2C1 -> TXDR = OLED_I2C_ADDR;
    while (!(I2C1->ISR & I2C_ISR_TXE)){;}

//	//sending the register address, basically to determine read or write
//    while (!(I2C1->ISR & I2C_ISR_TXE)){;}
//    I2C1 -> TXDR = RegisterAddress;

    for(int i = 0; i < n_data; i++ )
    {

        I2C1 -> TXDR = *data;
        while (!(I2C1->ISR & I2C_ISR_TXE)){;}

        //while(!((I2C1 -> ISR & (1 << 5))));
        data++;
    }

    void i2c_stop(void);


}



void i2c_slave_address(uint8_t slave_address)
{
	I2C1-> TXDR = slave_address;
//	while(!(I2C1 -> ISR & (1 << 3)));
}

void i2c_stop(void)
{
	//Generating the start bit
	I2C1->CR2 |= I2C_CR2_STOP;

	//waiting while TC flag is cleared
	while((I2C1 -> ISR & (1<< 6)) != 0);
}

/*
 * Delay function.
 */
void delay(uint32_t t)
{
	// Simple delay loop.
	uint32_t cnt = 0;
	for (cnt = 0; cnt < t; cnt++)
	{
		__asm("nop");
	};
}















