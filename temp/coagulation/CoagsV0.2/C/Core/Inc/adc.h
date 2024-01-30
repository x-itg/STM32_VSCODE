#ifndef __ADC_H
#define __ADC_H	
typedef struct
{
    void (*SDA_H)(void);
    void (*SDA_L)(void);
    unsigned char (*SDA_Read)(void);
    void (*SCL_H)(void);
    void (*SCL_L)(void);
} I2cHandle_t;

#define READADDRESS 0x91
#define WRITADDRESS 0x90

void initi2chandle(void);
void ReadChannel(void);

extern unsigned char ADCdata[16];
extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
#endif