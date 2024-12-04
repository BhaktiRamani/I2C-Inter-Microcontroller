/******************************************************************************
* @file         timer1.c
* @brief        Timer1 Configuration Implementation for ADC Triggering
* @details      Implements Timer1 setup for precise ADC trigger timing control
*               on STM32F091RC microcontroller.
*
* @author       Bhakti Ramani
* @date         November 26, 2024
* @course       ECEN 5813 Principles of Embedded Software
* @institution  University of Colorado Boulder
*
* @copyright    Copyright (C) 2024 by Bhakti Ramani
*               All Rights Reserved
******************************************************************************/

#include "timer1.h"

/* Timer1 Configuration Constants */
#define TIMER1_PRESCALER     (0U)        /* No prescaling for maximum frequency */
#define TIMER1_AUTO_RELOAD   (499U)      /* Auto-reload for 96kHz sampling frequency */
#define OLD_TIMER_PRESCALER  (71U)       /* Legacy prescaler value for 1MHz timing */
#define OLD_TIMER_RELOAD     (999U)      /* Legacy reload value for 1kHz overflow */
#define OLD_COMPARE_VALUE    (499U)      /* Legacy 50% duty cycle value */

/******************************************************************************
* @function     TIM1_ADC_Trigger_Config
* @brief        Configures Timer1 as ADC trigger source
* @param        None
* @return       None
*
* @details      Performs following Timer1 configurations:
*               1. Enables Timer1 clock on APB2 bus
*               2. Sets period for 96kHz sampling rate
*               3. Configures master mode for ADC triggering
*               4. Activates Timer1
*
* @note         Current implementation uses update event as trigger output
******************************************************************************/
void TIM1_ADC_Trigger_Config(void)
{
    /* Enable Timer1 peripheral clock in APB2 bus */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

    /* Configure Timer1 period for 96kHz sampling frequency */
    TIM1->ARR = TIMER1_AUTO_RELOAD;  /* Set auto-reload for 96kHz rate */
    
    /* Configure timing parameters */
    TIM1->PSC = TIMER1_PRESCALER;    /* No prescaling */
    
    /* Set master mode: Update event as trigger output */
    TIM1->CR2 |= TIM_CR2_MMS_1;

    /* Enable Timer1 counter */
    TIM1->CR1 |= TIM_CR1_CEN;
}

