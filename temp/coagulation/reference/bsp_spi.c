/**
 * file name : bsp_spi.c
 * date      : 
 * brief     : 
 *
 *  硬件连接(SPI1)
 *
 *  SPI_SCK   <---> PA5
 *  SPI_MISO  <---> PA6
 *  SPI_MOSI  <---> PA7
 *  SPI_CS1   <---> PC4
 *  SYNC1     <---> PC5
 *
 *  
 */
#include "bsp.h"
#include "temp.h"
u8_t spi_rx_buf[16];
u8_t spi_tx_buf[10];   
u8_t spi4_rx_buf[16];
u8_t spi4_tx_buf[10];

SPI_HandleTypeDef hspi1,hspi4;


/**
 * @brief  : 通过SPI接口发送数据，
 *           并返回接收到的数据
 * @input  :
 * @output :
 * @retval :
 */
static void BSP_SPISend(SPI_TypeDef* SPIx, u16_t dat)//static u16_t BSP_SPISend(SPI_TypeDef* SPIx, u16_t dat)
{
    size_t i=0;

    SPIx->DR = dat;

    // wait for receive
    while( ((SPIx->SR) & 0x01) == 0)
    {
        if( ++i > 100)
        {
            break;
        }
        else
        {
            BSP_DelayUS(10);
        }
        
    }

//    return (u16_t)(SPIx->DR);
}

/**
 * @brief  : 切换AD的采样通道
 * @input  : idx - 通道索引号
 *           dev - 设备索引号（片选号）
 * @output : 
 * @retval : 
 */
void BSP_SwitchADCChannel(u8_t id, u8_t dev)
{
    u16_t tmp=0;
//    id = id & 0x3;
//    id |= 0x4;
//
//    if( dev == 0 )
//    {
//        SPI_CS1_L();
//    }
//    else
//    {
//        SPI_CS2_L();
//    }
//    
//    BSP_DelayUS(10);
//    
//    /*
//     * PGA  = 2 (+/-2.048V)
//     * MODE = 0 (连续转换模式)
//     * DR   = 7 (860SPS，采样间隔约为1.2ms)
//     * TS_MODE = 0
//     * PULL_UP_EN = 1
//     * NOP = 1
//     */
//    tmp = (id<<12) | (0x02<<9) 
//        | (0x07<<5) | (0x01<<3) 
//        | (0x01<<1) | 0x01;
//
//    BSP_SPISend(SPI2, tmp);
//
//    BSP_DelayUS(10);
//
//    if( dev == 0 )
//    {
//        SPI_CS1_H();
//    }
//    else
//    {
//        SPI_CS2_H();
//    }
}
/**
 * @brief  : 读取ADS1118的数据
 * 
 *          电压值与ADC值的转换关系:
 *          V = ADC * 4.096 / 32767
 * 
 *          FSR = 4.096,
 *          由于是补码，所以最大值为0x7FFF
 * 
 * @input  : dev - 设备索引号（片选号）
 * @output : 
 * @retval : 通道的ADC值
 */
u32_t BSP_GetADSData(u8_t dev)
{
    u32_t dat=0;
    u8_t tmp[4],i,j=0;
    u16_t count=1500;
    
    spi_tx_buf[0]=0x01;//写控制寄存器
    spi_tx_buf[1]=0x00;//
    spi_tx_buf[2]=0x85;//
    if(dev<2)
    {
      for(i=0;i<1;i++)
      {    
          SPI_CS1_L();
          HAL_SPI_TransmitReceive(&hspi1,spi_tx_buf,spi_rx_buf,3,1000);
          SPI_CS1_H();
      }
    }
    tmp[1]=0x80;
    while(count>0)
    {
      spi_tx_buf[0]=0x40;//读状态寄存器
      spi_tx_buf[1]=0x00;//
      if(dev<2)
      {
        for(i=0;i<1;i++)
        {   
          SPI_CS1_L();
          HAL_SPI_TransmitReceive(&hspi1,spi_tx_buf,tmp,2,1000);
          SPI_CS1_H();
        }
      }
      if(tmp[1]>=0x80)
      {
        count--;
        if(count==0)
        {
          dat=0x8D34F6;//9.9度
          return dat;
        }
        AOS_TaskDelay(1);
      }
      else
      {
          break;
      } 
    }
    spi_tx_buf[0]=0x42;//读数据寄存器
    spi_tx_buf[1]=0x00;//
    spi_tx_buf[2]=0x00;//
    spi_tx_buf[3]=0x00;//
    if(dev<2)
    {
      for(i=0;i<1;i++)
      {    
        SPI_CS1_L();
        HAL_SPI_TransmitReceive(&hspi1,spi_tx_buf,tmp,4,2000);
        SPI_CS1_H();
      }
    }
    for(i=1;i<4;i++)
    {
      dat<<=8;
      dat|=tmp[i];      
    }  
    return dat;
}


/**
 * @brief  : 准备采集通道ch的温度
 *           这里首先切换到对应的通道
 * @input  : ch - 温控通道索引号
 * @output :
 * @retval :
 */
 
void BSP_PrepareGetTempADC(u8_t ch)
{
  u8_t i,j;  
  
  for(j=0;j<6;j++)
  {
    spi_tx_buf[0]=0x09+j;//写通道寄存器
    spi_tx_buf[1]=0x00;//
    spi_tx_buf[2]=0x11|j<<5;//
    for(i=0;i<2;i++)
    {    
      SPI_CS1_L();
      HAL_SPI_TransmitReceive(&hspi1,spi_tx_buf,spi_rx_buf,3,1000);
      SPI_CS1_H();
    }
  }
  switch(ch)
  {
//  case 0: j=5;   
//    break;
//  case 1: j=6;
//    break;
//  case 2: j=4;
//    break;
//  case 3: j=1;
//    break;
//  case 4: j=2;
//    break;
//  case 5: j=3;
//    break;
//  case 6: j=0;
//    break;
//  case 7: j=7;
//    break;
//  default: j=0;     
  case 0: j=1;   
    break;
  case 1: j=2;
    break;
  case 2: j=3;
    break;
  case 3: j=5;
    break;
  case 4: j=6;
    break;
  case 5: j=4;
    break;
  case 6: j=0;
    break;
  case 7: j=7;
    break;
  default: j=1;
  }
  
  spi_tx_buf[0]=0x09+j;//写通道寄存器
  spi_tx_buf[1]=0x80;//
  spi_tx_buf[2]=0x11|j<<5;//
  for(i=0;i<2;i++)
  {    
    SPI_CS1_L();
    HAL_SPI_TransmitReceive(&hspi1,spi_tx_buf,spi_rx_buf,3,1000);
    SPI_CS1_H();
  }
}

/**
 * @brief  : 获取温控通道ch的ADC值
 *           调用此函数前，必须先调用
 *           BSP_PrepareGetTempADC()切换
 *          通道。
 * @input  : ch - 温控通道索引
 * @output :
 * @retval : ch通道对应的adc值
 */
u32_t BSP_GetTempADC(u8_t ch)
{
    if( ch  < 6 )
    {
        ch = 1;
    }
    else
    {
        ch = 0;
    }

    return BSP_GetADSData(ch);
}

/**
 * @brief  : 切换到PMT采集通道
 * @input  :
 * @output :
 * @retval :
 */
void BSP_PrepareGetPMTADC(void)
{
//    BSP_SwitchADCChannel(0, 0);
}

/**
 * @brief  : 获取PMT通道的ADC值
 * @input  :
 * @output :
 * @retval :
 */
u32_t BSP_GetPMTADC(void)
{
     
//   return BSP_GetADSData(3);
//待添加
}

/**
 * @brief  : 切换到PMT采集通道
 * @input  :
 * @output :
 * @retval :
 */
void BSP_PrepareGetPMTVRef(void)
{
//    BSP_SwitchADCChannel(1, 0);
}

/**
 * @brief  : 获取PMT通道的ADC值
 * @input  :
 * @output :
 * @retval :
 */
u16_t BSP_GetPMTVref(void)
{
    return BSP_GetADSData(0);
}

//u8_t TMC_write_com(u32_t str)
//{
//  u8_t i,j;
//  u32_t ch;
//  ch=str;
//  SPI_CLK_L();
//  ch<<=12;
//  for(i=0;i<20;i++)
//  {
//    if(ch>=0x80000000)
//    {
//      SPI_MOSI_H(); 
//    }
//    else
//    {
//      SPI_MOSI_L();  
//    }
//    SPI_CLK_H();
//    for(j=0;j<200;j++);//AOS_TaskDelay(10);
//    SPI_CLK_L(); 
//    for(j=0;j<200;j++);//AOS_TaskDelay(10);AOS_TaskDelay(10);
//    ch<<=1;
//  }  
//  return 0;  
//}

u8_t TMC_write_chip(u32_t str,u8_t cs)
{
  u8_t err=0;

  spi4_tx_buf[2]=str;
  spi4_tx_buf[1]=str>>8;
  spi4_tx_buf[0]=str>>16;
  
  switch(cs)
  {
  case 0:
    MOTO1_CS_L();
    MOTO2_CS_H();
    L_MOTO_CS_H();
//    TMC_write_com(str);
    err=HAL_SPI_TransmitReceive(&hspi4,spi4_tx_buf,spi4_rx_buf,3,1000);
    err |=0x10;
    MOTO1_CS_H();
    MOTO2_CS_H();
    L_MOTO_CS_H();    
    break;
  case 1:
    MOTO1_CS_H();
    MOTO2_CS_L();
    L_MOTO_CS_H();
//    TMC_write_com(str);
    err=HAL_SPI_TransmitReceive(&hspi4,spi4_tx_buf,spi4_rx_buf,3,1000);
    err |=0x20;
    MOTO1_CS_H();
    MOTO2_CS_H();
    L_MOTO_CS_H();  
    break;
  case 4:
    MOTO1_CS_H();
    MOTO2_CS_H();
    L_MOTO_CS_L();
//    TMC_write_com(str);
    err=HAL_SPI_TransmitReceive(&hspi4,spi4_tx_buf,spi4_rx_buf,3,1000);
    err |=0x30;
    MOTO1_CS_H();
    MOTO2_CS_H();
    L_MOTO_CS_H(); 
    break;
  default: return err;
  }
  if(err&0x0F>0)
  {
    Log("%s-%d: SPI4 moto%d read/write error \r\n",  filename(__FILE__), __LINE__,err>>4);
  }
  return err;
}

u8_t TMC_init(u8_t channel)
{
  if(channel>MT_NUMS)
  {
    return 1;
  }
    TMC_write_chip(CHOPCONF|DBL_16|CHM_S|RNDTF_F|HDEC_16|HEND_7|HSTRT_2|TOFF_1,channel);
    TMC_write_chip(SGCSCONF|SFILT_STAND|0x00<<SFILT_STAND|0x14<<CS_SHIFT,channel);
    TMC_write_chip(DRVCONF|TST_0|SLPH_0|SLPL_0|DISS2G_EN|TS2G_0|SDOFF_EN|VSENSE_160|RDSEL_0,channel);
    TMC_write_chip(SMARTEN|SEIMIN_4|SEDN_8|2<<SEMAX_SHIFT|SEUP_4|2<<SEMIN_SHIFT,channel);
    if(channel==4)
    {
        TMC_write_chip(DRVCTRL| MICRO_STEP_16 | INTPOL ,channel);//设置细分16
    }
    else
    {
       TMC_write_chip(DRVCTRL| MICRO_STEP_32  ,channel);//设置细分32
    }
    
    return 0;
  
}




/**
 * @brief  : SPI接口初始化
 * @input  :
 * @output :
 * @retval :
 */
void BSP_SPIInit(void)
{
  hspi1.Instance=SPI1;          //温度ADC
  hspi1.Init.Mode=SPI_MODE_MASTER;
  hspi1.Init.Direction=SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize=SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity=SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase=SPI_PHASE_2EDGE;
  hspi1.Init.NSS=SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit=SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode=SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial=7;
  HAL_SPI_Init(&hspi1);
  
  hspi4.Instance=SPI4;          //电机控制TMC2660
  hspi4.Init.Mode=SPI_MODE_MASTER;
  hspi4.Init.Direction=SPI_DIRECTION_2LINES;
  hspi4.Init.DataSize=SPI_DATASIZE_8BIT;
  hspi4.Init.CLKPolarity=SPI_POLARITY_LOW;
  hspi4.Init.CLKPhase=SPI_PHASE_1EDGE;
  hspi4.Init.NSS=SPI_NSS_SOFT;
  hspi4.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_64;
  hspi4.Init.FirstBit=SPI_FIRSTBIT_MSB;
  hspi4.Init.TIMode=SPI_TIMODE_DISABLE;
  hspi4.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;
  hspi4.Init.CRCPolynomial=7;
  HAL_SPI_Init(&hspi4);
  
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET); // AD7124 CS
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,GPIO_PIN_SET); // AD7124 SYNC
  
  __HAL_SPI_ENABLE(&hspi1);
  __HAL_SPI_ENABLE(&hspi4);
    
} 
// *  温度通道
// *  SPI_CS1   <---> PC4
// *  SPI_CS2   <---> PB2
// *  SYNC1     <---> PC5
// *  SYNC2     <---> PF11

// *   PMT通道
// *  SPI_CS3   <---> PE0
// *  SYNC3     <---> PE1



void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
     GPIO_InitTypeDef GPIO_InitStruct = {0};
 //  SPI_InitTypeDef  spi_initype={0};
  /* GPIO Ports Clock Enable */ 
  
  if(hspi->Instance==SPI1)
  {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 ;//SCK,MISO,MOSI
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;  
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;  
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
      
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5 ;//CS,SYNC
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);   
    
    __HAL_SPI_ENABLE(&hspi1);
  }
  else if(hspi->Instance==SPI4)
  {    
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_SPI4_CLK_ENABLE(); 
    
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_5|GPIO_PIN_6 ;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;  
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI4;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;  
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    
//    GPIO_InitStruct.Pin = GPIO_PIN_0 ;  // MOTO_LINE_CS
    GPIO_InitStruct.Pin = GPIO_PIN_1 ;  // MOTO_LINE_CS
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_12 ;  // MOTO2_CS、MOTO1_CS
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
    
    __HAL_SPI_ENABLE(&hspi4);
  }
  
   
}

