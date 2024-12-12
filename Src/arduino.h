/**
 * @file arduino.h
 * @brief Implementation file for arduino as a slave for i2c communication
 *
 * @author Bhakti Ramani
 * @date December 12, 2024
 * @course ECEN 5813 Principles of Embedded Software
 *
 * This file provides implementation of communicating with Arduino as a slave via I2C protocol
 */

#ifndef ARDUINO_H_
#define ARDUINO_H_

/* Standard Library Includes */
#include <stdio.h>     /* Standard I/O operations */
#include <stdint.h>    /* Standard integer types */
#include <string.h>

/* MCU Hardware Includes */
#include <stm32f091xc.h> /* MCU specific definitions */
#include <stm32f0xx.h>   /* STM32F0 common definitions */

/* Project-Specific Includes */
#include <utilities.h>    /* Utility functions */
#include "i2c1.h"

/******************************************************************************
* @function    arduino_send_msg
* @brief       Sends acknowledgment message to Arduino via I2C
* @param       None
* @return      None
*
* @details     Sends a confirmation message to Arduino after receiving and
*              displaying temperature/humidity data. Uses I2C1 peripheral
*              with auto-end feature.
*/
void arduino_send_msg();


/******************************************************************************
* @function    arduino_rcv_msg
* @brief       Receives temperature and humidity data from Arduino
* @param       str: Pointer to buffer for storing received data
* @param       len: Length of data to receive (expected 2 bytes)
* @return      None
*
* @details     Reads temperature and humidity values from Arduino using I2C1.
*              First byte is temperature, second byte is humidity.
*              Uses auto-end feature and handles busy flags.
******************************************************************************/
void arduino_rcv_msg(char *str, size_t len);

#endif /* ARDUINO_H_ */
