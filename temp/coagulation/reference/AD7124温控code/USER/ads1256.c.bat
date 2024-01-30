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


/* �Ĵ������壺 Table 23. Register Map --- ADS1256�����ֲ��30ҳ */
enum
{
	/* �Ĵ�����ַ�� �����Ǹ�λ��ȱʡֵ */
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

/* ����壺 TTable 24. Command Definitions --- ADS1256�����ֲ��34ҳ */
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
	0xF0,		/* ��λʱȱʡֵ */
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
	delay_us(10);	/* ��С�ӳ� 6.5uS, �˴�ȡ10us */
}

static void ADS1256_ResetHard(void)
{
	/* ADS1256�����ֲ��7ҳ */
	RESET_0();			/* ��λ */
	delay_us(5);
	RESET_1();
	delay_us(5);
}



static void ADS1256_WriteReg(uint8_t _RegID, uint8_t _RegValue)
{
	CS_0();	/* SPIƬѡ = 0 */
	
	SPI1_WriteByte(CMD_WREG | _RegID);	/* д�Ĵ���������, �����ͼĴ�����ַ */
	SPI1_WriteByte(0x00);		/* �Ĵ������� - 1, �˴�д1���Ĵ��� */
	SPI1_WriteByte(_RegValue);	/* ���ͼĴ���ֵ */
	
	CS_1();	/* SPIƬѡ = 1 */
}


static uint8_t ADS1256_ReadReg(uint8_t _RegID)
{
	uint8_t read;

	CS_0();	/* SPIƬѡ = 0 */
	SPI1_WriteByte(CMD_RREG | _RegID);	/* д�Ĵ���������, �����ͼĴ�����ַ */
	SPI1_WriteByte(0x00);	/* �Ĵ������� - 1, �˴���1���Ĵ��� */
	
	ADS1256_DelayDATA();	/* �����ӳٲ��ܶ�ȡоƬ�������� */
	
	read = SPI1_ReadByte();	/* ���Ĵ���ֵ */
	CS_1();	/* SPIƬѡ = 1 */

	return read;
}


static void ADS1256_WriteCmd(uint8_t _cmd)
{
	CS_0();	/* SPIƬѡ = 0 */
	SPI1_WriteByte(_cmd);
	CS_1();	/* SPIƬѡ = 1 */
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
	ADS1256_WriteReg(REG_MUX, (_ch << 4) | (1 << 3));	/* Bit3 = 1, AINN �̶��� AINCOM */
}



static void ADS1256_SetDiffChannal(uint8_t _ch)
{
	if(_ch > 4)
	{
		return ;
	}
	
	if (_ch == 0)
	{
		ADS1256_WriteReg(REG_MUX, (0 << 4) | 1);	/* ������� AIN0�� AIN1 */
	}
	else if (_ch == 1)
	{
		ADS1256_WriteReg(REG_MUX, (2 << 4) | 3);	/* ������� AIN2�� AIN3 */
	}
	else if (_ch == 2)
	{
		ADS1256_WriteReg(REG_MUX, (4 << 4) | 5);	/* ������� AIN4�� AIN5 */
	}
	else if (_ch == 3)
	{
		ADS1256_WriteReg(REG_MUX, (6 << 4) | 7);	/* ������� AIN6�� AIN7 */
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

	CS_0();	/* SPIƬѡ = 0 */

	SPI1_WriteByte(CMD_RDATA);	/* �����ݵ����� */
	
	ADS1256_DelayDATA();	/* �����ӳٲ��ܶ�ȡоƬ�������� */

	/* �����������3���ֽڣ����ֽ���ǰ */
	read = SPI1_ReadByte() << 16;
	read += SPI1_ReadByte() << 8;
	read += SPI1_ReadByte() << 0;

	CS_1();	/* SPIƬѡ = 1 */
	
	/* ����������չ��24λ�з�������չΪ32λ�з����� */
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
	/* ADS1256 �����ֲ��21ҳ */
	
#if 0	/* ����30Ksps �������� */
	int32_t read;
	
	while (DRDY_IS_LOW());	/* �ȴ� DRDY �� */			
	while (!DRDY_IS_LOW());	/* �ȴ� DRDY �� */			
	
	ADS1256_SwitchChannal(_ch);	/* �л�ģ��ͨ�� */	
	bsp_DelayUS(5);
	
	ADS1256_WriteCmd(CMD_SYNC);
	bsp_DelayUS(5);
	
	ADS1256_WriteCmd(CMD_WAKEUP);  /* ��������£����ʱ�� DRDY �Ѿ�Ϊ�� */
	bsp_DelayUS(25);
			
	read =  (int32_t)ADS1256_ReadData();

	while (DRDY_IS_LOW());	/* �ȴ� DRDY �� */			
	while (!DRDY_IS_LOW());	/* �ȴ� DRDY �� */			
	
	read =  (int32_t)ADS1256_ReadData();

	return read;
#else	
	//while (DRDY_IS_LOW());
		
	/* ADS1256 �����ֲ��21ҳ */
	ADS1256_WaitDRDY();		/* �ȴ� DRDY = 0 */
	
	ADS1256_SwitchChannal(_ch);	/* �л�ģ��ͨ�� */	
	delay_us(5);
	
	ADS1256_WriteCmd(CMD_SYNC);
	delay_us(5);
	
	ADS1256_WriteCmd(CMD_WAKEUP);
	delay_us(25);
	
	//ADS1256_WaitDRDY();		/* �ȴ� DRDY = 0 */
	
	return (int32_t)ADS1256_ReadData();
#endif	
}



void ADS1256_StartScan(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* ʹ��SYSCFGʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* ���� EXTI Line9 �� PH9 ���� */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource1);
	
	/* ���� EXTI LineXXX */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	/* �½���(�ȴ� DRDY ��1��0��ʱ��) */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
		
	
	/* �ж����ȼ����� ������ȼ� ����һ��Ҫ�ֿ��������жϣ����ܹ��ϲ���һ���������� */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* ��ʼɨ��ǰ, ������������ */	
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
	
	/* ���� EXTI LineXXX */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	/* �½���(�ȴ� DRDY ��1��0��ʱ��) */
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;		/* ��ֹ */
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
	
	//INTX_DISABLE();  			/* ���ж� */
	//iTemp = g_tADS1256.AdcNow[_ch];
	//INTX_ENABLE();  				/* ���ж� */		
	iTemp = ADS1256_ReadAdcAver();
	
	re = ((int64_t)iTemp * 2500000) / 4194303.0;
	return re;
}

void ADS1256_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);


	/* ����GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* ��Ϊ����� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* ��Ϊ����ģʽ */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* �������������� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	/* IO������ٶ� */

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
	
	ADS1256_StopScan();			/* ��ͣCPU�ж� */
	ADS1256_ResetHard();		/* Ӳ����λ */
	ADS1256_WaitDRDY();

	{
		uint8_t buf[4];		/* �ݴ�ADS1256 �Ĵ������ò�����֮������д4���Ĵ��� */
		buf[0] = (0 << 3) | (1 << 2) | (1 << 1);
		//ADS1256_WriteReg(REG_STATUS, (0 << 3) | (1 << 2) | (1 << 1));
		
		buf[1] = 0x08;	/* ����λ0��ʾAINP�� AIN0,  ����λ8��ʾ AINN �̶��� AINCOM */
		buf[2] = (0 << 5) | (0 << 2) | (_gain << 1);
		buf[3] = s_tabDataRate[_drate];	// DRATE_10SPS;	/* ѡ������������� */
		
		CS_0();	/* SPIƬѡ = 0 */
		SPI1_WriteByte(CMD_WREG | 0);	/* д�Ĵ���������, �����ͼĴ�����ַ */
		SPI1_WriteByte(0x03);			/* �Ĵ������� - 1, �˴�3��ʾд4���Ĵ��� */
		
		SPI1_WriteByte(buf[0]);	/* ����״̬�Ĵ��� */
		SPI1_WriteByte(buf[1]);	/* ��������ͨ������ */
		SPI1_WriteByte(buf[2]);	/* ����ADCON���ƼĴ��������� */
		SPI1_WriteByte(buf[3]);	/* ��������������� */
		
		CS_1();	/* SPIƬѡ = 1 */		
	}

	delay_us(50);	
}

void ADS1256_ISR(void)
{
	g_tADS1256.AdcNow[0] = ADS1256_ReadAdcAver();	/* ע�Ᵽ�������һ��ͨ�������� */
}


void EXTI1_IRQHandler(void)
{	
	if (EXTI_GetITStatus(EXTI_Line1) != RESET)
	{	
		EXTI_ClearITPendingBit(EXTI_Line1);		/* ����жϱ�־λ */
	
		ADS1256_ISR();
		
		/* ִ������Ĵ�����Ϻ��ٴ������жϱ�־ */
		EXTI_ClearITPendingBit(EXTI_Line1);		/* ����жϱ�־λ */
	}			   
}


