#ifndef __AD7124_H__
#define __AD7124_H__

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include <stdint.h>
#include "AD7124_regs.h"




typedef struct ad7124_device ad7124_device;

/*
 * The structure describes the device and is used with the ad7124 driver.
 * @slave_select_id: The ID of the Slave Select to be passed to the SPI calls.
 * @regs: A reference to the register list of the device that the user must
 *       provide when calling the Setup() function.
 * @userCRC: Whether to do or not a cyclic redundancy check on SPI transfers.
 * @check_ready: When enabled all register read and write calls will first wait
 *               until the device is ready to accept user requests.
 * @spi_rdy_poll_cnt: Number of times the driver should read the Error register
 *                    to check if the device is ready to accept user requests,
 *                    before a timeout error will be issued.
 */
struct ad7124_device {
	int slave_select_id;
	ad7124_st_reg *regs;
	int useCRC;
	int check_ready;
	int spi_rdy_poll_cnt;
};

/******************************************************************************/
/******************* AD7124 Constants *****************************************/
/******************************************************************************/
#define AD7124_CRC8_POLYNOMIAL_REPRESENTATION 0x07 /* x8 + x2 + x + 1 */
#define AD7124_DISABLE_CRC 0
#define AD7124_USE_CRC 1

/******************************************************************************/
/************************ Functions Declarations ******************************/
/******************************************************************************/
/*! Reads the value of the specified register. */
int32_t AD7124_ReadRegister(ad7124_device *device, ad7124_st_reg* pReg);

/*! Writes the value of the specified register. */
int32_t AD7124_WriteRegister(ad7124_device *device, ad7124_st_reg reg);

/*! Reads the value of the specified register without a device state check. */
int32_t AD7124_NoCheckReadRegister(ad7124_device *device, ad7124_st_reg* pReg);

/*! Writes the value of the specified register without a device state check. */
int32_t AD7124_NoCheckWriteRegister(ad7124_device *device, ad7124_st_reg reg);

/*! Resets the device. */
int32_t AD7124_Reset(ad7124_device *device);

/*! Waits until the device can accept read and write user actions. */
int32_t AD7124_WaitForSpiReady(ad7124_device *device, uint32_t timeout);

/*! Waits until a new conversion result is available. */
int32_t AD7124_WaitForConvReady(ad7124_device *device, uint32_t timeout);

/*! Reads the conversion result from the device. */
double AD7124_ReadData(ad7124_device *device);

double AD7124_ReadAverData(ad7124_device *device);

/*! Computes the CRC checksum for a data buffer. */
uint8_t AD7124_ComputeCRC8(uint8_t* pBuf, uint8_t bufSize);

/*! Updates the CRC settings. */
void AD7124_UpdateCRCSetting(ad7124_device *device);

/*! Updates the device SPI interface settings. */
void AD7124_UpdateDevSpiSettings(ad7124_device *device);

int32_t AD7124_GetChannel(ad7124_device *device);

/*! Initializes the AD7124. */
int32_t AD7124_Setup(ad7124_device *device);

#endif /* __AD7124_H__ */
