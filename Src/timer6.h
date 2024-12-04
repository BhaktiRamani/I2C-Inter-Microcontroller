/******************************************************************************
* @file         timer6.h
* @brief        Timer6 Configuration and Control Interface
* @details      This header file provides interface for Timer6 initialization and
*               control for STM32F091RC microcontroller. Timer6 is a basic timer
*               used for timing and delay operations.
*
* @author       Bhakti Ramani
* @date         November 25, 2024
* @course       ECEN 5813 Principles of Embedded Software
* @institution  University of Colorado Boulder
*
* @copyright    Copyright (C) 2024 by Bhakti Ramani
*               All Rights Reserved
*
* @note         This module depends on STM32F091xc and STM32F0xx HAL libraries
*               for timer configuration and control.
******************************************************************************/

#ifndef TIMER6_H_
#define TIMER6_H_

/* Required header includes for Timer6 functionality */
#include <utilities.h>      /* Utility functions and common definitions */
#include <stm32f091xc.h>   /* STM32F091RC specific definitions */
#include <stm32f0xx.h>     /* STM32F0 series common definitions */

/******************************************************************************
* @function     Timer6_Init
* @brief        Initializes and configures Timer6 peripheral
* @param        None
* @return       None
*
* @details      This function performs the following operations:
*               1. Enables Timer6 clock
*               2. Configures Timer6 parameters
*               3. Sets up Timer6 for basic timing operations
*
* @note         Must be called before using any Timer6 operations
******************************************************************************/
void Timer6_Init(void);

#endif /* TIMER6_H_ */