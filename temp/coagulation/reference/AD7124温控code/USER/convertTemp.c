#include "convertTemp.h"
#include "math.h"


float  KTempToVoltage(float Temperature)
{
	float c[] = { -0.017600413686,0.038921204975,0.000018558770032,-9.9457592874E-08, 3.1840945719E-10,-5.6072844889E-13,5.6075059059E-16,-3.2020720003E-19,9.7151147152E-23,-1.2104721275E-26};
	float d[] = {0,0.394501280250E-01,0.236223735980E-04,-0.328589067840E-06,-0.499048287770E-08,-0.675090591730E-10,-0.574103274280E-12,-0.310888728940E-14,-0.104516093650E-16,-0.198892668780E-19,-0.163226974860E-22};

	float Voltage = 0;
	float a0 = 0.1185976;
	float a1 = -0.0001183432;
	float a2 = 126.9686;

	u8 i=0;
	if (Temperature == 0)
	{
		Voltage = 0;
	}
	else if (Temperature > 0)
	{
		for (i = 0; i < sizeof(c)/sizeof(c[0]); i++)
		{
			Voltage += c[i] * pow(Temperature, i);
		}
		Voltage += a0 * exp(a1 * (Temperature - a2) * (Temperature - a2));
	}
	else 
	{
		for (i = 0; i < sizeof(d)/sizeof(d[0]); i++)
		{
			Voltage = Voltage + d[i] * pow(Temperature, i);
		}
	}
	return Voltage;
}


float KVoltageToTemp(float Voltage)
{
	float c0[] = { 0, 25.08355,0.07860106,-0.2503131,0.0831527, -0.01228034,0.0009804036,-0.0000441303,0.000001057734,-0.00000001052755};
	float c1[] = {  -131.8058, 48.30222,-1.646031, 0.05464731,-0.0009650715,0.000008802193,-0.0000000311081}; 
	float c2[] = {0,25.173462,-1.1662878,-1.0833638 ,-0.89773540,-0.37342377 ,-0.086632643 ,-0.010450598,0.00051920577};

	float temperature = 0;
	u8 i=0;
	
	if (Voltage >= 0.0 && Voltage < 20.644)
	{
		for ( i = 0; i < sizeof(c0)/sizeof(c0[0]); i++)
		{
			temperature = temperature + c0[i] * pow(Voltage, i);
		}
	}
	else if (Voltage >= 20.644 && Voltage < 54.886)
	{
		for ( i = 0; i < sizeof(c1)/sizeof(c1[0]); i++)
		{
			temperature = temperature + c1[i] * pow(Voltage, i);
		}
	 }
	else if (Voltage < 0)
	{
		for ( i = 0; i < sizeof(c2)/sizeof(c2[0]); i++)
		{
			temperature = temperature + c2[i] * pow(Voltage, i);
		}
	}  
	return temperature;
}
		

float convertTemp(TETYPE_e teType,float voltage)
{
	float TgetValue    = 0;
	float ResistancegetValue =voltage/1000;
	float vol =0;
	
	switch(teType)
	{
		case eKtype:
			vol = KTempToVoltage(25);
			TgetValue = KVoltageToTemp(vol + ResistancegetValue); 
		break;
		default:
			vol = KTempToVoltage(25);
			TgetValue = KVoltageToTemp(vol + ResistancegetValue); 
			break;
	}
	
	return 	TgetValue;	
}


