#include "sys.h"	
#include "delay.h"	
#include "usart.h"
#include "string.h"
#include "stdlib.h"
#include "ad7124.h"

static ad7124_device ad7124Dev;
extern ad7124_st_reg ad7124_regs[];	
	
extern void test_adc(ad7124_device *device);
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	delay_init();	    	 //延时函数初始化	  
	uart_init1(19200);
	AD7124_Setup(&ad7124Dev,0, ad7124_regs); 
	printf("is run*************\r\n");

	while(1)
	{
		test_adc(&ad7124Dev);
		delay_ms(100);
	}

}

