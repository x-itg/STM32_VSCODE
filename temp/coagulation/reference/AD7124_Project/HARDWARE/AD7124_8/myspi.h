#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
/* ¶¨ÒåGPIO¶Ë¿Ú */
#define RCC_SCK 	RCC_AHB1Periph_GPIOB
#define PORT_SCK	GPIOB
#define PIN_SCK		GPIO_Pin_13

#define RCC_DIN 	RCC_AHB1Periph_GPIOB
#define PORT_DIN	GPIOB
#define PIN_DIN		GPIO_Pin_15

#define RCC_CS 		RCC_AHB1Periph_GPIOB
#define PORT_CS		GPIOB
#define PIN_CS		GPIO_Pin_12


#define RCC_DOUT 	RCC_AHB1Periph_GPIOB
#define PORT_DOUT	GPIOB
#define PIN_DOUT	GPIO_Pin_14

#define RCC_SYNC 	RCC_AHB1Periph_GPIOB
#define PORT_SYNC	GPIOB
#define PIN_SYNC	GPIO_Pin_11




#define CS_0()		GPIO_ResetBits(PORT_CS, PIN_CS)
#define CS_1()		GPIO_SetBits(PORT_CS, PIN_CS)

#define SCK_0()		GPIO_ResetBits(PORT_SCK, PIN_SCK)
#define SCK_1()		GPIO_SetBits(PORT_SCK, PIN_SCK)

#define DI_0()		GPIO_ResetBits(PORT_DIN, PIN_DIN)
#define DI_1()		GPIO_SetBits(PORT_DIN, PIN_DIN)

#define DO_IS_HIGH()	(GPIO_ReadInputDataBit(PORT_DOUT, PIN_DOUT) == Bit_SET)
#define SYNC_0	GPIO_ResetBits(PORT_SYNC, PIN_SYNC)
#define SYNC_1 	GPIO_SetBits(PORT_SYNC, PIN_SYNC)


#define RDY_IS_LOW()	(GPIO_ReadInputDataBit(PORT_DOUT, GPIO_Pin_10) == Bit_RESET)


void ADS7124_Init(void);
unsigned char AD7124_Recive8Bit(void);
void AD7124_Send8Bit(unsigned char _data);

#endif

