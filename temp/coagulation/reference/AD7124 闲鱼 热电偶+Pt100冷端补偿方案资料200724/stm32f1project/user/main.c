 

#include "header.h"
#define k_coef	1.0014
#define b_value	2.4932
#define Rref 2000.0 //定义基准电阻 单位Ω

#define Num 20
float value_buf[Num];
char i=0;
float AD_filter()
{
    char count;
    float sum=0;
		value_buf[i++] = TH_voltage;
    if ( i == Num ) 
        i = 0;
    for ( count=0;count<Num;count++)
        sum += value_buf[count];
    return (float)(sum/Num);

}

#define Num2 20
float value_buf2[Num2];
char i2=0;
float AD_filter2()
{
    char count2;
    float sum2=0;
		value_buf2[i2++] = Resistance;
    if ( i2 == Num ) 
        i2 = 0;
    for ( count2=0;count2<Num2;count2++)
        sum2 += value_buf2[count2];
    return (float)(sum2/Num2);

}
int main(void){
	bspinit();
	delay_us(20);
	global_counter=0;
	ad_start_working = 0;
	TIM_Cmd(TIM4, ENABLE);
	ad_start_working = 1;
 
	AD7124_HardInit();
#ifdef Single_Channel
	AD7124_SINGLE_INIT(Samprate_5Hz);	
#endif
#ifdef Multi_Channel
	AD7124_MUL_INIT(Samprate_5Hz);	
#endif
#ifdef Pt100_Channel_3Wire
	AD7124_PT100_INIT_3Wire();
	AD7124_Set_Gain(16);	
#endif
#ifdef Pt100_Channel_4Wire
	AD7124_PT100_INIT_4Wire();
	AD7124_Set_Gain(1);	
#endif
#ifdef Thermocouple
	AD7124_Thermocouple_init();
	AD7124_Set_Gain(16);	
#endif
	AD_ID_REG=Get_AD7124_ID();
	
	while(1){
		
#ifdef Pt100_Channel_3Wire
		AD7124_PT100_INIT_3Wire();
		AD7124_Set_Gain(16);	
		//三线制双恒流源比例法 
		//AD初始化AIN0-1为输入电压采集
		//AIN2 AIN3 双恒流源输出
		//REF1+和REF1-为基准电压输入
		Get_Once_Vol(Actual_Voltage);			
		//电压折算电阻过程开始
		//计算公式
		/*
		Vref=2.I.Rref
		Vrtd=I.Rrtd
		Vref/Rref=Vrtd/Rrtd
		∵ Vref对应Vmax=8388607 比例等价于2500mV
		   Vrtd对应实际Code 采集到的二进制代码  比例等价于Actual_Voltage
		∴按比例法计算公式如下*/
		Resistance = fabs(AD_Data/AD_Gain)*2.0*Rref/8388608.0;
#endif
		
#ifdef Pt100_Channel_4Wire
		AD7124_PT100_INIT_4Wire();
		AD7124_Set_Gain(8);	
		//四线制恒流源比例法
		//AD初始化AIN0-1为输入电压采集
		// AIN3  恒流源输出
		//REF1+和REF1-为基准电压输入
		Get_Once_Vol(Actual_Voltage);			
		//电压折算电阻过程开始
		//计算公式
		/*
		Vref=I.Rref
		Vrtd=I.Rrtd
		Vref/Rref=Vrtd/Rrtd
		∵ Vref对应Vmax=8388607 比例等价于2500mV
		   Vrtd对应实际Code 采集到的二进制代码  比例等价于Actual_Voltage
		∴按比例法计算公式如下*/
		Resistance = fabs(AD_Data/AD_Gain)*Rref/8388608.0;
 
#endif	
		//电压折算电阻过程结束


#ifdef Thermocouple
		AD7124_Thermocouple_init();
		AD7124_Set_Gain(32);
		Get_Once_Vol(Actual_Voltage);	
		//电压档位校准	
		//y=kx+b
		TH_voltage=Actual_Voltage[0]*k_coef+b_value/1000.0;
 
#endif	
		
		//计算温度
		TempPt100=CalculateTemperature(Resistance);
		//说明：温度转换的过程需要先通过T_TtoV函数把冷端温度折算到热电偶对照表的电压上
		//然后再把当前采集到的电压值加上刚才折算的温度电压  通过T_VtoT函数 把电压转换成最终温度
		tempvoltage=T_TtoV(TempPt100);
		THtemp=T_VtoT(TH_voltage+tempvoltage);
		printf("Voltage=%-8.4f\tResistance=%-5.2f\tTempPt100=%-5.2f\tTHtemp=%-5.2f\n",TH_voltage,Resistance,TempPt100,THtemp);
				CAN_measured1=AD_filter2()*1.000334;
				CAN_measured2=AD_filter()*1000.0;
				CAN_measured3=TempPt100;
				CAN_measured4=THtemp;
//			Get_Once_Vol(Actual_Voltage);
//			Get_Once_Vol(Actual_Voltage);
//			Get_Once_Vol(Actual_Voltage);
//			Get_Once_Vol(Actual_Voltage);			
//		sprintf (writeoneline,"d0:%-10.4f,%-10.4f,%-10.4f,%-10.4f\n", Actual_Voltage[0],Actual_Voltage[1],Actual_Voltage[2],Actual_Voltage[3]);
//		printf("%s",writeoneline);//先打印本机的4个数据
//		printf("hello world!\n");
//		delay_us(50);
	}
}
