#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 

void UsartInit(u32 baudRate);
void UsartWrite(u8 ch);

#endif


