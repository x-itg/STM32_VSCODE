#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "header.h"

extern union unionLong e2p_int32;
extern union unionLong2 e2p_float;
extern union unionLong3 wflash;
#define Rx_bufSize 128 
#define Tx_bufSize 128 

typedef struct
{
	 u8 tx_buffer[Tx_bufSize];
	 u8 rx_buffer[Rx_bufSize];
	 u8 temp_buffer[Rx_bufSize];
	__IO uint8_t rev_complete;
	__IO uint8_t timeout_flag,send_flag;
	__IO uint16_t	counter,timeout_counter;
	
}myusart;
extern	myusart	myusart1,myusart2;


//EEPROM start//
extern __IO uint32_t  PCBID;
extern __IO uint16_t  CANID;
//extern __IO uint32_t  BAUD;
extern __IO uint32_t  RESO;
extern __IO float  RADIO;
extern __IO float  OFFSET;
extern __IO u8  MODE;
//extern __IO u8  RATE;
//EEPROM end//
 
enum ENUM_CAN_BAUD{

	CAN_1M	=0x0000,
	CAN_500K=0x0001,
	CAN_250K=0x0002,
	CAN_125K=0x0003,
	CAN_100K=0x0004,
	CAN_50K	=0x0005,
};
extern __IO enum ENUM_CAN_BAUD CAN_BAUD;

enum ENUM_ADRATE{

	AD500HZ=0x01,
	AD200HZ=0x02,
	AD100HZ=0x03,
	AD60HZ =0x04,
	AD50HZ =0x05,
	AD40HZ =0x06,
	AD30HZ =0x07,
	AD20HZ =0x08,
	AD15HZ =0x09,
	AD14HZ =0x0A,
	AD12HZ =0x0B,
	AD10HZ =0x0C,
	AD8HZ =0x0D,
	AD6HZ =0x0E,
	AD5HZ =0x0F,
};
extern __IO enum ENUM_ADRATE ADRATE;


union bit_def
{
 unsigned char b8;
 struct bit8_def
 {
 unsigned  char b0:1;
 unsigned  char b1:1;
 unsigned  char b2:1;
 unsigned  char b3:1;
 unsigned  char b4:1;
 unsigned  char b5:1;
 unsigned  char b6:1;
 unsigned  char b7:1;
 }flag;
};

extern union bit_def myflag;
#define usart1_ComAnalysis_flag 	myflag.flag.b0
#define usart2_ComAnalysis_flag 	myflag.flag.b1
#define boot_config_flag 	myflag.flag.b2
#define samprate_change_flag 	myflag.flag.b3

#define e2p_initall_flag 			myflag.flag.b4
#define e2p_writezero_flag 		myflag.flag.b5
#define e2p_resetzero_flag 		myflag.flag.b6

extern   uint8_t DeviceType;
extern   uint8_t Trig;
extern  CanTxMsg can_tx;    
extern  CanRxMsg can_rx; 
extern  CanRxMsg can_recv[32]; 

extern __IO uint8_t CAN_addr;
extern __IO int32_t  CAN_baud ;
  
extern char writeoneline[128]; 

extern __IO uint32_t global_counter;
 

extern uint16_t AD7175_ID;

extern __IO int32_t POS_30mV;
extern __IO int32_t Zero_30mV;
extern __IO int32_t NEG_30mV;

extern __IO int32_t POS_300mV;
extern __IO int32_t Zero_300mV;
extern __IO int32_t NEG_300mV;

extern __IO int32_t POS_3V;
extern __IO int32_t Zero_3V;
extern __IO int32_t NEG_3V;

extern __IO  int Acq_value,old_value,counter,Acq_value1;
extern __IO float measured,old_measured,hold_measured,max_measured,min_measured,temp_measured;
extern __IO float current_zero,init_zero,Calradio,Offset;
extern uint16_t cycle;
extern __IO float radio[4]; 
extern  uint8_t k_zero,k_relative,k_reset;
extern  uint8_t current_unit;
extern __IO float measured1,measured2,measured3,measured4;
extern __IO float CAN_measured1,CAN_measured2,CAN_measured3,CAN_measured4;
extern __IO float current_zero1,current_zero2,current_zero3,current_zero4;
extern __IO float init_zero1,init_zero2,init_zero3,init_zero4;

extern float Resistance,TempPt100,tempvoltage,THtemp;
#endif
