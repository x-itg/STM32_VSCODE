/**
 * @file name: motor.c
 * @verison:0.1
 * @date:2013-10-30
 * @author: FH wu
 * @brief: 电机控制库，采用查表法
 *
     +---------- waiting for trigger
     |
    \|/
      ____t1____
     /          \
    / t0         \ t2
   /              \

   总运行步长 = t0 + t1 + t2，当某个阶段的值为0时，表示运行时跳过该阶段


 *
 *
 */
#include "motor.h"
#include "bsp_uart.h"
#include "bsp.h"
#include "math.h"

/*正常S型曲线参数生成的表格*/
uint16_t Motor1TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor1StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor2TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor2StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor3TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor3StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor4TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor4StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor5TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor5StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor6TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor6StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};	

/*将参数降为2/3S型曲线参数生成的表格*/
uint16_t Motor1_23TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor1_23StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor2_23TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor2_23StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor3_23TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor3_23StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor4_23TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor4_23StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor5_23TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor5_23StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor6_23TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor6_23StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};

/*将参数降为1/3S型曲线参数生成的表格*/
uint16_t Motor1_13TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor1_13StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor2_13TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor2_13StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor3_13TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor3_13StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor4_13TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor4_13StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor5_13TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor5_13StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor6_13TimeTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};
uint16_t Motor6_13StepTable[2*(STEP_AA+STEP_UA+STEP_RA)+1] = { 0};

uint16_t MotorTableLength = 2*(STEP_AA+STEP_UA+STEP_RA)+1;
//void Motor_Reinitial(unsigned char MotorID)；
/**
 *@brief  : reset开始
 *          清除sns_trigger_cnt，防止电机在SNS触发外位置向里运动时，sns_trigger_cnt计数异常
 *          设置reset标志位
 * @input  :
 *           mt - 电机控制器
 * @output :
 **/
static uint16_t Mt_reset_start(mt_t *mt)
{
    if (!mt)
    {
        Log("%s-%d,ERROR: Motor reset start failed\n", filename(__FILE__), __LINE__);
        return MT_ERR_PARAM;
    }
    mt->sns_trigger_cnt = 0;    //清除sns计数
    MT_ClrFlag(mt, MT_FLAG_SNS);//清除传感器状态位
    mt->rstflg = MT_S_RESET;
    return MT_ERR_NONE;
}

/**
 *@brief  : reset完成
 *          清除reset标志位，防止moveto流程走reset分支
 * @input  :
 *           mt - 电机控制器
 * @output :
 **/
static u16_t MT_reset_done(mt_t* mt)
{
    if (!mt)
    {
        Log("%s-%d,ERROR: Motor reset done failed\n", filename(__FILE__), __LINE__);
        return MT_ERR_PARAM;
    }

    mt->rstflg = 0;
    return MT_ERR_NONE;
}

/**
 * @brief  : 传感器检测
 *           通常，电机每走一步，就会检测
 *           一次传感器状态
 * @input  :
 *           mt - 电机控制器
 * @output :
 */
static void MT_SNSCheck(mt_t *mt) 
{
    if( mt->IsSNSValid(mt->sns_id) == true)//传感器被遮住
    {
        if ((mt->id <4 && mt->dir == M1_CLOCKWISE) || (mt->id >3 && mt->dir == M1_UNCLOCKWISE))
        {
            mt->sns_trigger_cnt++;
//            is_rising = true;	//上升沿触发
            if (mt->sns_trigger_cnt == mt->rst_npos)
            {
                MT_SetFlag(mt, MT_FLAG_SNS);//置位传感器状态位   
            }
        }
        else //if (mt->dir == M1_UNCLOCKWISE)
        {
            if (mt->sns_trigger_cnt > 0)
              mt->sns_trigger_cnt--;
            if (mt->sns_trigger_cnt == 0)
                mt->sns_trigger_cnt = 0;
//            MT_ClrFlag(mt, MT_FLAG_SNS);//清除传感器状态位
        }
    }
    else
    {
        mt->sns_trigger_cnt = 0;
        MT_ClrFlag(mt, MT_FLAG_SNS);//清除传感器状态位
    }
}

static void MT_Stop(mt_t *mt)
{
    mt->en=0;
    mt->running=0;
    mt->const_over_time = 0;
    mt->remain_t = 0;
    mt->TimerStop(mt->id);
    mt->ResetClk(mt->id);
    mt->LPWEn(mt->id);
    AOS_SemPost( &(mt->sem) );
}

u8_t MT_Wait(mt_t *mt, u32_t timeov)
{
    u8_t ret = MT_ERR_NONE;

//    if (mt == NULL || timeov == 0 || mt->running == 0)
    if (mt == NULL || timeov == 0 )
        return MT_ERR_PARAM;

    while(mt->running != 0)
    {
        if (AOS_SemPend( &(mt->sem), timeov) != 0)
        {
            Log("%s-%d,ERROR: Motor(%d) is timeout once, wait again.\n", filename(__FILE__), __LINE__, mt->id);
            if (AOS_SemPend( &(mt->sem), timeov) != 0)
            {
            //电机运行超时
                ret = MT_ERR_TIMEOV;
                Log("%s-%d,ERROR: Motor(%d) is timeout twice, failed.\n", filename(__FILE__), __LINE__, mt->id);
            //重置电机Timer
                mt->en=0;
                mt->running=0;
                mt->const_over_time = 0;
                mt->remain_t = 0;
                mt->TimerStop(mt->id);
                mt->ResetClk(mt->id);
                mt->LPWEn(mt->id);
            }
        }
    }

    return ret;
}
 /*中断处理函数*/
void MT_Handler(mt_t* pmotor)
{
    if (pmotor == NULL)
        return;
    if(1 == pmotor->en)
    {
        if (pmotor->const_over_time > 0 && pmotor->remain_t > 0)
        {
            if (pmotor->remain_t > 0xffff)
            {
                pmotor->remain_t -= 0xffff;
                pmotor->TimerSet(pmotor->id, 0xffff);
            }
            else
            {
                pmotor->TimerSet(pmotor->id, pmotor->remain_t);
                pmotor->remain_t = 0;
            }
            return;
        }
        if( MT_IsFlagSet(pmotor, MT_FLAG_CLK) )//clk为高电平
        {
            MT_ClrFlag(pmotor, MT_FLAG_CLK);//置clk标志位为低电平
            MT_SNSCheck(pmotor);					//更新位置传感器状态
            if (MT_IsFlagSet(pmotor, MT_FLAG_SNS) == true)
            {
                if ((pmotor->id <4 &&pmotor->dir == M1_CLOCKWISE) || (pmotor->id >3 && pmotor->id < MT_NUMS && pmotor->dir == M1_UNCLOCKWISE)) {
                    pmotor->CurrentIndex=0;
                    pmotor->CurrentPosition = 0;
                    pmotor->CurrentPosition_Pulse = 0;
                    MT_Stop(pmotor);
                    return;
                }
            }
            pmotor->ResetClk(pmotor->id);//置clk为低电平
            if (pmotor->id <4)
            {
                if(pmotor->clockwise == 1) //==pmotor->dir)
                {
                    pmotor->CurrentPosition_Pulse++;
                    if(pmotor->CurrentPosition_Pulse > pmotor->MaxPosition_Pulse)
                    {
                        pmotor->CurrentPosition_Pulse=0;
                    }
                }
                else
                {
                    pmotor->CurrentPosition_Pulse--;
                    if(pmotor->CurrentPosition_Pulse==0xffffffff)
                    {
                        pmotor->CurrentPosition_Pulse=pmotor->MaxPosition_Pulse-1;
                    }
                }
            }
            else
            {
                if(pmotor->clockwise == 0) //==pmotor->dir)
                {
                    pmotor->CurrentPosition_Pulse++;
                    if(pmotor->CurrentPosition_Pulse > pmotor->MaxPosition_Pulse)
                    {
                        pmotor->CurrentPosition_Pulse=0;
                    }
                }
                else
                {
                    pmotor->CurrentPosition_Pulse--;
                    if(pmotor->CurrentPosition_Pulse==0xffffffff)
                    {
                        pmotor->CurrentPosition_Pulse=pmotor->MaxPosition_Pulse-1;
                    }
                }
            }
            pmotor->CurrentPosition=pmotor->CurrentPosition_Pulse/pmotor->divnum; //根据脉冲数计算移动的位置

            if(pmotor->speedenbale&&(pmotor->CurrentIndex==pmotor->TargetIndex||pmotor->TargetIndex+pmotor->CurrentIndex==pmotor->StartTableLength+pmotor->StopTableLength-1))
            {
                return;
            }
            pmotor->PulsesHaven++;
            pmotor->pulsecount++;

            if(pmotor->RevetDot==pmotor->PulsesHaven)
            {
                pmotor->pulsecount=pmotor->Step_Table[pmotor->CurrentIndex];
            }
            if(pmotor->pulsecount>=pmotor->Step_Table[pmotor->CurrentIndex])
            {
                if(pmotor->PulsesHaven<=pmotor->StartSteps)
                {
                    if(pmotor->CurrentIndex<pmotor->StartTableLength-1)
                    {
                        pmotor->CurrentIndex++;
                        pmotor->pulsecount=0;
                        if(pmotor->CurrentIndex>=pmotor->StartTableLength) pmotor->CurrentIndex=pmotor->StartTableLength;
                    }
                }

                if((pmotor->PulsesGiven-pmotor->PulsesHaven<=pmotor->StopSteps&&pmotor->speedenbale==1)||
                    (pmotor->PulsesGiven-pmotor->PulsesHaven<=pmotor->StopSteps&&pmotor->speedenbale==0&&pmotor->PulsesHaven>=(pmotor->PulsesGiven>>1)))
                {
                    if(pmotor->CurrentIndex<pmotor->StartTableLength-1)
                    {
                        pmotor->CurrentIndex=pmotor->StartTableLength+pmotor->StopTableLength-pmotor->CurrentIndex;
                    }
                    pmotor->CurrentIndex++;
                    pmotor->pulsecount=0;
                    if(pmotor->CurrentIndex>=pmotor->StartTableLength+pmotor->StopTableLength)
                        pmotor->CurrentIndex=pmotor->StartTableLength+pmotor->StopTableLength-1;
                }
            }
            pmotor->timer_cnt=(pmotor->Counter_Table[pmotor->CurrentIndex])>>1;
            pmotor->TimerSet(pmotor->id, pmotor->timer_cnt);

            if(pmotor->PulsesHaven>=pmotor->PulsesGiven&&pmotor->PulsesHaven>3 || pmotor->status == MT_S_BREAK)
            {
                pmotor->en=0;
                pmotor->running=0;
                pmotor->CurrentIndex=0;
                MT_Stop(pmotor);
            }
            else
            {
                pmotor->Time_Cost_Act+=pmotor->Counter_Table[pmotor->CurrentIndex];
            }
        }
        else
        {
            MT_SetFlag(pmotor, MT_FLAG_CLK);//设置clk标志位为高
            pmotor->SetClk(pmotor->id);//置CLK为高电平
            pmotor->TimerSet(pmotor->id, pmotor->timer_cnt);//设置定时器，等待高电平计数周期完成
            if (pmotor->const_over_time > 0 && pmotor->remain_t == 0)
            {
                pmotor->remain_t = pmotor->const_over_time;
            }
        }
	}
}

/*根据S型曲线参数获取某个时刻的频率*/
float GetFreAtTime(float fstart,float faa,float taa,float tua,float tra,float t)
{
    //根据公式计算从开始到最高速过程中，t时刻的转动频率
    if(t>=0&&t<=taa){
        //加加速阶段
        return fstart+0.5*faa*t*t;
    }else if(taa<t&&t<=(taa+tua)){
        //匀加速阶段
        return fstart+0.5*faa*taa*taa+(t-taa)*faa*taa;
    }else if((taa+tua)<t&&t<=(taa+tua+tra)){
        //减加速阶段
        return fstart+0.5*faa*taa*taa+(tua)*faa*taa+0.5*faa*taa*tra-0.5*faa*taa*(taa+tua+tra-t)*(taa+tua+tra-t)/(tra);
    }
    return 0;
}

 /*计算S型曲线算法的每一步定时器周期及步数*/
void CalcMotorPeriStep_CPF(float fstart,float faa,float taa,float tua,float tra,uint16_t MotorTimeTable[],uint16_t MotorStepTable[])
{
    int  i;
    float fi;

    for(i=0;i<STEP_AA;i++)
    {
        fi=GetFreAtTime(fstart, faa, taa, tua, tra, taa/STEP_AA*i);
        MotorTimeTable[i]=(uint16_t)(F2TIME_PARA/fi);
        MotorStepTable[i]=(uint16_t)(fi*(taa/STEP_AA)/STEP_PARA);
    }
    for(i=STEP_AA;i<STEP_AA+STEP_UA;i++)
    {
        fi=GetFreAtTime(fstart, faa, taa, tua, tra, taa+(tua/STEP_UA)*(i-STEP_AA));
        MotorTimeTable[i]=(uint16_t)(F2TIME_PARA/fi);
        MotorStepTable[i]=(uint16_t)(fi*(tua/STEP_UA)/STEP_PARA);
    }
    for(i=STEP_AA+STEP_UA;i<STEP_AA+STEP_UA+STEP_RA;i++)
    {
        fi=GetFreAtTime(fstart, faa, taa, tua, tra, taa+tua+tra/STEP_RA*(i-STEP_AA-STEP_UA));
        MotorTimeTable[i]=(uint16_t)(F2TIME_PARA/fi);
        MotorStepTable[i]=(uint16_t)(fi*(tra/STEP_RA)/STEP_PARA);
    }
    fi=GetFreAtTime(fstart, faa, taa, tua, tra, taa+tua+tra);
    MotorTimeTable[STEP_AA+STEP_UA+STEP_RA]=(uint16_t)(F2TIME_PARA/fi);
    MotorStepTable[STEP_AA+STEP_UA+STEP_RA]=(uint16_t)(fi*(tra/STEP_RA)/STEP_PARA);

    for(i=STEP_AA+STEP_UA+STEP_RA+1;i<2*(STEP_AA+STEP_UA+STEP_RA)+1;i++)
    {
        MotorTimeTable[i]=MotorTimeTable[2*(STEP_AA+STEP_UA+STEP_RA)-i];
        MotorStepTable[i]=MotorStepTable[2*(STEP_AA+STEP_UA+STEP_RA)-i];
    }
}
/**************************************************************************************
电机运行参数初始化*/
void MotorRunParaInitial(void)
{ 
    /*FIXME:用户可改变该参数实现S型曲线的升降特性*/
    CalcMotorPeriStep_CPF(M_FRE_START,M_FRE_AA,M_T_AA,M_T_UA,M_T_RA,Motor1TimeTable,Motor1StepTable); 
    CalcMotorPeriStep_CPF(M_FRE_START,M_FRE_AA,M_T_AA,M_T_UA,M_T_RA,Motor2TimeTable,Motor2StepTable); 	
    CalcMotorPeriStep_CPF(M_FRE_START,M_FRE_AA,M_T_AA,M_T_UA,M_T_RA,Motor3TimeTable,Motor3StepTable);
    CalcMotorPeriStep_CPF(M_FRE_START,M_FRE_AA,M_T_AA,M_T_UA,M_T_RA,Motor4TimeTable,Motor4StepTable); 
    CalcMotorPeriStep_CPF(M_FRE_START,M_FRE_AA,M_T_AA,M_T_UA,M_T_RA,Motor5TimeTable,Motor5StepTable); 	
    CalcMotorPeriStep_CPF(M_FRE_START,M_FRE_AA,M_T_AA,M_T_UA,M_T_RA,Motor6TimeTable,Motor6StepTable);

    /*更改参数降为2/3生成的表格*/
    CalcMotorPeriStep_CPF(M_FRE_START*2.0/3,M_FRE_AA*2.0/3,M_T_AA*2.0/3,M_T_UA*2.0/3,M_T_RA*2.0/3,Motor1_23TimeTable,Motor1_23StepTable);
    CalcMotorPeriStep_CPF(M_FRE_START*2.0/3,M_FRE_AA*2.0/3,M_T_AA*2.0/3,M_T_UA*2.0/3,M_T_RA*2.0/3,Motor2_23TimeTable,Motor2_23StepTable);
    CalcMotorPeriStep_CPF(M_FRE_START*2.0/3,M_FRE_AA*2.0/3,M_T_AA*2.0/3,M_T_UA*2.0/3,M_T_RA*2.0/3,Motor3_23TimeTable,Motor3_23StepTable);
    CalcMotorPeriStep_CPF(M_FRE_START*2.0/3,M_FRE_AA*2.0/3,M_T_AA*2.0/3,M_T_UA*2.0/3,M_T_RA*2.0/3,Motor4_23TimeTable,Motor4_23StepTable);
    CalcMotorPeriStep_CPF(M_FRE_START*2.0/3,M_FRE_AA*2.0/3,M_T_AA*2.0/3,M_T_UA*2.0/3,M_T_RA*2.0/3,Motor5_23TimeTable,Motor5_23StepTable);
    CalcMotorPeriStep_CPF(M_FRE_START*2.0/3,M_FRE_AA*2.0/3,M_T_AA*2.0/3,M_T_UA*2.0/3,M_T_RA*2.0/3,Motor6_23TimeTable,Motor6_23StepTable);

    /*更改参数降为1/3生成的表格*/
    CalcMotorPeriStep_CPF(M_FRE_START*1.0/3,M_FRE_AA*1.0/3,M_T_AA*1.0/3,M_T_UA*1.0/3,M_T_RA*1.0/3,Motor1_13TimeTable,Motor1_13StepTable);
    CalcMotorPeriStep_CPF(M_FRE_START*1.0/3,M_FRE_AA*1.0/3,M_T_AA*1.0/3,M_T_UA*1.0/3,M_T_RA*1.0/3,Motor2_13TimeTable,Motor2_13StepTable);
    CalcMotorPeriStep_CPF(M_FRE_START*1.0/3,M_FRE_AA*1.0/3,M_T_AA*1.0/3,M_T_UA*1.0/3,M_T_RA*1.0/3,Motor3_13TimeTable,Motor3_13StepTable);
    CalcMotorPeriStep_CPF(M_FRE_START*1.0/3,M_FRE_AA*1.0/3,M_T_AA*1.0/3,M_T_UA*1.0/3,M_T_RA*1.0/3,Motor4_13TimeTable,Motor4_13StepTable);
    CalcMotorPeriStep_CPF(M_FRE_START*1.0/3,M_FRE_AA*1.0/3,M_T_AA*1.0/3,M_T_UA*1.0/3,M_T_RA*1.0/3,Motor5_13TimeTable,Motor5_13StepTable);
    CalcMotorPeriStep_CPF(M_FRE_START*1.0/3,M_FRE_AA*1.0/3,M_T_AA*1.0/3,M_T_UA*1.0/3,M_T_RA*1.0/3,Motor6_13TimeTable,Motor6_13StepTable);
}

/*************************************************************
计算S型曲线反转点，S型曲线在运行时，加减速过程是完全相对的
***********************************************************/
unsigned long long Get_TimeCost_ReverDot_S(unsigned char MotorID)
{
	unsigned long long time_cost=0;
	unsigned long long time_cost2=0;
	unsigned int pulsecnt=0;
	int i=0,j;
	mt_t *pmotor=NULL; 
    pmotor = &mt_array[MotorID];

    if(pmotor->PulsesGiven>=pmotor->StartSteps+pmotor->StopSteps)
    {
        for(i=0;i<pmotor->StartTableLength;i++)
            time_cost+=(pmotor->Step_Table[i]*pmotor->Counter_Table[i]);
        for(i=0;i<pmotor->StopTableLength;i++)
            time_cost+=(pmotor->Step_Table[i+pmotor->StartTableLength]*pmotor->Counter_Table[i+pmotor->StartTableLength]);
        time_cost+=(pmotor->PulsesGiven-pmotor->StartSteps-pmotor->StopSteps)*pmotor->Counter_Table[pmotor->StartTableLength-1];
        pmotor->RevetDot=pmotor->PulsesGiven-pmotor->StopSteps;
    }
    else
    {
        while((pulsecnt+pmotor->Step_Table[i])<=(pmotor->PulsesGiven>>1))
        {
            time_cost+=(pmotor->Step_Table[i]*pmotor->Counter_Table[i]);
            time_cost2+=(pmotor->Step_Table[i]*pmotor->Counter_Table[i]);
            pulsecnt+=pmotor->Step_Table[i];
            i++;
        }
        time_cost+=time_cost2;
        if(pmotor->Step_Table[i]<pmotor->PulsesGiven-2*pulsecnt)
        {
            pmotor->Step_Table[i]=pmotor->PulsesGiven-2*pulsecnt;
            pmotor->StartSteps=0;
            pmotor->StopSteps=0;
            for(j=0;j<pmotor->StartTableLength;j++)
                pmotor->StartSteps+=pmotor->Step_Table[j];
            for(j=0;j<pmotor->StopTableLength;j++)
                pmotor->StopSteps+=pmotor->Step_Table[j+pmotor->StartTableLength];
        }
        time_cost+=(pmotor->Counter_Table[i]*(pmotor->PulsesGiven-2*pulsecnt));
        pmotor->RevetDot=pmotor->PulsesGiven-pulsecnt;
    }
    pmotor->Time_Cost_Cal=time_cost;
    return time_cost;
}

/*******************************************************
重新初始化电机运行时相关参数
***********************************************************/
void Motor_Reinitial(unsigned char MotorID)
{
    int i=0;
    mt_t *pmotor=NULL;
    uint16_t *MotorTimeTable;
    uint16_t *MotorStepTable;
    uint16_t *MotorTime23Table;
    uint16_t *MotorStep23Table;
    uint16_t *MotorTime13Table;
    uint16_t *MotorStep13Table;

    pmotor = &mt_array[MotorID];
    switch(MotorID)
    {
        case 0:
            MotorTimeTable=Motor1TimeTable;
            MotorStepTable=Motor1StepTable;
            MotorTime23Table=Motor1_23TimeTable;
            MotorStep23Table=Motor1_23StepTable;
            MotorTime13Table=Motor1_13TimeTable;
            MotorStep13Table=Motor1_13StepTable;
            break;
        case 1:
            MotorTimeTable=Motor2TimeTable;
            MotorStepTable=Motor2StepTable;
            MotorTime23Table=Motor2_23TimeTable;
            MotorStep23Table=Motor2_23StepTable;
            MotorTime13Table=Motor2_13TimeTable;
            MotorStep13Table=Motor2_13StepTable;
            break;
        case 2:
            MotorTimeTable=Motor3TimeTable;
            MotorStepTable=Motor3StepTable;
            MotorTime23Table=Motor3_23TimeTable;
            MotorStep23Table=Motor3_23StepTable;
            MotorTime13Table=Motor3_13TimeTable;
            MotorStep13Table=Motor3_13StepTable;
            break;
        case 3:
            MotorTimeTable=Motor4TimeTable;
            MotorStepTable=Motor4StepTable;
            MotorTime23Table=Motor4_23TimeTable;
            MotorStep23Table=Motor4_23StepTable;
            MotorTime13Table=Motor4_13TimeTable;
            MotorStep13Table=Motor4_13StepTable;
            break;
        case 4:
            MotorTimeTable=Motor5TimeTable;
            MotorStepTable=Motor5StepTable;
            MotorTime23Table=Motor5_23TimeTable;
            MotorStep23Table=Motor5_23StepTable;
            MotorTime13Table=Motor5_13TimeTable;
            MotorStep13Table=Motor5_13StepTable;
            break;
        case 5:
            MotorTimeTable=Motor6TimeTable;
            MotorStepTable=Motor6StepTable;
            MotorTime23Table=Motor6_23TimeTable;
            MotorStep23Table=Motor6_23StepTable;
            MotorTime13Table=Motor6_13TimeTable;
            MotorStep13Table=Motor6_13StepTable;
            break;
        default:
            return ;
    }
#if DEBUG_S
    for(i=0;i<pmotor->StartTableLength;i++)
        startSteps += MotorStepTable[i];
    for(i=0;i<pmotor->StopTableLength;i++)
        StopSteps += MotorStepTable[i+pmotor->StartTableLength];
    printf("start:%d + stop:%d = %d\n", startSteps, StopSteps, startSteps+StopSteps);

    startSteps = 0; StopSteps = 0;
    for(i=0;i<pmotor->StartTableLength;i++)
        startSteps += Motor2_23StepTable[i];
    for(i=0;i<pmotor->StopTableLength;i++)
        StopSteps += Motor2_23StepTable[i+pmotor->StartTableLength];
    printf("start:%d + stop:%d = %d\n", startSteps, StopSteps, startSteps+StopSteps);

    startSteps = 0; StopSteps = 0;
    for(i=0;i<pmotor->StartTableLength;i++)
        startSteps += Motor2_13StepTable[i];
    for(i=0;i<pmotor->StopTableLength;i++)
        StopSteps += Motor2_13StepTable[i+pmotor->StartTableLength];
    printf("start:%d + stop:%d = %d\n", startSteps, StopSteps, startSteps+StopSteps);
#endif
    pmotor->pulsecount=0;
    pmotor->CurrentIndex=0;
    pmotor->speedenbale=0;

    pmotor->Counter_Table=MotorTimeTable;
    pmotor->Step_Table=MotorStepTable;
    pmotor->StartSteps=0;                  //必须清零，后面是累加
    pmotor->StopSteps=0;                   //同上
    for(i=0;i<pmotor->StartTableLength;i++)
        pmotor->StartSteps+=pmotor->Step_Table[i];
    for(i=0;i<pmotor->StopTableLength;i++)
        pmotor->StopSteps+=pmotor->Step_Table[i+pmotor->StartTableLength];
    if(pmotor->PulsesGiven<pmotor->StartSteps+pmotor->StopSteps)
    {
        //如果给定的运行步数小于最大S型曲线，则尝试选择2/3曲线
        pmotor->Counter_Table=MotorTime23Table;
        pmotor->Step_Table=MotorStep23Table;
        pmotor->StartSteps=0;
        pmotor->StopSteps=0;
        for(i=0;i<pmotor->StartTableLength;i++)
            pmotor->StartSteps+=pmotor->Step_Table[i];
        for(i=0;i<pmotor->StopTableLength;i++)
            pmotor->StopSteps+=pmotor->Step_Table[i+pmotor->StartTableLength];
        if(pmotor->PulsesGiven<pmotor->StartSteps+pmotor->StopSteps)
        {
            //如果给定的运行步数小于最大S型曲线，则尝试选择1/3曲线
            pmotor->Counter_Table=MotorTime13Table;
            pmotor->Step_Table=MotorStep13Table;
            pmotor->StartSteps=0;
            pmotor->StopSteps=0;
            for(i=0;i<pmotor->StartTableLength;i++)
                pmotor->StartSteps+=pmotor->Step_Table[i];
            for(i=0;i<pmotor->StopTableLength;i++)
                pmotor->StopSteps+=pmotor->Step_Table[i+pmotor->StartTableLength];
        }
    }
    Get_TimeCost_ReverDot_S(MotorID);
}

/**************************************************************************************
启动电机按照S型曲线参数运行
*********************************************************************************/
int Start_Motor_S(unsigned char MotorID,unsigned char dir,unsigned int Degree)
{
    unsigned int PulsesGiven=0;
	mt_t *pmotor=NULL;

    if (MotorID >= MT_NUMS || Degree == 0)
    {
        Log("motor id is invalid, id = %d， Degree = %d\n", MotorID, Degree);
        return MT_ERR_PARAM;
    }

    pmotor = &mt_array[MotorID];
    if (dir)
        pmotor->ClkDir(pmotor->id);
    else
        pmotor->AntiDir(pmotor->id);
	pmotor->en=1;
	pmotor->dir=dir;
	pmotor->running=1;
	pmotor->PulsesHaven=0;
	PulsesGiven=Degree;
	pmotor->Time_Cost_Act=0;
	pmotor->PulsesGiven=PulsesGiven*pmotor->divnum;
//	Motor_Reinitial(MotorID);
	pmotor->CurrentIndex=0;
	pmotor->speedenbale=0;
    pmotor->LPWDis(pmotor->id);
    pmotor->ResetClk(pmotor->id);
    MT_ClrFlag(pmotor, MT_FLAG_TRIG);
    MT_ClrFlag(pmotor, MT_FLAG_CLK);
    pmotor->timer_cnt = pmotor->Counter_Table[0]>>1;
    pmotor->TimerStart(pmotor->id, pmotor->timer_cnt);

    return MT_ERR_NONE;
}
int Find_BestTimeCost(unsigned char ID,unsigned long long time_cost,unsigned char dir,unsigned int Degree);
 /**************************************************************************************
 初始化电机的参数，电机ID， 细分数， 电机运行的最大位置
 **************************************************************************************/
void MT_Init(unsigned char MotorID, unsigned char StepDive, u16_t rst_ppos, u16_t rst_npos, unsigned int maxposition)
{
    unsigned int i=0;
    mt_t *pmotor=NULL;
    uint16_t *MotorTimeTable;
    uint16_t *MotorStepTable;

    pmotor=&mt_array[MotorID];
    pmotor->id = MotorID;
    pmotor->sns_id = MotorID;
    pmotor->sns_trigger_cnt = 0;
    pmotor->rst_ppos = rst_ppos;
    pmotor->rst_npos = rst_npos;
    pmotor->clockwise=M1_CLOCKWISE;
    pmotor->flag = 0;
    pmotor->status = MT_S_WAIT;
    switch(MotorID)
    {
        case 0:
            MotorTimeTable = Motor1TimeTable;
            MotorStepTable = Motor1StepTable;
            break;
        case 1:
            MotorTimeTable = Motor2TimeTable;
            MotorStepTable = Motor2StepTable;
            break;
        case 2:
            MotorTimeTable = Motor3TimeTable;
            MotorStepTable = Motor3StepTable;
            break;
        case 3:
            MotorTimeTable = Motor4TimeTable;
            MotorStepTable = Motor4StepTable;
            break;
        case 4:
            MotorTimeTable = Motor5TimeTable;
            MotorStepTable = Motor5StepTable;
            break;
        case 5:
            MotorTimeTable = Motor6TimeTable;
            MotorStepTable = Motor6StepTable;
            break;
        default:break;
    }

    AOS_SemInit(&(pmotor->sem), 0);
    pmotor->divnum=StepDive;
    pmotor->MaxPosition=maxposition;
    pmotor->MaxPosition_Pulse=maxposition*StepDive;

    pmotor->CurrentPosition=0;
    pmotor->CurrentPosition_Pulse=0;
    pmotor->StartTableLength=STEP_AA+STEP_UA+STEP_RA+1;
    pmotor->StopTableLength=STEP_AA+STEP_UA+STEP_RA;
    pmotor->Counter_Table=MotorTimeTable;
    pmotor->Step_Table=MotorStepTable;

    pmotor->CurrentIndex=0;
    pmotor->speedenbale=0;
    pmotor->StartSteps=0;
    pmotor->StopSteps=0;
    for(i=0;i<pmotor->StartTableLength;i++)
        pmotor->StartSteps+=pmotor->Step_Table[i];
    for(i=0;i<pmotor->StopTableLength;i++)
        pmotor->StopSteps+=pmotor->Step_Table[i+pmotor->StartTableLength];
}

/**************************************************************************************
根据时间，步数，重新计算S型曲线参数
*********************************************************************************/
int Find_BestTimeCost(unsigned char ID,unsigned long long time_cost,unsigned char dir,unsigned int Degree)
{
    int id=ID;
    float i=0,j=0;
    float fi=M_FRE_START,fj=M_FRE_START;
    int cal_ij=1,step_aa;
    unsigned int PulsesGiven=0;
    uint16_t *MotorTimeTable;
    uint16_t *MotorStepTable;
    mt_t *pmotor=NULL;
    unsigned long long time_cost_tmp=0;
    unsigned long long time_cost_min=0xffffffffff;
    float i_o=0,j_o=0;
    float fi_o=0,fj_o=0;
    int tmp = 0;
    
    step_aa=STEP_AA;
    if (ID >= MT_NUMS || Degree == 0 || time_cost == 0)
        return MT_ERR_PARAM;

    pmotor=&mt_array[ID];
    Motor_Reinitial(id);
    MotorTimeTable=pmotor->Counter_Table;
    MotorStepTable=pmotor->Step_Table;
    pmotor->en=1;
    pmotor->dir=dir;
    pmotor->running=1;
    pmotor->PulsesHaven=0;
    PulsesGiven=Degree;
    pmotor->Time_Cost_Act=0;
    pmotor->PulsesGiven=PulsesGiven*pmotor->divnum;

    if (id == 4)
    {
        for (tmp = 0; tmp<MotorTableLength; tmp++)
        {
            MotorTimeTable[tmp] = M5_STATIC_RATE; //6:采样电机最快，对其他电机影响大，采样电机运动时其他电机不运动；120：采样电机慢，对其他电机影响小。
            MotorStepTable[tmp] = pmotor->PulsesGiven/MotorTableLength;
        }
        pmotor->StartSteps=0;
        pmotor->StopSteps=0;
        for(tmp=0;tmp<pmotor->StartTableLength;tmp++)
            pmotor->StartSteps+=pmotor->Step_Table[tmp];
        for(tmp=0;tmp<pmotor->StopTableLength;tmp++)
            pmotor->StopSteps+=pmotor->Step_Table[tmp+pmotor->StartTableLength];
        Get_TimeCost_ReverDot_S(ID);
        return MT_ERR_NONE;
    }
    else if (id == 5)
    {
        for (tmp = 0; tmp<MotorTableLength; tmp++)
        {
            MotorTimeTable[tmp] = M6_STATIC_RATE;
            MotorStepTable[tmp] = pmotor->PulsesGiven/MotorTableLength;
        }
        pmotor->StartSteps=0;
        pmotor->StopSteps=0;
        for(tmp=0;tmp<pmotor->StartTableLength;tmp++)
            pmotor->StartSteps+=pmotor->Step_Table[tmp];
        for(tmp=0;tmp<pmotor->StopTableLength;tmp++)
            pmotor->StopSteps+=pmotor->Step_Table[tmp+pmotor->StartTableLength];
        Get_TimeCost_ReverDot_S(ID);
        return MT_ERR_NONE;
    }
    else if ((time_cost/pmotor->PulsesGiven < 2550) || (time_cost/pmotor->PulsesGiven > 56000))
    {
        if ((time_cost/pmotor->PulsesGiven<50) || (time_cost/pmotor->PulsesGiven >= 0xffffffff))
        {
            Log("%s-%d:[ERROR]Motor outof range", filename(__FILE__), __LINE__);
//            for (tmp = 0; tmp<MotorTableLength; tmp++)
//            {
//                MotorTimeTable[tmp] = 100;
//                MotorStepTable[tmp] = pmotor->PulsesGiven/MotorTableLength;
//            }
            return MT_ERR_PARAM;
        }
        else if (time_cost/pmotor->PulsesGiven >= 0xffff)
        {
            pmotor->remain_t = time_cost/pmotor->PulsesGiven - 0xffff;
            pmotor->const_over_time = pmotor->remain_t;
            for (tmp = 0; tmp<MotorTableLength; tmp++)
            {
                MotorTimeTable[tmp] = 0xffff;
                MotorStepTable[tmp] = pmotor->PulsesGiven/MotorTableLength;
            }
        }
        else
        {
//           if( pmotor->PulsesGiven>600)
//           { 
            for (tmp = 0; tmp<MotorTableLength; tmp++)
            {
              if(tmp<2*STEP_AA)
              {
                MotorTimeTable[tmp] = time_cost/pmotor->PulsesGiven+1*(step_aa-tmp);
                MotorStepTable[tmp] = pmotor->PulsesGiven/MotorTableLength;
//                Log("ID=%d, MotorTimeTable[%d]=%d , MotorStepTable[%d]=%d \r\n",pmotor->id,tmp,MotorTimeTable[tmp],MotorStepTable[tmp]);
              }
              else if(tmp>=2*STEP_AA&&tmp<2*STEP_AA+2*STEP_UA)
              {
                if(time_cost/pmotor->PulsesGiven>step_aa+20)
                {
                  MotorTimeTable[tmp] = time_cost/pmotor->PulsesGiven-step_aa-10;
                }
//                else if(time_cost/pmotor->PulsesGiven>step_aa+10)
//                {
//                  MotorTimeTable[tmp] = time_cost/pmotor->PulsesGiven-step_aa;
//                }
                else 
                {
                  MotorTimeTable[tmp] = time_cost/pmotor->PulsesGiven-step_aa;
                }
//                MotorTimeTable[tmp] = time_cost/pmotor->PulsesGiven-20;
                MotorStepTable[tmp] = pmotor->PulsesGiven/MotorTableLength;    
//                Log("ID=%d, MotorTimeTable[%d]=%d , MotorStepTable[%d]=%d \r\n",pmotor->id,tmp,MotorTimeTable[tmp],MotorStepTable[tmp]);
              }
              else 
              {
                MotorTimeTable[tmp] = time_cost/pmotor->PulsesGiven+1*(tmp-(5*step_aa));
                MotorStepTable[tmp] = pmotor->PulsesGiven/MotorTableLength;  
//                Log("ID=%d, MotorTimeTable[%d]=%d , MotorStepTable[%d]=%d \r\n",pmotor->id,tmp,MotorTimeTable[tmp],MotorStepTable[tmp]);
              }
//                MotorTimeTable[tmp] = time_cost/pmotor->PulsesGiven-20;
//                MotorStepTable[tmp] = pmotor->PulsesGiven/MotorTableLength;
//                Log("ID=%d, MotorTimeTable[%d]=%d \r\n",pmotor->id,tmp,MotorTimeTable[tmp]);
            }
//           }
//           else
//           {
//             for (tmp = 0; tmp<MotorTableLength; tmp++)
//             {
//               MotorTimeTable[tmp] = time_cost/pmotor->PulsesGiven-20;
//               MotorStepTable[tmp] = pmotor->PulsesGiven/MotorTableLength;
//             }
//           }
        }
        pmotor->StartSteps=0;
        pmotor->StopSteps=0;
        for(tmp=0;tmp<pmotor->StartTableLength;tmp++)
            pmotor->StartSteps+=pmotor->Step_Table[tmp];
        for(tmp=0;tmp<pmotor->StopTableLength;tmp++)
            pmotor->StopSteps+=pmotor->Step_Table[tmp+pmotor->StartTableLength];
        Get_TimeCost_ReverDot_S(ID);
        return MT_ERR_NONE;
    }

    j=M_FRE_AA;
    i=0;
    while(1)
    {
        if(cal_ij)
        {
            CalcMotorPeriStep_CPF(M_FRE_START,(i+j)/2.0,M_T_AA,M_T_UA,M_T_RA,MotorTimeTable,MotorStepTable);
        }
        else
        {
            CalcMotorPeriStep_CPF((fi+fj)/2,0,M_T_AA,M_T_UA,M_T_RA,MotorTimeTable,MotorStepTable);
        }
        pmotor->en=1;
        pmotor->dir=dir;
        pmotor->running=1;
        pmotor->PulsesHaven=0;
        PulsesGiven=Degree;
        pmotor->Time_Cost_Act=0;
        pmotor->PulsesGiven=PulsesGiven*pmotor->divnum;
        //pmotor->PulsesGiven+=300;
        Motor_Reinitial(id);
        //time_cost_tmp=Get_Time_Cost2(id);
        time_cost_tmp=pmotor->Time_Cost_Cal;
        if(time_cost_tmp<time_cost)
        {
            if(time_cost-time_cost_tmp<time_cost_min)
            {
                time_cost_min=time_cost-time_cost_tmp;
                i_o=i;
                j_o=j;
                fi_o=fi;
                fj_o=fj;
            }
        }
        else
        {
            if(time_cost_tmp-time_cost<time_cost_min)
            {
                time_cost_min=time_cost_tmp-time_cost;
                i_o=i;
                j_o=j;
                fi_o=fi;
                fj_o=fj;
            }
        }
        if(time_cost_tmp>=time_cost-32*4&&time_cost_tmp<=time_cost+32*4)
        {
            break ;
        }
        if(cal_ij)
        {
            if(j<0.1)
            {
                //说明即使是使用最基本的启动速度都无法满足，则修改为降低速度的梯形
                i_o=0;
                j_o=0;
                j=0;
                i=0;
                cal_ij=0;
                fi=0;
            }
        }
        if(cal_ij)
        {
            if((i>j&&i-j<0.02||(i<j)&&j-i<0.02))
            {
                break;
            }
            if(time_cost_tmp>time_cost)
            {
                i=(i+j)/2.0;
            }
            else
            {
                j=(i+j)/2.0; 
            }
        }
        else
        {
            if((fi>fj&&fi-fj<0.02||(fi<fj)&&fj-fi<0.02))
            {
                break;
            }
            if(time_cost_tmp>time_cost)
            {
                fi=(fi+fj)/2.0;
            }
            else
            {
                fj=(fi+fj)/2.0; 
            }
        }
    }
    CalcMotorPeriStep_CPF((fi_o+fj_o)/2,(i_o+j_o)/2.0,M_T_AA,M_T_UA,M_T_RA,MotorTimeTable,MotorStepTable);
    return MT_ERR_NONE;
}

u8_t SetPosition(unsigned char MotorID, unsigned int dest)
{
	mt_t *pmotor_s=NULL;
    u8_t ret = MT_ERR_NONE;

    if (MotorID >= MT_NUMS)
    {
         Log("%s-%d, ERROR, mt id(%d) is wrong\n", filename(__FILE__), __LINE__, MotorID);
        return MT_ERR_PARAM;
    }

    pmotor_s = &mt_array[MotorID];
    if(dest<=pmotor_s->MaxPosition && dest != 0)
    {
        ret = Start_Motor_S(MotorID, pmotor_s->dir, dest);
        if (ret != MT_ERR_NONE)
        {
            Log("%s-%d, ERROR, start motor failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
            return ret;
        }
        ret = MT_Wait(pmotor_s, pmotor_s->time_give /*0xffffffpmotor_s->Time_Cost_Cal*1000/MT_TIMER_CLK + 1000*/);
        if (ret != MT_ERR_NONE)
            Log("%s-%d, ERROR, wait motor failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
    }
    else
    {
        Log("%s-%d, ERROR, dest is error, dest=%d\n", filename(__FILE__), __LINE__, dest);
        return MT_ERR_PARAM;
    }

    return ret;
}

u8_t MT_Home(mt_t *mt, u32_t speed)
{
    u8_t ret = MT_ERR_NONE;
    
    if (!mt || mt->id > MT_NUMS)
        return MT_ERR_PARAM;

    ret = MT_MoveTo(mt, 0, mt->CurrentPosition/speed);
    if ((ret != MT_ERR_NONE) && (ret != MT_ERR_BREAK))
        return MT_ERR_BUSY;

    return MT_ERR_NONE;
}

/*复位电机*/
u8_t MT_Rst_Internal(mt_t *mt, u32_t speed);
u8_t MT_Rst(mt_t *mt, u32_t speed)
{
    u8_t ret = MT_ERR_NONE;
    int i = 0;
	
	//-----------------屏蔽未使用的通道电机
    if(mt->id==2 || mt->id ==3 || mt->id ==5)
    {
      return MT_ERR_NONE;
    }	

//    for (i = 0; i<10; i++)
    for (i = 0; i<3; i++)
    {
        ret = MT_Rst_Internal(mt, speed);
        if (ret == MT_ERR_NONE || ret == MT_ERR_BREAK)
            break;
        Log("%s-%d ERROR, mt(%d) reset error, retry %d times\n", filename(__FILE__), __LINE__, mt->id, i);
    }
    if (i == 10)
    {
        Log("%s-%d ERROR, mt(%d) reset error\n", filename(__FILE__), __LINE__, mt->id);
    }
    return ret;
}

u8_t MT_Rst_Internal(mt_t *mt, u32_t speed)
{
    u8_t ret = MT_ERR_NONE;
    unsigned long long cost_time = 0;
    unsigned int reset_position = mt->MaxPosition;

    if (mt == NULL)
    {
        Log("%s-%d ERROR, mt is null\n", filename(__FILE__), __LINE__);
        return MT_ERR_PARAM;
    }
    mt->time_give = speed;
    if(mt->id==4)
    {
      mt->time_give = 2*speed;
    }
    if (mt->IsSNSValid(mt->sns_id) == true)
    {
        if (mt->id < 4)
        {
//            cost_time = (unsigned long long)speed*F2TIME_PARA/5000;
            cost_time = (unsigned long long)speed*F2TIME_PARA/5000;
            ret = Find_BestTimeCost(mt->id, cost_time, M1_UNCLOCKWISE, MOTOR_RESET_POSITION_0);
            if (ret != MT_ERR_NONE)
            {
                Log("%s-%d ERROR, find bast time cost failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
                return ret;
            }
            mt->clockwise = 1;
            mt->dir = 1;
            ret = SetPosition(mt->id, MOTOR_RESET_POSITION_0);
            reset_position = MOTOR_RESET_POSITION_0+mt->rst_npos;
            AOS_TaskDelay(100);
        }
        else if(mt->id == 4)
        {
//            cost_time = (unsigned long long)speed*F2TIME_PARA/1000;
            cost_time = (unsigned long long)speed*F2TIME_PARA/2000;
            ret = Find_BestTimeCost(mt->id, cost_time, M1_CLOCKWISE, MOTOR_RESET_POSITION_4);
            if (ret != MT_ERR_NONE)
            {
                Log("%s-%d ERROR, find bast time cost failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
                return ret;
            }
            mt->clockwise = 0;
            mt->dir = 0;
            ret = SetPosition(mt->id, MOTOR_RESET_POSITION_4);
            reset_position = MOTOR_RESET_POSITION_4+mt->rst_npos;
            AOS_TaskDelay(100);
        }
        else if(mt->id == 5)
        {
            cost_time = (unsigned long long)speed*F2TIME_PARA/1000/2;
            ret = Find_BestTimeCost(mt->id, cost_time, M1_CLOCKWISE, MOTOR_RESET_POSITION_5);
            if (ret != MT_ERR_NONE)
            {
                Log("%s-%d ERROR, find bast time cost failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
                return ret;
            }
            mt->clockwise = 0;
            mt->dir = 0;
            ret = SetPosition(mt->id, MOTOR_RESET_POSITION_5);
            reset_position = mt->MaxPosition;
            AOS_TaskDelay(100);
        }
        if(mt->status == MT_S_BREAK)
        {
            mt->status = MT_S_WAIT;
            Log("%s-%d DEBUG, motor(%d) break\n", filename(__FILE__), __LINE__, mt->id);
            ret = MT_ERR_BREAK;
            return ret;
        }
        if ((mt->PulsesGiven - 3) > mt->PulsesHaven)
        {
            Log("%s-%d ERROR, motor(%d) reset error\n", filename(__FILE__), __LINE__, mt->id);
            return MT_ERR_RISK;
        }

        if(mt->IsSNSValid(mt->sns_id) == true)
        {
            Log("%s-%d ERROR, mt(%d) reset sns error, pulsegiven:%d, pulsehave:%d, rst_npos:%d, trigger_cnt:%d\n",
                filename(__FILE__), __LINE__, mt->id, mt->PulsesGiven, mt->PulsesHaven, mt->rst_npos, mt->trigger_cnt);
            return MT_ERR_SNS;
        }
    }
    if (mt->id < 4)
    {
        cost_time = (unsigned long long)speed*F2TIME_PARA/1000;
        ret = Find_BestTimeCost(mt->id, cost_time, M1_CLOCKWISE, reset_position);
        if (ret != MT_ERR_NONE)
        {
            Log("%s-%d ERROR, find bast time cost failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
            return ret;
        }
        mt->clockwise = 0;
        mt->dir = 0;
        ret = SetPosition(mt->id, reset_position);
    }
    else if (mt->id == 4)
    {
        cost_time = (unsigned long long)speed*F2TIME_PARA/2000;
        ret = Find_BestTimeCost(mt->id, cost_time, M1_UNCLOCKWISE, reset_position);
        if (ret != MT_ERR_NONE)
        {
            Log("%s-%d ERROR, find bast time cost failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
            return ret;
        }
        mt->clockwise = 1;
        mt->dir = 1;
        ret = SetPosition(mt->id, reset_position);
    }
    else if (mt->id == 5)
    {
        cost_time = (unsigned long long)speed*F2TIME_PARA/1000;
        ret = Find_BestTimeCost(mt->id, cost_time, M1_UNCLOCKWISE, reset_position);
        if (ret != MT_ERR_NONE)
        {
            Log("%s-%d ERROR, find bast time cost failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
            return ret;
        }
        mt->clockwise = 1;
        mt->dir = 1;
        ret = SetPosition(mt->id, reset_position);
    }
    if(mt->status == MT_S_BREAK)
    {
        mt->status = MT_S_WAIT;
        Log("%s-%d DEBUG, motor(%d) break\n", filename(__FILE__), __LINE__, mt->id);
        ret = MT_ERR_BREAK;
        return ret;
    }
    if (mt->sns_trigger_cnt != mt->rst_npos)
    {
        Log("%s-%d ERROR, motor(%d) reset error\n", filename(__FILE__), __LINE__, mt->id);
        return MT_ERR_RISK;
    }

    if(mt->IsSNSValid(mt->sns_id) != true)
    {
        Log("%s-%d ERROR, mt(%d) reset sns error, pulsegiven:%d, pulsehave:%d, rst_npos:%d, trigger_cnt:%d\n",
            filename(__FILE__), __LINE__, mt->id, mt->PulsesGiven, mt->PulsesHaven, mt->rst_npos, mt->trigger_cnt);
        return MT_ERR_SNS;
    }

    return ret;
}

u8_t MT_MoveTo(mt_t *mt, u32_t pos2, u32_t ms)
{
    u8_t ret = MT_ERR_NONE;
    unsigned long long time_cost = 0;
    u32_t dest = 0;
    u32_t pos = 0;
    
    if(mt->id==2 || mt->id ==3 || mt->id ==5)
    {
      return MT_ERR_NONE;
    }
    
    if (mt == NULL || ms<10)
        return MT_ERR_PARAM;

    switch (mt->id)
    {
        case 0:
            pos = pos2/M1DIV;
            break;
        case 1:
            pos = pos2/M2DIV;
            break;
        case 2:
            pos = pos2/M3DIV;
            break;
        case 3:
            pos = pos2/M4DIV;
            break;
        case 4:
            pos = pos2/M5DIV;
            break;
        case 5:
            pos = pos2/M6DIV;
            break;
        default:
            Log("%s-%d,ERROR: Motor id is wrong\n", filename(__FILE__), __LINE__);
            return MT_ERR_PARAM;
    }
    if(pos == mt->CurrentPosition)
    {
        ret = MT_ERR_NONE;
        return ret;
    }
//    Log("id:%d, pos=%d, ms = %d\n", mt->id, pos, ms);
    mt->time_give = ms;
    time_cost = (unsigned long long)(ms-10)*MT_TIMER_CLK*MT_ADAPT_VALUE/1000;
    if(pos <= mt->MaxPosition)
    {
        if (mt->id == 5) //直线通道(正向运动，dir=false
        {
            if (pos > mt->CurrentPosition)
            {
                dest = pos - mt->CurrentPosition;
                mt->clockwise = 0;
                mt->dir = 0;
                ret = Find_BestTimeCost(mt->id, time_cost, M1_CLOCKWISE, dest);
                if (ret != MT_ERR_NONE)
                {
                    Log("%s-%d ERROR, find bast time cost failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
                    return ret;
                }
            }
            else
            {
                dest = mt->CurrentPosition - pos;
                mt->clockwise = 1;
                mt->dir = 1;
                ret = Find_BestTimeCost(mt->id, time_cost, M1_UNCLOCKWISE, dest);
                if (ret != MT_ERR_NONE)
                {
                    Log("%s-%d ERROR, find bast time cost failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
                    return ret;
                }
            }
        }
        if (mt->id == 4)
        {
            if (pos > mt->CurrentPosition)
            {
                dest = pos - mt->CurrentPosition;
                mt->clockwise = 0;
                mt->dir = 0;
                ret = Find_BestTimeCost(mt->id, time_cost, M1_CLOCKWISE, dest);
                if (ret != MT_ERR_NONE)
                {
                    Log("%s-%d ERROR, find bast time cost failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
                    return ret;
                }
            }
            else
            {
                dest = mt->CurrentPosition - pos;
                mt->clockwise = 1;
                mt->dir = 1;
                ret = Find_BestTimeCost(mt->id, time_cost, M1_UNCLOCKWISE, dest);
                if (ret != MT_ERR_NONE)
                {
                    Log("%s-%d ERROR, find bast time cost failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
                    return ret;
                }
            }
        }
        else if(mt->id < 4)
        {
            if (pos > mt->CurrentPosition)
            {
                dest = pos - mt->CurrentPosition;
                mt->clockwise = 1;
                mt->dir = 1;
               if(MT_IsFlagSet(mt,MT_FLAG_GOBACK)==false)
               {
                ret = Find_BestTimeCost(mt->id, time_cost, M1_UNCLOCKWISE, dest);               
                if (ret != MT_ERR_NONE)
                {
                    Log("%s-%d ERROR, find bast time cost failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
                    return ret;
                }
               } 
            }
            else
            {
                dest = mt->CurrentPosition - pos;
                mt->clockwise = 0;
                mt->dir = 0;
               if(MT_IsFlagSet(mt,MT_FLAG_GOBACK)==false)
               {
                ret = Find_BestTimeCost(mt->id, time_cost, M1_CLOCKWISE, dest);               
                if (ret != MT_ERR_NONE)
                {
                    Log("%s-%d ERROR, find bast time cost failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
                    return ret;
                }
               }
            }
        }
    }
    else
    {
        return MT_ERR_PARAM;
    }

    ret = SetPosition(mt->id, dest);

    if (mt->status == MT_S_BREAK)
    {
        Log("%s-%d DEBUG, motor(%d) break\n", filename(__FILE__), __LINE__, mt->id);
        mt->status = MT_S_WAIT;
        ret = MT_ERR_BREAK;
    }

    return ret;
}

u8_t MT_Move(mt_t *mt, u32_t pos, u32_t ms, 
           bool is_clk_dir)
{
    u8_t ret = MT_ERR_NONE;
    unsigned long long time_cost = 0;

    if (mt == NULL || pos > mt->MaxPosition)
        return MT_ERR_PARAM;
    
    if(mt->id==2 || mt->id ==3 || mt->id ==5)
    {
      return MT_ERR_BUSY;
    }
    
    mt->time_give = ms;
    time_cost = (unsigned long long)ms*MT_TIMER_CLK*MT_ADAPT_VALUE/1000;
    if (is_clk_dir)
    {
        ret = Find_BestTimeCost(mt->id, time_cost, M1_CLOCKWISE, pos);
        if (ret != MT_ERR_NONE)
        {
            Log("%s-%d ERROR, find bast time cost failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
            return ret;
        }
    }
    else
    {
        ret = Find_BestTimeCost(mt->id, time_cost, M1_UNCLOCKWISE, pos);
        if (ret != MT_ERR_NONE)
        {
            Log("%s-%d ERROR, find bast time cost failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
            return ret;
        }
    }
    ret = SetPosition(mt->id, pos);
    return ret;
}
