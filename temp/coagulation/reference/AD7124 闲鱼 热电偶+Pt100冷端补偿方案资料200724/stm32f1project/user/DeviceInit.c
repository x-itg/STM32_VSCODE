
/**
  ******************************************************************************
  * @file    DeviceInit.c
  * @author   
  * @version V1.0
  * @date    08/10/2010
  * @brief   �豸��ʼ��
  ******************************************************************************
*/

#include "header.h"

/**
  * @brief ��ʼ���豸����
  * @param none
  * @retval none
*/
void bspinit(void)
{
 
		RCC_Configuration();
     GPIO_Configuration();
     NVIC_Configuration();
 
     TIM4_configuration();
     USART_configuration();     
 
}
/**
  * @brief ��ʼ���豸ʱ��
  * @param none
  * @retval none
*/
void RCC_Configuration(void)
{
//	ErrorStatus HSEStartUpStatus;
//        RCC_DeInit();
//	RCC_HSEConfig(RCC_HSE_ON);
//	HSEStartUpStatus = RCC_WaitForHSEStartUp();
//	if(HSEStartUpStatus == SUCCESS)
//	{
//	    RCC_HCLKConfig(RCC_SYSCLK_Div1);
//	    RCC_PCLK2Config(RCC_HCLK_Div1);
//	    RCC_PCLK1Config(RCC_HCLK_Div2);
//	    FLASH_SetLatency(FLASH_Latency_2);
//            FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

//            RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_3);

//            RCC_PLLCmd(ENABLE);
//	    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
//	    {}

//	    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//	    while(RCC_GetSYSCLKSource() != 0x08)
//	    {}
//	}
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
												 RCC_APB2Periph_GPIOB | 
												 RCC_APB2Periph_USART1 | 
												 RCC_APB2Periph_AFIO 
													,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4 | RCC_APB1Periph_CAN1,ENABLE);
//NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x1000);  
	
}
 

/**
  * @brief ��ʼ��GPIO
  * @param none
  * @retval none
*/
void GPIO_Configuration(void)
{
	      GPIO_InitTypeDef GPIO_InitStructure;
        GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
        
        GPIO_DeInit(GPIOA);
        GPIO_DeInit(GPIOB);
	
			GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15;

			 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

			GPIO_Init(GPIOC, &GPIO_InitStructure);
			PWR_BackupAccessCmd(ENABLE);//�����޸�RTC �ͺ󱸼Ĵ���
			RCC_LSEConfig(RCC_LSE_OFF);//�ر��ⲿ�����ⲿʱ���źŹ��� ��PC13 PC14 PC15 �ſ��Ե���ͨIO�á�
			BKP_TamperPinCmd(DISABLE);//�ر����ּ�⹦�ܣ�Ҳ���� PC13��Ҳ���Ե���ͨIO ʹ��
			BKP_ITConfig(DISABLE); //�����Ϳ��Ե����������
	
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
//				GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE);
        //uart1
        //TXD1_MCU
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        //RXD1_MCU
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA, &GPIO_InitStructure); 
	      //uart1
 
				//LED BEEP
				GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
				GPIO_Init(GPIOA,&GPIO_InitStructure);  
				LED1_L;
				GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_2;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
				GPIO_Init(GPIOB,&GPIO_InitStructure);  
				 
//				//AD7124
//				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
//				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//				GPIO_Init(GPIOB,&GPIO_InitStructure);
//				
//				GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//				GPIO_Init(GPIOB,&GPIO_InitStructure);
				

			
}

/**
  * @brief �ж�����
  * @param none
  * @retval none
*/
void NVIC_Configuration(void)
{
	      NVIC_InitTypeDef NVIC_InitStructure;
        EXTI_DeInit();

         //TIM4�ж�
        NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
        NVIC_Init(&NVIC_InitStructure);
 
        //USART1�ж�
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_Init(&NVIC_InitStructure);
 
//        //TIM3�ж�
//        NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//        NVIC_Init(&NVIC_InitStructure);
//				
//        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//        NVIC_Init(&NVIC_InitStructure);
}

void TIM2_configuration() 
{
    TIM_TimeBaseInitTypeDef time_init;
    //��ʼ��
    TIM_DeInit(TIM2);
    time_init.TIM_CounterMode = TIM_CounterMode_Up;         //��������
    time_init.TIM_Period = 1000;                   //��������
    time_init.TIM_Prescaler = 8-1;                        //Ԥ��Ƶ,����ʱPSC=N+1
    time_init.TIM_ClockDivision = TIM_CKD_DIV1;             //��������

    TIM_TimeBaseInit(TIM2, &time_init);

    //����ɳ�ʼ�������ĸ��±�־����ֹ�����ж�
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    //ֱ�Ӹ��¼Ĵ���
    TIM_ARRPreloadConfig(TIM2, DISABLE);

    //�ж��趨
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    //�ض�ʱ��
    TIM_Cmd(TIM2, DISABLE);
}
void TIM3_configuration() //5ms
{
    TIM_TimeBaseInitTypeDef time_init;
    //��ʼ��
    TIM_DeInit(TIM3);
    time_init.TIM_CounterMode = TIM_CounterMode_Down;         //��������
    time_init.TIM_Period = 1000;                   //��������
    time_init.TIM_Prescaler = 24-1;                        //Ԥ��Ƶ,����ʱPSC=N+1
    time_init.TIM_ClockDivision = TIM_CKD_DIV1;             //��������

    TIM_TimeBaseInit(TIM3, &time_init);

    //����ɳ�ʼ�������ĸ��±�־����ֹ�����ж�
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);

    //ֱ�Ӹ��¼Ĵ���
    TIM_ARRPreloadConfig(TIM3, DISABLE);

    //�ж��趨
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    //�ض�ʱ��
    TIM_Cmd(TIM3, DISABLE);
}

void TIM4_configuration()
{
  TIM_TimeBaseInitTypeDef time_init;
    //��ʼ��
  TIM_DeInit(TIM4);
  time_init.TIM_CounterMode = TIM_CounterMode_Up;         //��������
  time_init.TIM_Period = 1000;                               //��������
  time_init.TIM_Prescaler = 72-1;                        //Ԥ��Ƶ,����ʱPSC=N+1
  time_init.TIM_ClockDivision = TIM_CKD_DIV1;             //��������

  TIM_TimeBaseInit(TIM4, &time_init);

  //����ɳ�ʼ�������ĸ��±�־����ֹ�����ж�
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);

  //ֱ�Ӹ��¼Ĵ���
  TIM_ARRPreloadConfig(TIM4, DISABLE);

  //�ж��趨
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

  //�ض�ʱ��
  TIM_Cmd(TIM4, DISABLE);
}
 
void USART_configuration(void)
{
 
        USART_InitTypeDef usart_init;
        //��ʼ��
        USART_DeInit(USART1);
        usart_init.USART_BaudRate = 115200;                          //������
        usart_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;     //���ա�����
        usart_init.USART_WordLength = USART_WordLength_8b;         //����λ
        usart_init.USART_StopBits = USART_StopBits_1;              //ֹͣλ
        usart_init.USART_Parity = USART_Parity_No;                 //��żУ��λ
        usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //Ӳ������

        USART_Init(USART1, &usart_init);
        USART_Cmd(USART1, ENABLE);                                  //������
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
        USART_ClearFlag(USART1, USART_FLAG_TC);                     //�������λTCĬ��Ϊ1


        
        //��ʼ��
        USART_DeInit(USART2);
        usart_init.USART_BaudRate = 256000;                          //������
        usart_init.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;     //���ա�����
        usart_init.USART_WordLength = USART_WordLength_8b;         //����λ
        usart_init.USART_StopBits = USART_StopBits_1;              //ֹͣλ
        usart_init.USART_Parity = USART_Parity_No;                 //��żУ��λ
        usart_init.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //Ӳ������

        USART_Init(USART2, &usart_init);
        USART_Cmd(USART2, ENABLE);                                  //������
        USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
        USART_ClearFlag(USART2, USART_FLAG_TC);                     //�������λTCĬ��Ϊ1
}
	 