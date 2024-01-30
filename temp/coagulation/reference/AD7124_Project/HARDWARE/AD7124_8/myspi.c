#include "myspi.h"
#include "delay.h"

//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��


void ADS7124_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);	

	GPIO_InitStructure.GPIO_Pin = PIN_SCK | PIN_DIN | PIN_CS | PIN_SYNC;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //����ٶȿ�����Ҫ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,PIN_SCK | PIN_DIN | PIN_CS | PIN_SYNC);
	
  GPIO_InitStructure.GPIO_Pin = PIN_DOUT |GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //����ٶȿ�����Ҫ����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(PORT_SYNC, &GPIO_InitStructure);


	CS_1();
	SCK_1();		/* SPI���߿���ʱ�������Ǹߵ�ƽ */
	DI_1();	
	SYNC_1;
}


void AD7124_DelaySCLK(void)
{
  uint16_t i;
  for (i = 0; i < 10; i++);
}


void AD7124_DelayDATA(void)
{
  delay_us(10);
}



void AD7124_Send8Bit(uint8_t _data)
{
  uint8_t i;
  
  /* �������Ͷ���ֽ�ʱ����Ҫ�ӳ�һ�� */
  AD7124_DelaySCLK();
  AD7124_DelaySCLK();
  
  /*��AD7124 Ҫ�� SCL�ߵ�ƽ�͵͵�ƽ����ʱ����С 200ns  */
  for(i = 0; i < 8; i++)
  {   
    if (_data & 0x80)
    {
      DI_1();
    }
    else
    {
      DI_0();
    }
    SCK_0();				
    AD7124_DelaySCLK();		
    SCK_1();			/* <----  AD7124 ����SCK�½��ز���DIN����, ���ݱ���ά�� 50nS */
    
    _data <<= 1;
    AD7124_DelaySCLK();		
  }
}


uint8_t AD7124_Recive8Bit(void)
{
  uint8_t i;
  uint8_t read = 0;
  
  AD7124_DelaySCLK();
  /*��AD7124 Ҫ�� SCL�ߵ�ƽ�͵͵�ƽ����ʱ����С 200ns  */
  for (i = 0; i < 8; i++)
  {
    SCK_1();
    AD7124_DelaySCLK();
    SCK_0();
    
    read = read<<1;
    if (DO_IS_HIGH())
    {
      read++;
    }		
    AD7124_DelaySCLK();
  }
  return read;
}






