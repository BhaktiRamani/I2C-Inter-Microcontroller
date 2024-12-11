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

	//Recieve interrupt setup
    I2C1->CR1 |= I2C_CR1_RXIE;

    NVIC_SetPriority(I2C1_IRQn, 0);
    NVIC_EnableIRQ(I2C1_IRQn);


//	I2C1->TIMINGR |= (uint32_t)0x2000090E;
	I2C1->TIMINGR |= (uint32_t)0x00B01A4B;
	I2C1 -> CR1 |= I2C_CR1_NOSTRETCH;
	I2C1->CR1 |= I2C_CR1_PE;


}

volatile uint32_t i2c_rcv;
volatile int isr_flag = 0;

void I2C1_IRQHandler(void)
{
    // Verify interrupt status.
    if ((I2C1->ISR & I2C_ISR_RXNE) == I2C_ISR_RXNE) {
        // Read byte (which clears RXNE flag).
        i2c_rcv = I2C1->RXDR;
        isr_flag = 1;

        printf("i2c_rcv %ld\n\r", i2c_rcv);
        printf("\n\r");
        printf("isr_flag %d\n\r", isr_flag);
    }
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


	while((I2C1 -> ISR & I2C_ISR_BUSY));

	I2C1 -> CR2 = 0;

	I2C1 -> CR2 = I2C_CR2_AUTOEND | (2<<16) | (OLED_I2C_ADDR << 1 );


    i2c_start();
    //check if txdr is empty (means if bit is set)
	//sending the slave address


	//sending the register address, basically to determine read or write
    while (!(I2C1->ISR & I2C_ISR_TXE)){;}
    I2C1 -> TXDR = RegisterAddress;

	//sending the actual data
    while (!(I2C1->ISR & I2C_ISR_TXE)){;}
    I2C1 -> TXDR = data;

	//waiting for I2C to generate the stop bit
     while(!((I2C1 -> ISR & (1 << 5))));
     I2C1 -> ICR |= I2C_ICR_STOPCF;

	//generating the stop bit mannually 
//	i2c_stop();
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
    

//	//sending the register address, basically to determine read or write
//    while (!(I2C1->ISR & I2C_ISR_TXE)){;}
//    I2C1 -> TXDR = RegisterAddress;

    for(int i = 0; i < n_data; i++ )
    {
		while (!(I2C1->ISR & I2C_ISR_TXE)){;}
        I2C1 -> TXDR = *data;
        data++;
    }

   	//waiting for I2C to generate the stop bit
     while(!((I2C1 -> ISR & (1 << 5))));
     I2C1 -> ICR |= I2C_ICR_STOPCF;

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















