//=============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//=============================================================================
// Project   :  SHT3x Sample Code (V1.1)
// File      :  i2c_hal.c (V1.1)
// Author    :  RFU
// Date      :  6-Mai-2015
// Controller:  STM32F100RB
// IDE       :  礦ision V5.12.0.0
// Compiler  :  Armcc
// Brief     :  I2C hardware abstraction layer
//=============================================================================

//-- Defines ------------------------------------------------------------------
// I2C IO-Pins                        /* -- adapt the defines for your uC -- */
#include "main.h"

typedef enum {
  ACK = 0,
  NACK = 1,
}etI2cAck;
// I2C acknowledge
typedef enum {
  NO_ERROR = 0x00, // no error
  ACK_ERROR = 0x01, // no acknowledgment error
  CHECKSUM_ERROR = 0x02, // checksum mismatch error
  TIMEOUT_ERROR = 0x04, // timeout error
  PARM_ERROR = 0x80, // parameter out of range error
}etError;

//2     3       4       5              座上序号
//scl   sda     scl     sda             光源板标号
//con2  con1    con4    con3            原理图标号
//PA8   PA11     PC8     PC9           主板mcu标号
#define SDA1_LOW()  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET)//(GPIOB->BSRR = 0x40000000) // set SDA to low
#define SDA1_OPEN() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET)//(GPIOB->BSRR = 0x00004000) // set SDA to open-drain
#define SDA1_READ   (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_11)==GPIO_PIN_SET)
#define SCL1_LOW()  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET)//(GPIOB->BSRR = 0x20000000) // set SCL to low
#define SCL1_OPEN() HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET)//(GPIOB->BSRR = 0x00002000) // set SCL to open-drain

#define SDA2_LOW()  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET)//(GPIOB->BSRR = 0x40000000) // set SDA to low
#define SDA2_OPEN() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET)//(GPIOB->BSRR = 0x00004000) // set SDA to open-drain
#define SDA2_READ   (HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)==GPIO_PIN_SET)
#define SCL2_LOW()  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET)//(GPIOB->BSRR = 0x20000000) // set SCL to low
#define SCL2_OPEN() HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET)//(GPIOB->BSRR = 0x00002000) // set SCL to open-drain



extern void delay_us(uint32_t udelay);


void DelayMicroSeconds(unsigned int nbrOfUs)   /* -- adapt this delay for your uC -- */
{
  unsigned int i;
  for (i = 0; i < nbrOfUs; i++)
  {
    delay_us(1);
  }
}


//-----------------------------------------------------------------------------
void I2c_StartCondition(unsigned char X)
{
  if (X == 0)
  {
    SDA1_OPEN();
    DelayMicroSeconds(1);
    SCL1_OPEN();
    DelayMicroSeconds(1);
    SDA1_LOW();
    DelayMicroSeconds(10);  // hold time start condition (t_HD;STA)
    SCL1_LOW();
    DelayMicroSeconds(10);
  }
  else
  {
    SDA2_OPEN();
    DelayMicroSeconds(1);
    SCL2_OPEN();
    DelayMicroSeconds(1);
    SDA2_LOW();
    DelayMicroSeconds(10);  // hold time start condition (t_HD;STA)
    SCL2_LOW();
    DelayMicroSeconds(10);
  }
}

//-----------------------------------------------------------------------------
void I2c_StopCondition(unsigned char X)
{
  if (X == 0)
  {
    SCL1_LOW();
    DelayMicroSeconds(1);
    SDA1_LOW();
    DelayMicroSeconds(1);
    SCL1_OPEN();
    DelayMicroSeconds(10);  // set-up time stop condition (t_SU;STO)
    SDA1_OPEN();
    DelayMicroSeconds(10);
  }
  else
  {
    SCL2_LOW();
    DelayMicroSeconds(1);
    SDA2_LOW();
    DelayMicroSeconds(1);
    SCL2_OPEN();
    DelayMicroSeconds(10);  // set-up time stop condition (t_SU;STO)
    SDA2_OPEN();
    DelayMicroSeconds(10);
  }
}

//-----------------------------------------------------------------------------
unsigned char I2c_WriteByte(unsigned char X, unsigned char txByte)
{
  unsigned char error = NO_ERROR;
  unsigned char mask;
  if (X == 0)
  {
    for (mask = 0x80; mask > 0; mask >>= 1)// shift bit for masking (8 times)
    {
      if ((mask & txByte) == 0)
      {
        SDA1_LOW(); // masking txByte, write bit to SDA-Line
      }
      else
      {
        SDA1_OPEN();
      }
      DelayMicroSeconds(1);               // data set-up time (t_SU;DAT)
      SCL1_OPEN();                         // generate clock pulse on SCL
      DelayMicroSeconds(5);               // SCL high time (t_HIGH)
      SCL1_LOW();
      DelayMicroSeconds(1);               // data hold time(t_HD;DAT)
    }

    //[A等应答相当于I2C_WaitAck]
    SDA1_OPEN();                           // release SDA-line
    SCL1_OPEN();                           // clk #9 for ack
    DelayMicroSeconds(1);                 // data set-up time (t_SU;DAT)
    if (SDA1_READ)
    {
      error = ACK_ERROR;       // check ack from i2c slave
    }
    SCL1_LOW();
    DelayMicroSeconds(20);                // wait to see byte package on scope
  }
  else
  {
    for (mask = 0x80; mask > 0; mask >>= 1)// shift bit for masking (8 times)
    {
      if ((mask & txByte) == 0)
      {
        SDA2_LOW(); // masking txByte, write bit to SDA-Line
      }
      else
      {
        SDA2_OPEN();
      }
      DelayMicroSeconds(1);               // data set-up time (t_SU;DAT)
      SCL2_OPEN();                         // generate clock pulse on SCL
      DelayMicroSeconds(5);               // SCL high time (t_HIGH)
      SCL2_LOW();
      DelayMicroSeconds(1);               // data hold time(t_HD;DAT)
    }

    //[A等应答相当于I2C_WaitAck]
    SDA2_OPEN();                           // release SDA-line
    SCL2_OPEN();                           // clk #9 for ack
    DelayMicroSeconds(1);                 // data set-up time (t_SU;DAT)
    if (SDA2_READ)
    {
      error = ACK_ERROR;       // check ack from i2c slave
    }
    SCL2_LOW();
    DelayMicroSeconds(20);                // wait to see byte package on scope
  }
  return error;                         // return error code
}


//-----------------------------------------------------------------------------
//dac5571 iic

void I2c_GeneralCallReset(unsigned char id, unsigned char data)
{
  unsigned char h = (data >> 4);
  unsigned char l = (data << 4);
  if (id == 0)
  {
    I2c_StartCondition(0);
    I2c_WriteByte(0, 0x9A);//0x98 0x9A
    I2c_WriteByte(0, h);
    I2c_WriteByte(0, l);
    I2c_StopCondition(0);
  }
  if (id == 1)
  {
    I2c_StartCondition(0);
    I2c_WriteByte(0, 0x98);//0x98 0x9A
    I2c_WriteByte(0, h);
    I2c_WriteByte(0, l);
    I2c_StopCondition(0);
  }
  if (id == 2)
  {
    I2c_StartCondition(1);
    I2c_WriteByte(1, 0x9A);//0x98 0x9A
    I2c_WriteByte(1, h);
    I2c_WriteByte(1, l);
    I2c_StopCondition(1);
  }
  if (id == 3)
  {
    I2c_StartCondition(1);
    I2c_WriteByte(1, 0x98);//0x98 0x9A
    I2c_WriteByte(1, h);
    I2c_WriteByte(1, l);
    I2c_StopCondition(1);
  }

}


