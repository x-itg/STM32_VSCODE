#ifndef _TMC2660_H_
#define	_TMC2660_H_
#include "sys.h"
#include "spi_core.h"
#include "spi_hw.h"

#define   YMOTOR      	0
#define   ZMOTOR      	1

//�Ĵ���
#define		REG_DRVCTRL			0X00000000
#define		REG_CHOPCONF		0X00080000
#define		REG_SMARTEN			0X000A0000
#define		REG_SGCSCONF		0X000C0000
#define		REG_DRVCONF			0X000E0000

//reg value
#define		MICROSTEP_256		0X00
#define		MICROSTEP_128		0X01
#define		MICROSTEP_64		0X02
#define		MICROSTEP_32		0X03
#define		MICROSTEP_16		0X04
#define		MICROSTEP_8			0X05
#define		MICROSTEP_4			0X06
#define		MICROSTEP_2			0X07
#define		MICROSTEP_1			0X08
//current

//SCG
//#define		SCG_DEFAULT			0x10000

/*********Y MOTOR**********/
//spi cs
#define		TMC2660_PORTY_CS		GPIOA
#define		TMC2660_GPIOY_CS		GPIO_Pin_4				//PA4
#define		TMC2660_OUTY_CS			PAout(4)


//enable
#define		TMC2660_PORTY_EN		GPIOC							//PC1
#define		TMC2660_GPIOY_EN		GPIO_Pin_1
#define		TMC2660_OUTY_EN			PCout(1)	
//step 
#define		TMC2660_PORTY_STEP	GPIOC							//PC2
#define		TMC2660_GPIOY_STEP	GPIO_Pin_2
#define		TMC2660_OUTY_STEP		PCout(2)	
//dir	
#define		TMC2660_PORTY_DIR		GPIOC							//PC3
#define		TMC2660_GPIOY_DIR		GPIO_Pin_3
#define		TMC2660_OUTY_DIR		PCout(3)	

//light sense
#define	  Y_SENSE				PAin(8)								//optDetect		 
#define		SenseY_PORT		GPIOA
#define		SenseY_GPIO		GPIO_Pin_8

#define u8 unsigned char
#define u32 unsigned int


extern void tmc2660_init(void);
extern void tmc2660_move_onestep(u8 direct, u8 motor_index);
extern void tmc2660_set_force(u8 force,u8 motor_index);
extern char tmc2660_light_sw_state(u8 motor_index);
extern void tmc2660_set_subdivide(char mode,u8 motor_index);
extern u32 tmc2660_read_state(u8 motor_index);


#endif
