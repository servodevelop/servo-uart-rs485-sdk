#include "sys_tick.h"

// Record system time, timer
// __IO = volatile
static __IO u32 sysTickCnt;

// System timer initialization
void SysTick_Init(void){
		// STM32F407 kernel clock is 168MHz 
		// SystemCoreClock = SYSCLK_FREQ_72MHz = 72000000 
		// SysTick_Config function passes in the number of ticks to generate a system interrupt 
		// Interrupt in 1s -> SystemCoreClock / 1 // Interrupt in 1ms -> SystemCoreClock / 1000, 1s divided into 1000 time segments, each segment is 1ms 
		// Interrupt in 1us SystemCoreClock / 1 
		// 1ms interrupt -> SystemCoreClock / 1000, 1s is divided into 1000 time segments, each segment is 1ms 
		// 1us interrupt -> SystemCoreClock / 1000000
	
    // Set to 1ms interrupt once
	if(SysTick_Config(SystemCoreClock / 1000)){
		// Capture exception
		while(1);
	}
    
    // Disable the tick timer
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

// Wait for the timer to complete
void SysTick_Wait(){
    // Enable the timer
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    // Wait until the timer reaches 0
    while (sysTickCnt > 0);
    // Disable the timer
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

// Delay in microseconds
void SysTick_DelayUs(__IO u32 nTime){
    // Set the clock interrupt to microsecond level
    SysTick_Config(SystemCoreClock / 1000000);
    sysTickCnt = nTime;
    // Wait for the timer to complete
    SysTick_Wait();
    // Reconfigure the system interrupt to millisecond level
    SysTick_Config(SystemCoreClock / 1000);
    // Disable the timer
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}
// Delay in milliseconds
void SysTick_DelayMs(__IO u32 nTime){
    sysTickCnt = nTime;
    SysTick_Wait();
}

// Delay in seconds
void SysTick_DelayS(__IO u32 nTime){
    SysTick_DelayMs(nTime * 1000);
}

// Start a countdown (non-blocking)
void SysTick_CountdownBegin(__IO u32 nTime){
    // Set to interrupt every 1ms here
    sysTickCnt = nTime;
    // Enable the timer
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

// Cancel the countdown
void SysTick_CountdownCancel(void){
    // Reset the tick counter value
    sysTickCnt = 0;
    // Disable the systick timer
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

// Check if the countdown has timed out
uint8_t SysTick_CountdownIsTimeout(void){
    return sysTickCnt == 0;
}

// Set the callback function for the system timer interrupt
void SysTick_Handler(void)
{
    if(sysTickCnt > 0){
        sysTickCnt--;
    }else{
        sysTickCnt = 0;
    }
}
