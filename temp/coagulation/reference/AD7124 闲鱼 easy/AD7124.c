/***************************************************************************//**
*   @file    AD7124.c
*   @brief   AD7124 implementation file.
*   @devices AD7124-4, AD7124-8
*
********************************************************************************/

#include "AD7124.h"
#include "header.h"

 
uint8_t AD_ID_REG;//复位值为0x12或0x14或0x04；
uint32_t Data;
float data_temp,Vol;
float Actual_Voltage[8];
uint8_t DATA_STATUS;
uint8_t AD_Gain=1;
void AD_Delay(u32 time)
{
	u16 i=0;  
   while(time--)
   {
      i=10;  //自己定义
      while(i--) ;    
   }
}
void AD7124_HardInit(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);
		//AD7124
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_Init(GPIOB,&GPIO_InitStructure);
}
void AD7124_SPI_WB(uint8_t Data)
{
	uint8_t i=0;
  uint8_t data;
  data=Data;
  for( i=0;i<8;i++)
  {
    AD7124_SCLK_L;
    if(data&0x80)
    AD7124_DIN_H;
    else
    AD7124_DIN_L;
    AD_Delay(5);
    AD7124_SCLK_H;
    AD_Delay(5);
    data<<=1;
  }
}
uint32_t AD7124_RDATA(void)
{
	uint8_t i=0;
  uint32_t DATA;
  DATA=0;
  for( i=0;i<24;i++)
  {
    DATA<<=1;
    AD7124_SCLK_L;
    AD_Delay(5);
    if(AD7124_DOUT)
    DATA++;
    AD7124_SCLK_H;
    AD_Delay(5);
  }
  return DATA;
}
uint8_t AD7124_SPI_RB(void)
{
	uint8_t i=0;
  uint8_t DATA;
  DATA=0;
  for( i=0;i<8;i++)
  {
    DATA<<=1;
    AD7124_SCLK_L;
    AD_Delay(5);
    if(AD7124_DOUT)
    DATA++;
    AD7124_SCLK_H;
    AD_Delay(5);
  }
//  AD_Delay(5);
  return DATA;
}
//函数:	读取ID
uint16_t Get_AD7124_ID()
{
  uint16_t ID;
  ID=0;
  ID=AD7124_Read_Reg(AD7124_ID_REG,1);
  return ID;
}

 
void AD7124_Reset(void)
{
	uint8_t a=0;
  AD7124_CS_L;
  AD7124_DIN_H;
  for( a=0;a<65;a++)//提供大于64个写操作，复位AD7124
  {
    AD7124_SCLK_L;
    AD_Delay(10);
    AD7124_SCLK_H;
    AD_Delay(10);
  }
  AD7124_CS_H;
}



//函数:	读AD7124寄存器
//变量:	addr:寄存器地址 byte:要写入的字节数
//返回值: reg  Ye
uint32_t AD7124_Read_Reg(uint8_t addr,uint8_t byte)
{
  uint32_t reg;
  AD7124_CS_L;
  AD7124_SPI_WB(0x40| addr);
	switch(byte)
	{
		case 1:
			reg=AD7124_SPI_RB();
			break;
		case 2:
			reg=AD7124_SPI_RB();
			reg<<=8;
			reg|=AD7124_SPI_RB();
			break;
		case 3:
			reg=AD7124_SPI_RB();
			reg<<=16;
			reg|=AD7124_SPI_RB();
			reg<<=8;
			reg|=AD7124_SPI_RB();
			break;
	}
  AD7124_CS_H;
  return reg;
}
//函数:	写AD7124寄存器
//变量:	addr:寄存器地址 byte:要写入的字节数 data 写入值
//返回值:无 Ye
uint16_t AD7124_Write_Reg(uint8_t addr,uint8_t byte,uint32_t data)
{
  uint16_t reg,i;
  AD7124_CS_L;
  AD7124_SPI_WB(addr);
	switch(byte)
	{
		case 1:
			AD7124_SPI_WB(data >> 0);
			break;
		case 2:
			AD7124_SPI_WB(data >> 8);
			AD7124_SPI_WB(data >> 0);
			break;
		case 3:
			AD7124_SPI_WB(data >> 16);
			AD7124_SPI_WB(data >> 8);
			AD7124_SPI_WB(data >> 0);
			break;
	}
  AD7124_CS_H;
  return reg;
}

void AD7124_Set_Gain(uint8_t gain)
{
	uint8_t PGA;
	uint16_t REGtemp;
	AD_Gain = gain;
	switch(gain)
	{
		case 1:
			PGA = 0;
			break;
		case 2:
			PGA = 1;
			break;
		case 4:
			PGA = 2;
			break;
		case 8:
			PGA = 3;
			break;
		case 16:
			PGA = 4;
			break;
		case 32:
			PGA = 5;
			break;
		case 64:
			PGA = 6;
			break;
		case 128:
			PGA = 7;
	}
	REGtemp=AD7124_Read_Reg(AD7124_CFG0_REG,2) & 0xFFF8;
	//写AD7124 配置0寄存器
	AD7124_Write_Reg(AD7124_CFG0_REG,2, REGtemp | AD7124_CFG_REG_PGA(PGA));	
}
void AD7124_SINGLE_INIT()//单通道初始化函数
{
	AD7124_Reset();
	AD_Delay(500);
	//写AD7124 控制寄存器
	 AD7124_Write_Reg(AD7124_ADC_CTRL_REG,2,AD7124_ADC_CTRL_REG_REF_EN | AD7124_ADC_CTRL_REG_POWER_MODE(3) | AD7124_ADC_CTRL_REG_MODE(0) | AD7124_ADC_CTRL_REG_CLK_SEL(0) | AD7124_ADC_CTRL_REG_CS_EN);
	//内部参考打开 | POWER_MODE(x) x=0 低功耗 x=1 中功率 x=2全功率 | CTRL_REG_MODE(x) x=0 连续转换模式 x=1 单次转换模式 其他模式请看手册 | CLK_SEL(x) x=0 内部 x=2 外部
	
#ifdef AD7124_4
//	//写AD7124 IO控制寄存器1
//	AD7124_Write_Reg(AD7124_IO_CTRL1_REG,3,AD7124_IO_CTRL1_REG_IOUT0(0) | AD7124_IO_CTRL1_REG_IOUT_CH0(15));
//	//IOUT0(x) 0-7电流分别是关闭 50 100 250 500 750 1000 1000uA | CH0(x)x=0 AIN0 x=1 AIN1 x=4 AIN2 x=5 AIN3 x=10 AIN4 x=11 AIN5  x=14 AIN6 x=15 AIN7
//		AD_Delay(50000); //修改IO控制寄存器恒流源 需要延迟久一些
//	//写AD7124 IO控制寄存器2
//	AD7124_Write_Reg(AD7124_IO_CTRL2_REG,2,AD7124_IO_CTRL2_REG_GPIO_VBIAS1);
//	//内部偏置电压使能 VBIAS0-7 对应通道0-7
	
	//写AD7124 通道0寄存器
	AD7124_Write_Reg(AD7124_CH0_MAP_REG,2,AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(0) | AD7124_CH_MAP_REG_AINM(1));	
	//使能通道0 | 用配置0 | AINP(x) ADC+选择 0-7对应通道0-7  | AINM(x) ADC-选择 0-7对应通道0-7 其他通道请查看手册
#endif
	
#ifdef AD7124_8
//	 AD7124_Write_Reg(AD7124_IO_CTRL1_REG,2,AD7124_IO_CTRL1_REG_IOUT0(0) | AD7124_IO_CTRL1_REG_IOUT_CH0(15));
//	//IOUT0(x) 0-7电流分别是关闭 50 100 250 500 750 1000 1000uA | CH0(x) 0-15分别是0-15通道
//		AD_Delay(50000);//修改IO控制寄存器恒流源 需要延迟久一些
//	//写AD7124 IO控制寄存器2
//	AD7124_Write_Reg(AD7124_IO_CTRL2_REG,2,AD7124_IO_CTRL2_REG_GPIO_VBIAS1);
//	//内部偏置电压使能 VBIAS0-15 对应通道0-15

	//写AD7124 通道0寄存器
	AD7124_Write_Reg(AD7124_CH0_MAP_REG,2,AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(0) | AD7124_CH_MAP_REG_AINM(1));	
	//使能通道0 | 用配置0 | AINP(x) ADC+选择 0-15对应通道0-15  | AINM(x) ADC-选择 0-15对应通道0-15 其他通道请查看手册
#endif

	//写AD7124 配置0寄存器
	AD7124_Write_Reg(AD7124_CFG0_REG,2,
		AD7124_CFG_REG_BIPOLAR | //双极性
		AD7124_CFG_REG_AIN_BUFP | //打开ADC+缓冲
		AD7124_CFG_REG_AINN_BUFM | //打开ADC-缓冲
		AD7124_CFG_REG_REF_SEL(2) | //REF_SEL(x) x=0 REFIN1(+)/REFIN1(-) x=1 REFIN2(+)/REFIN2(-) x=2 内部基准电压源 x=3 AVDD
		AD7124_CFG_REG_PGA(0));	//REG_PGA(x) 0-7对应增益1(±2.5V) 2(±1.25V) 4(± 625mV ) 8(±312.5 mV ) 16(±156.25 mV) 32(±78.125 mV) 64(±39.06mV) 128(±19.53mV)
	
	//写AD7124 滤波寄存器0
	AD7124_Write_Reg(AD7124_FILT0_REG,3,AD7124_FILT_REG_FILTER(0) | AD7124_FILT_REG_REJ60 | AD7124_FILT_REG_POST_FILTER(0) | AD7124_FILT_REG_FS(384));
	//FILTER(x)x=0 sinc4 x=2 sinc3 x=7 后置滤波器 其他请查看手册 | 打开60 Hz陷波 | POST_FILTER(x) 后置滤波器设置查看手册 | FS(x)具体滤波配置查看网址
	//http://beta-tools.analog.com/virtualeval/#tool_pid=AD7124-4&tab=fbd
	//此网页可以用于详细配置 滤波频率 因为不是整数值
}

void AD7124_MUL_INIT()//多通道初始化函数
{
	AD7124_Reset();
	AD_Delay(500);
	//写AD7124 控制寄存器
	 AD7124_Write_Reg(AD7124_ADC_CTRL_REG,2,AD7124_ADC_CTRL_REG_DATA_STATUS | AD7124_ADC_CTRL_REG_REF_EN | AD7124_ADC_CTRL_REG_POWER_MODE(3) | AD7124_ADC_CTRL_REG_MODE(0) | AD7124_ADC_CTRL_REG_CLK_SEL(0) | AD7124_ADC_CTRL_REG_CS_EN);
	//通道指示 | 内部参考打开 | POWER_MODE(x) x=0 低功耗 x=1 中功率 x=2、3全功率 | CTRL_REG_MODE(x) x=0 连续转换模式 x=1 单次转换模式 其他模式请看手册 | CLK_SEL(x) x=0 内部 x=2 外部
	
#ifdef AD7124_4
//	//写AD7124 IO控制寄存器1
//	AD7124_Write_Reg(AD7124_IO_CTRL1_REG,3,AD7124_IO_CTRL1_REG_IOUT0(0) | AD7124_IO_CTRL1_REG_IOUT_CH0(15));
//	//IOUT0(x) 0-7电流分别是关闭 50 100 250 500 750 1000 1000uA | CH0(x)x=0 AIN0 x=1 AIN1 x=4 AIN2 x=5 AIN3 x=10 AIN4 x=11 AIN5  x=14 AIN6 x=15 AIN7
//		AD_Delay(50000);//修改IO控制寄存器恒流源 需要延迟久一些
//	//写AD7124 IO控制寄存器2
//	AD7124_Write_Reg(AD7124_IO_CTRL2_REG,2,AD7124_IO_CTRL2_REG_GPIO_VBIAS1);
//	//内部偏置电压使能 VBIAS0-7 对应通道0-7
	
	//写AD7124 通道0寄存器
	AD7124_Write_Reg(AD7124_CH0_MAP_REG,2,AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(0) | AD7124_CH_MAP_REG_AINM(1));	
	//使能通道0 | 用配置0 | AINP(x) ADC+选择 0-7对应通道0-7  | AINM(x) ADC-选择 0-7对应通道0-7 其他通道请查看手册
	
	//写AD7124 通道1寄存器
	AD7124_Write_Reg(AD7124_CH1_MAP_REG,2,AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(2) | AD7124_CH_MAP_REG_AINM(3));	
	//使能通道1 | 用配置0 | AINP(x) ADC+选择 0-7对应通道0-7  | AINM(x) ADC-选择 0-7对应通道0-7 其他通道请查看手册
	
	//写AD7124 通道2寄存器
	AD7124_Write_Reg(AD7124_CH2_MAP_REG,2,AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(4) | AD7124_CH_MAP_REG_AINM(5));	
	//使能通道2 | 用配置0 | AINP(x) ADC+选择 0-7对应通道0-7  | AINM(x) ADC-选择 0-7对应通道0-7 其他通道请查看手册
	
	//写AD7124 通道3寄存器
	AD7124_Write_Reg(AD7124_CH3_MAP_REG,2,AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(6) | AD7124_CH_MAP_REG_AINM(7));	
	//使能通道3 | 用配置0 | AINP(x) ADC+选择 0-7对应通道0-7  | AINM(x) ADC-选择 0-7对应通道0-7 其他通道请查看手册
#endif
	
#ifdef AD7124_8
//	 AD7124_Write_Reg(AD7124_IO_CTRL1_REG,2,AD7124_IO_CTRL1_REG_IOUT0(0) | AD7124_IO_CTRL1_REG_IOUT_CH0(15));
//	//IOUT0(x) 0-7电流分别是关闭 50 100 250 500 750 1000 1000uA | CH0(x) 0-15分别是0-15通道
//		AD_Delay(50000);//修改IO控制寄存器恒流源 需要延迟久一些
//	//写AD7124 IO控制寄存器2
//	AD7124_Write_Reg(AD7124_IO_CTRL2_REG,2,AD7124_IO_CTRL2_REG_GPIO_VBIAS1);
//	//内部偏置电压使能 VBIAS0-15 对应通道0-15

	//写AD7124 通道0寄存器
	AD7124_Write_Reg(AD7124_CH0_MAP_REG,2,AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(0) | AD7124_CH_MAP_REG_AINM(1));	
	//使能通道0 | 用配置0 | AINP(x) ADC+选择 0-15对应通道0-15  | AINM(x) ADC-选择 0-15对应通道0-15 其他通道请查看手册
	
	//写AD7124 通道1寄存器
	AD7124_Write_Reg(AD7124_CH1_MAP_REG,2,AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(2) | AD7124_CH_MAP_REG_AINM(3));	
	//使能通道1 | 用配置0 | AINP(x) ADC+选择 0-15对应通道0-15  | AINM(x) ADC-选择 0-15对应通道0-15 其他通道请查看手册
	
	//写AD7124 通道2寄存器
	AD7124_Write_Reg(AD7124_CH2_MAP_REG,2,AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(4) | AD7124_CH_MAP_REG_AINM(5));	
	//使能通道2 | 用配置0 | AINP(x) ADC+选择 0-15对应通道0-15  | AINM(x) ADC-选择 0-15对应通道0-15 其他通道请查看手册
	
	//写AD7124 通道3寄存器
	AD7124_Write_Reg(AD7124_CH3_MAP_REG,2,AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(6) | AD7124_CH_MAP_REG_AINM(7));	
	//使能通道3 | 用配置0 | AINP(x) ADC+选择 0-15对应通道0-15  | AINM(x) ADC-选择 0-15对应通道0-15 其他通道请查看手册
#endif
	//写AD7124 配置0寄存器
	AD7124_Write_Reg(AD7124_CFG0_REG,2,
		AD7124_CFG_REG_BIPOLAR | //双极性
		AD7124_CFG_REG_AIN_BUFP | // 打开ADC+缓冲
		AD7124_CFG_REG_AINN_BUFM | //打开ADC-缓冲
		AD7124_CFG_REG_REF_SEL(2) | //REF_SEL(x) x=0 REFIN1(+)/REFIN1(-) x=1 REFIN2(+)/REFIN2(-) x=2 内部基准电压源 x=3 AVDD
		AD7124_CFG_REG_PGA(0));	//REG_PGA(x) 0-7对应增益1(±2.5V) 2(±1.25V) 4(± 625mV ) 8(±312.5 mV ) 16(±156.25 mV) 32(±78.125 mV) 64(±39.06mV) 128(±19.53mV)
	
	
	//写AD7124 滤波寄存器0
	AD7124_Write_Reg(AD7124_FILT0_REG,3,AD7124_FILT_REG_FILTER(0) | AD7124_FILT_REG_REJ60 | AD7124_FILT_REG_POST_FILTER(0) | AD7124_FILT_REG_FS(384));
	//FILTER(x)x=0 sinc4 x=2 sinc3 x=7 后置滤波器 其他请查看手册 | 打开60 Hz陷波 | POST_FILTER(x) 后置滤波器设置查看手册 | FS(x)具体滤波配置查看网址
	//http://beta-tools.analog.com/virtualeval/#tool_pid=AD7124-4&tab=fbd
	//此网页可以用于详细配置 滤波频率 因为不是整数值
}
//函数:	获取一次电压值mV
//变量:	*value 获取到的电压值 单通道都在value[0]	多通道顺序排列
//返回值: 无  Ye
void Get_Once_Vol(float *value)
{
	float data_temp;
	uint16_t AD_statusReg;
	long tempp;
#ifdef Multi_Channel
	AD7124_CS_L;
	while(AD7124_DOUT);
	AD7124_SPI_WB(0x42);//退出连续读取模式，在为RDY低电平时发出一个伪读取ADC数据寄存器命令(0x42)
	tempp=AD7124_RDATA();
	AD_statusReg=AD7124_SPI_RB() & 0x0F;
	AD7124_CS_H;
	data_temp=tempp;
	value[AD_statusReg]=(data_temp-8388608.0)*Vref/8388608.0/AD_Gain;
#endif
#ifdef Single_Channel
	AD7124_CS_L;
	while(AD7124_DOUT);
	AD7124_SPI_WB(0x42);//退出连续读取模式，在为RDY低电平时发出一个伪读取ADC数据寄存器命令(0x42)
	tempp=AD7124_RDATA();
	AD7124_CS_H;
	data_temp=tempp;
	*value=(data_temp-8388608.0)*Vref/8388608.0/AD_Gain;
#endif
}
/*调用
#ifdef Single_Channel
	AD7124_SINGLE_INIT();	
#endif
#ifdef Multi_Channel
	AD7124_MUL_INIT();	
#endif
	AD_ID_REG=Get_AD7124_ID();//复位值为0x12或0x14或0x04
	AD7124_Set_Gain(128);
	while(1){
		Get_Once_Vol(Actual_Voltage);
	}

*/