#include "pidAutoTune.h"
#include "math.h"

extern long long millis(void);

void Cancel(PID_ATune_tp aPid)
{
	aPid->running = 0;
} 
 
int Runtime(PID_ATune_tp aPid)
{
	int i=0;
	long long now =0;
	double refVal=0;
	double val=0;
	double avgSeparation=0;
	
	aPid->justevaled=0;
	if(aPid->peakCount>9 && aPid->running)
	{
		aPid->running = 0;
		aPid->FinishUp(aPid);
		return 1;
	}
	now = millis();
	
	if((now-aPid->lastTime) < aPid->sampleTime)
	{
		return 0;
	}
	
	aPid->lastTime = now;
	refVal = *(aPid->input);
	aPid->justevaled=1;
	
	if(!aPid->running)
	{ 
		aPid->peakType = 0;
		aPid->peakCount=0;
		aPid->justchanged=0;
		aPid->absMax=refVal;
		aPid->absMin=refVal;
		aPid->setpoint = refVal;
		aPid->running = 1;
		aPid->outputStart = *aPid->output;
		*aPid->output = aPid->outputStart + aPid->oStep;
	}
	else
	{
		if(refVal > aPid->absMax)
		{
			aPid->absMax=refVal;
		}
		if(refVal < aPid->absMin)
		{
			aPid->absMin=refVal;
		}
	}
	
	//oscillate the output base on the input's relation to the setpoint
	
	if(refVal > aPid->setpoint + aPid->noiseBand) 
	{
		*aPid->output = aPid->outputStart - aPid->oStep;
	}
	else if (refVal < aPid->setpoint- aPid->noiseBand) 
	{
		*aPid->output = aPid->outputStart + aPid->oStep;
	}
	
	
  aPid->isMax = 1;
  aPid->isMin = 1;
  //id peaks
  for(i=aPid->nLookBack-1;i>=0;i--)
  {
    val = aPid->lastInputs[i];
    if(aPid->isMax) aPid->isMax = refVal>val;
    if(aPid->isMin) aPid->isMin = refVal<val;
    aPid->lastInputs[i+1] = aPid->lastInputs[i];
  }
  aPid->lastInputs[0] = refVal;  
  
  if(aPid->nLookBack<9)
  {  
	return 0;
  }
  
  if(aPid->isMax)
  {
    if(aPid->peakType==0)aPid->peakType=1;
    if(aPid->peakType==-1)
    {
      aPid->peakType = 1;
      aPid->justchanged=1;
      aPid->peak2 = aPid->peak1;
    }
    aPid->peak1 = now;
    aPid->peaks[aPid->peakCount] = refVal;
   
  }
  else if(aPid->isMin)
  {
    if(aPid->peakType==0)aPid->peakType=-1;
    if(aPid->peakType==1)
    {
      aPid->peakType=-1;
      aPid->peakCount++;
      aPid->justchanged=1;
    }
    
    if(aPid->peakCount<10)aPid->peaks[aPid->peakCount] = refVal;
  }
  
  if(aPid->justchanged && aPid->peakCount>2)
  { //we've transitioned.  check if we can autotune based on the last peaks
    avgSeparation = (fabs(aPid->peaks[aPid->peakCount-1]-aPid->peaks[aPid->peakCount-2])+fabs(aPid->peaks[aPid->peakCount-2]-aPid->peaks[aPid->peakCount-3]))/2;
    if( avgSeparation < 0.05*(aPid->absMax-aPid->absMin))
    {
	  aPid->FinishUp(aPid);
      aPid->running = 0;
	  return 1;
	 
    }
  }
   aPid->justchanged=0;
  
   return 0;
}

void FinishUp(PID_ATune_tp aPid)
{
	  *aPid->output = aPid->outputStart;
      aPid->Ku = 4*(2*aPid->oStep)/((aPid->absMax-aPid->absMin)*3.14159);
      aPid->Pu = (double)(aPid->peak1-aPid->peak2) / 1000;
}

double GetKp(PID_ATune_tp aPid)
{
	return aPid->controlType==1 ? 0.6 * aPid->Ku : 0.4 * aPid->Ku;
}

double GetKi(PID_ATune_tp aPid)
{
	return aPid->controlType==1? 1.2*aPid->Ku / aPid->Pu : 0.48 * aPid->Ku / aPid->Pu;  // Ki = Kc/Ti
}

double GetKd(PID_ATune_tp aPid)
{
	return aPid->controlType==1? 0.075 * aPid->Ku * aPid->Pu : 0;  //Kd = Kc * Td
}

void SetOutputStep(PID_ATune_tp aPid,double Step)
{
	aPid->oStep = Step;
}

double GetOutputStep(PID_ATune_tp aPid)
{
	return aPid->oStep;
}

void SetControlType(PID_ATune_tp aPid,int Type) //0=PI, 1=PID
{
	aPid->controlType = Type;
}
int GetControlType(PID_ATune_tp aPid)
{
	return aPid->controlType;
}
	
void SetNoiseBand(PID_ATune_tp aPid,double Band)
{
	aPid->noiseBand = Band;
}

double GetNoiseBand(PID_ATune_tp aPid)
{
	return aPid->noiseBand;
}

void SetLookbackSec(PID_ATune_tp aPid,int value)
{
    if (value<1) value = 1;
	
	if(value<25)
	{
		aPid->nLookBack = value * 4;
		aPid->sampleTime = 250;
	}
	else
	{
		aPid->nLookBack = 100;
		aPid->sampleTime = value*10;
	}
}

int GetLookbackSec(PID_ATune_tp aPid)
{
	return aPid->nLookBack * aPid->sampleTime / 1000;
}


void autoPidInit(PID_ATune_tp aPid,double* Input, double* Output)
{
	aPid->Runtime =  Runtime;  
	aPid->Cancel =  Cancel;  
	aPid->SetOutputStep =  SetOutputStep;  
	aPid->GetOutputStep =  GetOutputStep;  
	aPid->SetControlType =  SetControlType;  
	aPid->GetControlType =  GetControlType;  
	aPid->SetLookbackSec =  SetLookbackSec;  
	aPid->GetLookbackSec =  GetLookbackSec;  
	aPid->SetNoiseBand =  SetNoiseBand;  
	aPid->GetNoiseBand =  GetNoiseBand;  

	aPid->GetKp =  GetKp;  
	aPid->GetKi =  GetKi;  
	aPid->GetKd =  GetKd;  
	aPid->FinishUp =  FinishUp;  	
	
	aPid->input = Input;
	aPid->output = Output;
	aPid->controlType =1 ; //default to PI
	aPid->noiseBand = 0.5;
	aPid->running = 1;
	aPid->oStep = 250;
	aPid->SetLookbackSec(aPid,20);
	aPid->lastTime = millis();
}


