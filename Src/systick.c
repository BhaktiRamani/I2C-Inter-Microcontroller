/**
 * @file systick.c
 * @brief PES Assignment 4: Airport Trains - System Tick Timer Implementation
 *
 * This file contains the implementation of System Tick Timer functions
 * used for timing and delay operations in the Airport Train Control System.
 *
 * @author Bhakti Ramani
 * @date 16/10/2024
 *
 * @copyright Copyright (c) 2024 Bhakti Ramani. All rights reserved.
 */

#include "systick.h"

// System tick counter
volatile uint32_t msTicks = 0;
volatile int signal_state = 0;
/**
 * @brief SysTick interrupt handler
 *
 * This function is called every SysTick interrupt (1s) and increments
 * the system tick counter.
 */
void SysTick_Handler(void)
{

    msTicks++;
    if(msTicks >= TICK_VALUE_FOR_2S)
    {
    	GPIOA -> ODR ^= (1<< TRIAL_PIN_POSITION);



    }

}

/**
 * @brief Delay function using milliseconds
 *
 * This function provides a blocking delay for the specified number of milliseconds.
 *
 * @param ms Number of milliseconds to delay
 */
void delay_ms(uint32_t ms)
{
    uint32_t start = msTicks;

    while ((msTicks - start) < ms)
    {
        // Wait
    }
}

/**
 * @brief Initialize the SysTick timer
 *
 * This function sets up the SysTick timer to trigger an interrupt every 1ms,
 * assuming a 48MHz system clock.
 */
void SysTick_Init(void)
{
    // Configure SysTick to trigger every 1 ms second

	SysTick->LOAD = (SystemCoreClock /4); // 250ms tick

	// Assuming 48MHz clock, for 1ms intervals
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;

    // Enable global interrupts
    __enable_irq();
}

/**
 * @brief Get the current system tick count
 *
 * @return uint32_t Current system tick count in milliseconds
 */
uint32_t GetTick(void)
{
    return msTicks;
}
