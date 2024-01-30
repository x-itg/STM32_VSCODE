#include "led.h"
#include "delay.h"
#include "sys.h"
#include "tmc2660.h"
#include "motor.h"
//ALIENTEK miniSTM32开发板实验1
//跑马灯实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司

 
 int main(void)
 {
	SystemInit();//系统72M
	delay_init();	    	 //延时函数初始化	  
	LED_Init();		  	//初始化与LED连接的硬件接口
	Motor_init();
//unsigned char time=1;
	tmc2660_init();
	motor_run(3200,1);
	
	
	while(1)
	{ 
		LED1=1 ;
		//GPIO_ResetBits(TMC2660_PORTY_STEP,TMC2660_GPIOY_STEP);
		//GPIO_ResetBits(GPIOB,GPIO_Pin_12); //LED0输出低
	//	GPIO_SetBits(GPIOD,GPIO_Pin_2);//LED1输出高
		delay_ms(10);
		LED1 = 0;
	//	GPIO_SetBits(GPIOB,GPIO_Pin_12);//LED0输出高
	//	GPIO_SetBits(TMC2660_PORTY_STEP,TMC2660_GPIOY_STEP);
	//	GPIO_ResetBits(GPIOD,GPIO_Pin_2);//LED1输出低
	 delay_ms(10);
	}
 }

