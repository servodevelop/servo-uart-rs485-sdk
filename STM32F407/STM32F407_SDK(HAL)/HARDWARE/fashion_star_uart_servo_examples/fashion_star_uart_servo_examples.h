/*
 * UART Servo Example Code
 * ---------------------------------------------------
 * ---------------------------------------------------
 */
#ifndef __FASHION_STAR_USART_SERVO_EXAMPLE_H
#define __FASHION_STAR_USART_SERVO_EXAMPLE_H

// Import system dependency libraries
#include "fashion_star_uart_servo.h"

// Variable definition
extern Usart_DataTypeDef *servo_usart;

/* Servo communication test */
void FSUSExample_PingServo(void);

/* Control single servo angle */
void FSUSExample_SetServoAngle(void);

/* Control multiple servos angle */
void FSUSExample_SetNServoAngle(void);

/* Set servo angle (multi-turn mode) */
void FSUSExample_SetServoAngleMTurn(void);

/* Servo damping mode and angle feedback */
void FSUSExample_SetServoDamping(void);

/* User-defined data - Reset */
void FSUSExample_ResetData(void);

/* User-defined data - Read */
void FSUSExample_ReadData(void);

/* User-defined data - Write */
void FSUSExample_WriteData(void);

/* Synchronous command control */
void FSUSExample_SYNC(void);

/* Data monitoring - Read servo parameters */
void FSUSExample_MONTIOR(void);

/* Control mode stop state */
void FSUSExample_StopOnControlMode(void);

/* Enable stop asynchronous */
void FSUSExample_BEGIN_ENDASYNC(void);
#endif
