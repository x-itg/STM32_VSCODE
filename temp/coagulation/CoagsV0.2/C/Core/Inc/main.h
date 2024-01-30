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
  unsigned char  Motor_SetAuto;	        //读写 0未到位       1到位
  unsigned char  Motor_SetManu;	        //读写 0手动停止电机 1手动启动电机
  unsigned char  Beng_Set;		//读写 0不转         1-100转速
  unsigned char  MotoSpeed;             //读写 马达速度
  unsigned char  Fa1_Set;		//读写 0断开         1闭合
  unsigned char  Fa2_Set;		//读写 0断开         1闭合
  unsigned char  led1Set;		//读写 0关			 1开
  unsigned char  led2Set;		//读写 0关			 1开
  unsigned char  led3Set;		//读写 0关			 1开
  unsigned char  led4Set;		//读写 0关			 1开
  
  unsigned char  TempMod;               //读写 温控制模式 0手动 1自动
  unsigned char  TempManuSw;            //读写 手动模式下 0关   1开
  unsigned short TempRech;		//读写 自动模式下 32.0-43.0 目标温度
  
  unsigned char  TempSWSta;             //只读 加热开关的状态
  unsigned char  MotorSta;              //只读 0没到位 1到位
  unsigned int 	 CurretTemp;	        //只读 当前温度370表示37.0
  unsigned int   CurrentPress;	        //只读 当前压力值
  unsigned int   DP1light;		//只读 当前DP1感光点
  unsigned int   DP2light;		//只读 当前DP2感光点
  unsigned int   DP3light;		//只读 当前DP3感光点
  unsigned int   DP4light;		//只读 当前DP4感光点
  unsigned char  kong1Sta;		//只读 孔1状态0未到位 1到位
  unsigned char  kong2Sta;		//只读 孔2状态0未到位 1到位
  unsigned char  kong3Sta;		//只读 孔3状态0未到位 1到位
  unsigned char  kong4Sta;		//只读 孔4状态0未到位 1到位
  unsigned char  KwSta;			//只读 试剂卡 0未到位 1到位
  unsigned char  YwSta;			//只读 样本   0未到位 1到位

}SETSTADATA;
extern SETSTADATA setSta;
extern unsigned int TimeCnt;
extern unsigned int TemptureTimeCnt;
extern unsigned int TemptureDecTwice;//两次温度之差：表示升温速率
extern unsigned int TemptureDecReach;//离目标的温度差：表示欠温程度
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
