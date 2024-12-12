/**
******************************************************************************
* @file           : main.c
* @brief          : Audio Signal Generation and Analysis System
* @details        : This program implements a digital audio system that generates
*                  and analyzes multiple frequency sine waves using DAC and ADC.
*                  System operates at 48MHz with real-time frequency detection.
*
* @author         : Bhakti Ramani
* @date           : November 26, 2024
* @course         : ECEN 5813 Principles of Embedded Software
* @institution    : University of Colorado Boulder
*
* @note           : Based on template by STM32CubeIDE
*
* @attention
*
* Copyright (c) 2023 STMicroelectronics.
* All rights reserved.
*
* This software is licensed under terms that can be found in the LICENSE file
* in the root directory of this software component.
* If no LICENSE file comes with this software, it is provided AS-IS.
******************************************************************************
*/

/* Standard Library Includes */
#include <stdio.h>      /* Standard I/O operations */
#include <stdint.h>     /* Standard integer types */
#include <math.h>       /* Mathematical functions */
#include <string.h>     /* String operations */


/* MCU Hardware Includes */
#include <stm32f091xc.h> /* MCU specific definitions */
#include <stm32f0xx.h>   /* STM32F0 common definitions */

/* Project-Specific Includes */
#include <utilities.h>    /* Utility functions */

#include"i2c1.h"
#include "oled.h"


/* System Configuration Constants */
#define SYSTEM_CLOCK_FREQ   (48000000U) /* 48 MHz system clock */
#define AHB_CLOCK_FREQ      (48000000U) /* 48 MHz AHB bus clock */
#define APB_CLOCK_FREQ      (24000000U) /* 24 MHz APB bus clock */
#define UART_BAUD_RATE      (9600U)     /* UART communication rate */
#define DAC_SAMPLING_FREQ   (48000U)
#define SINE_WAVE_440       (440U)
#define SINE_WAVE_587       (587U)
#define SINE_WAVE_659       (659U)
#define SINE_WAVE_880       (880U)


/******************************************************************************
* @function    main
* @brief       Main program entry point
* @return      int - Should never return in this application
*
* @details     Program initialization and operation sequence:
*              1. Generate sine wave samples for multiple frequencies
*              2. Initialize peripherals (DMA, Timer6, DAC, ADC)
*              3. Configure GPIO PA4 for DAC and ADC
*              4. Start audio signal generation and analysis
*              5. Enter infinite monitoring loop
*
* @note        Serial console settings: 9600 baud, No parity, 1 stop bit
******************************************************************************/

void arduino_rcv_msg(char *str, size_t len);
void arduino_send_msg();
int main(void)
{
    /* Initialize printf output via ST-Link COM port */
    printf("\n\r");

    uint8_t slave_address = 0x3C;
    i2c1__init__(slave_address);
    oled_trial_commands();


    printf("Entering the loop\n\r");
    printf("\n\r");

    /* Main program loop */
    while(1)
    {
    	char receivedData[3];

    	printf("rcv req send \n\r");
    	printf("\n\r");
    	arduino_rcv_msg(receivedData, 2);
    	receivedData[2] = '\0';
    	printf("\n\rRecieved data %d %d", receivedData[0], receivedData[1]);


    	// Display temperature on the first line
    	gotoXY_SSD1306(0, 15); // Position for temperature line
    	puts_SSD1306("Temp: ", &Font_7x10, SSD1306_COLOR_WHITE);
    	char tempBuffer[10];
    	sprintf(tempBuffer, "%d", receivedData[0]); // Format temperature
    	puts_SSD1306(tempBuffer, &Font_7x10, SSD1306_COLOR_WHITE);

    	// Display humidity on the second line
    	gotoXY_SSD1306(0, 25); // Position for humidity line
    	puts_SSD1306("Humidity: ", &Font_7x10, SSD1306_COLOR_WHITE);
    	char humidityBuffer[10];
    	sprintf(humidityBuffer, "%d", receivedData[1]); // Format humidity
    	puts_SSD1306(humidityBuffer, &Font_7x10, SSD1306_COLOR_WHITE);



//        	//send arduino msg back as data recieved
        	printf("\n\rsend req send \n\r");
        	arduino_send_msg();
        	gotoXY_SSD1306(0, 35); // Position for humidity line
        	puts_SSD1306("ACK SENT TO ARDUINO", &Font_7x10, SSD1306_COLOR_WHITE);
        	updateScreen_SSD1306();
        	delay(1000000);


    }

    return 0;  /* Should never reach this point */
}

void arduino_send_msg()
{
	while((I2C1 -> ISR & I2C_ISR_BUSY));

	I2C1 -> CR2 = 0;
	char *buffer = "Data  recieved";
	I2C1 -> CR2 = I2C_CR2_AUTOEND | (14<<16) | (8 << 1 );
	I2C1 -> CR2 &= ~I2C_CR2_RD_WRN;

	int size = sizeof(buffer);
	(void)size;
    i2c_start();



        for(int i = 0; i < 14; i++)
        {
        	//sending the actual data
            while (!(I2C1->ISR & I2C_ISR_TXE)){;}
            I2C1 -> TXDR = *buffer;
            buffer++;
        }

        while(!((I2C1 -> ISR & (1 << 5))));
        I2C1 -> ICR |= I2C_ICR_STOPCF;
}

void arduino_rcv_msg(char *str, size_t len)
{
	while((I2C1 -> ISR & I2C_ISR_BUSY));

	I2C1 -> CR2 = 0;

	I2C1 -> CR2 = I2C_CR2_AUTOEND | I2C_CR2_RD_WRN |  (2<<16) | (8 << 1 );

	i2c_start();
    for (size_t i = 0; i < len; i++) {
        // Wait until the RXNE (Receive Not Empty) flag is set
        while (!(I2C1->ISR & I2C_ISR_RXNE)) {
            // Optionally, implement a timeout mechanism here
        }

        // Read a byte from the RXDR register
        str[i] = (char)(I2C1->RXDR & 0xFF);  // Mask to ensure only lower 8 bits are taken
    }

    //isr_flag = 1;

    while(!((I2C1 -> ISR & (1 << 5))));
    I2C1 -> ICR |= I2C_ICR_STOPCF;
//    printf("out of interuupt \n\r");



}
