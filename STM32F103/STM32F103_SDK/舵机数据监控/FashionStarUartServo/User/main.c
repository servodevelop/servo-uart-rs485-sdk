/********************************************************
 *测试舵机的数据回读，并通过串口打印全部数据
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

/*数据监控的数据
* id：舵机的id号
* voltage：舵机的电压
* current：舵机的电流
* power：舵机的执行功率
* temperature：舵机的温度 
* status：舵机的状态
* angle：舵机的角度
* circle_count：舵机的转动圈数*/
ServoData servodata_single[1];//读取一个舵机的数据

//要读取的舵机id号
uint8_t servo_id=0;

int main (void)
{
    // 嘀嗒定时器初始化
    SysTick_Init();
    // 串口初始化
    Usart_Init();

    while (1){
			//每两秒读取一次
			FSUS_DampingMode(servo_usart,servo_id,500);
			FSUS_ServoMonitor(servo_usart,servo_id,servodata_single);
			printf("read ID: %d\r\n", servodata_single[0].id);
			printf("read sucess, voltage: %d mV\r\n", servodata_single[0].voltage);
			printf("read sucess, current: %d mA\r\n", servodata_single[0].current);
			printf("read sucess, power: %d mW\r\n", servodata_single[0].power);
			printf("read sucess, temperature: %d \r\n", servodata_single[0].temperature);
			if ((servodata_single[0].status >> 3) & 0x01)
			printf("read sucess, voltage too high\r\n");
			if ((servodata_single[0].status >> 4) & 0x01)
			printf("read sucess, voltage too low\r\n");
			printf("read sucess, angle: %f\r\n", servodata_single[0].angle);
			printf("read sucess, circle_count: %d\r\n", servodata_single[0].circle_count);
			SysTick_DelayMs(1000);
  }
}
