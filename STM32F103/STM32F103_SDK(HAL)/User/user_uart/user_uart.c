#include "user_uart.h"

uint8_t usart1SendBuf[USART_SEND_BUF_SIZE + 1];
uint8_t usart1RecvBuf[USART_RECV_BUF_SIZE + 1];
RingBufferTypeDef usart1SendRingBuf;
RingBufferTypeDef usart1RecvRingBuf;
Usart_DataTypeDef FSUS_usart1;
uint8_t rc1;

void User_Uart_Init(UART_HandleTypeDef *huartx)
{
	// Create buffer groups
	RingBuffer_Init(&usart1SendRingBuf, USART_SEND_BUF_SIZE, usart1SendBuf);
	RingBuffer_Init(&usart1RecvRingBuf, USART_RECV_BUF_SIZE, usart1RecvBuf);
	// Initialize custom user serial port structure
	FSUS_usart1.recvBuf = &usart1RecvRingBuf;
	FSUS_usart1.sendBuf = &usart1SendRingBuf;
	FSUS_usart1.huartX = huartx;
	// Enable receive interrupt
	HAL_UART_Receive_IT(FSUS_usart1.huartX, (uint8_t *)&rc1, 1);
}
// Send data
void Usart_SendAll(Usart_DataTypeDef *usart)
{
	uint8_t value;
	while (RingBuffer_GetByteUsed(usart->sendBuf))
	{
		value = RingBuffer_Pop(usart->sendBuf);
		HAL_UART_Transmit(usart->huartX, &value, 1, 1);
	}
}
// Interrupt receive
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t ucTemp;
	if (huart->Instance == FSUS_usart1.huartX->Instance)
	{
		ucTemp = rc1;
		RingBuffer_Push(FSUS_usart1.recvBuf, ucTemp);// Put the received data into the buffer, no specific data processing in the interrupt
		HAL_UART_Receive_IT(FSUS_usart1.huartX, (uint8_t *)&rc1, 1);
	}
}
