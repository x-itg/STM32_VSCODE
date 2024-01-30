#include "myusart.h"
#include "queue.h"  
#include "messagePro.h"

extern struct msgPro_t msgDeal;

void NVICConfiguration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void UsartInit(u32 baudRate)
{
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	
	USART_InitStruct.USART_BaudRate = baudRate;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStruct);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	NVICConfiguration();
	
	USART_Cmd(USART1,ENABLE);
}



void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 		
		Enqueue(&(msgDeal.recvPollQueue),USART1->DR);
	}
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);	 
}




void UsartWrite(u8 ch)
{
	USART_SendData(USART1, (unsigned char) ch);
	while (!(USART1->SR & USART_FLAG_TXE));
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}

