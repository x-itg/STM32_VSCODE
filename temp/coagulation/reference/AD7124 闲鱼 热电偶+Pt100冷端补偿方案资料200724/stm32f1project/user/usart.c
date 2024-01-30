#include "header.h"
#include "string.h"
 
 
 
 
u16 usart_SendBuffer(u16 size)
{
	if(myusart1.send_flag == 1)
	{
		myusart1.send_flag = 0;
		//DMA��������-Ҫ�ȹ� ���÷��ͳ��� ����DMA
//		while(DMA_GetCurrDataCounter(DMA1_Channel4)!=0);	//�ȴ�ͨ��7�������   
		DMA_Cmd(DMA1_Channel4, DISABLE );  //�ر� ָʾ��ͨ��        
		DMA_SetCurrDataCounter(DMA1_Channel4,size);//DMAͨ����DMA����Ĵ�С	
		DMA_Cmd(DMA1_Channel4, ENABLE);           //����DMA����
	}
}

 
//======================================================================
// �������ƣ�
// USART_Tx(uint8_t data)
//
//˵����USART1���ݷ���
//======================================================================
void USART_Tx_Byte(USART_TypeDef *USARTx, uint8_t data)
{
    USART_SendData(USARTx, data);

    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET); //�ȴ��������
//    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) != SET);
//	USART_ClearFlag(USARTx, USART_FLAG_TXE);
    USART_ClearFlag(USARTx, USART_FLAG_TC);
}

//======================================================================
// �������ƣ�
// USART1_Tx_nByte(uint8_t data)
//
//˵����USART1���ݷ��Ͷ������
//======================================================================

void USART_Tx_nByte(USART_TypeDef *USARTx, uint8_t *address, uint8_t n)
{
		uint8_t i = 0;
    for( i = 0; i < n; i++)
    {
        USART_Tx_Byte(USARTx, address[i]);
    }
}

//======================================================================
// �������ƣ�
// USART1_Tx_String(uint8_t data)
//
//˵����USART1�����ַ���
//======================================================================
void USART_Tx_String(USART_TypeDef *USARTx, uint8_t *address)
{
    uint8_t *str = (uint8_t *)address;

    if (str == NULL)
    {
        return;
    }

    while(*str != '\0')
    {
        USART_Tx_Byte(USARTx, *str);
        str++;
    }
}

void Trans_Byte1(uint8_t byte)
{
    USART_Tx_Byte(USART1, byte);
}
/******************************************************************
*��������:UART1�жϽ�������
*�ֲ�����:
*���ر���:
******************************************************************/
void Trans_String(uchar const *s)
{
    uchar i = 0;
    while(s[i] != '\0')
    {
        Trans_Byte1(s[i]);
        i++;
    }
}
static delay_s()
{
    uint16_t i = 0;
    for(i = 0; i < 10000; i++)
        {}
}
//======================================================================
// �������ƣ�
// USART_Rx_Byte(USART_TypeDef* USARTx)
//
//˵����USART���ݽ���
//======================================================================
uint16_t USART_Rx_Byte(USART_TypeDef *USARTx)
{
    while(!USART_GetFlagStatus(USARTx, USART_FLAG_RXNE));
    USART_ClearFlag(USARTx, USART_FLAG_RXNE);
    return USART_ReceiveData(USARTx);
}
 

/////////////////////////////////////////printf_scanf//////////////////////////////////////////////////// 
/*******************************************************************************
* Function Name : int fputc(int ch, FILE *f)
* Description : Retargets the C library printf function to the USART.printf�ض���
* Input : None
* Output : None
* Return : None
*******************************************************************************/
    int SendChar(int ch)  
    {  
        while(!(USART1->SR & USART_FLAG_TXE));  
        USART1->DR = (ch & 0x1FF);  
      
        return ch;  
    }  
      
    int GetKey(void)  
    {  
        while(!(USART1->SR & USART_FLAG_RXNE));  
        return ((int)(USART1->DR & 0X1FF));  
    }  
    #if 1  
    #pragma import (__use_no_semihosting_swi)  
    /*��׼����Ҫ��֧�ֺ�����use_no_semihosting_swi�Ա���ʹ�ð�����ģʽ*/  
    struct __FILE  
    {  
        int handle;  
    };  
      
    FILE __stdout;  
    FILE __stdin;  
    /*�ض���Printf����*/  
    int fputc(int ch,FILE *f)  
    {  
        return (SendChar(ch));  
    }  
    /*�ض���Scanf����*/  
    int fgetc(FILE *f)  
    {  
        return (SendChar(GetKey()));  
        /*����scanf()�ڴ�������������ʱ�������Կո�����������޷���ɷ���*/  
    }  
      
    void _ttywrch(int ch)  
    {  
        SendChar(ch);  
    }  
      
    int _ferror(FILE *f) {  
      /* Your implementation of ferror */  
      return EOF;  
    }  
      
    //����_sys_exit()�Ա���ʹ�ð�����ģʽ  
    void _sys_exit(int return_code){  
        //x = x;  
    label:goto label;  
    }  
      
    #endif  
/////////////////////////////////////////printf_scanf//////////////////////////////////////////////////// 

 
		

/******************************************************************
*��������:���ڷ���һ���ַ�
*�ֲ�����:
*���ر���:
******************************************************************/
void Put_Char(uint8_t data)
{
	USART_Tx_Byte(USART2,data);
//    USART_SendData(USART2,data);

//    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET); //�ȴ��������
//    USART_ClearFlag(USART2, USART_FLAG_TXE);
}
 
  