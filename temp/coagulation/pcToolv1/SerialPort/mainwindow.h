#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QFileDialog>
#include <QTime>
#include <QMetaType>
#include "qcustomplot.h"
#pragma pack(1)
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    void on_clearButton_clicked();
    void on_sendButton_clicked();
    void on_openButton_clicked();
    void Read_Data();
    void ReceivedTimeUpdate(void);//串口收完回调
    void on_setButton_clicked();


    void on_ChooseFile_clicked();
    void on_GetNewFile_clicked();
    void on_UpdateCode_clicked();
    void on_EnterBoot_clicked();
    void on_qPack_clicked();
    void on_escBoot_clicked();
    void on_readVersion_clicked();
    void on_setMotorOn_clicked();

    void on_setMotorOff_clicked();

    void on_setBengOn_clicked();

    void on_setFa1on_clicked();

    void on_setFa1off_clicked();

    void on_setFa2on_clicked();

    void on_setFa2off_clicked();
    void changeObjectSize(const QObject &o, double objectRate);
    void refreshDPI();
    void on_setMotorRelease_clicked();

    void on_setMotorReset_clicked();

    void on_setL1OnOff_clicked();

    void on_setL2OnOff_clicked();

    void on_setL3OnOff_clicked();

    void on_setL4OnOff_clicked();

    void on_Box_TempMode_activated(const QString &arg1);

    void on_manuTempHeatOn_clicked();

    void on_manuTempHeatOff_clicked();

    void on_checkBox_1_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_Savegp_clicked();

    void on_checkBox_3_stateChanged(int arg1);

    void on_checkBox_4_stateChanged(int arg1);



private:
    QByteArray HexStringToByteArray(QString HexString);
    int str2inthandle(char * pstr,unsigned char dilen);
    QTimer *m_timer;
    void ComInitTimer(void);
    void ComQYPreSend(unsigned char *TxBufU3,unsigned short Len,unsigned short Cmd,unsigned char *pData );
    Ui::MainWindow *ui;
    QSerialPort *serial;
    unsigned short CRC16x( unsigned char * pucFrame, unsigned short usLen );
    void appendCrcLH(unsigned char * ahead,unsigned short lens);
    void sendset (unsigned short regaddr,unsigned short value);
    QString SendAppend(QByteArray appQbyte);
    QByteArray ComRecBuf;
    unsigned char  decodeRec(unsigned char *pChar,unsigned short *cmd,unsigned short len);
    QString Traversal(QString path);
    void delaymsec(int msec);
    QList<QByteArray> part_list;//升级所用分包
    QTimer count ;
    void Data2window(void);//数据显示到窗口
    void window2Data(void);//窗口数据显示
    double progress_value = 0.0;//进度条
    double each_part_bar;       //每包所占进度条比率大小
    int a_part_size = 128;      //一包所占字节数
    int allpartsize=0;
    int part_num;               //包数
    int part_index;             //包序号
    int part_curIndexLen=0;     //当前包长度
    int currprogress;           //当前进度条显示
    int statusCode=1;           //0 开始第一包  1发的过程当中  2发完了
    QByteArray SendPack;        //当前发送的内容
    int failedflag=0;
    int readcoeflag=0;
    unsigned char  SendAPart(unsigned char *recp,unsigned int reclen);
    void findCom(void);
    QString fileName;
    QFile recfile;//文件对象

    int recpartnum;//文件包数
    int recfileSize;//文件大小
    int recedSize;

    QSharedPointer<QCPGraphDataContainer> mData;//用于波形显示// 绘图控件的指针

    QCustomPlot *pPlot1;
    float GGvalue1;
    float GGvalue2;
    float GGvalue3;
    float GGvalue4;
    // 绘图控件中曲线的指针
    QCPGraph *pGraph1_1;
    QCPGraph *pGraph1_2;
    QCPGraph *pGraph1_3;
    QCPGraph *pGraph1_4;

    void QPlot_init(QCustomPlot *customPlot);
    void Show_Plot(QCustomPlot *customPlot, double num1, double num2, double num3, double num4);


typedef struct {
    unsigned char  Motor_SetAuto;	        //读写 0未到位       1到位
    unsigned char  Motor_SetManu;	        //读写 0手动停止电机 1手动启动电机
    unsigned char  Beng_Set;              //读写 0不转         1-100转速
    unsigned char  MotoSpeed;     //读写 马达速度
    unsigned char  Fa1_Set;		//读写 0断开         1闭合
    unsigned char  Fa2_Set;		//读写 0断开         1闭合
    unsigned char  led1Set;		//读写 0关			 1开
    unsigned char  led2Set;		//读写 0关			 1开
    unsigned char  led3Set;		//读写 0关			 1开
    unsigned char  led4Set;		//读写 0关			 1开
    unsigned char  TempMod;       //读写 温控制模式 0手动 1自动
    unsigned char  TempManuSw;    //读写 手动模式下 0关   1开
    unsigned short TempRech;		//读写 自动模式下 32.0-43.0 目标温度  370
    unsigned char  TempSWSta;     //只读 加热开关的状态
    unsigned char  MotorSta;      //只读 0没到位 1到位
    unsigned int 	 CurretTemp;	//只读 当前温度370表示37.0
    unsigned int   CurrentPress;	//只读 当前压力值
    unsigned int   DP1light;		//只读 当前DP1感光点
    unsigned int   DP2light;		//只读 当前DP2感光点
    unsigned int   DP3light;		//只读 当前DP3感光点
    unsigned int   DP4light;		//只读 当前DP4感光点
    unsigned char  kong1Sta;		//只读 孔1状态0未到位 1到位
    unsigned char  kong2Sta;		//只读 孔2状态0未到位 1到位
    unsigned char  kong3Sta;		//只读 孔3状态0未到位 1到位
    unsigned char  kong4Sta;		//只读 孔4状态0未到位 1到位
    unsigned char  KwSta;			//只读 试剂卡 0未到位 1到位
    unsigned char  YwSta;			//只读 样本   0未到位 1到位
}SETSTADATA;

    SETSTADATA setSta;
};

typedef struct uS0
{
    unsigned short Freq;
    unsigned short Dbuv;
    unsigned short Swon;
}unitPara;
typedef struct uS1
{
    unitPara nPara[8];
    unsigned short allsw;
}RDNAdata;
#endif // MAINWINDOW_H
