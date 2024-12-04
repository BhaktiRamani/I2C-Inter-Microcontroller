/******************************************************************************
* @file         timer6.c
* @brief        Timer6 Peripheral Implementation
* @details      This source file implements Timer6 initialization and configuration
*               for STM32F091RC microcontroller. Timer6 is configured for 
*               precise timing control with DMA update events.
*
* @author       Bhakti Ramani
* @date         November 25, 2024
* @course       ECEN 5813 Principles of Embedded Software
* @institution  University of Colorado Boulder
*
* @copyright    Copyright (C) 2024 by Bhakti Ramani
*               All Rights Reserved
******************************************************************************/

#include "timer6.h"

/* Timer6 Configuration Constants */
#define TIMER6_PRESCALER     (0U)      /* Timer6 prescaler value for maximum frequency */
#define TIMER6_AUTO_RELOAD   (999U)    /* Auto-reload value for desired timing interval */

/******************************************************************************
* @function     Timer6_Init
* @brief        Initializes and configures Timer6 peripheral
* @param        None
* @return       None
*
* @details      Configures Timer6 with following settings:
*               - Enables Timer6 peripheral clock
*               - Sets prescaler for timer frequency division
*               - Configures auto-reload value for period control
*               - Enables DMA update event
*               - Activates Timer6
*
* @note         Timer is configured without prescaling for maximum frequency
*               Auto-reload value of 999 gives required period timing
******************************************************************************/
void Timer6_Init(void) {
   /* Enable Timer6 peripheral clock in APB1 bus */
   RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

   /* Note: Commented configuration shows calculation method for reference
   uint32_t timer_clock = 24000000;  // APB1 clock at 24 MHz
   uint32_t prescaler = (timer_clock / TIMER6_FREQ) - 1; // For 24 kHz timer freq
   uint32_t auto_reload = (TIMER6_FREQ / WAVE_FREQ / SINE_TABLE_SIZE) - 1; */

   /* Configure Timer6 timing parameters */
   TIM6->PSC = TIMER6_PRESCALER;      /* Set prescaler for direct clock input */
   TIM6->ARR = TIMER6_AUTO_RELOAD;    /* Configure period via auto-reload value */

   /* Enable DMA update request */
   TIM6->DIER |= TIM_DIER_UDE;

   /* Note: Master mode configuration commented out
   TIM6->CR2 |= TIM_CR2_MMS_1; // Master mode: Update event as TRGO */

   /* Enable Timer6 counter */
   TIM6->CR1 |= TIM_CR1_CEN;
}