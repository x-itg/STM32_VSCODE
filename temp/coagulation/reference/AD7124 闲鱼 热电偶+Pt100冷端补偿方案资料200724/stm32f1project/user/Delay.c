
/**
  ******************************************************************************
  * @file    Delay.c
  * @author   
  * @version V1.0
  * @date    07/27/2010
  * @brief   延时函数
  ******************************************************************************
*/

#include "header.h"

__IO uint32_t TimingDelay;  
//__IO uint32_t TIMCounter;
/**
  * @brief ms级延时
  * @param time: 延时时间
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
  * @brief us级延时
  * @param time: 延时时间
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
  * @brief 延时参数递减函数，用于延时中断中
  * @param none
  * @retval none
*/
void TimingDelay_Decrement(void)
{
  if(TimingDelay != 0x00)    
    TimingDelay--;
}
















