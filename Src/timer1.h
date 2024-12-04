/******************************************************************************
* @file         timer1.h
* @brief        Timer1 Configuration Interface for ADC Triggering
* @details      This header file provides interface for Timer1 initialization
*               and configuration, specifically for ADC trigger timing control
*               on STM32F091RC microcontroller.
*
* @author       Bhakti Ramani
* @date         November 26, 2024
* @course       ECEN 5813 Principles of Embedded Software
* @institution  University of Colorado Boulder
*
* @copyright    Copyright (C) 2024 by Bhakti Ramani
*               All Rights Reserved
*
* @note         This module depends on STM32F091xc and STM32F0xx HAL libraries
******************************************************************************/

#ifndef _TIMER_1_H_
#define _TIMER_1_H_

/* Required header includes for Timer1 functionality */
#include <utilities.h>      /* Utility functions and common definitions */
#include <stm32f091xc.h>   /* STM32F091RC specific definitions */
#include <stm32f0xx.h>     /* STM32F0 series common definitions */

/******************************************************************************
* @function     TIM1_ADC_Trigger_Config
* @brief        Configures Timer1 as trigger source for ADC conversions
* @param        None
* @return       None
*
* @details      Configures Timer1 to generate precise timing triggers for ADC
*               sampling at 96kHz rate. Sets up Timer1 in master mode with
*               update event as trigger output.
*
* @note         Must be called before starting ADC conversions
******************************************************************************/
void TIM1_ADC_Trigger_Config(void);

#endif
