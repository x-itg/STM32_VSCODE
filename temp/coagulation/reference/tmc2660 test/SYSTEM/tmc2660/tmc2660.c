#include  "tmc2660.h"


#define USE_TMC2660 1
#if USE_TMC2660

//���spi�����豸
static struct spi_dev_device	tmc2660_spi_dev[1];
static struct spi_bus_device 	spi_bus0;
//Ĭ������
#define		SCG_DEFAULT			0x10000
static void spi0_cs(unsigned char state)
{
		if (state)
				GPIO_SetBits(TMC2660_PORTY_CS, TMC2660_GPIOY_CS);
    else
				GPIO_ResetBits(TMC2660_PORTY_CS, TMC2660_GPIOY_CS);
}
//tmc2660 spi
static u32 tmc2660_spi_xfer(u8 spi_no,u32 write_data)
{
		u8 send_buff[3],recv_buff[3];
		u32 recv_data= 0;
	
		send_buff[0] = (write_data>>16)&0xff;
		send_buff[1] = (write_data>>8)&0xff;
		send_buff[2] = (write_data&0xff);
		spi_send_recv(&tmc2660_spi_dev[spi_no],send_buff,recv_buff,3);
		recv_data = recv_buff[0]<<16 | recv_buff[1]<<8 | recv_buff[2];
		
		return (recv_data&0x0fffff);
}

//tmc2660 init
//void tmc2660_init(module_param_t *motor_param,u8 motor_index)
void tmc2660_init(void)	
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	//spi io
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	//spi cs PA4
	GPIO_InitStructure.GPIO_Pin = TMC2660_GPIOY_CS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(TMC2660_PORTY_CS, &GPIO_InitStructure);
	//device init
	stm32f1xx_spi_init(&spi_bus0,8);   //����spi_hw.h����
	tmc2660_spi_dev[0].spi_cs = spi0_cs;
	tmc2660_spi_dev[0].spi_bus = &spi_bus0;
			
	//ԭ���翪�� PB11
	GPIO_InitStructure.GPIO_Pin =  SenseY_GPIO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;										//��������
	GPIO_Init(SenseY_PORT, &GPIO_InitStructure);
				
	TMC2660_OUTY_CS 	= 1;
	TMC2660_OUTY_EN 	= 0;																					//enable
	TMC2660_OUTY_DIR 	= 0;																					//����ת����
				
	tmc2660_spi_xfer(YMOTOR,REG_DRVCONF | 0X0050); 									//0X0010->305mV 0X0050->165mV �����������
	tmc2660_spi_xfer(YMOTOR,REG_DRVCTRL | 0x00204);					 
	tmc2660_spi_xfer(YMOTOR,0x901b4);																//0x94557
	tmc2660_spi_xfer(YMOTOR,0xa8202);																//0xa0202->1/2CS 0xa8202->1/4CS 
	tmc2660_spi_xfer(YMOTOR,REG_SGCSCONF | SCG_DEFAULT | 0x00);			//��5λΪ������С
}

/***********************************************************
 * ��������tmc2660_move_onestep
 * ����  ��������ƶ˿ڳ�ʼ��		  
 * ����  : ��
 * ���  ����    
 ***********************************************************/

/*
void tmc2660_move_onestep(u8 direct,u8 motor_index)
{
		if(motor_index == YMOTOR)
		{
				TMC2660_OUTY_DIR = direct;
		}
		else if(motor_index == ZMOTOR)
		{
				TMC2660_MOTORZ_DIR = direct;
		}
}

*/

//�趨�������
void tmc2660_set_force(u8 force,u8 motor_index)
{
		u8 temp;
		int cmd = 0;
	
		temp = force/8;
		cmd = REG_SGCSCONF | SCG_DEFAULT | temp;
		tmc2660_spi_xfer(motor_index,cmd);
}

//��ȡԭ��λ
char tmc2660_light_sw_state(u8 motor_index)
{
		u8 status = 0;
		if(motor_index == YMOTOR) 
		{
					status = Y_SENSE;
		}
		else if(motor_index == ZMOTOR)
		{	
			
		}
		return status;
}

// ����tmc2660ϸ��
void tmc2660_set_subdivide(char mode,u8 motor_index)
{
		u8 	step = 0;
		u32 cmd = 0;
	
		switch(mode)
		{
				case 0:
						step = MICROSTEP_1;
				break;
				case 1:
						step = MICROSTEP_2;
				break;
				case 2:
						step = MICROSTEP_4;
				break;
				case 3:
						step = MICROSTEP_8;
				break;
				case 4:
						step = MICROSTEP_16;
				break;
				case 5:
						step = MICROSTEP_32;
				break;
				case 6:
						step = MICROSTEP_64;
				break;
				case 7:
						step = MICROSTEP_128;
				break;
				case 8:
						step = MICROSTEP_256;
				break;
				default:
						step = MICROSTEP_16;
				break;
		}
		cmd = REG_DRVCTRL | step;
		tmc2660_spi_xfer(motor_index,cmd);
}
	
//��ȡtmc2660״̬��Ϣ
u32 tmc2660_read_state(u8 motor_index)
{
		tmc2660_spi_xfer(motor_index,0x00);
	
		return 0;
}

#endif
