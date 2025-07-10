#include "fashion_star_uart_servo_examples.h"


int main(void)
{ 
  SysTick_Init();           // System Clock Initialization
  Usart_Init();              // Serial port initialization
	
 	FSUSExample_PingServo();		// <---   update here
//	FSUSExample_SetServoAngle();
	while(1)
	{
		
	}

}
