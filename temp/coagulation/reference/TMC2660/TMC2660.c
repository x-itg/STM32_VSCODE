#include "TMC2660.h"

/******************************************************************************
** 函数    :   void TMC2660_CS_Initial(void)
** 功能    :   TMC2660片选初始化
** 参数    :   无
** 返回    :   无  
** 修改时间 : 2013-12-06 
** 说明    :   
******************************************************************************/
void TMC2660_CS_Initial(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	

  RCC_APB2PeriphClockCmd(TMC2660_CS_CLK, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin   = TMC2660_CS_Pin;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  
  GPIO_Init(TMC2660_CS_Port, &GPIO_InitStructure);
	

  TMC2660_CS_NoSelect;
	
  TMC2660_CS_Select;     
  printf("TMC2660_CS_ 选中: %d    ", TMC2660_CS_State);  
  
  TMC2660_CS_NoSelect;    
  printf(" 未选: %d\r\n", TMC2660_CS_State);   
}

unsigned int TMC2660_ReadWrite(unsigned int dat_)
{
  unsigned char tmp_1, tmp_2, tmp_3;
  unsigned char tmp_4, tmp_5, tmp_6;	
  unsigned int ret_;
	
  tmp_4 = (dat_ >> 16) & 0xFF;
  tmp_5 = (dat_ >>  8) & 0xFF;	
  tmp_6 =  dat_        & 0xFF;		

	printf("%02X %02X %02X\r\n\r\n", tmp_4, tmp_5, tmp_6);

  TMC2660_CS_LOW;

  tmp_1 = SPI_ReadWrite_Byte(TMC2660_SPI_Port, tmp_4);

  tmp_2 = SPI_ReadWrite_Byte(TMC2660_SPI_Port, tmp_5);  

  tmp_3 = SPI_ReadWrite_Byte(TMC2660_SPI_Port, tmp_6);

  TMC2660_CS_HIGH;

  ret_ = (tmp_1 << 16) | (tmp_2 << 8) |tmp_3;  

  return ret_;
}

