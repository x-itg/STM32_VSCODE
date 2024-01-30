#ifndef __HEADER_H
#define __HEADER_H
#include "stm32f10x.h"
 

union unionLong
{
  int32_t  tmp;
  uint8_t arry[4];
};

union unionLong2
{
  float  tmp;
  uint8_t arry[4];
};
union unionLong3
{
  float  tmp;
  unsigned short int arry[2];
};
//LED
#define LED1_H  GPIO_SetBits(GPIOA,GPIO_Pin_2)     
#define LED1_L  GPIO_ResetBits(GPIOA,GPIO_Pin_2) 
 
  
#define uint  unsigned int
#define uchar unsigned char
   
typedef enum {false = 0, true = !false} bool; 
#include "stm32f10x.h"
#include "DeviceInit.h"
//#include "HMI_K600.h"
#include "global.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <Delay.h>
 
 
#include "usart.h"
#include "stm32f10x_conf.h"
#include "stdio.h"
 
#include "AD7124.h"
 
#include "pt100.h"
#include "THERMOCOUPLE.H"
#endif
