/***************************************************************************//**

*   @file    AD7124.c

*   @brief   AD7124 implementation file.

*   @devices AD7124-4, AD7124-8

*

********************************************************************************

* Copyright 2016(c) Avnet.
*******************************************************************************/
/******************************************************************************/

/***************************** Include Files **********************************/

/******************************************************************************/
#include "communication.h"

#include "AD7124.h"

#include "usart.h"

/* Error codes */

#define INVALID_VAL -1 /* Invalid argument */

#define COMM_ERR    -2 /* Communication error on receive */

#define TIMEOUT     -3 /* A timeout has occured */


ad7124_st_reg ad7124_regs[] =

{

	{0x00, 0x00,   1, 2}, /* AD7124_Status */
  //全功率
	{0x01, 0x00c0, 2, 1}, /* AD7124_ADC_Control */

	{0x02, 0x0000, 3, 2}, /* AD7124_Data */

	{0x03, 0x00, 3, 1}, /* AD7124_IOCon1 */

	{0x04, 0x00, 2, 1}, /* AD7124_IOCon2 */

	{0x05, 0x02,   1, 2}, /* AD7124_ID */

	{0x06, 0x0000, 3, 2}, /* AD7124_Error */

	{0x07, 0x0044, 3, 1}, /* AD7124_Error_En */

	{0x08, 0x00,   1, 2}, /* AD7124_Mclk_Count */
	//使能通道寄存器0,配置寄存器0,AIN0->AINP,AIN1->AINM
	{0x09, 0x8001, 2, 1}, /* AD7124_Channel_0 */ //0-1
	{0x0A, 0x8043, 2, 1}, /* AD7124_Channel_1 */ //2-3
	{0x0B, 0x8085, 2, 1}, /* AD7124_Channel_2 */ //4-5
	{0x0C, 0x80C7, 2, 1}, /* AD7124_Channel_3 */ //6-7
	{0x0D, 0x8109, 2, 1}, /* AD7124_Channel_4 */ //8-9
	{0x0E, 0x814B, 2, 1}, /* AD7124_Channel_5 */ //10-11 
	{0x0F, 0x018D, 2, 1}, /* AD7124_Channel_6 */ //12-13
	{0x10, 0x01CF, 2, 1}, /* AD7124_Channel_7 */ //14-15
	{0x11, 0x0001, 2, 1}, /* AD7124_Channel_8 */
	{0x12, 0x0001, 2, 1}, /* AD7124_Channel_9 */
	{0x13, 0x0001, 2, 1}, /* AD7124_Channel_10 */
	{0x14, 0x0001, 2, 1}, /* AD7124_Channel_11 */
	{0x15, 0x0001, 2, 1}, /* AD7124_Channel_12 */
	{0x16, 0x0001, 2, 1}, /* AD7124_Channel_13 */
	{0x17, 0x0001, 2, 1}, /* AD7124_Channel_14 */
	{0x18, 0x0001, 2, 1}, /* AD7124_Channel_15 */
	{0x19, 0x09e2, 2, 1}, /* AD7124_Config_0 */     //开启缓冲器 使用ref1 增益4
	{0x1A, 0x09e0, 2, 1}, /* AD7124_Config_1 */
	{0x1B, 0x09e0, 2, 1}, /* AD7124_Config_2 */
	{0x1C, 0x09e0, 2, 1}, /* AD7124_Config_3 */
	{0x1D, 0x09e0, 2, 1}, /* AD7124_Config_4 */
	{0x1E, 0x09e0, 2, 1}, /* AD7124_Config_5 */
	{0x1F, 0x09e0, 2, 1}, /* AD7124_Config_6 */
	{0x20, 0x09e0, 2, 1}, /* AD7124_Config_7 */
	{0x21, 0x160180, 3, 1}, /* AD7124_Filter_0 */
	{0x22, 0x160180, 3, 1}, /* AD7124_Filter_1 */
	{0x23, 0x160180, 3, 1}, /* AD7124_Filter_2 */
	{0x24, 0x160180, 3, 1}, /* AD7124_Filter_3 */
	{0x25, 0x160180, 3, 1}, /* AD7124_Filter_4 */
	{0x26, 0x160180, 3, 1}, /* AD7124_Filter_5 */
	{0x27, 0x160180, 3, 1}, /* AD7124_Filter_6 */
	{0x28, 0x160180, 3, 1}, /* AD7124_Filter_7 */
	{0x29, 0x800000, 3, 1}, /* AD7124_Offset_0 */
	{0x2A, 0x800000, 3, 1}, /* AD7124_Offset_1 */
	{0x2B, 0x800000, 3, 1}, /* AD7124_Offset_2 */
	{0x2C, 0x800000, 3, 1}, /* AD7124_Offset_3 */
	{0x2D, 0x800000, 3, 1}, /* AD7124_Offset_4 */
	{0x2E, 0x800000, 3, 1}, /* AD7124_Offset_5 */
	{0x2F, 0x800000, 3, 1}, /* AD7124_Offset_6 */
	{0x30, 0x800000, 3, 1}, /* AD7124_Offset_7 */
	{0x31, 0x500000, 3, 1}, /* AD7124_Gain_0 */
	{0x32, 0x500000, 3, 1}, /* AD7124_Gain_1 */
	{0x33, 0x500000, 3, 1}, /* AD7124_Gain_2 */
	{0x34, 0x500000, 3, 1}, /* AD7124_Gain_3 */
	{0x35, 0x500000, 3, 1}, /* AD7124_Gain_4 */
	{0x36, 0x500000, 3, 1}, /* AD7124_Gain_5 */
	{0x37, 0x500000, 3, 1}, /* AD7124_Gain_6 */
	{0x38, 0x500000, 3, 1}, /* AD7124_Gain_7 */

};

ad7124_st_reg ad7124_regs1[] =

{

	{0x00, 0x00,   1, 2}, /* AD7124_Status */

	//使能REF_EN
	{0x01, 0x0000, 2, 1}, /* AD7124_ADC_Control */

	{0x02, 0x0000, 3, 2}, /* AD7124_Data */
	//IOUT0 -> AIN10, IOUT1 -> AIN11, IOUT0 -> 500uA, IOUT1 -> 500uA
	{0x03, 0x24ab, 3, 1}, /* AD7124_IOCon1 */
	//使能VBIAS0
	{0x04, 0x0001, 2, 1}, /* AD7124_IOCon2 */

	{0x05, 0x02,   1, 2}, /* AD7124_ID */

	{0x06, 0x0000, 3, 2}, /* AD7124_Error */

	{0x07, 0x0044, 3, 1}, /* AD7124_Error_En */

	{0x08, 0x00,   1, 2}, /* AD7124_Mclk_Count */
	//使能通道寄存器0,配置寄存器0,AIN0->AINP,AIN1->AINM
	{0x09, 0x8001, 2, 1}, /* AD7124_Channel_0 */
//	{0x09, 0x818d, 2, 1}, /* AD7124_Channel_0 */
	//使能通道寄存器1,配置寄存器1,AIN12->AINP,AIN13->AINM
	{0x0A, 0x918d, 2, 1}, /* AD7124_Channel_1 */
	//{0x0A, 0x8001, 2, 1}, /* AD7124_Channel_1 */

	{0x0B, 0x0001, 2, 1}, /* AD7124_Channel_2 */

	{0x0C, 0x0001, 2, 1}, /* AD7124_Channel_3 */

	{0x0D, 0x0001, 2, 1}, /* AD7124_Channel_4 */

	{0x0E, 0x0001, 2, 1}, /* AD7124_Channel_5 */

	{0x0F, 0x0001, 2, 1}, /* AD7124_Channel_6 */

	{0x10, 0x0001, 2, 1}, /* AD7124_Channel_7 */

	{0x11, 0x0001, 2, 1}, /* AD7124_Channel_8 */

	{0x12, 0x0001, 2, 1}, /* AD7124_Channel_9 */

	{0x13, 0x0001, 2, 1}, /* AD7124_Channel_10 */

	{0x14, 0x0001, 2, 1}, /* AD7124_Channel_11 */

	{0x15, 0x0001, 2, 1}, /* AD7124_Channel_12 */

	{0x16, 0x0001, 2, 1}, /* AD7124_Channel_13 */

	{0x17, 0x0001, 2, 1}, /* AD7124_Channel_14 */

	{0x18, 0x0001, 2, 1}, /* AD7124_Channel_15 */
	//双极性,REFBUF,AINBUF ON,REF_SEL=REFIN2,PGA=32
	{0x19, 0x09e1, 2, 1}, /* AD7124_Config_0 */
	//单极性,REFBUF,AINBUF ON,REF_SEL=REFIN2,PGA=16
	//{0x1A, 0x09e5, 2, 1}, /* AD7124_Config_1 */
	{0x1A, 0x01ec, 2, 1}, /* AD7124_Config_2 */

	{0x1B, 0x0860, 2, 1}, /* AD7124_Config_2 */

	{0x1C, 0x0860, 2, 1}, /* AD7124_Config_3 */

	{0x1D, 0x0860, 2, 1}, /* AD7124_Config_4 */

	{0x1E, 0x0860, 2, 1}, /* AD7124_Config_5 */

	{0x1F, 0x0860, 2, 1}, /* AD7124_Config_6 */

	{0x20, 0x0860, 2, 1}, /* AD7124_Config_7 */

	{0x21, 0x060180, 3, 1}, /* AD7124_Filter_0 */

	{0x22, 0x060180, 3, 1}, /* AD7124_Filter_1 */

	{0x23, 0x060180, 3, 1}, /* AD7124_Filter_2 */

	{0x24, 0x060180, 3, 1}, /* AD7124_Filter_3 */

	{0x25, 0x060180, 3, 1}, /* AD7124_Filter_4 */

	{0x26, 0x060180, 3, 1}, /* AD7124_Filter_5 */

	{0x27, 0x060180, 3, 1}, /* AD7124_Filter_6 */

	{0x28, 0x060180, 3, 1}, /* AD7124_Filter_7 */

	{0x29, 0x800000, 3, 1}, /* AD7124_Offset_0 */

	{0x2A, 0x800000, 3, 1}, /* AD7124_Offset_1 */

	{0x2B, 0x800000, 3, 1}, /* AD7124_Offset_2 */

	{0x2C, 0x800000, 3, 1}, /* AD7124_Offset_3 */

	{0x2D, 0x800000, 3, 1}, /* AD7124_Offset_4 */

	{0x2E, 0x800000, 3, 1}, /* AD7124_Offset_5 */

	{0x2F, 0x800000, 3, 1}, /* AD7124_Offset_6 */

	{0x30, 0x800000, 3, 1}, /* AD7124_Offset_7 */

	{0x31, 0x500000, 3, 1}, /* AD7124_Gain_0 */

	{0x32, 0x500000, 3, 1}, /* AD7124_Gain_1 */

	{0x33, 0x500000, 3, 1}, /* AD7124_Gain_2 */

	{0x34, 0x500000, 3, 1}, /* AD7124_Gain_3 */

	{0x35, 0x500000, 3, 1}, /* AD7124_Gain_4 */

	{0x36, 0x500000, 3, 1}, /* AD7124_Gain_5 */

	{0x37, 0x500000, 3, 1}, /* AD7124_Gain_6 */

	{0x38, 0x500000, 3, 1}, /* AD7124_Gain_7 */

};


/*
************************************************************
	函数名		AD7124_NoCheckReadRegister
	功能		读AD7124读寄存器
	输入参数	*device,pReg
	输出参数	接收数据
	调用		
	变量		
************************************************************
*/

int32_t AD7124_NoCheckReadRegister(ad7124_device *device, ad7124_st_reg* pReg)

{
	int32_t ret       = 0;
	uint8_t buffer[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t i         = 0;
	uint8_t check8    = 0;
	uint8_t msgBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	if(!device || !pReg)
		return INVALID_VAL;
	/* Build the Command word */
	buffer[0] = AD7124_COMM_REG_WEN | AD7124_COMM_REG_RD |
	  		AD7124_COMM_REG_RA(pReg->addr);
	/* Read data from the device */
	ret = AD7124_SPI_Read(device->slave_select_id,
			buffer,
			((device->useCRC != AD7124_DISABLE_CRC) ? pReg->size + 1
							: pReg->size) + 1);
	if(ret < 0)
		return ret;
	/* Check the CRC */
	if(device->useCRC == AD7124_USE_CRC)
	{
		msgBuf[0] = AD7124_COMM_REG_WEN | AD7124_COMM_REG_RD | AD7124_COMM_REG_RA(pReg->addr);
		for(i = 1; i < pReg->size + 2; ++i)
		{
			msgBuf[i] = buffer[i];
		}
		check8 = AD7124_ComputeCRC8(msgBuf, pReg->size + 2);
	}
	if(check8 != 0)
	{
		/* ReadRegister checksum failed. */
		return COMM_ERR;
	}
	/* Build the result */
	pReg->value = 0;
	for(i = 1; i < pReg->size + 1; i++)
	{
		pReg->value <<= 8;
		pReg->value += buffer[i];
	}
	return ret;
}
/*
************************************************************
	函数名		AD7124_NoCheckWriteRegister
	功能		读AD7124写寄存器
	输入参数	*device,pReg
	输出参数	接收数据
	调用		
	变量		
************************************************************
*/

int32_t AD7124_NoCheckWriteRegister(ad7124_device *device, ad7124_st_reg reg)

{
	int32_t ret      = 0;
	int32_t regValue = 0;
	uint8_t wrBuf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t i        = 0;
	uint8_t crc8     = 0;
	if(!device)
		return INVALID_VAL;
	/* Build the Command word */
	wrBuf[0] = AD7124_COMM_REG_WEN | AD7124_COMM_REG_WR | AD7124_COMM_REG_RA(reg.addr);
	/* Fill the write buffer */
	regValue = reg.value;
	for(i = 0; i < reg.size; i++)
	{
		wrBuf[reg.size - i] = regValue & 0xFF;
		regValue >>= 8;
	}
	/* Compute the CRC */
	if(device->useCRC != AD7124_DISABLE_CRC)
	{	
		crc8 = AD7124_ComputeCRC8(wrBuf, reg.size + 1);
		wrBuf[reg.size + 1] = crc8;
	}
	/* Write data to the device */
	ret = AD7124_SPI_Write(device->slave_select_id,
			wrBuf,
			(device->useCRC != AD7124_DISABLE_CRC) ? reg.size + 2
							: reg.size + 1);
	return ret;
}
/*
************************************************************
	函数名		AD7124_ReadRegister
	功能		AD7124读寄存器
	输入参数	
	输出参数	
	调用		
	变量		
************************************************************
*/

int32_t AD7124_ReadRegister(ad7124_device *device, ad7124_st_reg* pReg)
{

	int32_t ret;
	if (pReg->addr != AD7124_ERR_REG && device->check_ready)
	{
		ret = AD7124_WaitForSpiReady(device, device->spi_rdy_poll_cnt);
		if (ret < 0)
			return ret;
	}
	ret = AD7124_NoCheckReadRegister(device, pReg);
	return ret;
}
/*
************************************************************
	函数名		AD7124_WriteRegister
	功能		AD7124写寄存器
	输入参数	
	输出参数	
	调用		
	变量		
************************************************************
*/

int32_t AD7124_WriteRegister(ad7124_device *device, ad7124_st_reg pReg)

{
	int32_t ret;
	if (device->check_ready)
	{  
		ret = AD7124_WaitForSpiReady(device, device->spi_rdy_poll_cnt);
		if (ret < 0)
			return ret;
	}
	ret = AD7124_NoCheckWriteRegister(device, pReg);
	return ret;
}

/*
************************************************************
	函数名		AD7124_Reset
	功能		AD7124复位
	输入参数	
	输出参数	
	调用		
	变量		
************************************************************
*/

int32_t AD7124_Reset(ad7124_device *device)

{
	int32_t ret = 0;
	uint8_t wrBuf[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	if(!device)
		return INVALID_VAL;
	ret = AD7124_SPI_Write(device->slave_select_id, wrBuf, 8);
	return ret;
}

/*************************************************************
	函数名		AD7124_WaitForSpiReady
	功能		AD7124复位
	输入参数	
	输出参数	
	调用		
	变量		
*************************************************************/
int32_t AD7124_WaitForSpiReady(ad7124_device *device, uint32_t timeout)
{
	ad7124_st_reg *regs;
	int32_t ret;
	int8_t ready = 0;
	if(!device)
		return INVALID_VAL;
	regs = device->regs;
	while(!ready && --timeout)
	{
		/* Read the value of the Error Register */
		ret = AD7124_ReadRegister(device, &regs[AD7124_Error]);
		if(ret < 0)
			return ret;
		/* Check the SPI IGNORE Error bit in the Error Register */

		ready = (regs[AD7124_Error].value & AD7124_ERR_REG_SPI_IGNORE_ERR) == 0;

	}
	return timeout ? 0 : TIMEOUT;
}
/*************************************************************
	函数名		AD7124_WaitForConvReady
	功能		查询状态寄存器RDY是否准备好
	输入参数	
	输出参数	
	调用		
	变量		
*************************************************************/

int32_t AD7124_WaitForConvReady(ad7124_device *device, uint32_t timeout)
{
	ad7124_st_reg *regs;
	int32_t ret;
	int8_t ready = 0;
	if(!device)
		return INVALID_VAL;
	regs = device->regs;
	while(!ready && --timeout)
	{
		/* Read the value of the Status Register */
		ret = AD7124_ReadRegister(device, &regs[AD7124_Status]);
		if(ret < 0)
			return ret;
		/* Check the RDY bit in the Status Register */
		ready = (regs[AD7124_Status].value & AD7124_STATUS_REG_RDY) == 0;
	}
	return timeout ? 0 : TIMEOUT;
}

/*
************************************************************
	函数名		AD7124_ReadData
	功能		读AD7124数据寄存器
	输入参数	
	输出参数	
	调用		
	变量		
*************************************************************/

int32_t AD7124_ReadData(ad7124_device *device, int32_t* pData)

{
	ad7124_st_reg *regs;
	int32_t ret;
	if(!device)
		return INVALID_VAL;
	regs = device->regs;
	/* Read the value of the Data Register */
	ret = AD7124_ReadRegister(device, &regs[AD7124_Data]);
	/* Get the read result */
	*pData = regs[AD7124_Data].value;
	return ret;
}
/*
************************************************************
	函数名		AD7124_ComputeCRC8
	功能		AD7124计算CRC8
	输入参数	
	输出参数	
	调用		
	变量		
*************************************************************/
uint8_t AD7124_ComputeCRC8(uint8_t * pBuf, uint8_t bufSize)

{
	uint8_t i   = 0;
	uint8_t crc = 0;
	while(bufSize)
	{
		for(i = 0x80; i != 0; i >>= 1)
		{
			if(((crc & 0x80) != 0) != ((*pBuf & i) != 0)) /* MSB of CRC register XOR input Bit from Data */
			{
				crc <<= 1;
				crc ^= AD7124_CRC8_POLYNOMIAL_REPRESENTATION;
			}
			else
			{
				crc <<= 1;
			}
		}
		pBuf++;
		bufSize--;
	}
	return crc;
}

/*
************************************************************
	函数名		AD7124_UpdateCRCSetting
	功能		根据SPI_CRC_ERR_EN更新CRC设置
	输入参数	
	输出参数	
	调用		
	变量		
************************************************************
*/
void AD7124_UpdateCRCSetting(ad7124_device *device)

{
	ad7124_st_reg *regs;
	if(!device)
		return;
	regs = device->regs;
	/* Get CRC State. */
	if (regs[AD7124_Error_En].value & AD7124_ERREN_REG_SPI_CRC_ERR_EN)
	{
		device->useCRC = AD7124_USE_CRC;
	}
	else
	{
		device->useCRC = AD7124_DISABLE_CRC;
	}
}

/*************************************************************
	函数名		AD7124_UpdateDevSpiSettings
	功能		根据SPI_IGNORE_ERR_EN状态更新设备check_ready状态
	输入参数	
	输出参数	
	调用		
	变量		
*************************************************************/

void AD7124_UpdateDevSpiSettings(ad7124_device *device)
{
	ad7124_st_reg *regs;
	if(!device)
		return;
	regs = device->regs;
	if (regs[AD7124_Error_En].value & AD7124_ERREN_REG_SPI_IGNORE_ERR_EN)
	{
		device->check_ready = 1;
	}
	else
	{
		device->check_ready = 0;
	}
}

/*************************************************************
	函数名		AD7124_Setup
	功能		初始化AD7124
	输入参数	
	输出参数	
	调用		
	变量		
*************************************************************/
int32_t AD7124_Setup(ad7124_device *device, int slave_select,ad7124_st_reg *regs)
{
	int32_t ret;
	enum ad7124_registers regNr;
	if(!device || !regs)
		return INVALID_VAL;
	device->regs = regs;
	device->slave_select_id = 0;
	device->spi_rdy_poll_cnt = 25000;
	/* Initialize the SPI communication. */
	ret = AD7124_SPI_Init(0, 1000000, 1, 0);
	if (ret < 0)
		return ret;
	/*  Reset the device interface.*/
	ret = AD7124_Reset(device);
	if (ret < 0)
		return ret;
	/* Update the device structure with power-on/reset settings */
	device->check_ready = 1;
	
	//读ID寄存器
	ret = AD7124_ReadRegister(device, &regs[AD7124_ID]);
	
	/* Initialize registers AD7124_ADC_Control through AD7124_Filter_7. */
	for(regNr = AD7124_Status; (regNr < AD7124_Offset_0) && !(ret < 0);regNr++)
	{
		if (regs[regNr].rw == AD7124_RW)
		{
			ret = AD7124_WriteRegister(device, regs[regNr]);
			if (ret < 0)
		  		break;
		}
		/* Get CRC State and device SPI interface settings */
		if (regNr == AD7124_Error_En)
		{
			AD7124_UpdateCRCSetting(device);
			AD7124_UpdateDevSpiSettings(device);
		}
	}
	//检查是否写成功
	for(regNr = AD7124_Status; (regNr < AD7124_REG_NO) && !(ret < 0);regNr++)
	{
		ret = AD7124_ReadRegister(device, &regs[regNr]);
		if (ret < 0)
			break;
	}
	
	return ret;
}


float changeV(int code)
{
	float vo=0;
	vo = 2500.0*(code - 0x800000)/(4.0*0x800000);
	return vo;
}

void test_adc(ad7124_device *device)
{
	 uint8_t u8Ch = 0;
   int32_t s32AdCode[6];
	 if( AD7124_WaitForConvReady(device,100) >= 0 )
	  {
		// u8Ch = AD7124_STATUS_REG_CH_ACTIVE(ad7124_regs[AD7124_Status].value);
		 u8Ch = (ad7124_regs[AD7124_Status].value); 
		 AD7124_ReadData(device, &s32AdCode[u8Ch]); 
		  switch(u8Ch)
		  {
			  case 0:
					printf("STATUS_REG_CH_ACTIVE== %x\r\n",u8Ch);
			    printf("Thermocouple Temperature: %X, %04f\r\n ",s32AdCode[u8Ch],changeV(s32AdCode[u8Ch]) ); 
				 
			  break;
			  case 1:
				 printf("STATUS_REG_CH_ACTIVE== %x\r\n",u8Ch);
			     printf("Thermocouple Temperature: %X, %04f\r\n ",s32AdCode[u8Ch],changeV(s32AdCode[u8Ch]) );  
			  break;
				case 2:
					printf("STATUS_REG_CH_ACTIVE== %x\r\n",u8Ch);
			     printf("Thermocouple Temperature: %X, %04f\r\n ",s32AdCode[u8Ch],changeV(s32AdCode[u8Ch]) );			 
			  break;
			  case 3:
				 printf("STATUS_REG_CH_ACTIVE== %x\r\n",u8Ch);
			     printf("Thermocouple Temperature: %X, %04f\r\n ",s32AdCode[u8Ch],changeV(s32AdCode[u8Ch]) ); 
			  break;
				case 4:
					printf("STATUS_REG_CH_ACTIVE== %x\r\n",u8Ch);
			     printf("Thermocouple Temperature: %X, %04f\r\n ",s32AdCode[u8Ch],changeV(s32AdCode[u8Ch]) ); 
	
				 
			  break;
			  case 5:
				 printf("STATUS_REG_CH_ACTIVE== %x\r\n",u8Ch);
			    printf("Thermocouple Temperature: %X, %04f\r\n ",s32AdCode[u8Ch],changeV(s32AdCode[u8Ch]) ); 
			  break;
				
				default:printf("state is error!! \r\n" ); 
		  }
		  
	  }
	
}

