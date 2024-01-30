/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define tempcon1_Pin GPIO_PIN_8
#define tempcon1_GPIO_Port GPIOE
#define tapcon2_Pin GPIO_PIN_9
#define tapcon2_GPIO_Port GPIOE
#define tapcon1_Pin GPIO_PIN_10
#define tapcon1_GPIO_Port GPIOE
#define pumpcon1_Pin GPIO_PIN_11
#define pumpcon1_GPIO_Port GPIOE
#define m_dr_Pin GPIO_PIN_15
#define m_dr_GPIO_Port GPIOE
#define m_en_Pin GPIO_PIN_10
#define m_en_GPIO_Port GPIOB
#define mm_cs_Pin GPIO_PIN_11
#define mm_cs_GPIO_Port GPIOB
#define mm_ck_Pin GPIO_PIN_13
#define mm_ck_GPIO_Port GPIOB
#define mm_mi_Pin GPIO_PIN_14
#define mm_mi_GPIO_Port GPIOB
#define mm_mo_Pin GPIO_PIN_15
#define mm_mo_GPIO_Port GPIOB
#define cpuled_Pin GPIO_PIN_11
#define cpuled_GPIO_Port GPIOD
#define led3_Pin GPIO_PIN_8
#define led3_GPIO_Port GPIOC
#define led4_Pin GPIO_PIN_9
#define led4_GPIO_Port GPIOC
#define led1_Pin GPIO_PIN_8
#define led1_GPIO_Port GPIOA
#define led2_Pin GPIO_PIN_11
#define led2_GPIO_Port GPIOA
#define ledshdn_Pin GPIO_PIN_12
#define ledshdn_GPIO_Port GPIOA
#define kong1_Pin GPIO_PIN_9
#define kong1_GPIO_Port GPIOG
#define kong2_Pin GPIO_PIN_10
#define kong2_GPIO_Port GPIOG
#define kong3_Pin GPIO_PIN_11
#define kong3_GPIO_Port GPIOG
#define kong4_Pin GPIO_PIN_12
#define kong4_GPIO_Port GPIOG
#define m_get_Pin GPIO_PIN_13
#define m_get_GPIO_Port GPIOG
#define adspics_Pin GPIO_PIN_15
#define adspics_GPIO_Port GPIOG
#define adspicld_Pin GPIO_PIN_3
#define adspicld_GPIO_Port GPIOB
#define adspimiso_Pin GPIO_PIN_4
#define adspimiso_GPIO_Port GPIOB
#define adspimosi_Pin GPIO_PIN_5
#define adspimosi_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#pragma pack(1)
typedef struct {
  unsigned char  Motor_SetAuto;	        //��д 0δ��λ       1��λ
  unsigned char  Motor_SetManu;	        //��д 0�ֶ�ֹͣ��� 1�ֶ��������
  unsigned char  Beng_Set;		//��д 0��ת         1-100ת��
  unsigned char  MotoSpeed;             //��д ����ٶ�
  unsigned char  Fa1_Set;		//��д 0�Ͽ�         1�պ�
  unsigned char  Fa2_Set;		//��д 0�Ͽ�         1�պ�
  unsigned char  led1Set;		//��д 0��			 1��
  unsigned char  led2Set;		//��д 0��			 1��
  unsigned char  led3Set;		//��д 0��			 1��
  unsigned char  led4Set;		//��д 0��			 1��
  
  unsigned char  TempMod;               //��д �¿���ģʽ 0�ֶ� 1�Զ�
  unsigned char  TempManuSw;            //��д �ֶ�ģʽ�� 0��   1��
  unsigned short TempRech;		//��д �Զ�ģʽ�� 32.0-43.0 Ŀ���¶�
  
  unsigned char  TempSWSta;             //ֻ�� ���ȿ��ص�״̬
  unsigned char  MotorSta;              //ֻ�� 0û��λ 1��λ
  unsigned int 	 CurretTemp;	        //ֻ�� ��ǰ�¶�370��ʾ37.0
  unsigned int   CurrentPress;	        //ֻ�� ��ǰѹ��ֵ
  unsigned int   DP1light;		//ֻ�� ��ǰDP1�й��
  unsigned int   DP2light;		//ֻ�� ��ǰDP2�й��
  unsigned int   DP3light;		//ֻ�� ��ǰDP3�й��
  unsigned int   DP4light;		//ֻ�� ��ǰDP4�й��
  unsigned char  kong1Sta;		//ֻ�� ��1״̬0δ��λ 1��λ
  unsigned char  kong2Sta;		//ֻ�� ��2״̬0δ��λ 1��λ
  unsigned char  kong3Sta;		//ֻ�� ��3״̬0δ��λ 1��λ
  unsigned char  kong4Sta;		//ֻ�� ��4״̬0δ��λ 1��λ
  unsigned char  KwSta;			//ֻ�� �Լ��� 0δ��λ 1��λ
  unsigned char  YwSta;			//ֻ�� ����   0δ��λ 1��λ

}SETSTADATA;
extern SETSTADATA setSta;
extern unsigned int TimeCnt;
extern unsigned int TemptureTimeCnt;
extern unsigned int TemptureDecTwice;//�����¶�֮���ʾ��������
extern unsigned int TemptureDecReach;//��Ŀ����¶Ȳ��ʾǷ�³̶�
typedef struct {
  
  unsigned int lightAD1[20];
  unsigned int lightAD2[20];
  unsigned int lightAD3[20];
  unsigned int lightAD4[20];

}LIGHTDATA;
 

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
