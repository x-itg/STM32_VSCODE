#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  


//ģʽ����
#define RS485_TX_EN		PGout(8)	//485ģʽ����.0,����;1,����.


														 
void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_Receive_Data(u8 *buf,u8 *len);		 
#endif	   
















