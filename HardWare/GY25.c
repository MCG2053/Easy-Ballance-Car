#include "GY25.h"

uint8_t DataBuf[8];
int32_t pitch=0,roll=0,yaw=0; //俯仰角，翻滚角，航向角
int32_t Read_State=0;

void GY25_Init(void)
{
	// 使能USART3和GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// 配置PB10为USART3_TX (复用推挽输出)
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 配置PB11为USART3_RX (浮空输入)
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 配置USART3参数
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStructure);
	
    // 配置DMA1通道3 (USART3_RX)
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_BufferSize = 8; 							// 缓冲区大小
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 				// 外设（USART）→ 内存
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 					// 非存储器到存储器
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DataBuf; 		// 内存地址（DataBuf）
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; // 内存数据大小：uint8_t
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 		// 内存地址自增（接收下一个数据）
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; 				// 循环模式（持续接收，避免溢出）
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR; // 外设地址（USART3数据寄存器）
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据大小：8位
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 外设地址固定
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;			// 优先级
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel3, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	
    // 配置DMA传输完成中断，用于处理接收的数据
    DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE); // 使能传输完成中断
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART3, ENABLE);
}

/*
*设置模式
*/
void GY25_SetMode(enum SY25_Mode mode)
{
	GY25_ClearBuffer();
	// 提取低8位
    uint8_t lowByte = mode & 0xFF;
	 // 提取高8位
    uint8_t highByte = (mode >> 8) & 0xFF;
    
    GY25_Usart_SendByte(highByte);
    GY25_Usart_SendByte(lowByte);
	GY25_ClearBuffer();
}

void GY25_ClearBuffer(void)
{
	// 1. 禁用USART的DMA接收请求，停止数据传输
	USART_DMACmd(USART3, USART_DMAReq_Rx, DISABLE);
	
	// 2. 停止DMA通道，确保当前传输终止
	DMA_Cmd(DMA1_Channel3, DISABLE);
	
	// 3. 清除DMA中断标志（避免残留中断）
	DMA_ClearITPendingBit(DMA1_IT_TC3 | DMA1_IT_HT3 | DMA1_IT_GL3);
	
	// 4. 清空软件缓冲区（DataBuf）
	for(int i=0; i<8; i++)
	{
		DataBuf[i] = 0;
	}
	
	// 5. 重置DMA计数器（从缓冲区起始位置重新接收）
	DMA1_Channel3->CNDTR = 8;  // 重置传输计数为8（缓冲区大小）
	
	// 6. 清除串口接收标志（避免硬件缓存残留数据）
	if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET)
	{
		USART_ReceiveData(USART3);  // 读DR寄存器清除RXNE标志
	}
	
	// 7. 重新启动DMA和USART接收
	DMA_Cmd(DMA1_Channel3, ENABLE);
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);
	
	// 8. 重置读取状态
	Read_State = 0;
}

void GY25_Usart_SendByte(uint8_t Byte)
{
	USART_SendData(USART3, Byte);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}
