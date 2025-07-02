/*
 * 串口总线舵机示例代码
 * ---------------------------------------------------
 * ---------------------------------------------------
 */
#ifndef __FASHION_STAR_USART_SERVO_EXAMPLE_H
#define __FASHION_STAR_USART_SERVO_EXAMPLE_H

// 导入系统依赖库
#include "fashion_star_uart_servo.h"

// 变量定义
extern Usart_DataTypeDef *servo_usart;

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

/* 同步命令 */
void demo_SYNC(void);

/* 数据监控 */
void demo_MONTIOR(void);

/* 控制模式停止状态 */
void demo_StopOnControlMode(void);

/* 开启停止异步命令 */
void demo_Begin_EndAsync(void);
#endif
