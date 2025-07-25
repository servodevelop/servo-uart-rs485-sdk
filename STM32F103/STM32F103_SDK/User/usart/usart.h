#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "ring_buffer.h"

#define USART_RECV_BUF_SIZE 500
#define USART_SEND_BUF_SIZE 500

// UART Enable
#define USART1_ENABLE 1
#define USART2_ENABLE 1
#define USART3_ENABLE 0


// UART1 related parameters
#if USART1_ENABLE
    // Baud rate
    #define USART1_BAUDRATE 115200
    // Clock definition
    #define USART1_CLK RCC_APB2Periph_USART1
    #define USART1_APBxClkCmd RCC_APB2PeriphClockCmd
    #define USART1_GPIO_CLK RCC_APB2Periph_GPIOA
    #define USART1_GPIO_APBxClkCmd RCC_APB2PeriphClockCmd
    // GPIO pin definition
    #define USART1_TX_GPIO_PORT GPIOA
    #define USART1_TX_GPIO_PIN GPIO_Pin_9
    #define USART1_RX_GPIO_PORT GPIOA
    #define USART1_RX_GPIO_PIN  GPIO_Pin_10
#endif

// UART2 related parameters
#if USART2_ENABLE
    #define USART2_BAUDRATE 115200
    // Clock definition
    #define USART2_CLK RCC_APB1Periph_USART2
    #define USART2_APBxClkCmd RCC_APB1PeriphClockCmd
    #define USART2_GPIO_CLK RCC_APB2Periph_GPIOA
    #define USART2_GPIO_APBxClkCmd RCC_APB2PeriphClockCmd
    // GPIO pin definition
    #define USART2_TX_GPIO_PORT GPIOA
    #define USART2_TX_GPIO_PIN GPIO_Pin_2
    #define USART2_RX_GPIO_PORT GPIOA
    #define USART2_RX_GPIO_PIN  GPIO_Pin_3
#endif

// UART3 related parameters
#if USART3_ENABLE
    #define USART3_BAUDRATE 115200
    // Clock definition
    #define USART3_CLK RCC_APB1Periph_USART3
    #define USART3_APBxClkCmd RCC_APB1PeriphClockCmd
    #define USART3_GPIO_CLK RCC_APB2Periph_GPIOB
    #define USART3_GPIO_APBxClkCmd RCC_APB2PeriphClockCmd
    // GPIO pin definition
    #define USART3_TX_GPIO_PORT GPIOB
    #define USART3_TX_GPIO_PIN GPIO_Pin_10
    #define USART3_RX_GPIO_PORT GPIOB
    #define USART3_RX_GPIO_PIN  GPIO_Pin_11
#endif

typedef struct
{  
    USART_TypeDef *pUSARTx;
    // Transmit buffer
    RingBufferTypeDef *sendBuf;
	// Receive buffer
    RingBufferTypeDef *recvBuf;
} Usart_DataTypeDef;

// Set all members of gc_block to 0, size is sizeof(parser_block_t)
#if USART1_ENABLE
    extern Usart_DataTypeDef usart1;
    extern uint8_t usart1SendBuf[USART_SEND_BUF_SIZE+1];
	extern uint8_t usart1RecvBuf[USART_RECV_BUF_SIZE+1];
	extern RingBufferTypeDef usart1SendRingBuf;
	extern RingBufferTypeDef usart1RecvRingBuf;
#endif

#if USART2_ENABLE
	extern uint8_t usart2SendBuf[USART_SEND_BUF_SIZE+1];
	extern uint8_t usart2RecvBuf[USART_RECV_BUF_SIZE+1];
	extern RingBufferTypeDef usart2SendRingBuf;
	extern RingBufferTypeDef usart2RecvRingBuf;
	extern Usart_DataTypeDef usart2;

#endif

#if USART3_ENABLE
    extern uint8_t usart3SendBuf[USART_SEND_BUF_SIZE+1];
	extern uint8_t usart3RecvBuf[USART_RECV_BUF_SIZE+1];
	extern RingBufferTypeDef usart3SendRingBuf;
	extern RingBufferTypeDef usart3RecvRingBuf;
	extern Usart_DataTypeDef usart3;
#endif

// Initialize UART
void Usart_Init(void);
// Send a byte
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch);
// Send an array of bytes
void Usart_SendByteArr(USART_TypeDef *pUSARTx, uint8_t *byteArr, uint16_t size);
// Send a string
void Usart_SendString(USART_TypeDef *pUSARTx, char *str);
// Send all content in the UART transmit buffer
void Usart_SendAll(Usart_DataTypeDef *usart);
#endif
