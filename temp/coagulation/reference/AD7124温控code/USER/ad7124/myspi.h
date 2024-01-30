#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

#define AD7124_CS_0 GPIO_ResetBits(GPIOB,GPIO_Pin_6);
#define AD7124_CS_1 GPIO_SetBits(GPIOB,GPIO_Pin_6);

void SPI1_Init(void);			 //初始化SPI1口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI1速度   
u8 SPI1_ReadWriteByte(u8 TxData);

void ADS7124_Init(void);
uint8_t AD7124_Recive8Bit(void);
void AD7124_Send8Bit(uint8_t _data);

#endif

