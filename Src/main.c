/**
******************************************************************************
* @file           : main.c
* @brief          : Temperature and Humidity Monitoring System
* @details        : This program implements a system that monitors temperature and
*                  humidity using Arduino as a sensor interface and displays the
*                  data on an OLED display. The system uses I2C communication
*                  between STM32 (master) and Arduino (slave).
*
* @author         : Bhakti Ramani
* @date           : December 12, 2024
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
#include "i2c1.h"
#include "oled.h"
#include "arduino.h"

/* System Configuration Constants */
#define SYSTEM_CLOCK_FREQ   (48000000U) /* 48 MHz system clock */
#define AHB_CLOCK_FREQ      (48000000U) /* 48 MHz AHB bus clock */
#define APB_CLOCK_FREQ      (24000000U) /* 24 MHz APB bus clock */
#define UART_BAUD_RATE      (9600U)     /* UART communication rate */

/* I2C Configuration Constants */
#define ARDUINO_I2C_ADDR    (0x08)      /* Arduino slave address */
#define OLED_I2C_ADDR      (0x3C)      /* OLED display address */
#define I2C_DATA_LENGTH     (2)         /* Length of temperature/humidity data */
#define I2C_AUTOEND_BIT    (1 << 25)   /* I2C auto-end bit position */
#define I2C_BUSY_BIT       (1 << 15)   /* I2C busy bit position */
#define I2C_STOP_BIT       (1 << 5)    /* I2C stop bit position */

/* OLED Display Constants */
#define OLED_TEMP_X        (0)         /* X coordinate for temperature display */
#define OLED_TEMP_Y        (15)        /* Y coordinate for temperature display */
#define OLED_HUM_X         (0)         /* X coordinate for humidity display */
#define OLED_HUM_Y         (25)        /* Y coordinate for humidity display */
#define OLED_ACK_Y         (35)        /* Y coordinate for acknowledgment display */
#define OLED_ERR_TITLE_X   (25)        /* X coordinate for error title */
#define OLED_ERR_TITLE_Y   (5)         /* Y coordinate for error title */
#define RCV_DATA_BUFFER    (3)
#define TEMP				(0)
#define HUM					(1)
#define TITLE_X				(25)
#define TITLE_Y				(5)
#define ERR_X				(30)
#define ZERO				(0)
#define SENSOR_BFR_SIZE 	(10)
#define DELAY_WHILE			(1000000)
/* Function Prototypes */
void arduino_rcv_msg(char *str, size_t len);
void arduino_send_msg(void);

/******************************************************************************
* @function    main
* @brief       Main program entry point
* @return      int - Should never return in this application
*
* @details     Program initialization and operation sequence:
*              1. Initialize I2C communication
*              2. Setup OLED display
*              3. Enter main loop for temperature/humidity monitoring:
*                 - Request data from Arduino
*                 - Display data on OLED
*                 - Send acknowledgment
*              4. Handle error cases with appropriate display messages
******************************************************************************/
int main(void)
{
    /* Initialize printf output via ST-Link COM port */
    printf("\n\r");

    //Initiaalization of I2C1
    i2c1__init__();

    //Testing different commands for OLED
    oled_trial_commands();


    printf("****************Readings Begin**************\n\r");
    printf("\n\r");

    /* Main program loop */
    while(1)
    {
    	char receivedData[RCV_DATA_BUFFER];
    	printf("\n\r");
    	printf(">> Requesting Arduino for Temperature Humidity \n\r");

    	arduino_rcv_msg(receivedData, 2);
    	receivedData[2] = '\0';
    	printf(">> Recieving Temperature : %d Humidity : %d from Arduino\n\r", receivedData[TEMP], receivedData[HUM]);


    	if(receivedData[TEMP] > 0)
    	{
        	// Display temperature on the first line
        	gotoXY_SSD1306(OLED_TEMP_X, OLED_TEMP_Y); // Position for temperature line
        	puts_SSD1306("Temp: ", &Font_7x10, SSD1306_COLOR_WHITE);
        	char tempBuffer[SENSOR_BFR_SIZE];
        	sprintf(tempBuffer, "%d", receivedData[TEMP]); // Format temperature
        	puts_SSD1306(tempBuffer, &Font_7x10, SSD1306_COLOR_WHITE);

        	// Display humidity on the second line
        	gotoXY_SSD1306(OLED_HUM_X, OLED_HUM_Y); // Position for humidity line
        	puts_SSD1306("Humidity: ", &Font_7x10, SSD1306_COLOR_WHITE);
        	char humidityBuffer[SENSOR_BFR_SIZE];
        	sprintf(humidityBuffer, "%d", receivedData[HUM]); // Format humidity
        	puts_SSD1306(humidityBuffer, &Font_7x10, SSD1306_COLOR_WHITE);

           	//send arduino msg back as data recieved
           	printf(">> Sending Acknowledgement to Arduino\n\r");
           	arduino_send_msg();
           	gotoXY_SSD1306(ZERO, OLED_ACK_Y); // Position for humidity line
           	puts_SSD1306("ACK sent to arduino", &Font_7x10, SSD1306_COLOR_WHITE);
           	printf(">> Updating data on OLED\n\r");
           	updateScreen_SSD1306();
    	}
    	else
    	{
    		printf(">> ERROR : INCORRECT RESPONSE OF TEMPERATURE AND HUMIDITY \n\r");
    		//clearing the screen first
    		fill_SSD1306();
    		updateScreen_SSD1306();
    		//Updating the error on OLED
    		gotoXY_SSD1306 (TITLE_X,TITLE_Y);
    		puts_SSD1306 ("PES PROJECT", &Font_7x10, SSD1306_COLOR_WHITE);
    		updateScreen_SSD1306();
           	gotoXY_SSD1306(ZERO, OLED_ERR_TITLE_Y); // Position for humidity line
           	puts_SSD1306("ERROR:FAILURE", &Font_7x10, SSD1306_COLOR_WHITE);
           	gotoXY_SSD1306(ERR_X, OLED_ERR_TITLE_X); // Position for humidity line
           	puts_SSD1306("SENSOR READ", &Font_7x10, SSD1306_COLOR_WHITE);
    		fill_SSD1306();
    		updateScreen_SSD1306();
    	}


        delay(DELAY_WHILE);


    }

    return 0;  /* Should never reach this point */
}


