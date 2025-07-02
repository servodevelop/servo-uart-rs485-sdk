/********************************************************
 * 测试控制舵机的角度, 让舵机在两个角度之间做周期性旋转
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


//// 舵机控制相关的参数
// 运行时间ms  
// 可以尝试修改设置更小的运行时间，例如500ms
uint16_t interval=1000;
// 舵机执行功率 单位mV 默认为0   
uint16_t power = 0;

int main (void)
{
    // 嘀嗒定时器初始化
    SysTick_Init();
    // 串口初始化
    Usart_Init();

    while (1){
       // 简易角度控制指令，控制0和1号舵机
        FSUS_SetServoAngle(servo_usart, 0, 135.0, interval, power);
        FSUS_SetServoAngle(servo_usart, 1, 45.0, interval, power);
				
        // 等待动作完成
        SysTick_DelayMs(interval);

        // 等待2s
        SysTick_DelayMs(2000);

        // 简易角度控制指令，控制0和1号舵机
        FSUS_SetServoAngle(servo_usart, 0, -135.0, interval, power);
        FSUS_SetServoAngle(servo_usart, 1, -45.0, interval, power);
        // 等待动作完成
        SysTick_DelayMs(interval);

        // 等待2s
        SysTick_DelayMs(2000);
  }
}
