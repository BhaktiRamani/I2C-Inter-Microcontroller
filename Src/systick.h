/**
 * @file systick.h
 * @brief PES Assignment 4: Airport Trains - System Tick Timer Header
 *
 * This header file contains declarations for functions related to the
 * System Tick Timer, used for timing and delay operations in the
 * Airport Train Control System.
 *
 * @author Bhakti Ramani
 * @date 16/10/2024
 *
 * @copyright Copyright (c) 2024 Bhakti Ramani. All rights reserved.
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stdint.h>
#include <stm32f091xc.h>
#include <stm32f0xx.h>


#define TICK_VALUE_FOR_2S 8
#define LOAD_VALUE_FOR_1MS 47999
#define TOTAL_SIGNALS 4
#define TRIAL_PIN_POSITION 5    //A5 or D13
extern volatile int signal_state;

/**
 * @brief SysTick interrupt handler
 *
 * This function is called every SysTick interrupt and updates the system time.
 */
void SysTick_Handler(void);

/**
 * @brief Delay function using milliseconds
 *
 * @param ms Number of milliseconds to delay
 */
void delay_ms(uint32_t ms);

/**
 * @brief Initialize the SysTick timer
 *
 * This function sets up the SysTick timer for 1ms intervals.
 */
void SysTick_Init(void);

/**
 * @brief Get the current system tick count
 *
 * @return uint32_t Current system tick count in milliseconds
 */
uint32_t GetTick(void);

#endif /* SYSTICK_H_ */
