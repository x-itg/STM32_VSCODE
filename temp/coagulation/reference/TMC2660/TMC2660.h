#ifndef __TMC2660_H
#define __TMC2660_H

#include "main.h"  

#define TMC2660_CS_Select          GPIO_ResetBits(TMC2660_CS_Port, TMC2660_CS_Pin)     //设置为0
#define TMC2660_CS_NoSelect        GPIO_SetBits(TMC2660_CS_Port, TMC2660_CS_Pin)       //设置为1 

#define TMC2660_CS_LOW             GPIO_ResetBits(TMC2660_CS_Port, TMC2660_CS_Pin)     //设置为0
#define TMC2660_CS_HIGH            GPIO_SetBits(TMC2660_CS_Port, TMC2660_CS_Pin)       //设置为1 

#define TMC2660_CS_State           GPIO_ReadInputDataBit(TMC2660_CS_Port, TMC2660_CS_Pin)

void TMC2660_CS_Initial(void);

unsigned int TMC2660_ReadWrite(unsigned int dat_);

#endif
