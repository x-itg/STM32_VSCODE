 

#include "header.h"
#define k_coef	1.0014
#define b_value	2.4932
#define Rref 2000.0 //�����׼���� ��λ��

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
		//������˫����Դ������ 
		//AD��ʼ��AIN0-1Ϊ�����ѹ�ɼ�
		//AIN2 AIN3 ˫����Դ���
		//REF1+��REF1-Ϊ��׼��ѹ����
		Get_Once_Vol(Actual_Voltage);			
		//��ѹ���������̿�ʼ
		//���㹫ʽ
		/*
		Vref=2.I.Rref
		Vrtd=I.Rrtd
		Vref/Rref=Vrtd/Rrtd
		�� Vref��ӦVmax=8388607 �����ȼ���2500mV
		   Vrtd��Ӧʵ��Code �ɼ����Ķ����ƴ���  �����ȼ���Actual_Voltage
		�ఴ���������㹫ʽ����*/
		Resistance = fabs(AD_Data/AD_Gain)*2.0*Rref/8388608.0;
#endif
		
#ifdef Pt100_Channel_4Wire
		AD7124_PT100_INIT_4Wire();
		AD7124_Set_Gain(8);	
		//�����ƺ���Դ������
		//AD��ʼ��AIN0-1Ϊ�����ѹ�ɼ�
		// AIN3  ����Դ���
		//REF1+��REF1-Ϊ��׼��ѹ����
		Get_Once_Vol(Actual_Voltage);			
		//��ѹ���������̿�ʼ
		//���㹫ʽ
		/*
		Vref=I.Rref
		Vrtd=I.Rrtd
		Vref/Rref=Vrtd/Rrtd
		�� Vref��ӦVmax=8388607 �����ȼ���2500mV
		   Vrtd��Ӧʵ��Code �ɼ����Ķ����ƴ���  �����ȼ���Actual_Voltage
		�ఴ���������㹫ʽ����*/
		Resistance = fabs(AD_Data/AD_Gain)*Rref/8388608.0;
 
#endif	
		//��ѹ���������̽���


#ifdef Thermocouple
		AD7124_Thermocouple_init();
		AD7124_Set_Gain(32);
		Get_Once_Vol(Actual_Voltage);	
		//��ѹ��λУ׼	
		//y=kx+b
		TH_voltage=Actual_Voltage[0]*k_coef+b_value/1000.0;
 
#endif	
		
		//�����¶�
		TempPt100=CalculateTemperature(Resistance);
		//˵�����¶�ת���Ĺ�����Ҫ��ͨ��T_TtoV����������¶����㵽�ȵ�ż���ձ�ĵ�ѹ��
		//Ȼ���ٰѵ�ǰ�ɼ����ĵ�ѹֵ���ϸղ�������¶ȵ�ѹ  ͨ��T_VtoT���� �ѵ�ѹת���������¶�
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
//		printf("%s",writeoneline);//�ȴ�ӡ������4������
//		printf("hello world!\n");
//		delay_us(50);
	}
}
