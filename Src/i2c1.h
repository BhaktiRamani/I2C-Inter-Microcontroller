/**
******************************************************************************
* @file           : i2c1.h
* @brief          : I2C Communication Interface Header
* @details        : This header file defines the I2C1 interface functions and
*                  variables for STM32F091 microcontroller. It provides
*                  functionality for both master and slave I2C operations.
*
* @author         : Bhakti Ramani
* @date           : December 12, 2024
* @course         : ECEN 5813 Principles of Embedded Software
* @institution    : University of Colorado Boulder
******************************************************************************
*/

#ifndef I2C1_H_
#define I2C1_H_

/* Standard Library Includes */
#include <stdio.h>     /* Standard I/O operations */
#include <stdint.h>    /* Standard integer types */

/* MCU Hardware Includes */
#include <stm32f091xc.h> /* MCU specific definitions */
#include <stm32f0xx.h>   /* STM32F0 common definitions */

/* Project-Specific Includes */
#include <utilities.h>    /* Utility functions */

/*******************************************************************************
* Function Prototypes
*******************************************************************************/

/**
 * @brief  Initializes I2C1 peripheral with specified slave address
 * @param  None
 * @return None
 * @note   Must be called before any other I2C operations
 */
void i2c1__init__();

/**
 * @brief  Generates I2C start condition
 * @param  None
 * @return None
 * @note   Should be called at the beginning of each I2C transaction
 */
void i2c_start(void);

/**
 * @brief  Writes a single byte to a specified register address
 * @param  RegisterAddress: Target register address
 * @param  data: Byte to be written
 * @return int: Status of the write operation
 */
int I2C_WriteCommand(uint8_t RegisterAddress, uint8_t data);

/**
 * @brief  Sets the slave address for I2C communication
 * @param  slave_address: 7-bit I2C slave address
 * @return None
 */
void i2c_slave_address(uint8_t slave_address);

/**
 * @brief  Writes multiple bytes to a specified register address
 * @param  RegisterAddress: Target register address
 * @param  data: Pointer to data array to be written
 * @param  n_data: Number of bytes to write
 * @return None
 */
void i2c_multi_write(uint8_t RegisterAddress, uint8_t *data, uint8_t n_data);

/**
 * @brief  Generates I2C stop condition
 * @param  None
 * @return None
 */
void i2c_stop(void);

/**
 * @brief  Software delay function
 * @param  t: Number of cycles to delay
 * @return None
 */
void delay(uint32_t t);


#endif /* I2C1_H_ */