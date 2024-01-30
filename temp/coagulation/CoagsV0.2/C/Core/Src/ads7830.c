#include "main.h"
#include "adc.h"

I2cHandle_t adci2c1;
I2cHandle_t adci2c2;



void delay_ns(uint32_t ndelay)
{
    unsigned int i = 0;
    for (i = 0;i < 1;i++)
    {
        ;
    }
}

void delay_us(uint32_t udelay)
{

    unsigned int i = 0;
    for (i = 0;i < udelay;i++)
    {
        delay_ns(1000);
    }

}

void SCL1_LOW(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
}
void SCL1_OPEN(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
}
void SDA1_LOW(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
}
void SDA1_OPEN(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
}
unsigned char SDA1_READ(void) {
    return (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_SET);
}

void SCL2_LOW(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
}
void SCL2_OPEN(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
}
void SDA2_LOW(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
}
void SDA2_OPEN(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
}
unsigned char SDA2_READ(void) {
    return (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_11) == GPIO_PIN_SET);
}


void initi2chandle(void)
{
    //初始化I2C1
    adci2c1.SDA_H = SDA1_OPEN;
    adci2c1.SDA_L = SDA1_LOW;
    adci2c1.SDA_Read = SDA1_READ;
    adci2c1.SCL_H = SCL1_OPEN;
    adci2c1.SCL_L = SCL1_LOW;
    //初始化I2C2
    adci2c2.SDA_H = SDA2_OPEN;
    adci2c2.SDA_L = SDA2_LOW;
    adci2c2.SDA_Read = SDA2_READ;
    adci2c2.SCL_H = SCL2_OPEN;
    adci2c2.SCL_L = SCL2_LOW;

}

//==================================================================================================
//| 函数名称 | Bsp_I2cStart
//Bsp_I2cStart(&adci2c1);
//Bsp_I2cStart(&adci2c2);
unsigned char Bsp_I2cStart(I2cHandle_t* pst_Dev)
{
    (pst_Dev->SDA_H)();
    (pst_Dev->SCL_H)();

    delay_us(5);
    if ((pst_Dev->SDA_Read)() == 0)
        return 0; // SDA线为低电平则总线忙,退出

    (pst_Dev->SDA_L)();
    delay_us(5);
    if ((pst_Dev->SDA_Read)() != 0)
        return 0; // SDA线为高电平则总线出错,退出

    (pst_Dev->SCL_L)();
    delay_us(5);
    return 1;
}

//==================================================================================================
//| 函数名称 | Bsp_I2cStop
//Bsp_I2cStop(&adci2c1);
//Bsp_I2cStop(&adci2c2);
void Bsp_I2cStop(I2cHandle_t* pst_Dev)
{
    (pst_Dev->SCL_L)();
    delay_us(5);
    (pst_Dev->SDA_L)();
    delay_us(5);
    (pst_Dev->SCL_H)();
    delay_us(5);
    (pst_Dev->SDA_H)();
    delay_us(5);
}

//==================================================================================================
//| 函数名称 | Bsp_I2cAck
//Bsp_I2cAck(&adci2c1);
//Bsp_I2cAck(&adci2c2);
void Bsp_I2cAck(I2cHandle_t* pst_Dev)
{
    (pst_Dev->SCL_L)();
    delay_us(5);
    (pst_Dev->SDA_L)();
    delay_us(5);
    (pst_Dev->SCL_H)();
    delay_us(5);
    (pst_Dev->SCL_L)();
    delay_us(5);
}

//==================================================================================================
//| 函数名称 | Bsp_I2cNoAck
//Bsp_I2cNoAck(&adci2c1);
//Bsp_I2cNoAck(&adci2c2);
void Bsp_I2cNoAck(I2cHandle_t* pst_Dev)
{
    (pst_Dev->SCL_L)();
    delay_us(5);
    (pst_Dev->SDA_H)();
    delay_us(5);
    (pst_Dev->SCL_H)();
    delay_us(5);
    (pst_Dev->SCL_L)();
    delay_us(5);
}

//==================================================================================================
//| 函数名称 | Bsp_I2cWaitAck
//Bsp_I2cWaitAck(&adci2c1);
//Bsp_I2cWaitAck(&adci2c2);
unsigned char Bsp_I2cWaitAck(I2cHandle_t* pst_Dev)
{
    (pst_Dev->SCL_L)();
    delay_us(5);
    (pst_Dev->SDA_H)();
    delay_us(5);
    (pst_Dev->SCL_H)();
    delay_us(5);
    if ((pst_Dev->SDA_Read)())
    {
        (pst_Dev->SCL_L)();
        return 0;
    }
    else
    {
        (pst_Dev->SCL_L)();
        return 1;
    }
}

//==================================================================================================
//| 函数名称 | Bsp_I2cSendByte
//Bsp_I2cSendByte(&adci2c1,0x90);
//Bsp_I2cSendByte(&adci2c2,0x90);
void Bsp_I2cSendByte(I2cHandle_t* pst_Dev, unsigned char SendByte)
{
    unsigned char i = 8;
    while (i--)
    {
        (pst_Dev->SCL_L)();
        delay_us(5);

        if (SendByte & 0x80)
            (pst_Dev->SDA_H)();
        else
            (pst_Dev->SDA_L)();

        SendByte <<= 1;

        delay_us(5);
        (pst_Dev->SCL_H)();
        delay_us(5);
    }
    (pst_Dev->SCL_L)();
}

//==================================================================================================
//| 函数名称 | Bsp_I2cReceiveByte
//Bsp_I2cReceiveByte(&adci2c1);
//Bsp_I2cReceiveByte(&adci2c2);
unsigned char Bsp_I2cReceiveByte(I2cHandle_t* pst_Dev)
{
    unsigned char i = 8;
    unsigned char ReceiveByte = 0;

    (pst_Dev->SDA_H)();
    while (i--)
    {
        ReceiveByte <<= 1;
        (pst_Dev->SCL_L)();
        delay_us(5);
        (pst_Dev->SCL_H)();
        delay_us(5);
        if ((pst_Dev->SDA_Read)())
        {
            ReceiveByte |= 0x01;
        }
    }
    (pst_Dev->SCL_L)();
    return ReceiveByte;
}


unsigned char softi2creaddev(I2cHandle_t* pst_Dev, unsigned char cmdbuf)
{
    unsigned char ucret = 0;
    Bsp_I2cStart(pst_Dev);//start condition
    Bsp_I2cSendByte(pst_Dev, WRITADDRESS);//写入命令
    Bsp_I2cAck(pst_Dev);//Ack
    Bsp_I2cSendByte(pst_Dev, cmdbuf);//命令的内容
    Bsp_I2cAck(pst_Dev);//Ack
    Bsp_I2cStart(pst_Dev);//repeat start condition
    Bsp_I2cSendByte(pst_Dev, READADDRESS);//写入命令读取
    Bsp_I2cAck(pst_Dev);//Ack
    ucret = Bsp_I2cReceiveByte(pst_Dev);//读取数据
    Bsp_I2cNoAck(pst_Dev);//NACK
    Bsp_I2cStop(pst_Dev);//stop condition
    return ucret;
}
//ads7830 i2c 7bits address is 0x48
//if use HAL_I2C_Mem_Read from stm32hal library the address is 0x90 it can be read and write
//if use software i2c the steps as follow
//1.StartCondition         + Transmit[address+R/Wbit:0x90] + ACK + Trammit[CmdByte]   + ACK
//2.RepeatedStartCondition + Transmit[address+R/Wbit:0x91] + ACK + Receive[ReadByt]   + NACK + StopCondition
unsigned char  readads7830(unsigned char Channel)
{
    unsigned char buf = 0x00;
    unsigned char ret = 0x00;
    unsigned char tmp = 0x00;
    if (Channel < 8)
    {
        buf |= 0x80;            // 0b10000000  Single-Ended Inputs
        buf |= (Channel << 4);  // 0b00010000  Channel 0
#if 0
        ret = softi2creaddev(&adci2c1, buf);
#else
        HAL_I2C_Mem_Read(&hi2c1, 0x90, buf, 1, &ret, 1, 800);

#endif
    }
    else
    {
        buf |= 0x80;            // 0b10000000  Single-Ended Inputs
        tmp = Channel - 8;
        buf |= (tmp << 4);  // 0b00010000  Channel 0
#if 0
        ret = softi2creaddev(&adci2c2, buf);
#else
        HAL_I2C_Mem_Read(&hi2c2, 0x90, buf, 1, &ret, 1, 800);//buf是配置用的  ret是读取的数据

#endif
    }

    return ret;

}


void ReadChannel(void)
{
    ADCdata[0] = readads7830(0);
    ADCdata[1] = readads7830(1);
    ADCdata[2] = readads7830(2);
    ADCdata[3] = readads7830(3);
    ADCdata[4] = readads7830(4);
    ADCdata[5] = readads7830(5);
    ADCdata[6] = readads7830(6);
    ADCdata[7] = readads7830(7);
    ADCdata[8] = readads7830(8);
    ADCdata[9] = readads7830(9);
    ADCdata[10] = readads7830(10);
    ADCdata[11] = readads7830(11);
    ADCdata[12] = readads7830(12);
    ADCdata[13] = readads7830(13);
    ADCdata[14] = readads7830(14);
    ADCdata[15] = readads7830(15);
}

unsigned int lpoCnt = 0;
unsigned int lpmACnt = 0;
unsigned int lpoTime = 0;
unsigned int TimeMenu = 0;
unsigned char keyValue = 0;
unsigned char ADCdata[16] = { 0 };
void readKey(void)
{
    if (key1ent)
    {
        keyValue = 1;
    }
    else if (key2ent)
    {
        keyValue = 2;
    }
    else if (key3ent)
    {
        keyValue = 3;
    }
}