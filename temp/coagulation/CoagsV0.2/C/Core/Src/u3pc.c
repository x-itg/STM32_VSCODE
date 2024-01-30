#include "main.h"
#include "AD7124.h"
#include "string.h"
#define MAXRXU1 255
unsigned char RxBufU1[MAXRXU1];
unsigned char TxBufU1[255];
unsigned char RxLenU1;
unsigned char TxLenU1;
#define MAXRXU2 255
unsigned char RxBufU2[MAXRXU2];
unsigned char TxBufU2[255];
unsigned char RxLenU2;
unsigned char TxLenU2;
#define MAXRXU3 255
unsigned char RxBufU3[MAXRXU3];
unsigned char TxBufU3[512];
unsigned char RxLenU3;
unsigned char TxLenU3;
unsigned short ReCmd = 0;
unsigned char           NullChar;
unsigned char* PNULL = &NullChar;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart5;
extern TIM_HandleTypeDef htim1;
SETSTADATA setSta;
unsigned int UpdataTime = 0;//主动上传
unsigned int TemptureDecTwice = 0;//两次温度之差：表示升温速率
unsigned int TemptureDecReach = 0;//离目标的温度差：表示欠温程度

//协议解码
unsigned char  decodeRec(unsigned char* pChar, unsigned short* cmd, unsigned short len)
{
  unsigned short jlen;
  if (*pChar != 0x5E)
  {
    return 0;
  }
  pChar++;
  if (*pChar != 0xE5)
  {
    return 0;
  }
  pChar++;

  jlen = *(unsigned short*)pChar;

  if (jlen != len)//长度校验
  {
    return 0;
  }
  pChar++;
  pChar++;
  *cmd = *(unsigned short*)pChar;
  pChar++;
  pChar++;
  return 1;
}

void u1_pc_conf(void)
{
  HAL_UART_Receive_DMA(&huart1, RxBufU1, MAXRXU1);
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
}

void u2_pc_conf(void)
{
  HAL_UART_Receive_DMA(&huart2, RxBufU2, MAXRXU2);
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
}

void u3_pc_conf(void)
{
  HAL_UART_Receive_DMA(&huart3, RxBufU3, MAXRXU3);
  __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
}

void u3PreSend(unsigned short Len, unsigned short Cmd, unsigned char* pData)
{
  TxBufU3[0] = 0x5e;//头部
  TxBufU3[1] = 0xe5;
  TxBufU3[2] = Len >> 0;//长度
  TxBufU3[3] = Len >> 8;
  TxBufU3[4] = Cmd >> 0;//命令
  TxBufU3[5] = Cmd >> 8;
  if (Len > 10)//data
  {
    memcpy(&TxBufU3[6], pData, Len - 6);
  }
  if (Len > 6)
  {
    TxBufU3[Len - 4] = 0xF1;//crc_l
    TxBufU3[Len - 3] = 0x1F;
    TxBufU3[Len - 2] = 0xF0;//tail_l
    TxBufU3[Len - 1] = 0x0F;
  }

  //设置运行数据拷贝到接收
}
void u1rxProcess(void)
{
  char c1SendHead[10] = "u111111111";
  if ((__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET))//空闲中断
  {
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    HAL_UART_DMAStop(&huart1);
    RxLenU1 = MAXRXU1 - huart1.hdmarx->Instance->NDTR;//接收到的字节长度
    memcpy(TxBufU3, c1SendHead, 10);
    memcpy(&TxBufU3[10], RxBufU1, RxLenU1);
    HAL_UART_Transmit(&huart3, TxBufU3, RxLenU1 + 10, 1000); //发送应该发送的数据
    HAL_UART_Receive_DMA(&huart1, RxBufU1, MAXRXU1); //再次开启DMA接收
  }
}
void u2rxProcess(void)
{
  char c2SendHead[10] = "u222222222";
  if ((__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE) != RESET))//空闲中断
  {
    __HAL_UART_CLEAR_IDLEFLAG(&huart2);
    HAL_UART_DMAStop(&huart2);
    RxLenU2 = MAXRXU2 - huart2.hdmarx->Instance->NDTR;//接收到的字节长度
    memcpy(TxBufU3, c2SendHead, 10);
    memcpy(&TxBufU3[10], RxBufU2, RxLenU2);
    HAL_UART_Transmit(&huart3, TxBufU3, RxLenU2 + 10, 1000); //发送应该发送的数据
    HAL_UART_Receive_DMA(&huart2, RxBufU2, MAXRXU1); //再次开启DMA接收
  }
}
void u3rxProcess(void)
{
  if ((__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE) != RESET))//空闲中断
  {
    __HAL_UART_CLEAR_IDLEFLAG(&huart3);
    HAL_UART_DMAStop(&huart3);
    RxLenU3 = MAXRXU3 - huart3.hdmarx->Instance->NDTR;//接收到的字节长度
    if (RxBufU3[0] == 'p' && RxBufU3[1] == 'r' && RxBufU3[2] == 'i' && RxBufU3[3] == 'n' && RxBufU3[4] == 't' && RxBufU3[5] == ':')
    {
      HAL_UART_Transmit(&huart5, &RxBufU3[6], RxLenU3 - 6, 1000);//转发给扫码枪
      HAL_UART_Receive_DMA(&huart3, RxBufU3, MAXRXU3); //再次开启DMA接收
      return;
    }
    if (decodeRec(RxBufU3, &ReCmd, RxLenU3) == 1)//校验一下并把命令提取除开，符合协议要求
    {
      switch (ReCmd)
      {
        //RxBufU3[6]里面开始放数据

      case 0x1000://读取所有参数
        TxLenU3 = sizeof(SETSTADATA) + 10;//6+data+4
        u3PreSend(TxLenU3, ReCmd, (unsigned char*)&setSta);
        break;

      case 0x1001://设置参数
        memcpy((unsigned char*)&setSta, &RxBufU3[6], RxLenU3 - 4);//设置数据
        TxLenU3 = 10;
        u3PreSend(TxLenU3, ReCmd, PNULL);//准备返回的缓存
        break;

      case 0xA000://设备主动上传样本条码机
        ;
        break;

      case 0xB000://设备主动上传试剂条码机
        ;
        break;

      case 0xC000://上位机下发打印信息
        ;
        break;
      }
    }

    HAL_UART_Transmit(&huart3, TxBufU3, TxLenU3, 1000); //发送应该发送的数据
    HAL_UART_Receive_DMA(&huart3, RxBufU3, MAXRXU3); //再次开启DMA接收
  }
}

void UpdataTimeCntFun(void)
{
  UpdataTime++;
}
void workPoll(void)
{
  unsigned short UpAdData[4];//主动上传的光强
  Get_Once_Vol(Actual_Voltage);
  //状态:马达到位情况、对射传感器1、2、3、4状态
  setSta.MotorSta = HAL_GPIO_ReadPin(m_get_GPIO_Port, m_get_Pin);
  setSta.kong1Sta = HAL_GPIO_ReadPin(kong1_GPIO_Port, kong1_Pin);
  setSta.kong2Sta = HAL_GPIO_ReadPin(kong2_GPIO_Port, kong2_Pin);
  setSta.kong3Sta = HAL_GPIO_ReadPin(kong3_GPIO_Port, kong3_Pin);
  setSta.kong4Sta = HAL_GPIO_ReadPin(kong4_GPIO_Port, kong4_Pin);
  //光强以及压力
  setSta.DP1light = Actual_Voltage[5];
  setSta.DP2light = Actual_Voltage[4];
  setSta.DP3light = Actual_Voltage[3];
  setSta.DP4light = Actual_Voltage[2];
  setSta.CurrentPress = Actual_Voltage[1];
  UpAdData[0] = setSta.DP1light >> 8;
  UpAdData[1] = setSta.DP2light >> 8;
  UpAdData[2] = setSta.DP3light >> 8;
  UpAdData[3] = setSta.DP4light >> 8;
  //温度
  double TemptureRet = 0;
  double Tempture16 = 0;
  Tempture16 = (double)(Actual_Voltage[0] >> 8);
  TemptureRet = 0.0726771063270515 * Tempture16 - 2006.71083145426;//单位0.1度
  setSta.CurretTemp = (unsigned int)(TemptureRet);
  static unsigned char MotoResetFlag = 0;//马达重新设置的标志

  //主动上传光强ADC
  if (UpdataTime > 200)
  {
    UpdataTime = 0;
    sprintf((char*)TxBufU3,
      "sample: %d %d %d %d\0",
      UpAdData[0],
      UpAdData[1],
      UpAdData[2],
      UpAdData[3]);//上传AD数据值
    HAL_UART_Transmit(&huart3, TxBufU3, strlen((char*)TxBufU3), 200); //发送数据
  }

  if (setSta.TempMod != 0 && TemptureTimeCnt > 200)//自动模式、每隔0.2秒控制一次
  {
    TemptureTimeCnt = 0;
    static unsigned int TemptureLast = 0;//上一次温度值

    if (setSta.CurretTemp < setSta.TempRech)//欠温状态：一般执行加温动作，接近目标或升温速率过快时提前关闭加热片、升温速率越快越接近目标温度越要关闭加热
    {
      setSta.TempSWSta = 1;//执行加温动作
      if (setSta.CurretTemp > TemptureLast)//加温有效  这次温度大于上次温度
      {
        TemptureDecTwice = setSta.CurretTemp - TemptureLast;//两次温度之差：表示升温速率
        TemptureDecReach = setSta.TempRech - setSta.CurretTemp;//离目标的温度差：表示欠温程度

        if (TemptureDecTwice > 10)//温度上升过快缓一缓
        {
          HeatOf;//不加热 
          setSta.TempSWSta = 0;
        }
        if (TemptureDecReach < 5 && TemptureDecTwice>2)//离目标还差0.5度以内、且温度上升速度大于0.2、提前中断加温
        {
          HeatOf;//不加热
          setSta.TempSWSta = 0;
        }
        if (TemptureDecReach < 10 && TemptureDecTwice>5)//离目标还差1度以内、且温度上升速度大于0.5、提前中断加温
        {
          HeatOf;//不加热
          setSta.TempSWSta = 0;
        }
        //其余情况保持加温
      }
    }
    else
    {
      TemptureDecReach = 0;
    }
    if (setSta.CurretTemp >= setSta.TempRech)//过温状态：减温
    {
      HeatOf;//不加热
      setSta.TempSWSta = 0;//

    }

    TemptureLast = setSta.CurretTemp;
  }
  //手动控温
  if (setSta.TempMod == 0 && setSta.TempManuSw != 0)
  {
    setSta.TempSWSta = 1;//去中断ms中另外识别一下50%的占空比开启加热
  }
  if (setSta.TempMod == 0 && setSta.TempManuSw == 0)
  {
    HeatOf;
    setSta.TempSWSta = 0;
  }
#if 0  //新做的板子需要控制亮度
  //灯1、2、3、4、阀1、2的控制
  if (setSta.led1Set != 0) { leden;led1on; }
  if (setSta.led1Set == 0) { leden;led1of; }
  if (setSta.led2Set != 0) { leden;led2on; }
  if (setSta.led2Set == 0) { leden;led2of; }
  if (setSta.led3Set != 0) { leden;led3on; }
  if (setSta.led3Set == 0) { leden;led3of; }
  if (setSta.led4Set != 0) { leden;led4on; }
  if (setSta.led4Set == 0) { leden;led4of; }
  if (setSta.led1Set == 0 && setSta.led2Set == 0 && setSta.led3Set == 0 && setSta.led4Set == 0) { ledds; }
#endif
  if (setSta.Fa1_Set != 0)HAL_GPIO_WritePin(tapcon1_GPIO_Port, tapcon1_Pin, GPIO_PIN_SET);
  if (setSta.Fa1_Set == 0)HAL_GPIO_WritePin(tapcon1_GPIO_Port, tapcon1_Pin, GPIO_PIN_RESET);
  if (setSta.Fa2_Set != 0)HAL_GPIO_WritePin(tapcon2_GPIO_Port, tapcon2_Pin, GPIO_PIN_SET);
  if (setSta.Fa2_Set == 0)HAL_GPIO_WritePin(tapcon2_GPIO_Port, tapcon2_Pin, GPIO_PIN_RESET);

  //电机调速
  if (setSta.Motor_SetManu == 1)
  {
    if (MotoResetFlag == 0)
    {
      mototimgpioConvter(1);//PE14设置成定时器
      init_tmc260();
      TIM1_Set(500000 / (setSta.MotoSpeed + 1));// 5000/
    }
    MotoResetFlag = 1;
    seten_l;//使能转
    setcs_h;//不片选设置
    setdr_h;//方向
  }
  else
  {
    if (MotoResetFlag == 1)
    {
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
      mototimgpioConvter(0);//设置成gpio
    }
    MotoResetFlag = 0;
    seten_h;//不转
  }
}

