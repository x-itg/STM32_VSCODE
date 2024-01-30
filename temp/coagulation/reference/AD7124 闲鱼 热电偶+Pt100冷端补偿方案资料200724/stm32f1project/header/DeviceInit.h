/**
  ******************************************************************************
  * @file    DeviceInit.h
  * @author   
  * @version V1.0
  * @date    08/10/2010
  * @brief   设备初始化函数头文件
  ******************************************************************************
*/

#ifndef __DEVICEINIT_H
#define __DEVICEINTI_H
void RCC_Configuration(void);
void bspinit(void);
void HSI_sysclock_config( uint32_t  RCC_PLLMul_x );
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void USART_configuration(void); 
void TIM2_configuration(void);
void TIM3_configuration(void);
void CAN_Configuration(uint8_t i);
void TIM4_configuration();
void DAC_configuration(void);
void  ADC_configuration(void);

void Adc_DMA_Init(void);
void Adc_Init(void);
#endif