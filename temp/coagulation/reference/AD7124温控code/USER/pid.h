#ifndef __PID_H_  
#define __PID_H_  

#include "sys.h"

typedef struct
{  
    float limit;        
    float target; 
    float targetC;
	float autotemp;
	
    float Kp;     
    float Ki;     
    float Kd; 
    
	float e;
    float ec;
	float ecd;
    float ecSum;  
	
	u8 stable;
}PID_t;  
  
void  PidInit(PID_t *pid,float limit,float startTemp,float targetTemp,float cv);
float PidCalc(PID_t *pid ,float feedback,int oldOout);
float PidCalcFix(PID_t *pid ,float feedback); 

#endif
