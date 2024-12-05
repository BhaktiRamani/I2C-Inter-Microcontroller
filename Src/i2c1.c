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

void I2C_WriteCommand(uint8_t command)
{

	while((I2C1 -> ISR & I2C_ISR_BUSY));

	I2C1 -> CR2 = 0;

	I2C1 -> CR2 = I2C_CR2_AUTOEND | (2<<16) | (OLED_I2C_ADDR << 1);


//    // Configure for write
//    //setting the 7 bit address mode
//    I2C1 -> CR2 &= ~(I2C_CR2_ADD10);
//    //setting the direction for transfer
//    I2C1->CR2 &= ~(I2C_CR2_RD_WRN);
//
//    //setting the number of bytes to be transferred
//    I2C1->CR2 |= (1 << 16);
//
//    I2C1->CR2 |= (slave_addr << 1); // Slave address

    i2c_start();
    while (!(I2C1->ISR & I2C_ISR_TXE)){;}
    I2C1 -> TXDR = OLED_I2C_ADDR;

    while (!(I2C1->ISR & I2C_ISR_TXE)){;}
    I2C1 -> TXDR = command;

    while(!((I2C1 -> ISR & (1 << 5))));

    I2C1 -> ICR |= I2C_ICR_STOPCF;

//    // Important: Reading the ISR register and then writing to the ICR register clears the ADDR flag
//    (void)I2C1->ISR;  // Read to clear
//    I2C1->ICR |= I2C_ICR_ADDRCF;  // Alternatively, you can use the ICR (Interrupt Clear Register)
//
//    while (!(I2C1->ISR & I2C_ISR_TXE)){;}
//	I2C1->TXDR = data; // For transmitting data
//
//    // Wait for Transmit Interrupt Status
//    while (!(I2C1->ISR & I2C_ISR_TXIS)){;}
//    i2c_stop();



}

// Example OLED Initialization Commands
void OLED_Init(void) {
	// Basic initialization sequence for SSD1306
	I2C_WriteCommand(0xAE);  // Display OFF
	I2C_WriteCommand(0xD5);  // Set Display Clock Divide Ratio
	I2C_WriteCommand(0x80);  // Default value
	I2C_WriteCommand(0xA8);  // Set Multiplex Ratio
	I2C_WriteCommand(0x3F);  // 64 COM lines
	I2C_WriteCommand(0xD3);  // Set Display Offset
	I2C_WriteCommand(0x00);  // No offset
	I2C_WriteCommand(0x40);
	// Set Start Line (Line #0)
	I2C_WriteCommand(0x8D);  // Charge Pump Setting
	I2C_WriteCommand(0x14);  // Enable Charge Pump
	I2C_WriteCommand(0x20);  // Memory Addressing Mode
	I2C_WriteCommand(0x00);  // Horizontal Addressing Mode

	I2C_WriteCommand(0xA1);  // Segment Remap
	I2C_WriteCommand(0xC8);  // COM Output Scan Direction
	I2C_WriteCommand(0xDA);  // COM Pins Configuration
	I2C_WriteCommand(0x12);  // Alternative COM pin config
	I2C_WriteCommand(0x81);  // Contrast Control
	I2C_WriteCommand(0x34);  // Brightness value
	I2C_WriteCommand(0xD9);  // Pre-charge Period
	I2C_WriteCommand(0xF1);  //
	I2C_WriteCommand(0xDB);  // VCOMH Deselect Level
	I2C_WriteCommand(0x40);  //
	I2C_WriteCommand(0xA4);  // Entire Display On/Off
	I2C_WriteCommand(0xA6);  // Normal display (not inverted)
	I2C_WriteCommand(0xAF);  // Display ON }
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
















