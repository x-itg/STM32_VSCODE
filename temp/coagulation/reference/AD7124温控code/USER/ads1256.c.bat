#include "sys.h"
#include "stdio.h"
#include "delay.h"
#include "ads1256.h"
#include "spi.h"

#define PIN_PWDN    GPIO_Pin_0
#define PIN_RESET   GPIO_Pin_1
#define PIN_CS      GPIO_Pin_2
#define PIN_DRDY    GPIO_Pin_6

#define PWDN_0()	GPIO_ResetBits(GPIOB, PIN_PWDN)
#define PWDN_1()	GPIO_SetBits(GPIOB, PIN_PWDN)

#define RESET_0()	GPIO_ResetBits(GPIOB, PIN_RESET)
#define RESET_1()	GPIO_SetBits(GPIOB, PIN_RESET)

#define CS_0()		GPIO_ResetBits(GPIOB, PIN_CS)
#define CS_1()		GPIO_SetBits(GPIOB, PIN_CS)

#define DRDY_IS_LOW()	(GPIO_ReadInputDataBit(GPIOB, PIN_DRDY) == Bit_RESET)


/* 寄存器定义： Table 23. Register Map --- ADS1256数据手册第30页 */
enum
{
	/* 寄存器地址， 后面是复位后缺省值 */
	REG_STATUS = 0,	// x1H
	REG_MUX    = 1, // 01H
	REG_ADCON  = 2, // 20H
	REG_DRATE  = 3, // F0H
	REG_IO     = 4, // E0H
	REG_OFC0   = 5, // xxH
	REG_OFC1   = 6, // xxH
	REG_OFC2   = 7, // xxH
	REG_FSC0   = 8, // xxH
	REG_FSC1   = 9, // xxH
	REG_FSC2   = 10, // xxH
};

/* 命令定义： TTable 24. Command Definitions --- ADS1256数据手册第34页 */
enum
{
	CMD_WAKEUP  = 0x00,	// Completes SYNC and Exits Standby Mode 0000  0000 (00h)
	CMD_RDATA   = 0x01, // Read Data 0000  0001 (01h)
	CMD_RDATAC  = 0x03, // Read Data Continuously 0000   0011 (03h)
	CMD_SDATAC  = 0x0F, // Stop Read Data Continuously 0000   1111 (0Fh)
	CMD_RREG    = 0x10, // Read from REG rrr 0001 rrrr (1xh)
	CMD_WREG    = 0x50, // Write to REG rrr 0101 rrrr (5xh)
	CMD_SELFCAL = 0xF0, // Offset and Gain Self-Calibration 1111    0000 (F0h)
	CMD_SELFOCAL= 0xF1, // Offset Self-Calibration 1111    0001 (F1h)
	CMD_SELFGCAL= 0xF2, // Gain Self-Calibration 1111    0010 (F2h)
	CMD_SYSOCAL = 0xF3, // System Offset Calibration 1111   0011 (F3h)
	CMD_SYSGCAL = 0xF4, // System Gain Calibration 1111    0100 (F4h)
	CMD_SYNC    = 0xFC, // Synchronize the A/D Conversion 1111   1100 (FCh)
	CMD_STANDBY = 0xFD, // Begin Standby Mode 1111   1101 (FDh)
	CMD_RESET   = 0xFE, // Reset to Power-Up Values 1111   1110 (FEh)
};

static void ADS1256_WaitDRDY(void);
static void ADS1256_ResetHard(void);


static void ADS1256_WriteCmd(uint8_t _cmd);
static void ADS1256_WriteReg(uint8_t _RegID, uint8_t _RegValue);
static uint8_t ADS1256_ReadReg(uint8_t _RegID);
static int32_t ADS1256_ReadData(void);

static void ADS1256_SetChannal(uint8_t _ch);
static void ADS1256_SetDiffChannal(uint8_t _ch);

ADS1256_VAR_T g_tADS1256;

static const uint8_t s_tabDataRate[ADS1256_DRATE_MAX] = 
{
	0xF0,		/* 复位时缺省值 */
	0xE0,
	0xD0,
	0xC0,
	0xB0,
	0xA1,
	0x92,
	0x82,
	0x72,
	0x63,
	0x53,
	0x43,
	0x33,
	0x20,
	0x13,
	0x03
};





static void ADS1256_DelayDATA(void)
{
	delay_us(10);	/* 最小延迟 6.5uS, 此处取10us */
}

static void ADS1256_ResetHard(void)
{
	/* ADS1256数据手册第7页 */
	RESET_0();			/* 复位 */
	delay_us(5);
	RESET_1();
	delay_us(5);
}



static void ADS1256_WriteReg(uint8_t _RegID, uint8_t _RegValue)
{
	CS_0();	/* SPI片选 = 0 */
	
	SPI1_WriteByte(CMD_WREG | _RegID);	/* 写寄存器的命令, 并发送寄存器地址 */
	SPI1_WriteByte(0x00);		/* 寄存器个数 - 1, 此处写1个寄存器 */
	SPI1_WriteByte(_RegValue);	/* 发送寄存器值 */
	
	CS_1();	/* SPI片选 = 1 */
}


static uint8_t ADS1256_ReadReg(uint8_t _RegID)
{
	uint8_t read;

	CS_0();	/* SPI片选 = 0 */
	SPI1_WriteByte(CMD_RREG | _RegID);	/* 写寄存器的命令, 并发送寄存器地址 */
	SPI1_WriteByte(0x00);	/* 寄存器个数 - 1, 此处读1个寄存器 */
	
	ADS1256_DelayDATA();	/* 必须延迟才能读取芯片返回数据 */
	
	read = SPI1_ReadByte();	/* 读寄存器值 */
	CS_1();	/* SPI片选 = 1 */

	return read;
}


static void ADS1256_WriteCmd(uint8_t _cmd)
{
	CS_0();	/* SPI片选 = 0 */
	SPI1_WriteByte(_cmd);
	CS_1();	/* SPI片选 = 1 */
}


uint8_t ADS1256_ReadChipID(void)
{
	uint8_t id;

	ADS1256_WaitDRDY();
	id = ADS1256_ReadReg(REG_STATUS);
	return (id >> 4);
}


static void ADS1256_SwitchChannal(uint8_t _ch)
{
	switch(g_tADS1256.model)
	{
		case SINGLE_e:
			ADS1256_SetChannal(_ch);
			break;
		case DIFF_e:
			ADS1256_SetDiffChannal(_ch);
			break;
		default:
			ADS1256_SetChannal(_ch);
			break;
	}
}

static void ADS1256_SetChannal(uint8_t _ch)
{
	if (_ch > 7)
	{
		return;
	}
	ADS1256_WriteReg(REG_MUX, (_ch << 4) | (1 << 3));	/* Bit3 = 1, AINN 固定接 AINCOM */
}



static void ADS1256_SetDiffChannal(uint8_t _ch)
{
	if(_ch > 4)
	{
		return ;
	}
	
	if (_ch == 0)
	{
		ADS1256_WriteReg(REG_MUX, (0 << 4) | 1);	/* 差分输入 AIN0， AIN1 */
	}
	else if (_ch == 1)
	{
		ADS1256_WriteReg(REG_MUX, (2 << 4) | 3);	/* 差分输入 AIN2， AIN3 */
	}
	else if (_ch == 2)
	{
		ADS1256_WriteReg(REG_MUX, (4 << 4) | 5);	/* 差分输入 AIN4， AIN5 */
	}
	else if (_ch == 3)
	{
		ADS1256_WriteReg(REG_MUX, (6 << 4) | 7);	/* 差分输入 AIN6， AIN7 */
	}
}


static void ADS1256_WaitDRDY(void)
{
	uint32_t i;

	for (i = 0; i < 40000000; i++)
	{
		if (DRDY_IS_LOW())
		{
			break;
		}
	}
}

static int32_t ADS1256_ReadData(void)
{
	uint32_t read = 0;

	CS_0();	/* SPI片选 = 0 */

	SPI1_WriteByte(CMD_RDATA);	/* 读数据的命令 */
	
	ADS1256_DelayDATA();	/* 必须延迟才能读取芯片返回数据 */

	/* 读采样结果，3个字节，高字节在前 */
	read = SPI1_ReadByte() << 16;
	read += SPI1_ReadByte() << 8;
	read += SPI1_ReadByte() << 0;

	CS_1();	/* SPI片选 = 1 */
	
	/* 负数进行扩展。24位有符号数扩展为32位有符号数 */
	if (read & 0x800000)
	{
		read += 0xFF000000;
	}
	
	return (int32_t)read;
}

static int32_t ADS1256_ReadAdcAver()
{
	int32_t AdcDatatable[10];
	int32_t temp=0.0;
	int32_t AdcDataRev = 0.0;
	int32_t sum =0.0;
	u8 i = 0;
	u8 j = 0;
	
	for(i=0;i<10;i++)
	{
		AdcDatatable[i] = ADS1256_ReadData();
	}
	
	for(i=0;i<9;i++)
	{
		for(j=i+1;j<10;j++)
		{
			if(AdcDatatable[i]>AdcDatatable[j])
			{
				temp = AdcDatatable[i];
				AdcDatatable[i] = AdcDatatable[j];
				AdcDatatable[j] = temp;
			}
		}
	}
	
	for(i=1;i<9;i++)
	{
		sum+=AdcDatatable[i];
	}
	
	AdcDataRev = sum/8;
	return AdcDataRev;
}

int32_t ADS1256_ReadAdc(uint8_t _ch)
{
	/* ADS1256 数据手册第21页 */
	
#if 0	/* 对于30Ksps 采样速率 */
	int32_t read;
	
	while (DRDY_IS_LOW());	/* 等待 DRDY 高 */			
	while (!DRDY_IS_LOW());	/* 等待 DRDY 低 */			
	
	ADS1256_SwitchChannal(_ch);	/* 切换模拟通道 */	
	bsp_DelayUS(5);
	
	ADS1256_WriteCmd(CMD_SYNC);
	bsp_DelayUS(5);
	
	ADS1256_WriteCmd(CMD_WAKEUP);  /* 正常情况下，这个时候 DRDY 已经为高 */
	bsp_DelayUS(25);
			
	read =  (int32_t)ADS1256_ReadData();

	while (DRDY_IS_LOW());	/* 等待 DRDY 高 */			
	while (!DRDY_IS_LOW());	/* 等待 DRDY 低 */			
	
	read =  (int32_t)ADS1256_ReadData();

	return read;
#else	
	//while (DRDY_IS_LOW());
		
	/* ADS1256 数据手册第21页 */
	ADS1256_WaitDRDY();		/* 等待 DRDY = 0 */
	
	ADS1256_SwitchChannal(_ch);	/* 切换模拟通道 */	
	delay_us(5);
	
	ADS1256_WriteCmd(CMD_SYNC);
	delay_us(5);
	
	ADS1256_WriteCmd(CMD_WAKEUP);
	delay_us(25);
	
	//ADS1256_WaitDRDY();		/* 等待 DRDY = 0 */
	
	return (int32_t)ADS1256_ReadData();
#endif	
}



void ADS1256_StartScan(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* 使能SYSCFG时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* 连接 EXTI Line9 到 PH9 引脚 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);
	
	/* 配置 EXTI LineXXX */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	/* 下降沿(等待 DRDY 由1变0的时刻) */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
		
	
	/* 中断优先级配置 最低优先级 这里一定要分开的设置中断，不能够合并到一个里面设置 */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* 开始扫描前, 清零结果缓冲区 */	
	{
		uint8_t i;
		
		g_tADS1256.Channel = 0;
		
		for (i = 0; i < 8; i++)
		{
			g_tADS1256.AdcNow[i] = 0;
		}	
	}
}

void ADS1256_StopScan(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
//	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* 配置 EXTI LineXXX */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	/* 下降沿(等待 DRDY 由1变0的时刻) */
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;		/* 禁止 */
	EXTI_Init(&EXTI_InitStructure);

}

float ADS1256_GetAdc(uint8_t _ch)
{
	int32_t iTemp;
	float re =0.0;
	
	if (_ch > 7)
	{
		return 0;
	}
	
	//INTX_DISABLE();  			/* 关中断 */
	//iTemp = g_tADS1256.AdcNow[_ch];
	//INTX_ENABLE();  				/* 开中断 */		
	iTemp = ADS1256_ReadAdcAver();
	
	re = ((int64_t)iTemp * 2500000) / 4194303.0;
	return re;
}

void ADS1256_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


	/* 配置GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输入口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 无需上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO口最大速度 */

	GPIO_InitStructure.GPIO_Pin = PIN_PWDN | PIN_RESET | PIN_CS |PIN_DRDY;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	SPI1_Init();
	SPI1_SetSpeed(1);
	
	RESET_1();
	PWDN_1();
	CS_1();
}


void ADS1256_CfgADC(ADS1256_GAIN_E _gain, ADS1256_DRATE_E _drate,ADS1256_MODE_E model)
{	
	g_tADS1256.model    = model;
	g_tADS1256.Gain     = _gain;
	g_tADS1256.DataRate = _drate;
	switch(model)
	{
		case SINGLE_e:
			g_tADS1256.MaxChannel =4;
			break;
		case DIFF_e:
			g_tADS1256.MaxChannel =8;
			break;
	}
	
	ADS1256_StopScan();			/* 暂停CPU中断 */
	ADS1256_ResetHard();		/* 硬件复位 */
	ADS1256_WaitDRDY();

	{
		uint8_t buf[4];		/* 暂存ADS1256 寄存器配置参数，之后连续写4个寄存器 */
		buf[0] = (0 << 3) | (1 << 2) | (1 << 1);
		//ADS1256_WriteReg(REG_STATUS, (0 << 3) | (1 << 2) | (1 << 1));
		
		buf[1] = 0x08;	/* 高四位0表示AINP接 AIN0,  低四位8表示 AINN 固定接 AINCOM */
		buf[2] = (0 << 5) | (0 << 2) | (_gain << 1);
		buf[3] = s_tabDataRate[_drate];	// DRATE_10SPS;	/* 选择数据输出速率 */
		
		CS_0();	/* SPI片选 = 0 */
		SPI1_WriteByte(CMD_WREG | 0);	/* 写寄存器的命令, 并发送寄存器地址 */
		SPI1_WriteByte(0x03);			/* 寄存器个数 - 1, 此处3表示写4个寄存器 */
		
		SPI1_WriteByte(buf[0]);	/* 设置状态寄存器 */
		SPI1_WriteByte(buf[1]);	/* 设置输入通道参数 */
		SPI1_WriteByte(buf[2]);	/* 设置ADCON控制寄存器，增益 */
		SPI1_WriteByte(buf[3]);	/* 设置输出数据速率 */
		
		CS_1();	/* SPI片选 = 1 */		
	}

	delay_us(50);	
}

void ADS1256_ISR(void)
{
	g_tADS1256.AdcNow[0] = ADS1256_ReadAdcAver();	/* 注意保存的是上一个通道的数据 */
}


void EXTI1_IRQHandler(void)
{	
	if (EXTI_GetITStatus(EXTI_Line1) != RESET)
	{	
		EXTI_ClearITPendingBit(EXTI_Line1);		/* 清除中断标志位 */
	
		ADS1256_ISR();
		
		/* 执行上面的代码完毕后，再次清零中断标志 */
		EXTI_ClearITPendingBit(EXTI_Line1);		/* 清除中断标志位 */
	}			   
}


