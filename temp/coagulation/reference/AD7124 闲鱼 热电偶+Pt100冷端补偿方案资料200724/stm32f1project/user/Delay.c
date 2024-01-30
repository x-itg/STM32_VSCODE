
/**
  ******************************************************************************
  * @file    Delay.c
  * @author   
  * @version V1.0
  * @date    07/27/2010
  * @brief   ��ʱ����
  ******************************************************************************
*/

#include "header.h"

__IO uint32_t TimingDelay;  
//__IO uint32_t TIMCounter;
/**
  * @brief ms����ʱ
  * @param time: ��ʱʱ��
  * @retval none
*/
void delay_ms(uint32_t time)
{    
    if(SysTick_Config(SystemCoreClock/1000))
    {
      while(1);
    }
    TimingDelay = time;  
    while(TimingDelay != 0);
}

/**
  * @brief us����ʱ
  * @param time: ��ʱʱ��
  * @retval none
*/
void delay_us(uint32_t time)
{
    if(SysTick_Config(SystemCoreClock/1000000))
    {
      while(1);
    } 
   TimingDelay = time; 
   while(TimingDelay != 0);
}
/*
void Delay_us(uint16_t nCount)
{
  TIMCounter = nCount;
  TIM_Cmd(TIM2, ENABLE);
  while (TIMCounter) {}
  TIM_Cmd(TIM2, DISABLE);
}*/


/**
  * @brief ��ʱ�����ݼ�������������ʱ�ж���
  * @param none
  * @retval none
*/
void TimingDelay_Decrement(void)
{
  if(TimingDelay != 0x00)    
    TimingDelay--;
}
















