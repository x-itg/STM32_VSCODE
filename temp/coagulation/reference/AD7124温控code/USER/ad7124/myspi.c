#include "myspi.h"
#include "delay.h"

//以下是SPI模块的初始化代码，配置成主机模式 						  
//SPI口初始化
//这里针是对SPI1的初始化
void SPI1_Init(void)
{	 
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);//使能SPI1时钟

	//GPIOFB3,4,5初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5复用功能输出	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PB3~5复用功能输出	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
		
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4复用为 SPI1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5复用为 SPI1

 
	//这里只针对SPI口初始化
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
}   
//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1 
} 
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{	
	u8 val =0;
		
     while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	 SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个byte  数据
		 
     while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
	 SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据		
	
	return 	val;
}




/* 定义GPIO端口 */
#define RCC_SCK 	RCC_AHB1Periph_GPIOB
#define PORT_SCK	GPIOB
#define PIN_SCK		GPIO_Pin_3

#define RCC_DIN 	RCC_AHB1Periph_GPIOB
#define PORT_DIN	GPIOB
#define PIN_DIN		GPIO_Pin_5

#define RCC_CS 		RCC_AHB1Periph_GPIOB
#define PORT_CS		GPIOB
#define PIN_CS		GPIO_Pin_6


#define RCC_DOUT 	RCC_AHB1Periph_GPIOB
#define PORT_DOUT	GPIOB
#define PIN_DOUT	GPIO_Pin_4

#define RCC_SYNC 	RCC_AHB1Periph_GPIOB
#define PORT_SYNC	GPIOB
#define PIN_SYNC	GPIO_Pin_7




#define CS_0()		GPIO_ResetBits(PORT_CS, PIN_CS)
#define CS_1()		GPIO_SetBits(PORT_CS, PIN_CS)

#define SCK_0()		GPIO_ResetBits(PORT_SCK, PIN_SCK)
#define SCK_1()		GPIO_SetBits(PORT_SCK, PIN_SCK)

#define DI_0()		GPIO_ResetBits(PORT_DIN, PIN_DIN)
#define DI_1()		GPIO_SetBits(PORT_DIN, PIN_DIN)

#define DO_IS_HIGH()	(GPIO_ReadInputDataBit(PORT_DOUT, PIN_DOUT) == Bit_SET)
#define SYNC_0	GPIO_ResetBits(PORT_SYNC, PIN_SYNC)
#define SYNC_1 	GPIO_SetBits(PORT_SYNC, PIN_SYNC)
	


void AD7124_Send8Bit(uint8_t _data);
uint8_t AD7124_Recive8Bit(void);
void AD7124_WaitDRDY(void);
void AD7124_DelaySCLK(void);
void AD7124_DelayDATA(void);



void ADS7124_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIO时钟 */
	RCC_AHB1PeriphClockCmd(RCC_SCK | RCC_DIN | RCC_DOUT | RCC_CS | RCC_SYNC, ENABLE);

	/* 配置几个推完输出IO */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		/* 设为输出口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 上下拉电阻不使能 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;	/* IO口最大速度 */

	GPIO_InitStructure.GPIO_Pin = PIN_SCK;
	GPIO_Init(PORT_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DIN;
	GPIO_Init(PORT_DIN, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_CS;
	GPIO_Init(PORT_CS, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = PIN_SYNC;
	GPIO_Init(PORT_SYNC, &GPIO_InitStructure);

	/* 配置GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输入口 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* 设为推挽模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	/* 无需上下拉电阻 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;	/* IO口最大速度 */

	GPIO_InitStructure.GPIO_Pin = PIN_DOUT;
	GPIO_Init(PORT_DOUT, &GPIO_InitStructure);


	CS_1();
	SCK_1();		/* SPI总线空闲时，钟线是低电平 */
	DI_1();	
	SYNC_1;
}

void AD7124_DelaySCLK(void)
{
	uint16_t i;
	for (i = 0; i < 10; i++);
}


void AD7124_DelayDATA(void)
{
	delay_us(10);	/* 最小延迟 6.5uS, 此处取10us */
}



void AD7124_Send8Bit(uint8_t _data)
{
	uint8_t i;

	/* 连续发送多个字节时，需要延迟一下 */
	AD7124_DelaySCLK();
	AD7124_DelaySCLK();

	/*　AD7124 要求 SCL高电平和低电平持续时间最小 200ns  */
	for(i = 0; i < 8; i++)
	{
		if (_data & 0x80)
		{
			DI_1();
		}
		else
		{
			DI_0();
		}
		SCK_0();				
		AD7124_DelaySCLK();		
		SCK_1();			/* <----  AD7124 是在SCK下降沿采样DIN数据, 数据必须维持 50nS */
		
		_data <<= 1;
		AD7124_DelaySCLK();		
	}
}


uint8_t AD7124_Recive8Bit(void)
{
	uint8_t i;
	uint8_t read = 0;

	AD7124_DelaySCLK();
	/*　AD7124 要求 SCL高电平和低电平持续时间最小 200ns  */
	for (i = 0; i < 8; i++)
	{
		SCK_1();
		AD7124_DelaySCLK();
		SCK_0();
		
		read = read<<1;
		if (DO_IS_HIGH())
		{
			read++;
		}		
		AD7124_DelaySCLK();
	}
	return read;
}






