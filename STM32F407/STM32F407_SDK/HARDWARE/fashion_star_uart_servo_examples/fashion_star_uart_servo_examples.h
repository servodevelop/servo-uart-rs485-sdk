/*
 * 串口总线舵机示例代码
 * ---------------------------------------------------
 * ---------------------------------------------------
 */
#ifndef __FASHION_STAR_USART_SERVO_EXAMPLE_H
#define __FASHION_STAR_USART_SERVO_EXAMPLE_H

// 导入系统依赖库
#include "sys_tick.h"
#include "led.h"
#include "usart.h"
#include "fashion_star_uart_servo.h"

// 演示例程序号(宏定义)
// #define FSUS_EXAMPLE_LED_BLINK 1			  // LED闪烁
// #define FSUS_EXAMPLE_USART_ECHO 2			  // USART收发测试
#define FSUS_EXAMPLE_PING_SERVO 3			  // 舵机通讯检测
#define FSUS_EXAMPLE_SET_SERVO_ANGLE 4		  // 控制单个舵机的角度
#define FSUS_EXAMPLE_SET_N_SERVO_ANGLE 5	  // 控制多个舵机的角度
#define FSUS_EXAMPLE_SET_SERVO_ANGLE_MTURN 6  // 控制单个舵机的角度(多圈模式)
#define FSUS_EXAMPLE_SET_SERVO_DAMPING 7	  // 舵机阻尼模式与角度回读
// #define FSUS_EXAMPLE_RESET_DATA 11			  // 重置用户自定义表
#define FSUS_EXAMPLE_READ_DATA 12			  // 读取数据
// #define FSUS_EXAMPLE_WRITE_DATA 13			  // 写入数据
#define FSUS_EXAMPLE_BEGIN_ENDASYNC 18       //开启停止异步
#define FSUS_EXAMPLE_MONTIOR 23          		  // 数据监控
#define FSUS_EXAMPLE_STOPONCOLMODE  24       //控制模式停止状态
#define FSUS_EXAMPLE_SYNC 25          		  // 同步命令

// 变量定义
extern Usart_DataTypeDef *servo_usart;

// 根据例程编号执行测试例程
void FSUSExample_Run(uint8_t example_id);

/* LED闪烁例程 */
void FSUSExample_LEDBlink(void);

/* USART收发测试 */
void FSUSExample_UsartEcho(void);

/* 舵机通讯检测 */
void FSUSExample_PingServo(void);

/* 控制单个舵机的角度 */
void FSUSExample_SetServoAngle(void);

/* 控制多个舵机的角度 */
void FSUSExample_SetNServoAngle(void);

/* 设置舵机角度(多圈模式) */
void FSUSExample_SetServoAngleMTurn(void);

/* 舵机阻尼模式与角度回传 */
void FSUSExample_SetServoDamping(void);

/* 用户自定义数据-重置 */
void FSUSExample_ResetData(void);

/* 用户自定义数据-读取 */
void FSUSExample_ReadData(void);

/* 用户自定义数据-写入 */
void FSUSExample_WriteData(void);

/* 同步命令控制 */
void FSUSExample_SYNC(void);

/* 数据监控读取舵机参数 */
void FSUSExample_MONTIOR(void);

/* 控制模式停止状态 */
void FSUSExample_StopOnControlMode(void);

/* 开启停止异步 */
void FSUSExample_BEGIN_ENDASYNC(void);
#endif
