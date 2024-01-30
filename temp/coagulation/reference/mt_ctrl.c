/**
 * file name : mt_ctrl.c
 * date      : 
 * brief     : 电机控制任务
 */
#include "app.h"
#include "aos.h"
#include "bsp.h"
#include "motor.h"
#include "mt_acc.h"
#include "icomm.h"

extern parameter_t parameter;
extern u32_t  uart5_write;
extern u32_t  uart5_read;
extern u8_t    mt_sem_1,mt_com_num;
extern u8_t MAC_ADDR;
extern u8_t group_id;

u8_t           mt_cmd[MT_NUMS];        // 每个任务的命令
u8_t           mt_err_code[MT_NUMS];   // 电机故障码 
u32_t          mt_param1[MT_NUMS];     // 命令参数1
u32_t          mt_param2[MT_NUMS];     // 命令参数2
u32_t          mt_cmd_source[MT_NUMS];     // 命令参数2
bool           mt_break[MT_NUMS];      // 电机运动中断标识

mt_t           mt_array[MT_NUMS];
aos_tcb_t      MTTaskTCB[MT_NUMS];
//size_t         MTTaskStk[MT_NUMS][APP_MT_TASK_STK_SIZE];
unsigned int   MTTaskStk[MT_NUMS][APP_MT_TASK_STK_SIZE];

s32_t MTCtrl_GetMotorPos(u8_t motorId, u32_t *pos)
{
    if(motorId < 6 && pos != NULL)
    {
        *pos = mt_array[motorId].CurrentPosition;
        return 0;
    }
    
    return -1;
}
/**
 * 从下位机自己发送的指令 
 *
 */ 
bool MTCtrl_SendCmd(u8_t id, u8_t cmd, u32_t param1, u32_t param2)
{ 
    if( mt_cmd[id] != MT_CTRL_CMD_NONE )
    {
        return false;
    }

    mt_cmd[id] = cmd;
    mt_param1[id] = param1;
    mt_param2[id] = param2;
    mt_cmd_source[id] = MT_CTRL_CMD_SOURCE_FROM_SELF;

    AOS_TaskPost(&MTTaskTCB[id]);

    return true;
}

/**
 * 从上位机发送的指令 
 *
 */ 
bool MTCtrl_SendCmdFromMaster(u8_t id, u8_t cmd, u32_t param1, u32_t param2)
{
    return MTCtrl_SendCmdInner(id, cmd, param1, param2, MT_CTRL_CMD_SOURCE_FROM_IMX6);
}
/**
 * @brief  : 向指定电机发送命令
 * @input  :
 *           id     - 电机索引号
 *           cmd    - 命令
 *           param1 - 参数1
 *           param2 - 参数2
 *           source - 消息来源 上位机/下位机自己
 * @output :
 * @input  : true  - 发送成功
 *           false - 当前电机正在运行
 */
bool MTCtrl_SendCmdInner(u8_t id, u8_t cmd, u32_t param1, u32_t param2, u32_t source)
{

    u32_t i;

    if( id >= MT_NUMS )
    {
        Log("%s-%d: ERROR id (%u) >= MT_NUMS.\n", filename(__FILE__), __LINE__, id);
        return false;
    }

    for(i=0; i<15; i++)
    {
        if( mt_cmd[id] == MT_CTRL_CMD_NONE )
        {
            break;
        }
        else
        {
            Log("%s-%d: ERROR TIMEOUT 100 ms , id = %d\n", filename(__FILE__), __LINE__, id);
            AOS_TaskDelay(100);
        }
        
    }

    if( i >= 15 )
    {
        Log("%s-%d: ERROR MTCtrl_SendCmdInner TIMEOUT 1500 ms \n", filename(__FILE__), __LINE__);
        return false;
    }

    mt_cmd[id] = cmd;
    mt_param1[id] = param1;
    mt_param2[id] = param2;
    mt_cmd_source[id] = source;

    AOS_TaskPost(&MTTaskTCB[id]);

    return true;
}

/**
 * @brief  : 是机往返运行
 * @input  : mt - 电机控制器
 * @output :
 * @retval :
 */
u8_t MTCtrl_GoBack(mt_t *mt, u32_t param1, u32_t param2)
{
    u32_t i, cnt, target, current;
    u32_t time;
    u8_t ret = MT_ERR_NONE;
    ts_t ts1, ts2;
    unsigned long long time_cost=0;
    
    //--------------------------屏蔽其他通道电机
    if(mt->id==2 || mt->id ==3 || mt->id ==5)
    {
      return MT_ERR_BUSY;
    }

//    mt_break[mt->id] = false;
    if (!mt || mt->id > MT_NUMS)
        return MT_ERR_PARAM;
    mt->status = MT_S_WAIT;
    time    = param2;            // 往返一次的时间,ms
    cnt     = param1>>16;        // 往返次数   
    target  = (u16_t)param1;     // 目标位置
    current = mt->CurrentPosition;

    switch (mt->id)
    {
        case 0:
            current *= M1DIV;
            break;
        case 1:
            current *= M2DIV;
            break;
        case 2:
            current *= M3DIV;
            break;
        case 3:
            current *= M4DIV;
            break;
        case 4:
            current *= M5DIV;
            break;
        case 5:
            current *= M6DIV;
            break;
        default:
            Log("%s-%d,ERROR: Motor id is wrong\n", filename(__FILE__), __LINE__);
            return ERR_PARAM;
    }
    //电机加速曲线移动到这里处理
    time_cost = (unsigned long long)(time/2-10)*MT_TIMER_CLK*MT_ADAPT_VALUE/1000;
    if(target>=current)
    {
      ret = Find_BestTimeCost(mt->id,time_cost, M1_CLOCKWISE,(u32_t)(target-current));
    }
    else 
    {
      ret = Find_BestTimeCost(mt->id,time_cost, M1_CLOCKWISE,(u32_t)(current-target));
    }
    
    if (ret != MT_ERR_NONE)
    {
      Log("%s-%d ERROR, GoBack find bast time cost failed, ret = %d\n", filename(__FILE__), __LINE__, ret);
      return ret;
    }
    MT_SetFlag(mt,MT_FLAG_GOBACK);
    for(i=0; i<cnt; i++)
    {
        ts1 = AOS_TickGet();
        
        ret = MT_MoveTo(mt, target, time/2);
        if (ret != ERR_NONE)
            break;
        
        ret = MT_MoveTo(mt, current, time/2);
        if (ret != ERR_NONE)
            break;
        
        ts2 = AOS_TickGet();
        if (time > (ts2-ts1) && time-(ts2-ts1) > 10)
            AOS_TaskDelay(time - (ts2-ts1));
    }
    MT_ClrFlag(mt,MT_FLAG_GOBACK);
    if ((ret != ERR_NONE) && (ret != MT_ERR_BREAK))
        return ret;

    return ret;
}

u8_t MTCtrl_GoBackWrapper(u8_t mtId, u32_t targetPos, u32_t duration, u32_t repeats)
{
    return  MTCtrl_GoBack(&mt_array[mtId], (repeats<<16) | targetPos, duration);
}

/**
 * @brief  : 运动到指定位置。非阻塞式调用
 * @input  :
 *           mt  - 电机控制器
 *           pos - 目标位置
 *           ms  - 运行时间，单位:ms
 * @output :
 * @retval :
 */
u8_t MTCtrl_MoveTo(mt_t *mt, u32_t pos, u32_t ms)
{
    u8_t ret = MT_ERR_NONE;
    if (!mt || mt->id > MT_NUMS)
        return MT_ERR_PARAM;

    ret = MT_MoveTo(mt, pos, ms);
    if ((ret != ERR_NONE) && (ret != MT_ERR_BREAK))
        return MT_ERR_BUSY;

    return MT_ERR_NONE;
}

u8_t MTCtrl_MoveToWrapper(u8_t mtId, u32_t targetPos, u32_t duration)
{
    u8_t ret = MT_ERR_NONE;
   //---------------------------屏蔽其他通道电机 
   if(mtId==2 || mtId ==3 ||  mtId ==5)
    {
      return ret;
    }
    
    ret = MT_MoveTo(&mt_array[mtId], targetPos, duration);
    if ((ret != ERR_NONE) && (ret != MT_ERR_BREAK))
        return MT_ERR_BUSY;

    return ret;
}

u8_t MT_RstWrapper(u8_t mtId, u32_t speed)
{
    return MT_Rst(&mt_array[mtId], speed);
}
/**
 * @brief  : 中断电机运动
 * @input  : id - 电机索引号
 * @output :
 * @retval :
 */
void MTCtrl_Break(u8_t id)
{
    if( id < MT_NUMS  && mt_array[id].en == 1)
    {
        mt_array[id].status = MT_S_BREAK;
//        mt_array[id].en = 0;
//        mt_array[id].TimerStop(id);
//        mt_array[id].ResetClk(id);
//        mt_array[id].LPWEn(id);
//        if (AOS_SemPend(&(mt_array[id].sem), 0) != 0)
//            AOS_SemPost(&(mt_array[id].sem));
        Log("motor break id:%d\n", id);
    }
}

/**
 * @brief  : 执行电机控制命令
 * @input  : mt - 电机控制器
 * @output :
 * @retval :
 */
void MTCtrl_DoCmd(mt_t *mt)
{
    OS_ERR  os_err;
    u8_t ret = MT_ERR_NONE;
    u8_t   tmp[8];
    u8_t i=0;

    CPU_SR_ALLOC();

    if( (mt == NULL) 
        || (mt->id >= MT_NUMS)
        || (mt_cmd[mt->id] == MT_CTRL_CMD_NONE) )
    {
        Log("%s-%d: ERROR bad parament.\n", filename(__FILE__), __LINE__);
        return ;
    }

    switch(mt_cmd[mt->id])
    {
        case MT_CTRL_CMD_RST:
        {
           ret = MT_Rst(mt, mt_param1[mt->id]);
           if (ret) {
               Log("%s-%d: ERROR Failed to MT_CTRL_CMD_Rst (%u) ret = %u.\n",
                                          filename(__FILE__), __LINE__, mt->id, ret);
           }
           if(mt->id < 4)
           {
               ret = MT_MoveTo(mt, parameter.mt_ch_pos[mt->id], 1200);
               if (ret) {
                   Log("%s-%d: ERROR Failed to MT_CTRL_CMD_MoveTo (%u) ret = %u.\n",
                                              filename(__FILE__), __LINE__, mt->id, ret);
               }
           }
            // ret = MT_Rst(mt, 500);
            tmp[i++] = CMD_MT_RST | 0x80 | MAC_ADDR;
            tmp[i++] = (u8_t)ret;
            tmp[i++] = mt->id+1;//tmp[i++] = mt->id;
            
            break;
        }
        case MT_CTRL_CMD_HOME:
        {
            ret = MT_Home(mt, mt_param1[mt->id]);            
            if (ret) {
                Log("%s-%d: ERROR Failed to MT_CTRL_CMD_HOME (%u) ret = %u.\n",
                                           filename(__FILE__), __LINE__, mt->id, ret);
            }

            tmp[i++] = CMD_MT_HOME | 0x80 | MAC_ADDR;
            tmp[i++] = (u8_t)ret;
            tmp[i++] = mt->id+1;//tmp[i++] = mt->id;
            
            break;
        }
        case MT_CTRL_CMD_MOVE_TO:
        {
            ret = MTCtrl_MoveTo(mt, mt_param1[mt->id], mt_param2[mt->id]);
            if (ret) {
                Log("%s-%d: ERROR Failed to MT_CTRL_CMD_Move_To (%u) ret = %u\n",
                                            filename(__FILE__), __LINE__, mt->id, ret);
            }
 
            tmp[i++] = CMD_MT_TO | 0x80 | MAC_ADDR;
            tmp[i++] = (u8_t)ret;
            tmp[i++] = mt->id+1;//tmp[i++] = mt->id;
            tmp[i++] = (u8_t)((mt->CurrentPosition)>>24);
            tmp[i++] = (u8_t)((mt->CurrentPosition)>>16);
            tmp[i++] = (u8_t)((mt->CurrentPosition)>>8);
            tmp[i++] = (u8_t)(mt->CurrentPosition);
            
            break;
        }
        case MT_CTRL_CMD_MOVE_CLK:
        {
            ret = MT_Move(mt, mt_param1[mt->id], mt_param2[mt->id], true);
            if (ret) {
                Log("%s-%d: ERROR Failed to MT_CTRL_CMD_MOVE_CLK (%u) ret = %u.\n",
                                              filename(__FILE__), __LINE__, mt->id, ret);
            }

            tmp[i++] = CMD_MT_MOVE | 0x80 | MAC_ADDR;
            tmp[i++] = (u8_t)ret;
            tmp[i++] = mt->id+1;//tmp[i++] = mt->id;
            tmp[i++] = (u8_t)((mt->CurrentPosition)>>24);
            tmp[i++] = (u8_t)((mt->CurrentPosition)>>16);
            tmp[i++] = (u8_t)((mt->CurrentPosition)>>8);
            tmp[i++] = (u8_t)(mt->CurrentPosition);
            
            break;
        }
        case MT_CTRL_CMD_MOVE_ANTI:
        {
            ret = MT_Move(mt, mt_param1[mt->id], mt_param2[mt->id], false);
            if (ret) {
                Log("%s-%d: ERROR Failed to MT_CTRL_CMD_ANTI (%u) ret = %u.\n",
                                              filename(__FILE__), __LINE__, mt->id, ret);
            }

            tmp[i++] = CMD_MT_MOVE | 0x80 | MAC_ADDR;
            tmp[i++] = (u8_t)ret;
            tmp[i++] = mt->id+1;//tmp[i++] = mt->id;
            tmp[i++] = (u8_t)((mt->CurrentPosition)>>24);
            tmp[i++] = (u8_t)((mt->CurrentPosition)>>16);
            tmp[i++] = (u8_t)((mt->CurrentPosition)>>8);
            tmp[i++] = (u8_t)(mt->CurrentPosition);            
            break;
        }
        case MT_CTRL_CMD_GO_BACK:
        {    
            ret = MTCtrl_GoBack(mt, mt_param1[mt->id], mt_param2[mt->id]);
            if (ret) {
                Log("%s-%d: ERROR Failed to MT_CTRL_CMD_GO_BACK (%u) ret = %u.\n",
                                              filename(__FILE__), __LINE__, mt->id, ret);
            }

            tmp[i++] = MT_CTRL_CMD_GO_BACK | 0x80 | MAC_ADDR;
            tmp[i++] = (u8_t)ret;
            tmp[i++] = mt->id+1;//tmp[i++] = mt->id;          
            break;
        }
        default: break;
    }

    if( ret != MT_ERR_NONE )
    {
        ret |= (((mt->id + 1)<<4) & 0xF0);
        mt_err_code[mt->id] = (u8_t)ret;
    }
    // 向主机上报执行结果
    if(mt_cmd_source[mt->id] == MT_CTRL_CMD_SOURCE_FROM_IMX6)
    {
        //发送反馈 电机移动完成后的反馈
        ICOMM_Write(tmp, i);
    }

    // 通过将命令置空的方法来通知其它程序，命令已执行完
    OSSchedLock(&os_err);//OS_SCHED_LOCK();
    mt_cmd[mt->id] = MT_CTRL_CMD_NONE;
    OSSchedUnlock(&os_err);//OS_SCHED_UNLOCK();
}

/**
 * @brief  : 电机控制任务
 * @input  :
 * @output :
 * @retval :
 */
int MTTask(void *arg)
{
    ts_t ts;
    mt_t *mt = (mt_t *)arg;

    while(1)
    {
        AOS_TaskPend(NULL);
        while(mt_cmd[mt->id] != MT_CTRL_CMD_NONE)
        {
            ts = AOS_TickGet();

            MTCtrl_DoCmd(mt);

            ts = AOS_TickGet() - ts;
        }
    }
}

/**
 * @brief  : 电机控制器初始化
 * @input  :
 * @output :
 * @retval :光耦灯亮运动rst_ppos，灯灭运动rst_npos
 */
void MTCtrlInit(void)
{
    u8_t i;

    // 初始化电机控制器
    MotorRunParaInitial();
    for(i=0; i<MT_NUMS; i++)
    {
        mt_array[i].IsSNSValid = BSP_MTIsSNSValid;
        mt_array[i].IsSafe     = BSP_MTIsSafe;
        mt_array[i].SetClk     = BSP_MTSetClk;
        mt_array[i].ResetClk   = BSP_MTResetClk;
        mt_array[i].ClkDir     = BSP_MTClkDir;
        mt_array[i].AntiDir    = BSP_MTAntiDir;
        mt_array[i].LPWEn      = BSP_MTLPWEn;
        mt_array[i].LPWDis     = BSP_MTLPWDis;
        mt_array[i].TimerSet   = BSP_MTTimerSet;
        mt_array[i].TimerStart = BSP_MTTimerStart;
        mt_array[i].TimerStop  = BSP_MTTimerStop;  
        
        TMC_init(i);//TMC2660 初始化    
        
        if (i == 4 || i == 5)
            continue;
        MT_Init(i, 1, 400/RESET_DIV1, 900/RESET_DIV1, 12000/RESET_DIV1);   
//        MT_Init(i, 1, 400/RESET_DIV1, 1300/RESET_DIV1, 12000/RESET_DIV1);   //仪器编号：0202
    }   
    
    MT_Init(4, 1, 300/RESET_DIV5, 90/RESET_DIV5, 2100000/RESET_DIV5);//MT_Init(4, 1, 300/RESET_DIV5, 400/RESET_DIV5, 2100000/RESET_DIV5);

    MT_Init(5, 1, 200/RESET_DIV6, 200/RESET_DIV6, 20000/RESET_DIV6);
    // 初始化命令
    for(i=0; i<MT_NUMS; i++)
    {
        mt_cmd[i] = MT_CTRL_CMD_NONE;
        mt_err_code[i] = MT_ERR_NONE;
    }

}

void MTCtrlStart(void)
{
    static uint32_t once = 0;
    
    if(!once)
    {
        once = 1;

        for(u32_t i=0; i<MT_NUMS; i++)
        {
            AOS_TaskInit((u8_t *)"mt",
                     APP_MT_TASK_PRIO,
                     &mt_array[i],
                     MTTask,
                     &MTTaskStk[i][0],
                     APP_MT_TASK_STK_SIZE,
                     &MTTaskTCB[i]);
        }  
    }
  
}
