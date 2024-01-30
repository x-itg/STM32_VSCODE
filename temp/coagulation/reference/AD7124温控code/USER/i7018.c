#include <stm32f4xx.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "i7018.h"
#include "rs485.h"
#include "delay.h"


#define range_15mV    0
#define range_50mV    1
#define range_100mV   2
#define range_500mV   3
#define range_1000mV  4
#define range_2500mV  5
#define range_K_type  0xF


static const u8 mCardChannel  = 10;
static const u8 mEveryDatalen = 8;
static u8 channel_range[10];

void init__channenl(void);
void init_range(void);
u8 i7018_parse(u8 *inData, u8 inData_len,float *vol);


void i7018_init(void)
{
	RS485_Init(9600);
	init__channenl();
	init_range();
}

void init__channenl(void)
{
	u8 recvCmd[] = "$01501\r\n";
	RS485_Send_Data(recvCmd,strlen((const char *)recvCmd));
}

void i7018_change_channel_range(u8 ch, u8 range)
{
	u8 recvCmd[10] =  "$017C";
	
	recvCmd[5] = ch + 0x30;
	recvCmd[6] = 'R';
	recvCmd[7] = '0';
	
	if(range != range_K_type)
	{
		recvCmd[8] =  range + 0x30;
	}
	else
	{
		recvCmd[8] =  'F';
	}
	recvCmd[9] = '\n';
	
	RS485_Send_Data(recvCmd, strlen((const char *)recvCmd));
}

u32 get_max_range(u8 range)
{
	int maxRange = 0;
	switch (range)
	{
		case range_15mV://+-15mv
			maxRange = 15;
			break;
		case range_50mV://+-50mv
			maxRange = 50;
			break;
		case range_100mV://+-100mv
			maxRange = 100;
			break;
		case range_500mV://+-500mv
			maxRange = 500;
			break;
		case range_1000mV://+-1000mv
			maxRange = 1000;
			break;
		case range_2500mV://+-2500mv
			maxRange = 2500;
			break;
		default:
			break;
	}
	return maxRange;
}

//0 正确
//其他错误
u8  adjust_channel_range(u8 channel,double channelVol)
{
	if (abs(channelVol) > get_max_range(channel_range[channel]) * 4 / 5
		&& channel_range[channel] < range_2500mV)
	{
		i7018_change_channel_range(channel, channel_range[channel]+ 1);
		return 0;
	}
	else if (abs(channelVol) < get_max_range(channel_range[channel]) * 1/ 10
		&& channel_range[channel] > range_15mV)
	{
		i7018_change_channel_range(channel, channel_range[channel] - 1);
		return 0;
	}
	return 1;
}

void init_range(void)
{
	u8 i =0;
	channel_range[0] = range_50mV;
	channel_range[1] = range_50mV;
	channel_range[2] = range_100mV;
	channel_range[3] = range_100mV;

	for(i=0; i<4; i++)
	{
		i7018_change_channel_range(i,channel_range[i]);
	}

}


float i7018_read_data(u8 ch)
{
	
	u8 strCmd[] = "#010\r\n";
	u8 strRecv[128]; 
	u8 recvLen = 0;
	u8 i=0;
	float vol =0.0;
	static float olVol =0.0;
	

	strRecv[0] =0;
	RS485_Send_Data(strCmd,strlen((const char *)strCmd));
	delay_ms(300);
	RS485_Receive_Data(strRecv,&recvLen);
	if(recvLen>6 && strRecv[0] == '>')
	{
		if(i7018_parse(strRecv, recvLen, &vol)==0)
		{
			olVol= vol;
		}
	}
	return olVol;
}


u8 i7018_parse(u8 *inData, u8 inData_len,float *vol)
{
	
	u8 i = 0;
	u8 split[7];
		
	if (inData == 0 || inData_len == 0 )
	{
		return 1;
	}

	
	for (i = 1; i < mEveryDatalen; i++)
	{
		split[i-1] = inData[i];
	}
	

	*vol       = atof((const char*)split);
	

	if (channel_range[0] >= range_1000mV)
	{
		*vol *= 1000000;
	}
	else 
	{
		*vol *= 1000;
	}

	return 0;
}

