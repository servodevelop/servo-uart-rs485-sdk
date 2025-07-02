#include "fashion_star_uart_servo_examples.h"


int main(void)
{ 
 // 系统资源初始化 
  SysTick_Init();           // 系统时钟初始化
  LED_Init();               // LED初始化
  Usart_Init();              // 串口初始化

  // 根据例程ID运行测试例程
  // 详情见fashion_star_uart_servo_examples.h里面的定义
  FSUSExample_Run(FSUS_EXAMPLE_BEGIN_ENDASYNC); // 运行例程
	
	while(1)
	{
//		FSUSExample_Run(FSUS_EXAMPLE_BEGIN_ENDASYNC); // 运行例程
	}
}
