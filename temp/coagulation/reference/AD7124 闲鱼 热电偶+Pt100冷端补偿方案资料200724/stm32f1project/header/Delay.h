
/**
  ******************************************************************************
  * @file    Delay.h
  * @author   
  * @version V1.0
  * @date    07/27/2010
  * @brief   ÑÓÊ±º¯Êý
  ******************************************************************************
*/


#ifndef __DELAY_H
#define __DELAY_H
#include "header.h"
void delay_ms(uint32_t time);
void delay_us(uint32_t time);
void TimingDelay_Decrement(void);
extern __IO uint32_t TimingDelay;
//extern __IO uint32_t TIMCounter;
#endif
