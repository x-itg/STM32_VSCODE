 
/**
  ******************************************************************************
  * @file    Global.c
  * @author   
  * @version V1.0
  * @date    08/11/2010
  * @brief   全局变量
  ******************************************************************************
*/

#include "header.h"
union unionLong e2p_int32;
union unionLong2 e2p_float;
union unionLong3 wflash;
myusart	myusart1,myusart2;

//EEPROM start//
__IO uint32_t  PCBID;
__IO uint16_t  CANID;
//__IO uint32_t  BAUD;
__IO uint32_t  RESO;
__IO float  RADIO;
__IO float  OFFSET;
__IO u8  MODE;
//__IO u8  RATE;
//EEPROM end//

 
__IO enum ENUM_CAN_BAUD CAN_BAUD;
__IO enum ENUM_ADRATE ADRATE;


__IO int8_t write_flag=0;
union bit_def can_recv_flag;
union bit_def canflag;
union bit_def myflag;
 
uint8_t DeviceType;
uint8_t Trig;
CanTxMsg can_tx;    
CanRxMsg can_rx; 
CanRxMsg can_recv[32]; 
 __IO uint8_t CAN_addr;
 __IO int32_t  CAN_baud;
 

 char writeoneline[128]; 

__IO uint32_t global_counter=0;
uint16_t AD7175_ID;
 __IO int32_t POS_30mV=83886;
 __IO int32_t Zero_30mV=0;
 __IO int32_t NEG_30mV=-83886;

 __IO int32_t POS_300mV=838861;
 __IO int32_t Zero_300mV=0;
 __IO int32_t NEG_300mV=-838861;

 __IO int32_t POS_3V=8388607;
 __IO int32_t Zero_3V=0;
 __IO int32_t NEG_3V=-8388607;
 
 __IO  int Acq_value,old_value,counter,Acq_value1;
__IO float measured,old_measured,hold_measured,max_measured,min_measured,temp_measured;
__IO float current_zero,init_zero,Calradio,Offset;
uint16_t cycle=0;

__IO float radio[4]; 
uint8_t k_zero=0,k_relative=0,k_reset=0;
__IO float measured1,measured2,measured3,measured4;
__IO float CAN_measured1,CAN_measured2,CAN_measured3,CAN_measured4;
__IO float current_zero1,current_zero2,current_zero3,current_zero4;
__IO float init_zero1,init_zero2,init_zero3,init_zero4;

float Resistance,TempPt100,tempvoltage,THtemp;