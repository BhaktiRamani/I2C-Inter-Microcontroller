/**
******************************************************************************
* @file           : i2c1.c
* @brief          : I2C Communication Interface Implementation
* @details        : This file implements I2C1 functionality for STM32F091
*                  including initialization, data transmission, and reception.
*                  Supports both single byte and multi-byte operations.
*
* @author         : Bhakti Ramani
* @date           : December 12, 2024
* @course         : ECEN 5813 Principles of Embedded Software
* @institution    : University of Colorado Boulder
******************************************************************************
*/

#include "i2c1.h"

/* I2C Configuration Constants */
#define OLED_I2C_ADDR          (0x3C)    /* OLED Display I2C Address */
#define I2C_TIMING_CONFIG      (0x00B01A4B) /* I2C timing configuration for 100kHz */
#define I2C_BUSY_FLAG         (I2C_ISR_BUSY)
#define I2C_TXE_FLAG         (I2C_ISR_TXE)
#define I2C_STOP_FLAG        (1 << 5)
#define I2C_TC_FLAG          (1 << 6)
#define GPIO_AF1             (1)         /* Alternate Function 1 for GPIO */
#define I2C_WRITE_CMD_BYTES   (2)


/**
 * @brief  Initializes I2C1 peripheral with specified slave address
 * @param  slave_address: 7-bit I2C slave address
 * @note   Configures GPIO PB8 (SCL) and PB9 (SDA) for I2C operation
 */
void i2c1__init__()
{
    /* Enable peripheral clocks */
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN_Msk;    /* Enable GPIOB clock */
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;       /* Enable I2C1 clock */

    /* Configure GPIO pins for I2C */
    /* Set alternate function mode for PB8 and PB9 */
    GPIOB->MODER |= GPIO_MODER_MODER8_1;
    GPIOB->MODER |= GPIO_MODER_MODER9_1;

    /* Set specific I2C alternate function */
    GPIOB->AFR[1] &= ~(0xF0F);
    GPIOB->AFR[1] |= (GPIO_AF1 << 0) | (GPIO_AF1 << 4);

    /* Configure as open-drain outputs */
    GPIOB->OTYPER |= GPIO_OTYPER_OT_8;
    GPIOB->OTYPER |= GPIO_OTYPER_OT_9;

    /* Enable pull-up resistors */
    GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPDR8 | GPIO_PUPDR_PUPDR9);
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR8_0;
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR9_0;

    /* Set high-speed mode */
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR8_0 | GPIO_OSPEEDR_OSPEEDR8_1;
    GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR9_0 | GPIO_OSPEEDR_OSPEEDR9_1;

    /* Configure I2C peripheral */
    I2C1->CR1 &= ~I2C_CR1_PE;                 /* Disable peripheral for configuration */
    I2C1->TIMINGR |= I2C_TIMING_CONFIG;       /* Set I2C timing */
    I2C1->CR1 |= I2C_CR1_NOSTRETCH;          /* Disable clock stretching */
    I2C1->CR1 |= I2C_CR1_PE;                 /* Enable peripheral */
}


/**
 * @brief  Generates I2C start condition
 */
void i2c_start(void)
{
    I2C1->CR2 |= I2C_CR2_START;              /* Generate START condition */
    while((I2C1->ISR & I2C_TC_FLAG) != 0);   /* Wait for transmission complete */
}

int I2C_WriteCommand(uint8_t RegisterAddress, uint8_t data)
{


	while((I2C1 -> ISR & I2C_ISR_BUSY));

	I2C1 -> CR2 = 0;

	I2C1 -> CR2 = I2C_CR2_AUTOEND | (I2C_WRITE_CMD_BYTES<<16) | (OLED_I2C_ADDR << 1 );


    i2c_start();

	//sending the register address, basically to determine read or write
    while (!(I2C1->ISR & I2C_ISR_TXE)){;}
    I2C1 -> TXDR = RegisterAddress;

	//sending the actual data
    while (!(I2C1->ISR & I2C_ISR_TXE)){;}
    I2C1 -> TXDR = data;

	//waiting for I2C to generate the stop bit
     while(!((I2C1 -> ISR & (1 << 5))));
     I2C1 -> ICR |= I2C_ICR_STOPCF;


	delay(50);
	return 0;
}


void i2c_multi_write(uint8_t RegisterAddress, uint8_t *data, uint8_t n_data)
{
	while((I2C1 -> ISR & I2C_ISR_BUSY));

	I2C1 -> CR2 = 0;

	I2C1 -> CR2 = I2C_CR2_AUTOEND | ((n_data + 1 )<<16) | (OLED_I2C_ADDR << 1 );


    i2c_start();
    //check if txdr is empty (means if bit is set)
	//sending the commaqnd
    while (!(I2C1->ISR & I2C_ISR_TXE)){;}
    I2C1 -> TXDR = RegisterAddress;
    

    for(int i = 0; i < n_data; i++ )
    {
		while (!(I2C1->ISR & I2C_ISR_TXE)){;}
        I2C1 -> TXDR = *data;
        data++;
    }

   	//waiting for I2C to generate the stop bit
     while(!((I2C1 -> ISR & I2C_ICR_STOPCF)));
     I2C1 -> ICR |= I2C_ICR_STOPCF;

}




void i2c_slave_address(uint8_t slave_address)
{
	I2C1-> TXDR = slave_address;
}

void i2c_stop(void)
{
	//Generating the start bit
	I2C1->CR2 |= I2C_CR2_STOP;

	//waiting while TC flag is cleared
	while((I2C1 -> ISR & (I2C_ISR_TC)) != 0);
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















