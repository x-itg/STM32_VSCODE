#ifndef PID_AutoTune_v0
#define PID_AutoTune_v0

#include "sys.h"

typedef struct autopid PID_ATune_t;
typedef struct autopid *PID_ATune_tp;

void autoPidInit(PID_ATune_tp aPid,double* Input, double* Output);
struct  autopid
{                     	
    int    (*Runtime)(PID_ATune_tp aPid);						   			   
	void   (*Cancel)(PID_ATune_tp aPid);									   
	
	void   (*SetOutputStep)(PID_ATune_tp aPid,double);						   	
	double (*GetOutputStep)(PID_ATune_tp aPid);							   	
	
	void   (*SetControlType)(PID_ATune_tp aPid,int); 						   
	int    (*GetControlType)(PID_ATune_tp aPid);							   			
	
	void   (*SetLookbackSec)(PID_ATune_tp aPid,int);						
	int    (*GetLookbackSec)(PID_ATune_tp aPid);							
	
	void   (*SetNoiseBand)(PID_ATune_tp aPid,double);							
	double (*GetNoiseBand)(PID_ATune_tp aPid);								
	
	double (*GetKp)(PID_ATune_tp aPid);										
	double (*GetKi)(PID_ATune_tp aPid);									
	double (*GetKd)(PID_ATune_tp aPid);										
	
    void (*FinishUp)(PID_ATune_tp aPid);
	
	u8 isMax, isMin;
	double *input, *output;
	double setpoint;
	double noiseBand;
	int controlType;
	u8 running;
	long long peak1, peak2, lastTime;
	int sampleTime;
	int nLookBack;
	int peakType;
	double lastInputs[101];
    double peaks[10];
	int peakCount;
	u8 justchanged;
	u8 justevaled;
	double absMax, absMin;
	double oStep;
	double outputStart;
	double Ku, Pu;
	
};

#endif


