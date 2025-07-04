#include "stm32f10x.h"
#include "usart.h"
#include "sys_tick.h"
#include "fashion_star_uart_servo.h"
#include "fashion_star_uart_servo_examples.h"


int main(void)
{ 
  SysTick_Init();           // 系统时钟初始化
  Usart_Init();              // 串口初始化
	
 	FSUSExample_PingServo();		// <---   update here
//	FSUSExample_SetServoAngle();
	while(1)
	{
		
	}

}
