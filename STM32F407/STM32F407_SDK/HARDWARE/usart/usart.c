#include "usart.h"

#if USART1_ENABLE
uint8_t usart1SendBuf[USART_SEND_BUF_SIZE + 1];
uint8_t usart1RecvBuf[USART_RECV_BUF_SIZE + 1];
RingBufferTypeDef usart1SendRingBuf;
RingBufferTypeDef usart1RecvRingBuf;
Usart_DataTypeDef usart1;

#endif

#if USART2_ENABLE
uint8_t usart2SendBuf[USART_SEND_BUF_SIZE + 1];
uint8_t usart2RecvBuf[USART_RECV_BUF_SIZE + 1];
RingBufferTypeDef usart2SendRingBuf;
RingBufferTypeDef usart2RecvRingBuf;
Usart_DataTypeDef usart2;
#endif

#if USART3_ENABLE
uint8_t usart3SendBuf[USART_SEND_BUF_SIZE + 1];
uint8_t usart3RecvBuf[USART_RECV_BUF_SIZE + 1];
RingBufferTypeDef usart3SendRingBuf;
RingBufferTypeDef usart3RecvRingBuf;
Usart_DataTypeDef usart3;
#endif

void Usart_Init(void)
{
    //GPIO Port Settings
    GPIO_InitTypeDef GPIO_InitStructure;
    // Serial Port Configuration Structure
    USART_InitTypeDef USART_InitStructure;
    // External Interrupt Structure
    NVIC_InitTypeDef NVIC_InitStructure;

#if USART1_ENABLE
    // Assignment structure usart pointer
    usart1.pUSARTx = USART1;
    // Initialization buffer (ring queue)
    RingBuffer_Init(&usart1SendRingBuf, USART_SEND_BUF_SIZE, usart1SendBuf);
    RingBuffer_Init(&usart1RecvRingBuf, USART_RECV_BUF_SIZE, usart1RecvBuf);
    usart1.recvBuf = &usart1RecvRingBuf;
    usart1.sendBuf = &usart1SendRingBuf;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //Enable GPIOA clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //Enable USART1 clock

    //Serial Port 1 Corresponding Pin Multiplexing Mapping
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  //GPIOA9 multiplexed to USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIOA10 is multiplexed to USART1

    //USART1 port configuration
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9 and GPIOA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            //multiplexing function
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //Speed 50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //push-pull multiplexed output
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //pull up
    GPIO_Init(GPIOA, &GPIO_InitStructure);                  //Initialize PA9, PA10

    //USART Initialization Settings
    USART_InitStructure.USART_BaudRate = USART1_BAUDRATE;                           //Baud rate setting
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //Word length in 8-bit data format
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //A stop bit 
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //No parity bit 
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //no hardware data flow control 
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //Transmit/receive mode 
    USART_Init(USART1, &USART_InitStructure);                                       //Initialize serial port 1

    USART_Cmd(USART1, ENABLE); //Enable serial port 1

    USART_ClearFlag(USART1, USART_FLAG_TC);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //Enable related interrupts

    //Usart1 NVIC Configuration
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;         //Serial port 1 interrupt channel
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //Grab Priority 3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //Sub-priority 3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ Channel Enable
    NVIC_Init(&NVIC_InitStructure);                           //Initialize the VIC registers according to the specified parameters

#endif
#if USART2_ENABLE

    // Assign the struct usart pointer.
    usart2.pUSARTx = USART2;
    // Initialize buffer (ring queue)
    RingBuffer_Init(&usart2SendRingBuf, USART_SEND_BUF_SIZE, usart2SendBuf);
    RingBuffer_Init(&usart2RecvRingBuf, USART_RECV_BUF_SIZE, usart2RecvBuf);
    usart2.recvBuf = &usart2RecvRingBuf;
    usart2.sendBuf = &usart2SendRingBuf;

    // Assignment structure usart pointer
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //Enable GPIOA Clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //Enable USART2 clock

    //Serial Port 2 Pin Multiplexing Mapping
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); // GPIOA2 multiplexed to USART2
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIOA3 multiplexed to USART2

    //USART2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2 and GPIOA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //multiplexing function
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;       //Speed 50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //push-pull multiplexed output
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //pull up
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //Initialize PA2, PA3

    //USART2 Initialization Settings
    USART_InitStructure.USART_BaudRate = USART2_BAUDRATE;                           //Baud rate setting
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //Word length 8-bit data format
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //A stop bit.
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //No parity bit
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //No hardware data flow control
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //Sending and Receiving Mode
    USART_Init(USART2, &USART_InitStructure);                                       //Initialize Serial Port 2

    USART_Cmd(USART2, ENABLE); //Enable Serial Port 2

    USART_ClearFlag(USART2, USART_FLAG_TC);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //Turn on acceptance of interrupts

    //Usart2 NVIC Configuration
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //Grab Priority 3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //Sub-priority 3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ Channel Enable
    NVIC_Init(&NVIC_InitStructure);                           //Initializes the VIC registers according to the specified parameters

#endif

#if USART3_ENABLE
    // 赋值结构体usart指针
    usart3.pUSARTx = USART3;
    // 初始化缓冲区(环形队列)
    RingBuffer_Init(&usart3SendRingBuf, USART_SEND_BUF_SIZE, usart3SendBuf);
    RingBuffer_Init(&usart3RecvRingBuf, USART_RECV_BUF_SIZE, usart3RecvBuf);
    usart3.recvBuf = &usart3RecvRingBuf;
    usart3.sendBuf = &usart3SendRingBuf;

    // 使能USART时钟
    USART3_APBxClkCmd(USART3_CLK, ENABLE);
    // 使能GPIO时钟
    USART3_GPIO_APBxClkCmd(USART3_GPIO_CLK, ENABLE);
    // 配置串口USART的发送管脚 TX
    GPIO_InitStructure.GPIO_Pin = USART3_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(USART3_TX_GPIO_PORT, &GPIO_InitStructure);

    // 配置串口USART的接收管脚 RX
    GPIO_InitStructure.GPIO_Pin = USART3_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(USART3_RX_GPIO_PORT, &GPIO_InitStructure);

    // Usart NVIC 配置串口外部中断
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        //子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                           //根据指定的参数初始化NVIC寄存器

    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = USART3_BAUDRATE;                           //串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 //收发模式
    // 初始化串口
    USART_Init(USART3, &USART_InitStructure);
    //开启串口接收中断 IT: Interupt
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    // 使能串口
    USART_Cmd(USART3, ENABLE);
#endif
}

/* Send a single byte */
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
    /* Send a byte to USART */
    USART_SendData(pUSARTx, ch);
    /* Wait for transmit register to be empty */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
        ;
}

/* Send 8-bit byte stream */
void Usart_SendByteArr(USART_TypeDef *pUSARTx, uint8_t *byteArr, uint16_t size)
{
    uint16_t bidx;
    for (bidx = 0; bidx < size; bidx++)
    {
        Usart_SendByte(pUSARTx, byteArr[bidx]);
    }
}

/* Send string */
void Usart_SendString(USART_TypeDef *pUSARTx, char *str)
{
    uint16_t sidx = 0;
    do
    {
        Usart_SendByte(pUSARTx, (uint8_t)(*(str + sidx)));
        sidx++;
    } while (*(str + sidx) != '\0');
}

// Sends out the entire contents of the serial port send buffer
void Usart_SendAll(Usart_DataTypeDef *usart)
{
    uint8_t value;
    while (RingBuffer_GetByteUsed(usart->sendBuf))
    {
        value = RingBuffer_Pop(usart->sendBuf);
        // printf("Usart_SendAll pop: %d", value);
        Usart_SendByte(usart->pUSARTx, value);
    }
}
#if USART1_ENABLE
// Define the serial port interrupt handler function
void USART1_IRQHandler(void)
{
    uint8_t ucTemp;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        // Read a character from the serial port
        ucTemp = USART_ReceiveData(USART1);
        // New characters are added to the ring buffer queue of the serial port
        RingBuffer_Push(usart1.recvBuf, ucTemp);
    }
}


#if 1
#pragma import(__use_no_semihosting)
//Support functions required by the standard library
struct __FILE
{
    int handle;
};

FILE __stdout;
//Define _sys_exit() to avoid using half-host mode
void _sys_exit(int x)
{
    x = x;
}

// Note: Not defining this function will result in an error.
void _ttywrch(int ch){

}

// Redirect the c library function printf to the serial port, after redirection you can use the printf function
int fputc(int ch, FILE *f)
{
    while ((usart1.pUSARTx->SR & 0X40) == 0)
    {
    }
    /* Send a byte of data to the serial port */
    USART_SendData(usart1.pUSARTx, (uint8_t)ch);
    /* Waiting for transmission completion */
    // while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
    return (ch);
}
#endif
#endif

#if USART2_ENABLE
// Define the serial port interrupt handler
void USART2_IRQHandler(void)
{
    uint8_t ucTemp;
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        // Read a character from the serial port
        ucTemp = USART_ReceiveData(usart2.pUSARTx);
        // New characters are added to the serial port's ring buffer queue
        RingBuffer_Push(usart2.recvBuf, ucTemp);
    }
}
#endif

#if USART3_ENABLE
void USART3_IRQHandler(void)
{
    uint8_t ucTemp;
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        // 从串口读取一个字符
        ucTemp = USART_ReceiveData(usart3.pUSARTx);
        // 新的字符添加到串口的环形缓冲队列中
        RingBuffer_Push(usart3.recvBuf, ucTemp);
    }
}
#endif
