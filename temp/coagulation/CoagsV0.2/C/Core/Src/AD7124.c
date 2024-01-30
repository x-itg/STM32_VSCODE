#include "AD7124.h"
uint8_t AD_ID_REG;//复位值为0x12或0x14或0x04；
uint32_t Data;
float data_temp, Vol;
unsigned int Actual_Voltage[8];
uint8_t DATA_STATUS;
uint8_t AD_Gain = 1;
uint32_t bufwrite = 0;
uint32_t bufread = 0;
uint32_t settemp[5];
unsigned char adresetflag = 0;
void ledlm(unsigned char lm)
{
  return;
}
void delay_ns(uint32_t ndelay)
{
  unsigned int i = 0;
  for (i = 0;i < 25;i++)
  {
    ;
  }
}
void delay_us(uint32_t udelay)
{
#if 1
  unsigned int i = 0;
  for (i = 0;i < udelay;i++)
  {
    delay_ns(1000);
  }
#endif
#if 0
  uint32_t startval, delays, wait;

  startval = SysTick->VAL;
  delays = udelay * 168; //sysc / 1000 * udelay;
  if (delays > startval)
  {
    wait = 168000 + startval - delays;
    while (wait < SysTick->VAL);
  }
  else
  {
    wait = startval - delays;
    while (wait < SysTick->VAL);
  }
#endif
}
void AD_Delay(u32 time)
{
  while (time--)
  {
    delay_us(10);
  }
}
void zdelayms(unsigned int nms)
{
  for (;nms > 0;nms--)
  {
    delay_us(1000);

  }
}
void delay_ms(unsigned int nms)
{
  for (;nms > 0;nms--)
  {
    delay_us(1000);

  }
}

void AD7124_SPI_WB(uint8_t Data)
{
  uint8_t i = 0;
  uint8_t data;
  data = Data;
  for (i = 0;i < 8;i++)
  {
    AD7124_SCLK_L;
    if (data & 0x80)
      AD7124_DIN_H;
    else
      AD7124_DIN_L;
    AD_Delay(5);
    AD7124_SCLK_H;
    AD_Delay(5);
    data <<= 1;
  }
}
uint32_t AD7124_RDATA(void)
{
  uint8_t i = 0;
  uint32_t DATA;
  DATA = 0;
  for (i = 0;i < 24;i++)
  {
    DATA <<= 1;
    AD7124_SCLK_L;
    AD_Delay(5);
    if (AD7124_DOUT)
    {
      DATA++;
      ledlm(1);
    }
    else
    {
      ledlm(0);
    }
    AD7124_SCLK_H;
    AD_Delay(5);
  }
  return DATA;
}
uint8_t AD7124_SPI_RB(void)
{
  uint8_t i = 0;
  uint8_t DATA;
  DATA = 0;
  for (i = 0;i < 8;i++)
  {
    DATA <<= 1;
    AD7124_SCLK_L;
    AD_Delay(5);
    if (AD7124_DOUT)
    {
      DATA++;
      ledlm(1);
    }
    else
    {
      ledlm(0);
    }
    AD7124_SCLK_H;
    AD_Delay(5);
  }
  AD_Delay(5);
  return DATA;
}
//函数:	读取ID
uint16_t Get_AD7124_ID()
{
  uint16_t ID;
  ID = 0;
  ID = AD7124_Read_Reg(AD7124_ID_REG, 1);
  return ID;
}


void AD7124_Reset(void)
{
  uint8_t a = 0;
  AD7124_CS_L;
  AD7124_DIN_H;
  for (a = 0;a < 65;a++)//提供大于64个写操作，复位AD7124
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
uint32_t AD7124_Read_Reg(uint8_t addr, uint8_t byte)
{
  uint32_t reg;
  AD7124_CS_L;
  AD7124_SPI_WB(0x40 | addr);
  switch (byte)
  {
  case 1:
    reg = AD7124_SPI_RB();
    break;
  case 2:
    reg = AD7124_SPI_RB();
    reg <<= 8;
    reg |= AD7124_SPI_RB();
    break;
  case 3:
    reg = AD7124_SPI_RB();
    reg <<= 16;
    reg |= AD7124_SPI_RB();
    reg <<= 8;
    reg |= AD7124_SPI_RB();
    break;
  }
  AD7124_CS_H;
  return reg;
}
//函数:	写AD7124寄存器
//变量:	addr:寄存器地址 byte:要写入的字节数 data 写入值
//返回值:无 Ye
void AD7124_Write_Reg(uint8_t addr, uint8_t byte, uint32_t data)
{
  AD7124_CS_L;
  AD7124_SPI_WB(addr);
  switch (byte)
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
}

void AD7124_Set_Gain(uint8_t gain)
{
  uint8_t PGA;
  uint16_t REGtemp;
  AD_Gain = gain;
  switch (gain)
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
  REGtemp = AD7124_Read_Reg(AD7124_CFG0_REG, 2) & 0xFFF8;
  //写AD7124 配置0寄存器
  AD7124_Write_Reg(AD7124_CFG0_REG, 2, REGtemp | AD7124_CFG_REG_PGA(PGA));
}

//写AD7124 IO控制寄存器1
  //AD7124_Write_Reg(AD7124_IO_CTRL1_REG,3,AD7124_IO_CTRL1_REG_IOUT0(0) | AD7124_IO_CTRL1_REG_IOUT_CH0(15));
  //IOUT0(x) 0-7电流分别是关闭 50 100 250 500 750 1000 1000uA | CH0(x)x=0 AIN0 x=1 AIN1 x=4 AIN2 x=5 AIN3 x=10 AIN4 x=11 AIN5  x=14 AIN6 x=15 AIN7
  //    AD_Delay(50000);//修改IO控制寄存器恒流源 需要延迟久一些
  //写AD7124 IO控制寄存器2
  //AD7124_Write_Reg(AD7124_IO_CTRL2_REG,2,AD7124_IO_CTRL2_REG_GPIO_VBIAS1);
  //内部偏置电压使能 VBIAS0-7 对应通道0-7
  //https://www.cnblogs.com/DriveTimeFlywheel/p/14726723.html
  //通道号无物理引脚无关可以任意编程连接
  /*
  AD7124_CH_MAP_REG_AINP 中文手册87页 英文手册也是87页
  AD7124_CH_MAP_REG_AINM
  00000 = AIN0 (default).
  00001 = AIN1.
  00010 = AIN2.
  00011 = AIN3.
  00100 = AIN4.
  00101 = AIN5.
  00110 = AIN6.
  00111 = AIN7.
  01000 to 01111 = reserved.
  10000 = temperature sensor.
  10001 = AVSS.
  10010 = internal reference.
  10011 = DGND.
  10100 = (AVDD ? AVSS)/6+. Use in conjunction with (AVDD ? AVSS)/6? to monitor supply AVDD ? AVSS.
  10101 = (AVDD ? AVSS)/6?. Use in conjunction with (AVDD ? AVSS)/6+ to monitor supply AVDD ? AVSS.
  10110 = (IOVDD ? DGND)/6+. Use in conjunction with (IOVDD ? DGND)/6? to monitor IOVDD ? DGND.
  10111 = (IOVDD ? DGND)/6?. Use in conjunction with (IOVDD ? DGND)/6+ to monitor IOVDD ? DGND.
  11000 = (ALDO ? AVSS)/6+. Use in conjunction with (ALDO ? AVSS)/6? to monitor the analog LDO.
  11001 = (ALDO ? AVSS)/6?. Use in conjunction with (ALDO ? AVSS)/6+ to monitor the analog LDO.
  11010 = (DLDO ? DGND)/6+. Use in conjunction with (DLDO ? DGND)/6? to monitor the digital LDO.
  11011 = (DLDO ? DGND)/6?. Use in conjunction with (DLDO ? DGND)/6+ to monitor the digital LDO.
  11100 = V_20MV_P. Use in conjunction with V_20MV_M to apply a 20 mV p-p signal to the ADC.
  11101 = V_20MV_M. Use in conjunction with V_20MV_P to apply a 20 mV p-p signal to the ADC.
  11110 = reserved.
  11111 = reserved.
  */
void AD7124_MUL_INIT()//多通道初始化函数
{

  AD7124_Reset();
  AD_Delay(500);
  //写AD7124 控制寄存器
  AD7124_Write_Reg(AD7124_ADC_CTRL_REG, Byte2x,
    AD7124_ADC_CTRL_REG_DATA_STATUS | //通道指示 
    AD7124_ADC_CTRL_REG_REF_EN | //内部参考打开
    AD7124_ADC_CTRL_REG_POWER_MODE(3) | //x=0 低功耗 x=1 中功率 x=2、3全功率
    AD7124_ADC_CTRL_REG_MODE(0) | //x=0 连续转换模式 x=1 单次转换模式 其他模式请看手册
    AD7124_ADC_CTRL_REG_CLK_SEL(0) | //CLK_SEL(x) x=0 内部 x=2 外部
    AD7124_ADC_CTRL_REG_CS_EN);


  //写AD7124 通道0寄存器
  //通道0       使能    用配置0   AINP(x) ADC+选择 0-7对应通道0-7  AINM(x) ADC-选择 0-7对应通道0-7 其他通道请查看手册       
  bufwrite = AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(1) | AD7124_CH_MAP_REG_AINM(0x13);
  AD7124_Write_Reg(AD7124_CH0_MAP_REG, Byte2x, bufwrite);
  bufread = AD7124_Read_Reg(AD7124_CH0_MAP_REG, Byte2x);

  bufwrite = AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(2) | AD7124_CH_MAP_REG_AINM(0x13);
  AD7124_Write_Reg(AD7124_CH1_MAP_REG, Byte2x, bufwrite);
  bufread = AD7124_Read_Reg(AD7124_CH1_MAP_REG, Byte2x);

  bufwrite = AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(4) | AD7124_CH_MAP_REG_AINM(0x13);
  AD7124_Write_Reg(AD7124_CH2_MAP_REG, Byte2x, bufwrite);
  bufread = AD7124_Read_Reg(AD7124_CH2_MAP_REG, Byte2x);

  bufwrite = AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(5) | AD7124_CH_MAP_REG_AINM(0x13);
  AD7124_Write_Reg(AD7124_CH3_MAP_REG, Byte2x, bufwrite);
  bufread = AD7124_Read_Reg(AD7124_CH3_MAP_REG, Byte2x);

  bufwrite = AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(6) | AD7124_CH_MAP_REG_AINM(0x13);
  AD7124_Write_Reg(AD7124_CH4_MAP_REG, Byte2x, bufwrite);
  bufread = AD7124_Read_Reg(AD7124_CH4_MAP_REG, Byte2x);

  bufwrite = AD7124_CH_MAP_REG_CH_ENABLE | AD7124_CH_MAP_REG_SETUP(0) | AD7124_CH_MAP_REG_AINP(7) | AD7124_CH_MAP_REG_AINM(0x13);
  AD7124_Write_Reg(AD7124_CH5_MAP_REG, Byte2x, bufwrite);
  bufread = AD7124_Read_Reg(AD7124_CH5_MAP_REG, Byte2x);

  //写AD7124 配置0寄存器
  AD7124_Write_Reg(AD7124_CFG0_REG, Byte2x,
    //AD7124_CFG_REG_BIPOLAR      |        //双极性
    //AD7124_CFG_REG_AIN_BUFP     |        //打开ADC+缓冲
    //AD7124_CFG_REG_AINN_BUFM    |        //打开ADC-缓冲
    AD7124_CFG_REG_REF_SEL(3) |        //0:REFIN1(+)/REFIN1(-) 1:REFIN2(+)/REFIN2(-) 2:内部基准电压源 3:AVDD
    AD7124_CFG_REG_PGA(0));	        //REG_PGA(x) 0-7对应增益1(±2.5V) 2(±1.25V) 4(± 625mV ) 8(±312.5 mV ) 16(±156.25 mV) 32(±78.125 mV) 64(±39.06mV) 128(±19.53mV)

  AD7124_Write_Reg(AD7124_FILT0_REG, Byte3x,
    AD7124_FILT_REG_FILTER(0) |        //FILTER(x)x=0 sinc4 x=2 sinc3 x=7 后置滤波器 其他请查看手册
    AD7124_FILT_REG_REJ60 |        //打开60 Hz陷波
    AD7124_FILT_REG_POST_FILTER(0) |     //POST_FILTER(x) 后置滤波器设置查看手册
    AD7124_FILT_REG_FS(384));            //FS(x)具体滤波配置查看网址http://beta-tools.analog.com/virtualeval/#tool_pid=AD7124-4&tab=fbd

}
//函数:	获取一次电压值mV
//变量:	*value 获取到的电压值 单通道都在value[0]	多通道顺序排列
//返回值: 无  Ye
void Get_Once_Vol(unsigned int* value)
{


  uint16_t AD_statusReg;
  unsigned int adCnt = 0;
  long tempp;
  AD7124_CS_L;
  adCnt = 0;
  while (AD7124_DOUT)
  {
    AD_Delay(1);
    adCnt++;
    if (adCnt > 1000)//1000*10us=10ms
    {
      break;
    }
  }
  adCnt = 0;
  AD7124_SPI_WB(0x42);
  tempp = AD7124_RDATA();
  AD_statusReg = AD7124_SPI_RB() & 0x0F;
  AD7124_CS_H;
  value[AD_statusReg] = tempp;

}


//因为motor配置代码比较少所以和这个ad7124写在一起----------------------------------------------------------

unsigned int spisend(unsigned int idat)//写入
{
  unsigned char i, n;
  unsigned int m, odat, temp;
  m = idat;
  setcs_h;
  setck_h;
  delay_us(1);
  setcs_l;
  delay_us(1);
  for (i = 20;i > 0;i--)
  {
    m = idat & 0x80000;
    n = m >> 19;

    setck_l;   // 下降沿放数据
    if ((n & 0x01) == 1)
    {
      setmo_h;
    }
    else
    {
      setmo_l;
    }
    delay_us(1);

    setck_h;//时钟上升沿 读取数据  
    temp = readmi;
    idat <<= 1;//右移一位
    if (temp != GPIO_PIN_RESET)//不是低电平就是1
    {
      odat |= 0x01;
    }
    odat <<= 1;//右移一位
    delay_us(1);
  }

  setcs_h;//不片选
  setck_h;
  delay_us(1);
  return(odat);
}

//寄存器
#define		REG_DRVCTRL     0X00000000
#define		REG_CHOPCONF    0X00080000
#define		REG_SMARTEN     0X000A0000
#define		REG_SGCSCONF    0X000C0000
#define		REG_DRVCONF     0X000E0000//1110 000 0000 0000
#define		SCG_DEFAULT     0x10000
//reg value
#define		MICROSTEP_256   0X00
#define		MICROSTEP_128   0X01
#define		MICROSTEP_64    0X02
#define		MICROSTEP_32    0X03
#define		MICROSTEP_16    0X04
#define		MICROSTEP_8     0X05
#define		MICROSTEP_4     0X06
#define		MICROSTEP_2     0X07
#define		MICROSTEP_1     0X08
void init_tmc260(void)
{
#if 1 
  spisend(REG_DRVCONF | 0X0010);                //0X0010->305mV 0X0050->165mV 与电机电流相关
  spisend(REG_DRVCTRL | MICROSTEP_2);                      //0x00204
  spisend(0x901b4);                             //0x901b4 0x94557
  spisend(0xa0202);                             //0xa0202->1/2CS 0xa8202->1/4CS 
  //spisend(REG_SGCSCONF | SCG_DEFAULT | 0x0F);   //后5位为电流大小
  spisend(REG_SGCSCONF | SCG_DEFAULT | 0x01);   //后5位为电流大小0x03
#endif
}


