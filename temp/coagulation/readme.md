# 1.文件夹说明

- 单片机控制端代码：CoagsV0.2

- 上位机源代码：pcToolv1

- 参考资料：reference
  
  git bat流程：
  
  1. 抓取远程分支git fetch origin main:fetchmain
  
  2. 查看比较本地的提交数据和远程的提交数
     
     远程的提交数较多的话拉取，否则推送
     

# 2. 试剂卡及里面的液体是否到位

- `PG14_HC_Test 试纸卡是否到位的检测开关量` 
- `xxxx_YB_TEST 样本液体是否到位 IO口还未确认` 

# 3. 阀机

- 阀1           `PE10_TAP1 阀门1的控制开关`
- 阀2        `PE08_TAP2 阀门2的控制开关--PE9`

# 4. 泵机

- `PE9_TEMP_CON    加热控制-->PE8`
- `PE11_PUMP        气泵的开关`

# 5. 样本点是否充盈

- X1        `PG09_BSen1    试剂卡检测小圆点1 检测高低电平`
- X2        `PG10_BSen2    试剂卡检测小圆点2 检测高低电平`
- X3        `PG11_Bsen3    试剂卡检测小圆点3 检测高低电平`
- X4        `PG12_BSen4    试剂卡检测小圆点4 检测高低电平`

# 6. 步进电机（用于摇均）

- 启动    `                PB15_SPI2_MOSI`
- 停止    `PB10_EN        PB14_SPI2_MISO`
- 速度    `PE14_M_STEP    PB13_SPI2_SCK`
- 复位    `PE15_M_DIR     PB11_CS`
- `PG13_MotoSen到位检测` 

# 7. 温度、压力、灯

- 温度值显示                            `ANA_1_TEMP` 
- 压力值显示                            `ANA_2_PRESS` 
- - SHDN    `PA12_SHDN_led恒流源供电使能`  
- - L1        `PA11_LED_CON1  控制    ANA_4_light 荧光反射模拟量检测`
- - L2        `PA08_LED_CON2    控制    ANA_5_light 荧光反射模拟量检测`
- - L3        `PC09_LED_CON3  控制    ANA_6_light 荧光反射模拟量检测`
- - L4        `PC08_LED_CON4    控制    ANA_7_light 荧光反射模拟量检测`
- - 后来L1~L4的驱动改成了DAC
- LED_CON2、SCL1 PA8   Led1:第一路iic地址  0x9A
- LED_CON1、SDA1 PA11  Led2:第一路iic1地址 0x98
- LED_CON4、SCL2 PC8   Led1:第一路iic2地址 0x9A
- LED_CON3、SDA2 PC9   Led1:第一路iic2地址 0x98
- ADC的SPI接口`PB03_CLK`
- - `PB04_MISO`
- - `PB05_MOSI`
- - `PG15_CS`
- - `SYNC引脚可以不接`

# 8. 打印机    🖨️

- `打印信息`

- `条码机1_采集样本信息`

- `条码机2_试剂卡信息`

# 9. 与上位机的协议    💻

## 串口配置：3.3V TTL电平，波特率115200 1位停止位 无校验位

### 9.1、下发

- 电脑串口给板子发送（读取/控制）请求，

### 9.2、上传

- 板子收到（读取/控制）请求后200ms内予以应答（读取应答/控制应答）

# 10. 命令格式

| 启始        | 总字节长度  | 命令         | 数据   | 结尾          |
| --------- | ------ | ---------- | ---- | ----------- |
| head      | length | cmd        | data | tail        |
| 0x5e 0xe5 | L    H | L        H | data | F1 1F F0 0F |

# 11 主要命令

| 功能:                | 启始    | 总字节长度  | 命令    | 数据          | 结尾          |
| ------------------ | ----- | ------ | ----- | ----------- | ----------- |
| 读取下发 cmd 0x1000  ： | 5e e5 | 0A 00  | 00 10 | NONE        | F1 1F F0 0F |
| 读取应答 cmd 0x1000  ： | 5e e5 | length | 00 10 | data结构体低位先传 | F1 1F F0 0F |
| 写入下发 cmd 0x1001  ： | 5e e5 | 0A 00  | 00 10 | data结构体低位先传 | F1 1F F0 0F |
| 写入应答 cmd 0x1001  ： | 5e e5 | length | 00 10 | NONE        | F1 1F F0 0F |

- cmd 0xA000  设备主动上传样本条码机         data中为透传的字符串

- cmd 0xB000  设备主动上传试剂条码机         data中为透传的字符串

- cmd 0xC000    上位机下发打印信息             data中为透传的字符串

- cmd 0xE000  设备应答出错                data为空

- 控制板发送感光AD数据为字符串数据

- 当打开一路LED后开始上传AD感光数据

- 格式为：

- sample: 第1路16位AD数 第2路16位AD数 第3路16位AD数 第4路16位AD数

- 中间用空格隔开方便上位机处理

## 11.1数据结构体

```
  typedef struct {
  unsigned char  Motor_SetAuto;     //读写 0未到位       1到位
  unsigned char  Motor_SetManu;     //读写 0手动停止电机 1手动启动电机
  unsigned char  Beng_Set;          //读写 0不转         1-100转速
  unsigned char  MotoSpeed;         //读写 马达速度
  unsigned char  Fa1_Set;           //读写 0断开         1闭合
  unsigned char  Fa2_Set;           //读写 0断开         1闭合
  unsigned char  led1Set;           //读写 0关             1~255控制开的强度
  unsigned char  led2Set;           //读写 0关             1~255控制开的强度
  unsigned char  led3Set;           //读写 0关             1~255控制开的强度
  unsigned char  led4Set;           //读写 0关             1~255控制开的强度
  unsigned short TempRech;          //读写 32.0-43.0 目标温度
  unsigned char  MotorSta;          //只读 0没到位 1到位
  unsigned int      CurretTemp;        //只读 当前温度370表示37.0
  unsigned int   CurrentPress;      //只读 当前压力值
  unsigned int   DP1light;          //只读 当前DP1感光点
  unsigned int   DP2light;          //只读 当前DP2感光点
  unsigned int   DP3light;          //只读 当前DP3感光点
  unsigned int   DP4light;          //只读 当前DP4感光点
  unsigned char  kong1Sta;          //只读 孔1状态0未到位 1到位     
  unsigned char  kong2Sta;          //只读 孔2状态0未到位 1到位
  unsigned char  kong3Sta;          //只读 孔3状态0未到位 1到位
  unsigned char  kong4Sta;          //只读 孔4状态0未到位 1到位
  unsigned char  KwSta;             //只读 试剂卡 0未到位 1到位
  unsigned char  YwSta;             //只读 样本   0未到位 1到位

}SETSTADATA;
```