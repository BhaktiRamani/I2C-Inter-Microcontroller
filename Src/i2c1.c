/*
 * i2c1.c
 *
 *  Created on: Dec 4, 2024
 *      Author: bhakti
 */

#include "i2c1.h"


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
	I2C1->TIMINGR |= (uint32_t)0x00B01A4B;
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

void i2c_write(uint8_t data)
{
	//Waiting for transmit buffer to be set
	/* Check Tx empty */

    // Wait for Transmit Buffer Empty
    while (!(I2C1->ISR & I2C_ISR_TXE));

    // Configure for write
    I2C1->CR2 = (I2C_CR2_AUTOEND |
                 (1 << 16) | // 1 byte
                 (0x78 << 1)); // Slave address

    i2c_start();
	I2C1->TXDR = data; // For transmitting data
    // Wait for the address to be transmitted and acknowledged
    while (!(I2C1->ISR & I2C_ISR_ADDR));

    // Clear the ADDR flag
    // Important: Reading the ISR register and then writing to the ICR register clears the ADDR flag
    (void)I2C1->ISR;  // Read to clear
    I2C1->ICR |= I2C_ICR_ADDRCF;  // Alternatively, you can use the ICR (Interrupt Clear Register)
    // Wait for Transmit Interrupt Status
    while (!(I2C1->ISR & I2C_ISR_TXIS)){;}

	// Issue start condition
	//i2c_start();

	// Wait for the address to be transmitted and acknowledged
//	while (!(I2C1->ISR & I2C_ISR_ADDR));
//
//	// Clear the ADDR flag
//	(void)I2C1->ISR;

	// Transmit or receive data as needed
	I2C1->TXDR = data; // For transmitting data




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
















