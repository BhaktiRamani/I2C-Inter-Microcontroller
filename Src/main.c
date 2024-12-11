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
#include"eeprom.h"

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
int main(void)
{
    /* Initialize printf output via ST-Link COM port */
    printf("\r\nHello world\n\r");

    uint8_t slave_address = 0x3C;
    i2c1__init__(slave_address);

    //oled_trial_commands();

    eeprom_write(0x00, 0x55, 0x55);
    eeprom_read(0x55);

//     while(1)
//     {
//     	printf("off\n\r");
//     	SSD1306_SEND_CMD(0xAE);  // Display OFF
//     	//for(int i = 0; i< 1000000; i++);
//     	printf("on\n\r");
//     	SSD1306_SEND_CMD(0xAf);  // Display On
//
//     }
//    oled_clear();
//   oled_set_cursor(0, 0);
//   oled_print_char('A');
//    oled_print_string("Hello World");

    printf("Initialization complete\n\r");

    printf("DONE\n\r");
    /* Main program loop */
    while(1)
    {
        /* System continues to operate via interrupts */
    }

    return 0;  /* Should never reach this point */
}
