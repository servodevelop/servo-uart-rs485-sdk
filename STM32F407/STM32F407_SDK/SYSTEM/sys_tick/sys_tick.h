#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f4xx.h" 

// Initialize system timer
void SysTick_Init(void);

// Delay functions
// Delay in microseconds
void SysTick_DelayUs(__IO u32 nTime);
// Delay in milliseconds
void SysTick_DelayMs(__IO u32 nTime);
// Delay in seconds
void SysTick_DelayS(__IO u32 nTime);

// Countdown functions
// Start a countdown (non-blocking)
void SysTick_CountdownBegin(__IO u32 nTime);
// Cancel the countdown
void SysTick_CountdownCancel(void);
// Check if countdown has timed out
uint8_t SysTick_CountdownIsTimeout(void);
#endif
