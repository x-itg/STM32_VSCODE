#ifndef __USART_H
#define __USART_H
#include "sys.h"
#include "stdio.h"

#define NMSG 0
#define CMSG 1
#define VMSG 2

#define ADDER   0x01
#define MAXMODE 0x0F
#define MAXLENTH 0x08 

#define VERSIONREADMODE 0x30
#define VREADMODE    0x33
#define STALLMODE    0x34
#define TREADMODE    0x35



void UsartInit(u32 baudRate);
void UsartWrite(u8 ch);


#endif
