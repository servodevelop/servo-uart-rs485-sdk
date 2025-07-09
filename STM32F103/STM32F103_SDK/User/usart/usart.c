#include "usart.h"


#if USART1_ENABLE
    uint8_t usart1SendBuf[USART_SEND_BUF_SIZE+1];
	uint8_t usart1RecvBuf[USART_RECV_BUF_SIZE+1];
	RingBufferTypeDef usart1SendRingBuf;
	RingBufferTypeDef usart1RecvRingBuf;
	Usart_DataTypeDef usart1;
	 
#endif

#if USART2_ENABLE
    uint8_t usart2SendBuf[USART_SEND_BUF_SIZE+1];
	uint8_t usart2RecvBuf[USART_RECV_BUF_SIZE+1];
	RingBufferTypeDef usart2SendRingBuf;
	RingBufferTypeDef usart2RecvRingBuf;
	Usart_DataTypeDef usart2;
#endif

#if USART3_ENABLE
    uint8_t usart3SendBuf[USART_SEND_BUF_SIZE+1];
	uint8_t usart3RecvBuf[USART_RECV_BUF_SIZE+1];
	RingBufferTypeDef usart3SendRingBuf;
	RingBufferTypeDef usart3RecvRingBuf;
	Usart_DataTypeDef usart3;
#endif

void Usart_Init(void){
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
		RingBuffer_Init(&usart1SendRingBuf,  USART_SEND_BUF_SIZE, usart1SendBuf);
		RingBuffer_Init(&usart1RecvRingBuf, USART_RECV_BUF_SIZE,  usart1RecvBuf);
		usart1.recvBuf = &usart1RecvRingBuf;
		usart1.sendBuf = &usart1SendRingBuf;
	
		// usart1.recvBuf = RingBuffer_Init(USART_RECV_BUF_SIZE);
		// usart1.sendBuf = RingBuffer_Init(USART_SEND_BUF_SIZE);
        // Enable USART Clock
        USART1_APBxClkCmd(USART1_CLK, ENABLE);
        // Enable GPIO Clock
        USART1_GPIO_APBxClkCmd(USART1_GPIO_CLK, ENABLE);
        // Configure the transmit pin of the serial USART port
        GPIO_InitStructure.GPIO_Pin = USART1_TX_GPIO_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//Multiplexing Push-Pull Outputs
        GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStructure);

        // Configure the receive pins of the serial USART port
        GPIO_InitStructure.GPIO_Pin = USART1_RX_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//float input
        GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStructure); 
		
				//USART Initialization Settings
        USART_InitStructure.USART_BaudRate = USART1_BAUDRATE;//serial port baud rate
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;//Word length 8-bit data format
        USART_InitStructure.USART_StopBits = USART_StopBits_1;//A stop bit.
        USART_InitStructure.USART_Parity = USART_Parity_No;//No parity bit
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//No hardware data flow control
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//Sending and Receiving Mode
		
        // NVIC Configuring Serial Port External Interrupts
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // Nested Vector Interrupt Controller Group Selection
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//preemptive prioritization
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//sub-priority
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ Channel Enable
        NVIC_Init(&NVIC_InitStructure);	//Initialize the NVIC registers according to the specified parameters
        

        USART_Init(USART1, &USART_InitStructure);

        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		// Enable Serial Port
        USART_Cmd(USART1, ENABLE);
    #endif
    #if USART2_ENABLE
        usart2.pUSARTx = USART2;

		RingBuffer_Init(&usart2SendRingBuf,  USART_SEND_BUF_SIZE, usart2SendBuf);
		RingBuffer_Init(&usart2RecvRingBuf, USART_RECV_BUF_SIZE,  usart2RecvBuf);
		usart2.recvBuf = &usart2RecvRingBuf;
		usart2.sendBuf = &usart2SendRingBuf;
		

        USART2_APBxClkCmd(USART2_CLK, ENABLE);

        USART2_GPIO_APBxClkCmd(USART2_GPIO_CLK, ENABLE);

        GPIO_InitStructure.GPIO_Pin = USART2_TX_GPIO_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
        GPIO_Init(USART2_TX_GPIO_PORT, &GPIO_InitStructure);


        GPIO_InitStructure.GPIO_Pin = USART2_RX_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(USART2_RX_GPIO_PORT, &GPIO_InitStructure);


        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
        NVIC_Init(&NVIC_InitStructure);	
        
        
        USART_InitStructure.USART_BaudRate = USART2_BAUDRATE;
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;
        USART_InitStructure.USART_StopBits = USART_StopBits_1;
        USART_InitStructure.USART_Parity = USART_Parity_No;
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
        
        USART_Init(USART2, &USART_InitStructure);
        
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		
        USART_Cmd(USART2, ENABLE);
    #endif

    #if USART3_ENABLE
		// 赋值结构体usart指针
        usart3.pUSARTx = USART3;
        // 初始化缓冲区(环形队列)
		RingBuffer_Init(&usart3SendRingBuf,  USART_SEND_BUF_SIZE, usart3SendBuf);
		RingBuffer_Init(&usart3RecvRingBuf, USART_RECV_BUF_SIZE,  usart3RecvBuf);
		usart3.recvBuf = &usart3RecvRingBuf;
		usart3.sendBuf = &usart3SendRingBuf;
		
        // 使能USART时钟
        USART3_APBxClkCmd(USART3_CLK, ENABLE);
        // 使能GPIO时钟
        USART3_GPIO_APBxClkCmd(USART3_GPIO_CLK, ENABLE);
        // 配置串口USART的发送管脚 TX
        GPIO_InitStructure.GPIO_Pin = USART3_TX_GPIO_PIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
        GPIO_Init(USART3_TX_GPIO_PORT, &GPIO_InitStructure);

        // 配置串口USART的接收管脚 RX
        GPIO_InitStructure.GPIO_Pin = USART3_RX_GPIO_PIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
        GPIO_Init(USART3_RX_GPIO_PORT, &GPIO_InitStructure);

        // Usart NVIC 配置串口外部中断
        NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
        NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
        
        //USART 初始化设置
        USART_InitStructure.USART_BaudRate = USART3_BAUDRATE;//串口波特率
        USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
        USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
        USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
        USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
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
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/* Send 8-bit byte stream */
void Usart_SendByteArr(USART_TypeDef *pUSARTx, uint8_t *byteArr, uint16_t size){
    uint16_t bidx;
    for (bidx=0; bidx<size; bidx++){
        Usart_SendByte(pUSARTx, byteArr[bidx]);
    }
    /* Wait for transmit register to be empty */
    // while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/* Send string */
void Usart_SendString(USART_TypeDef *pUSARTx, char *str){
    uint16_t sidx=0;
    do
    {
        Usart_SendByte(pUSARTx, (uint8_t)(*(str + sidx)));
        sidx++;
    } while(*(str + sidx) != '\0');

}

// Sends out the entire contents of the serial port send buffer
void Usart_SendAll(Usart_DataTypeDef *usart){
	uint8_t value;
	while(RingBuffer_GetByteUsed(usart->sendBuf)){
		value = RingBuffer_Pop(usart->sendBuf);
		// printf("Usart_SendAll pop: %d", value);
		Usart_SendByte(usart->pUSARTx, value);
	}
}
#if USART1_ENABLE
    // Define the serial port interrupt handler function
    void USART1_IRQHandler(void){
        uint8_t ucTemp;
	    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)
	    {
			// Read a character from the serial port
            ucTemp = USART_ReceiveData(USART1);
			// New characters are added to the ring buffer queue of the serial port
			RingBuffer_Push(usart1.recvBuf, ucTemp);
        }
    }
	

#endif
	
#if USART2_ENABLE
	// Define the serial port interrupt handler function
    void USART2_IRQHandler(void){
        uint8_t ucTemp;
	    if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
	    {
			// Read a character from the serial port
            ucTemp = USART_ReceiveData(usart2.pUSARTx);
			// New characters are added to the ring buffer queue of the serial port
            RingBuffer_Push(usart2.recvBuf, ucTemp);
        }
    }
#endif

#if USART3_ENABLE
    void USART3_IRQHandler(void){
        uint8_t ucTemp;
	    if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	    {
			// Read a character from the serial port
            ucTemp = USART_ReceiveData(usart3.pUSARTx);
			// Neue Zeichen werden in die Ringpuffer-Warteschlange der seriellen Schnittstelle eingefügt
            RingBuffer_Push(usart3.recvBuf, ucTemp);
        }
    }
#endif
