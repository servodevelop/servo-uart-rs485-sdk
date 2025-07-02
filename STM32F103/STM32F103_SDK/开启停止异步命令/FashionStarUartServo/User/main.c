/********************************************************
 * 存储一次命令，在下次发送命令的时候才执行
 ********************************************************/
#include "stm32f10x.h"
#include "usart.h"
#include "sys_tick.h"
#include "fashion_star_uart_servo.h"

// 使用串口1作为舵机控制的端口
// <接线说明>
// STM32F103 PA9(Tx)    <----> 总线伺服舵机转接板 Rx
// STM32F103 PA10(Rx)   <----> 总线伺服舵机转接板 Tx
// STM32F103 GND        <----> 总线伺服舵机转接板 GND
// STM32F103 V5         <----> 总线伺服舵机转接板 5V
// <注意事项>
// 使用前确保已设置usart.h里面的USART1_ENABLE为1
Usart_DataTypeDef* servo_usart = &usart1; 

// 使用串口2作为日志输出的端口
// <接线说明>
// STM32F103 PA2(Tx) <----> USB转TTL Rx
// STM32F103 PA3(Rx) <----> USB转TTL Tx
// STM32F103 GND     <----> USB转TTL GND
// STM32F103 V5      <----> USB转TTL 5V (可选)
Usart_DataTypeDef* logging_usart = &usart2;



// 重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
    while((logging_usart->pUSARTx->SR&0X40)==0){}
    /* 发送一个字节数据到串口 */
    USART_SendData(logging_usart->pUSARTx, (uint8_t) ch);
    /* 等待发送完毕 */
    // while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);       
    return (ch);
}


#define ID 0 // 舵机的ID号
float angle;           //舵机角度设置
float angle_read;			 // 读取的角度
uint16_t power = 1000; // 舵机执行功率 单位mV 默认为0
uint16_t interval = 0; // 舵机旋转的周期

uint8_t async_mode=0; //0:执行存储的命令  1:取消存储的命令

int main (void)
{
    // 嘀嗒定时器初始化
    SysTick_Init();
    // 串口初始化
    Usart_Init();

    while (1){
			
    //开启异步命令
		FSUS_BeginAsync(servo_usart);
	
		printf("GOTO: 135.0f\r\n");
    // 简易角度控制 + 当前角度查询
    angle = 135.0;
    interval = 2000;
    FSUS_SetServoAngle(servo_usart, ID, angle, interval, power);
    FSUS_QueryServoAngle(servo_usart, ID, &angle_read);
    printf("Cur Angle: %.1f\r\n", angle_read);
		
		printf("*******************\n");
		
	//第一次发送上面的命令是不会动的，只是存储了命令，再发一次
	//等待5秒
		SysTick_DelayMs(5000);
		
		//停止异步命令
		FSUS_EndAsync(servo_usart,async_mode);
  }
}
