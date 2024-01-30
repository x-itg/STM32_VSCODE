#include "header.h"
#include "string.h"
 
 
 
 
u16 usart_SendBuffer(u16 size)
{
	if(myusart1.send_flag == 1)
	{
		myusart1.send_flag = 0;
		//DMA发送数据-要先关 设置发送长度 开启DMA
//		while(DMA_GetCurrDataCounter(DMA1_Channel4)!=0);	//等待通道7传输完成   
		DMA_Cmd(DMA1_Channel4, DISABLE );  //关闭 指示的通道        
		DMA_SetCurrDataCounter(DMA1_Channel4,size);//DMA通道的DMA缓存的大小	
		DMA_Cmd(DMA1_Channel4, ENABLE);           //开启DMA传输
	}
}

 
//======================================================================
// 函数名称：
// USART_Tx(uint8_t data)
//
//说明：USART1数据发送
//======================================================================
void USART_Tx_Byte(USART_TypeDef *USARTx, uint8_t data)
{
    USART_SendData(USARTx, data);

    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET); //等待发送完成
//    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) != SET);
//	USART_ClearFlag(USARTx, USART_FLAG_TXE);
    USART_ClearFlag(USARTx, USART_FLAG_TC);
}

//======================================================================
// 函数名称：
// USART1_Tx_nByte(uint8_t data)
//
//说明：USART1数据发送多个数据
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
// 函数名称：
// USART1_Tx_String(uint8_t data)
//
//说明：USART1发送字符串
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
*函数描述:UART1中断接收数据
*局部变量:
*返回变量:
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
// 函数名称：
// USART_Rx_Byte(USART_TypeDef* USARTx)
//
//说明：USART数据接收
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
* Description : Retargets the C library printf function to the USART.printf重定向
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
    /*标准库需要的支持函数，use_no_semihosting_swi以避免使用半主机模式*/  
    struct __FILE  
    {  
        int handle;  
    };  
      
    FILE __stdout;  
    FILE __stdin;  
    /*重定向Printf函数*/  
    int fputc(int ch,FILE *f)  
    {  
        return (SendChar(ch));  
    }  
    /*重定向Scanf函数*/  
    int fgetc(FILE *f)  
    {  
        return (SendChar(GetKey()));  
        /*调用scanf()在串口中输入数据时，必须以空格结束，否则无法完成发送*/  
    }  
      
    void _ttywrch(int ch)  
    {  
        SendChar(ch);  
    }  
      
    int _ferror(FILE *f) {  
      /* Your implementation of ferror */  
      return EOF;  
    }  
      
    //定义_sys_exit()以避免使用半主机模式  
    void _sys_exit(int return_code){  
        //x = x;  
    label:goto label;  
    }  
      
    #endif  
/////////////////////////////////////////printf_scanf//////////////////////////////////////////////////// 

 
		

/******************************************************************
*函数描述:串口发送一个字符
*局部变量:
*返回变量:
******************************************************************/
void Put_Char(uint8_t data)
{
	USART_Tx_Byte(USART2,data);
//    USART_SendData(USART2,data);

//    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) != SET); //等待发送完成
//    USART_ClearFlag(USART2, USART_FLAG_TXE);
}
 
  