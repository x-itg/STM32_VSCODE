#include "led.h"
#include "delay.h"
#include "sys.h"
#include "tmc2660.h"
#include "motor.h"
//ALIENTEK miniSTM32������ʵ��1
//�����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾

 
 int main(void)
 {
	SystemInit();//ϵͳ72M
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	Motor_init();
//unsigned char time=1;
	tmc2660_init();
	motor_run(3200,1);
	
	
	while(1)
	{ 
		LED1=1 ;
		//GPIO_ResetBits(TMC2660_PORTY_STEP,TMC2660_GPIOY_STEP);
		//GPIO_ResetBits(GPIOB,GPIO_Pin_12); //LED0�����
	//	GPIO_SetBits(GPIOD,GPIO_Pin_2);//LED1�����
		delay_ms(10);
		LED1 = 0;
	//	GPIO_SetBits(GPIOB,GPIO_Pin_12);//LED0�����
	//	GPIO_SetBits(TMC2660_PORTY_STEP,TMC2660_GPIOY_STEP);
	//	GPIO_ResetBits(GPIOD,GPIO_Pin_2);//LED1�����
	 delay_ms(10);
	}
 }

