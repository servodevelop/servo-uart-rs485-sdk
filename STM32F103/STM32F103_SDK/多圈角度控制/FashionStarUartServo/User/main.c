/********************************************************
 * 舵机多圈控制模式演示
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
// 设置完成之后, 将下行取消注释
Usart_DataTypeDef* servo_usart = &usart1; 

// 使用串口2作为日志输出的端口
// <接线说明>
// STM32F103 PA2(Tx) <----> USB转TTL Rx
// STM32F103 PA3(Rx) <----> USB转TTL Tx
// STM32F103 GND     <----> USB转TTL GND
// STM32F103 V5      <----> USB转TTL 5V (可选)
// <注意事项>
// 使用前确保已设置usart.h里面的USART2_ENABLE为1
// 设置完成之后, 将下行取消注释
Usart_DataTypeDef* loggingUsart = &usart2;

// 重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
    while((loggingUsart->pUSARTx->SR&0X40)==0){}
    /* 发送一个字节数据到串口 */
    USART_SendData(loggingUsart->pUSARTx, (uint8_t) ch);
    /* 等待发送完毕 */
    // while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);       
    return (ch);
}


// 使用串口3作为舵机控制的端口
// <接线说明>
// STM32F103 PB10(Tx)   <----> 总线伺服舵机转接板 Rx
// STM32F103 PB11(Rx)   <----> 总线伺服舵机转接板 Tx
// STM32F103 GND        <----> 总线伺服舵机转接板 GND
// STM32F103 V5         <----> 总线伺服舵机转接板 5V
// <注意事项>
// 使用前确保已设置usart.h里面的USART1_ENABLE为1
// 设置完成之后, 将下行取消注释
// Usart_DataTypeDef* servo_usart = &usart3; 

//// 舵机控制相关的参数
// 舵机的ID号
uint8_t servo_id = 0;  
// 舵机的目标角度
// 舵机角度在-135度到135度之间, 精确到小数点后一位
float angle; 
uint32_t interval;  // 运行时间ms  
float velocity;         // 电机转速, 单位dps,°/s
// 舵机执行功率 单位mV，默认为0 
uint16_t power = 0;
// 加速时间(单位ms)
uint16_t t_acc;
// 减速时间
uint16_t t_dec;

// 读取的角度
float angle_read;
int main (void)
{
    // 嘀嗒定时器初始化
    SysTick_Init();
    // 串口初始化
    Usart_Init();

    while (1){  
        printf("MTurn GOTO: 720.0f\r\n");
        //简易多圈角度控制 + 当前多圈角度查询
        angle = 720.0f;
        interval = 2000;
        FSUS_SetServoAngleMTurn(servo_usart, servo_id, angle, interval, power);
        FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // 等待4s
        SysTick_DelayMs(4000);

			
        printf("MTurn GOTO: 0.0f\r\n");
        angle = 0.0;
        FSUS_SetServoAngleMTurn(servo_usart, servo_id, angle, interval, power);
        FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // 等待3s
        SysTick_DelayMs(3000);


        //带加减速的多圈角度控制(指定周期) + 当前多圈角度查询
        printf("MTurn+Interval GOTO: -180.0f\r\n");
        angle = 180.0f; 
        interval = 1000;
        t_acc = 100;
        t_dec = 200;
        FSUS_SetServoAngleMTurnByInterval(servo_usart, servo_id, angle, interval, t_acc, t_dec, power);
        FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // 等待2s
        SysTick_DelayMs(2000);

        //带加减速的多圈角度控制(指定转速) + 当前多圈角度查询
        printf("MTurn+Velocity GOTO: -180.0f\r\n");
        angle = -180.0f;
        velocity = 100.0f;
        t_acc = 100;
        t_dec = 200;
        FSUS_SetServoAngleMTurnByVelocity(servo_usart, servo_id, angle, velocity, t_acc, t_dec, power);
        FSUS_QueryServoAngleMTurn(servo_usart, servo_id, &angle_read);
        printf("Cur Angle: %.1f\r\n", angle_read);

        // 等待4s
        SysTick_DelayMs(4000);

  }
}
