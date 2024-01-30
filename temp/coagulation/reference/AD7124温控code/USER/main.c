#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "ads1256.h"
#include "math.h"
#include "pwm_output.h"
#include "messagePro.h"
#include "convertTemp.h"
#include "pid.h"
#include "pidAutoTune.h"
#include "timer.h"
#include "delay.h"
#include "dac.h"
#include "key.h"
//#include "i7018.h"
#include "ad7124.h"

static u16 startTemp =0;
static u8 run =0;
struct msgPro_t msgDeal;
PID_t pid_st;
static ad7124_device ad7124Dev;

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	static int cnt =0;
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) 
	{
		if(run)
		{
			cnt++;
			if(cnt > 500 && pid_st.autotemp < pid_st.target)
			{
				pid_st.autotemp++;
				cnt=0;
			}
		}
		else
		{
			cnt =0;
		}
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update); 
}





int main(void)
{ 
	float iTemp;
	float pwm=0;
	double temp =0;
	u8      key =0;
	u8      optMode = 0;
	
	
	u8 lcd_id[64];				//存放LCD ID字符串
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);      //初始化延时函数

	KEY_Init();
 	
	//i7018_init();
//	ADS1256_Init();
//	ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_2d5SPS,DIFF_e);	/* 配置ADC参数： 增益1:1, 数据输出速率 1KHz */
//	delay_ms(500);	
//	
//	//ADS1256_StartScan();	/* 启动中断扫描模式, 轮流采集8个通道的ADC数据. 通过 ADS1256_GetAdc() 函数来读取这些数据 */
	
	AD7124_Setup(&ad7124Dev);
	PidInit(&pid_st,40,20,30,4.0);
	startTemp =20;
	
	Dac1_Init();
	//TIM4_Int_Init(1000-1,84-1);
	TIM3_Int_Init(1000-1,84-1);
	
	POINT_COLOR=BLACK;	      //画笔颜色：红色
	
	LCD_Init();           //初始化LCD FSMC接口
  	while(1) 
	{	
		key = KEY_Scan(1);
		if(key==KEY2_PRES)
		{	
			run=0;
			optMode++;
			optMode%=5;
			
		}
		else if(key==WKUP_PRES)//++
		{
			switch(optMode)
			{
				case 0://sv
					pid_st.target+=0.5;
					break;
				case 1://kp
					pid_st.Kp+=0.2;
					break;
				case 2://ki
					pid_st.Ki +=0.0005;
					break;
				case 3://kd
					pid_st.Kd +=0.5;
				break;
				case 4:
					startTemp ++;
				break;
			}
		}	 
		else if(key==KEY1_PRES) //--
		{
			switch(optMode)
			{
				case 0://sv
					pid_st.target-=0.5;
					break;
				case 1://kp
					pid_st.Kp -=0.2;
					break;
				case 2://ki
					pid_st.Ki -=0.0005;
					break;
				case 3://kd
					pid_st.Kd -=0.5;
				break;
				case 4:
					startTemp--;
				break;
			}
		}
		else if(key==KEY0_PRES)//执行
		{
			run++;
			run%=2;
			if(run == 0)
			{
				Dac1_Set_Vol(380);
			}
			else
			{
				pid_st.autotemp = startTemp;
			}
		}
		
		
		if(run)
		{
			//iTemp = i7018_read_data(0);
			//iTemp= ADS1256_GetAdcAver();
			//iTemp = (((int64_t)AD7124_ReadData(&ad7124Dev))* 2500000.0) / 4194303;
			//iTemp = AD7124_GetChannel(&ad7124Dev);

			

				//iTemp = (((int64_t)AD7124_ReadData(&ad7124Dev))* 2500000.0) / 4194303;
			iTemp =AD7124_ReadAverData(&ad7124Dev);
			temp = convertTemp(eKtype,iTemp);
			pwm = PidCalcFix(&pid_st, temp);
			Dac1_Set_Vol(pwm*10);
			LCD_ShowString(20,170,40,24,24,"TV:");
			sprintf(lcd_id,"%.4f",iTemp);
			LCD_ShowString(80,170,100,24,24,lcd_id);	
			
			LCD_ShowString(20,70,40,24,24,"PV:");
			sprintf(lcd_id,"%.4f",temp);
			LCD_ShowString(80,70,100,24,24,lcd_id);	
		
		
			LCD_ShowString(20,150,40,24,24,"VO:");
			sprintf(lcd_id,"%.4f",pwm*10);
			LCD_ShowString(80,150,100,24,24,lcd_id);	
			
			LCD_ShowString(20,190,40,24,24,"IV:");
			sprintf(lcd_id,"%f",pid_st.autotemp);
			LCD_ShowString(80,190,100,24,24,lcd_id);
			
			delay_ms(100); 
		}
		else
		{
			
			switch(optMode)
			{
				case 0://sv
					LCD_ShowString(20,20,100,24,24,"MODE:SV");
					break;
				case 1://kp
					LCD_ShowString(20,20,100,24,24,"MODE:KP");
					break;
				case 2://ki
					LCD_ShowString(20,20,100,24,24,"MODE:KI");
					break;
				case 3://kd
					LCD_ShowString(20,20,100,24,24,"MODE:KD");
				case 4://kd
					LCD_ShowString(20,20,100,24,24,"MODE:IV");
				break;
			}
			
			LCD_ShowString(20,50,40,24,24,"SV:");
			sprintf(lcd_id,"%f",pid_st.target);
			LCD_ShowString(80,50,100,24,24,lcd_id);	
			
			LCD_ShowString(20,90,40,24,24,"KP:");
			sprintf(lcd_id,"%f",pid_st.Kp);
			LCD_ShowString(80,90,100,24,24,lcd_id);	
			
			LCD_ShowString(20,110,40,24,24,"KI:");
			sprintf(lcd_id,"%f",pid_st.Ki);
			LCD_ShowString(80,110,100,24,24,lcd_id);	
			
			LCD_ShowString(20,130,40,24,24,"KD:");
			sprintf(lcd_id,"%f",pid_st.Kd);
			LCD_ShowString(80,130,100,24,24,lcd_id);	

			LCD_ShowString(20,190,40,24,24,"IV:");
			sprintf(lcd_id,"%f",pid_st.autotemp);
			LCD_ShowString(80,190,100,24,24,lcd_id);
		}
		
	} 
}
