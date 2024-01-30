
#ifndef __USART_H
#define __USART_H

#include "header.h"

#define   Sign  1    
#define   Unsign 0   
typedef struct
{
  uint16_t x_loc;
  uint16_t y_loc;
}Loction;


//定义颜色
#define yellow 0xFFE0
#define black  0x0004//黑蓝
#define dark_green  0x4410//黑蓝
//#define black  0x0009//黑蓝
#define blue   0x001F
#define green  0xFFFF //0x87E0
#define cyan   0x07ff
#define red    0xF800
#define white  0xFFFF
//定义亮度
#define low_illumination 0x20
#define mid_illumination 0x30
#define high_illumination 0x40
 
u16 usart_SendBuffer(u16 size);
void USART_Tx_Byte(USART_TypeDef *USARTx, uint8_t data);
void USART_Tx_nByte(USART_TypeDef *USARTx, uint8_t *address, uint8_t n);
void USART_Tx_String(USART_TypeDef *USARTx, uint8_t *address);
uint16_t USART_Rx_Byte(USART_TypeDef *USARTx);
void Put_Char(uint8_t data);
void Float_to_Str(uint8_t *s,float num,uint8_t symbol) ;
void Int_to_Str(int32_t num, uint8_t *s, uint8_t n,uint8_t symbol);
void set_illumination(uint8_t n);
void buzzer(uint16_t n);
void dis_image(uint16_t n);
void Dis_Text(uint16_t addr, uint8_t *s, uint8_t s_length);
void ClearText(uint16_t addr, uint8_t s_length);
void Dis_Text_num(uint16_t addr, int num,uint8_t size);
void Dis_Curve(uint8_t ch,uint16_t *s1,uint16_t *s2,uint16_t *s3, uint8_t s_length);
#endif