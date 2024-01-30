#include "pid.h"
#include "math.h"

#define max(a, b)           (a>b? a:b)  
#define min(a, b)           (a<b? a:b)  
#define range(x, a, b)      (min(max(x, a), b))  
#define exchange(a, b, tmp) (tmp=a, a=b, b=tmp)  
#define myabs(x)            ((x<0)? -x:x)  
 
float fuzzy_kp(float e, float ec); 
float fuzzy_ki(float e, float ec);
float fuzzy_kd(float e, float ec);

void  PidInit(PID_t *pid,float limit,float startTemp,float targetTemp,float cv)
{
	pid->limit  = limit;
	pid->target = targetTemp;
	pid->autotemp = startTemp;
	pid->targetC  = cv;
	
	pid->e =0;
	pid->ec =0;
	pid->ecSum =0;
	
	pid->Kd =80;
	pid->Ki =0.03;
	pid->Kp =2.8;
	
	pid->stable =0;
}


float PidCalcFix(PID_t *pid ,float feedback)  
{
	//static float fkd[] ={80,68,56,39,32,30,20,10};
	float out =0; 
	float err =0,de =0;
	
	u8 index = ((int)pid->target)/100;
	index = range(index,0,8);
	err        = pid->autotemp - feedback; 
	pid->ecSum +=err;
	pid->ecSum = range(pid->ecSum,-200,200);

	de = err - pid->e;
	pid->e  = err;
	
	//pid->Kd = fkd[index];
	out = pid->Kp*err + pid->Ki * pid->ecSum + pid->Kd*de; 	
	if(out > 0)
	{
		out = range(out, 0, 110)+40;
	}
	else
	{
		out = 38;
	}

	return out;
}

float PidCalc(PID_t *pid ,float feedback,int oldOout)  
{  
    float out;  
    float v,dv,dvc,err; 
      
	err        = pid->target - feedback; 
	if(err >0 && !pid->stable)
	{
		if(pid->e <= 0.001)
		{
			out = 250;
			
			v          = (err - pid->e)/0.1;
			dv         = pid->targetC - v;
			pid->e     = err;
			
			pid->ecSum += dv;
			dvc         = dv - pid->ec;
			pid->ec     = dv;
			
			pid->Kp = fuzzy_kp(dv,dvc); 
			pid->Ki = fuzzy_ki(dv,dvc);
			pid->Kd = fuzzy_kd(dv,dvc);
		}
		else if(fabs(err - pid->e) > 0.1)
		{
			v          = (err - pid->e)/0.1;
			dv         = pid->targetC - v;
			pid->e     = err;
			
			pid->ecSum += dv;
			dvc         = dv - pid->ec;
			pid->ec     = dv;


			out = pid->Kp*dv + pid->Ki * pid->ecSum + pid->Kd*dvc; 
			out*=1.25;			
			out = range(out, 0, pid->limit);  
			
			pid->Kp = fuzzy_kp(dv,dvc); 
			pid->Ki = fuzzy_ki(dv,dvc);
			pid->Kd = fuzzy_kd(dv,dvc);
		}
		else
		{
			out  = oldOout;
		}
	}
	else
	{
		if(!pid->stable)
		{
			pid->stable = 1;
			pid->ecSum =0;
			pid->e =0;
		}
		
		err -= 3;
		pid->ecSum+=err;
		dvc = err - pid->e;
		pid->e = err;
		
		out = 35.2*err + 0.015 * pid->ecSum + 0.25*dvc;  
		out = range(out, 0, pid->limit);  
	}
    return out;  
} 


/***********************************************
*: 模糊 PID  Kp 的计算
************************************************/
float fuzzy_kp(float e, float ec) 
{
	float Kp_calcu;
	u8 num,pe,pec;
	float eRule[7]={-3.0,-2.0,-1.0,0.0,1.0,2.0,3.0};
	float ecRule[7]={-1.0,-0.6,-0.3,0.0,0.3,0.6,1.0};

	float eFuzzy[2]={0.0,0.0}; 
	float ecFuzzy[2]={0.0,0.0}; 
	float kpRule[4]={0.0,4.0,4.0,8.0}; 
	float KpFuzzy[4]={0.0,0.0,0.0,0.0}; 
	int  KpRule[7][7]= 
	{
		2,1,2,1,2,1,2,
		3,2,2,1,2,2,3,
		3,2,1,1,1,2,3,
		3,2,1,0,1,2,3,
		3,2,1,1,1,2,3,
		3,2,2,1,2,2,3,
		2,1,2,2,2,1,2
	};

	if(e<eRule[0])
	{
		eFuzzy[0] =1.0;
		pec = 0;
	}
	else if(eRule[0]<=e && e<eRule[1])
	{
		eFuzzy[0] = (eRule[1]-e)/(eRule[1]-eRule[0]);
		pec = 0;
	}
	else if(eRule[1]<=e && e<eRule[2])
	{
		eFuzzy[0] = (eRule[2] -e)/(eRule[2]-eRule[1]);
		pec = 1;
	}
	else if(eRule[2]<=e && e<eRule[3])
	{
		eFuzzy[0] = (eRule[3] -e)/(eRule[3]-eRule[2]);
		pec = 2;
	}
	else if(eRule[3]<=e && e<eRule[4])
	{ 
		eFuzzy[0] = (eRule[4]-e)/(eRule[4]-eRule[3]);
		pec = 3;
	}
	else if(eRule[4]<=e && e<eRule[5])
	{
		eFuzzy[0] = (eRule[5]-e)/(eRule[5]-eRule[4]);
		pec = 4;
	}
	else if(eRule[5]<=e && e<eRule[6])
	{
		eFuzzy[0] = (eRule[6]-e)/(eRule[6]-eRule[5]);
		pec = 5;
	}
	else
	{
		eFuzzy[0] =0.0;
		pec =5;
	}
	
	eFuzzy[1] =1.0 - eFuzzy[0];
	
	if(ec<ecRule[0])
	{
		ecFuzzy[0] =1.0;
		pe = 0;
	}
	else if(ecRule[0]<=ec && ec<ecRule[1])
	{
		ecFuzzy[0] = (ecRule[1] - ec)/(ecRule[1]-ecRule[0]);
		pe = 0 ;
	}
	else if(ecRule[1]<=ec && ec<ecRule[2])
	{
		ecFuzzy[0] = (ecRule[2] - ec)/(ecRule[2]-ecRule[1]);
		pe = 1;
	}
	else if(ecRule[2]<=ec && ec<ecRule[3])
	{
		ecFuzzy[0] = (ecRule[3] - ec)/(ecRule[3]-ecRule[2]);
		pe = 2 ;
	}else if(ecRule[3]<=ec && ec<ecRule[4])
	{ 
		ecFuzzy[0] = (ecRule[4]-ec)/(ecRule[4]-ecRule[3]);
		pe=3;
	}
	else if(ecRule[4]<=ec && ec<ecRule[5])
	{
		ecFuzzy[0] = (ecRule[5]-ec)/(ecRule[5]-ecRule[4]);
		pe=4;
	}
	else if(ecRule[5]<=ec && ec<ecRule[6])
	{ 
		ecFuzzy[0] = (ecRule[6]-ec)/(ecRule[6]-ecRule[5]);
		pe=5;
	}
	else
	{
		ecFuzzy[0] =0.0;
		pe = 5;
	}
	ecFuzzy[1] = 1.0 - ecFuzzy[0];
	
	
	num =KpRule[pe][pec];
	KpFuzzy[num] += eFuzzy[0]*ecFuzzy[0];
	num =KpRule[pe][pec+1];
	KpFuzzy[num] += eFuzzy[0]*ecFuzzy[1];
	num =KpRule[pe+1][pec];
	KpFuzzy[num] += eFuzzy[1]*ecFuzzy[0];
	num =KpRule[pe+1][pec+1];
	KpFuzzy[num] += eFuzzy[1]*ecFuzzy[1];
	
	Kp_calcu=KpFuzzy[0]*kpRule[0]+KpFuzzy[1]*kpRule[1]+KpFuzzy[2]*kpRule[2]+KpFuzzy[3]*kpRule[3];
	
	return(Kp_calcu);
}


/***********************************************
*模糊: PID  Ki 计算
************************************************/
float fuzzy_ki(float e, float ec)
{
	float Ki_calcu;
	u8 num,pe,pec;
	float eRule[7]={-3.0,-2.0,-1.0,0.0,1.0,2.0,3.0};
	float ecRule[7]={-1.0,-0.6,-0.3,0.0,0.3,0.6,1.0};
	float  eFuzzy[2]={0.0,0.0};
	float  ecFuzzy[2]={0.0,0.0};
	float  kiRule[4]={0.00,0.01,0.02,0.03};
	float  KiFuzzy[4]={0.0,0.0,0.0,0.0};
	int    KiRule[7][7]=
			{
				0,1,2,3,2,1,0,
				0,1,3,3,3,1,0,
				0,0,2,3,2,0,0,
				0,0,2,3,2,0,0,
				0,0,2,3,2,0,0,
				0,1,3,3,3,1,0,
				0,1,2,3,2,1,0
			};

	if(e<eRule[0])
	{
		eFuzzy[0] =1.0;
		pe = 0;
	}
	else if(eRule[0]<=e && e<eRule[1])
	{
		eFuzzy[0] = (eRule[1]-e)/(eRule[1]-eRule[0]);
		pec = 0;
	}
	else if(eRule[1]<=e && e<eRule[2])
	{
		eFuzzy[0] = (eRule[2] -e)/(eRule[2]-eRule[1]);
		pec = 1;
	}
	else if(eRule[2]<=e && e<eRule[3])
	{
		eFuzzy[0] = (eRule[3] -e)/(eRule[3]-eRule[2]);
		pec = 2;
	}
	else if(eRule[3]<=e && e<eRule[4])
	{ 
		eFuzzy[0] = (eRule[4]-e)/(eRule[4]-eRule[3]);
		pec = 3;
	}
	else if(eRule[4]<=e && e<eRule[5])
	{
		eFuzzy[0] = (eRule[5]-e)/(eRule[5]-eRule[4]);
		pec = 4;
	}
	else if(eRule[5]<=e && e<eRule[6])
	{
		eFuzzy[0] = (eRule[6]-e)/(eRule[6]-eRule[5]);
		pec = 5;
	}
	else
	{
		eFuzzy[0] =0.0;
		pec =5;
	}
	eFuzzy[1] =1.0 - eFuzzy[0];
	
	
	if(ec<ecRule[0])
	{
		ecFuzzy[0] =1.0;
		pe = 0;
	}
	else if(ecRule[0]<=ec && ec<ecRule[1])
	{
		ecFuzzy[0] = (ecRule[1] - ec)/(ecRule[1]-ecRule[0]);
		pe = 0 ;
	}
	else if(ecRule[1]<=ec && ec<ecRule[2])
	{
		ecFuzzy[0] = (ecRule[2] - ec)/(ecRule[2]-ecRule[1]);
		pe = 1;
	}
	else if(ecRule[2]<=ec && ec<ecRule[3])
	{
		ecFuzzy[0] = (ecRule[3] - ec)/(ecRule[3]-ecRule[2]);
		pe = 2 ;
	}
	else if(ecRule[3]<=ec && ec<ecRule[4])
	{ 
		ecFuzzy[0] = (ecRule[4]-ec)/(ecRule[4]-ecRule[3]);
		pe =3;
	}
	else if(ecRule[4]<=ec && ec<ecRule[5])
	{ 
		ecFuzzy[0] = (ecRule[5]-ec)/(ecRule[5]-ecRule[4]);
		pe=4;
	}
	else if(ecRule[5]<=ec && ec<ecRule[6])
	{ 
		ecFuzzy[0] = (ecRule[6]-ec)/(ecRule[6]-ecRule[5]);
		pe=5;
	}
	else
	{
		ecFuzzy[0] =0.0;
		pe = 5;
	}
	ecFuzzy[1] = 1.0 - ecFuzzy[0];
	
	
	num =KiRule[pe][pec];
	KiFuzzy[num] += eFuzzy[0]*ecFuzzy[0];
	num =KiRule[pe][pec+1];
	KiFuzzy[num] += eFuzzy[0]*ecFuzzy[1];
	num =KiRule[pe+1][pec];
	KiFuzzy[num] += eFuzzy[1]*ecFuzzy[0];
	num =KiRule[pe+1][pec+1];
	KiFuzzy[num] += eFuzzy[1]*ecFuzzy[1];
	
	Ki_calcu=KiFuzzy[0]*kiRule[0]+KiFuzzy[1]*kiRule[1]+KiFuzzy[2]*kiRule[2]+KiFuzzy[3]*kiRule[3];
	
	return(Ki_calcu);
}

/***********************************************
模糊: PID  Kd计算
************************************************/
float fuzzy_kd(float e, float ec)
{
	float Kd_calcu;
	u8 num,pe,pec;
	float eRule[7]={-3.0,-2.0,-1.0,0.0,1.0,2.0,3.0};
	float ecRule[7]={-1.0,-0.6,-0.3,0.0,0.3,0.6,1.0};
	float eFuzzy[2]={0.0,0.0};
	float ecFuzzy[2]={0.0,0.0};
	float kdRule[4]={0.0,1.0,2.0,3.0};
	float KdFuzzy[4]={0.0,0.0,0.0,0.0};
	int  KdRule[7][7]=
	{
		1,2,2,0,2,2,1,
		2,2,1,0,1,2,2,
		3,2,1,0,1,2,3,
		3,2,1,0,1,2,3,
		3,2,1,0,1,2,3,
		2,2,1,0,1,2,2,
		1,2,2,0,2,3,1
	};
	
	
	if(e<eRule[0])
	{
		eFuzzy[0] =1.0;
		pe = 0;
	}
	else if(eRule[0]<=e && e<eRule[1])
	{
		eFuzzy[0] = (eRule[1]-e)/(eRule[1]-eRule[0]);
		pe = 0;
	}
	else if(eRule[1]<=e && e<eRule[2])
	{
		eFuzzy[0] = (eRule[2] -e)/(eRule[2]-eRule[1]);
		pe = 1;
	}
	else if(eRule[2]<=e && e<eRule[3])
	{
		eFuzzy[0] = (eRule[3] -e)/(eRule[3]-eRule[2]);
		pe = 2;
	}
	else if(eRule[3]<=e && e<eRule[4])
	{ 
		eFuzzy[0] = (eRule[4]-e)/(eRule[4]-eRule[3]);
		pe = 3;
	}
	else if(eRule[4]<=e && e<eRule[5])
	{
		eFuzzy[0] = (eRule[5]-e)/(eRule[5]-eRule[4]);
		pe = 4;
	}
	else if(eRule[5]<=e && e<eRule[6])
	{
		eFuzzy[0] = (eRule[6]-e)/(eRule[6]-eRule[5]);
		pe = 5;
	}
	else
	{
		eFuzzy[0] =0.0;
		pe =5;
	}
	eFuzzy[1] =1.0 - eFuzzy[0];
	
	
	if(ec<ecRule[0])
	{
		ecFuzzy[0] =1.0;
		pec = 0;
	}
	else if(ecRule[0]<=ec && ec<ecRule[1])
	{
		ecFuzzy[0] = (ecRule[1] - ec)/(ecRule[1]-ecRule[0]);
		pec = 0 ;
	}
	else if(ecRule[1]<=ec && ec<ecRule[2])
	{
		ecFuzzy[0] = (ecRule[2] - ec)/(ecRule[2]-ecRule[1]);
		pec = 1;
	}
	else if(ecRule[2]<=ec && ec<ecRule[3])
	{
		ecFuzzy[0] = (ecRule[3] - ec)/(ecRule[3]-ecRule[2]);
		pec = 2 ;
	}
	else if(ecRule[3]<=ec && ec<ecRule[4])
	{ 
		ecFuzzy[0] = (ecRule[4]-ec)/(ecRule[4]-ecRule[3]);
		pec=3;
	}
	else if(ecRule[4]<=ec && ec<ecRule[5])
	{ 
		ecFuzzy[0] = (ecRule[5]-ec)/(ecRule[5]-ecRule[4]);
		pec=4;
	}
	else if(ecRule[5]<=ec && ec<ecRule[6])
	{ 
		ecFuzzy[0] = (ecRule[6]-ec)/(ecRule[6]-ecRule[5]);
		pec=5;
	}
	else
	{
		ecFuzzy[0] =0.0;
		pec = 5;
	}
	ecFuzzy[1] = 1.0 - ecFuzzy[0];
	
	num =KdRule[pe][pec];
	KdFuzzy[num] += eFuzzy[0]*ecFuzzy[0];
	num =KdRule[pe][pec+1];
	KdFuzzy[num] += eFuzzy[0]*ecFuzzy[1];
	num =KdRule[pe+1][pec];
	KdFuzzy[num] += eFuzzy[1]*ecFuzzy[0];
	num =KdRule[pe+1][pec+1];
	KdFuzzy[num] += eFuzzy[1]*ecFuzzy[1];
	
	Kd_calcu=KdFuzzy[0]*kdRule[0]+KdFuzzy[1]*kdRule[1]+KdFuzzy[2]*kdRule[2]+KdFuzzy[3]*kdRule[3];
	
	return(Kd_calcu);
}
