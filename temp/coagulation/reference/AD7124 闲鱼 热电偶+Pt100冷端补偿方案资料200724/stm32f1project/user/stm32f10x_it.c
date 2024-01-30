/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "header.h"
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 
 NVIC_InitTypeDef NVIC_InitStructure;
 EXTI_InitTypeDef EXTI_InitStructure;
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

	 TimingDelay_Decrement();

}
  
void USART1_IRQHandler(void)
{	
			uint8_t Usart1_Rec_Cnt = 0;
	
	uint32_t temp = 0;
	uint16_t i = 0;
 
  uint8_t data = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		data=USART1->DR & (uint8_t)0x00FF;
		myusart1.timeout_counter=0;
		myusart1.temp_buffer[myusart1.counter] = data;
		myusart1.counter++;	
		TIM_Cmd(TIM2,ENABLE);			          
	}
}
void USART2_IRQHandler(void)
{	
	uint32_t temp = 0;
	uint16_t i = 0;
  uint8_t data = 0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		data=USART2->DR & (uint8_t)0x00FF;
		myusart2.timeout_counter=0;
		myusart2.temp_buffer[myusart2.counter] = data;
		myusart2.counter++;	
		TIM_Cmd(TIM3,ENABLE);			          
	}
} 
void TIM2_IRQHandler(void)
{	
	static u8 count,i;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{ 	
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		myusart1.timeout_counter++;
		if(myusart1.timeout_counter>=20)//20*1=20ms
		{
			TIM_Cmd(TIM2,DISABLE);
			myusart1.timeout_counter =0;
			myusart1.timeout_flag=1;
			myusart1.counter=0;
			for(i=0;i<Rx_bufSize;i++)
			{
				myusart1.rx_buffer[i]=myusart1.temp_buffer[i];
				myusart1.temp_buffer[i]=0;
			}
			myusart1.rev_complete=1;
		}		
	}
}
void TIM3_IRQHandler(void)
{
  u8 i;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update))
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		myusart2.timeout_counter++;
		if(myusart2.timeout_counter>=2)//20*1=20ms
		{
			TIM_Cmd(TIM3,DISABLE);
			myusart2.timeout_counter =0;
			myusart2.timeout_flag=1;
			myusart2.counter=0;
			for(i=0;i<Rx_bufSize;i++)
			{
				myusart2.rx_buffer[i]=myusart2.temp_buffer[i];
				myusart2.temp_buffer[i]=0;
			}
			myusart2.rev_complete=1;
//			lcd_revack_process();
		}	
	}
	NVIC_ClearPendingIRQ(TIM3_IRQn);   
}
void TIM4_IRQHandler(void)
{
		static u16 tt,showcounter;
		static u32 counter,temp;
  if (TIM_GetITStatus(TIM4, TIM_IT_Update))
  {
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    Trig = 1;
		global_counter++;
		if(ad_start_working == 1){
			AD_Tim_Scan();
		}
//		if(++tt>=500){tt=0;
//			GPIO_WriteBit(GPIOA,GPIO_Pin_2,(BitAction)(1-(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2))));
//		}
  }
  NVIC_ClearPendingIRQ(TIM4_IRQn);    
}

  
/******************************************************************************/ 
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
