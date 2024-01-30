#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newdialog.h"
#include "qtimer.h"
#include <QMessageBox>
#include <QScreen>

//打开窗体即打开程序
MainWindow::MainWindow(QWidget *parent) :// 构造函数 特点：以类名作为函数名，无返回类型
    QMainWindow(parent),                //多重继承方式继承了QT中的QMainWindow类
    ui(new Ui::MainWindow)              //初始化列表 ：初始化一个MainWindow界面指针，其变量名为ui。
{
    ui->setupUi(this);
    QRegExp regExp("(25[0-5]|2[0-4][0-9]|[0-1]?[0-9]?[0-9])");
    ComInitTimer();
    refreshDPI();
    //查找可用的串口
    findCom();
    //设置波特率下拉菜单默认显示第三项
    ui->BaudBox->setCurrentIndex(6);
    //关闭发送按钮的使能
    ui->sendButton->setEnabled(false);
    ui->clearButton->setEnabled(false);

    qDebug() << tr("界面设定成功！");
    on_openButton_clicked();
    pPlot1 = ui->plot;// 给widget绘图控件，设置个别名，方便书写
    QPlot_init(pPlot1);// 初始化图表1


    //读取电脑端保存的参数
    QString     FilePath = qApp->applicationDirPath();//获取app当前路径
    QString     wrFile=FilePath+"/settings.ini";
    QFile file(wrFile);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
    }
    int i=0;
    bool OK;
    while(!file.atEnd()) {
        QByteArray line = file.readLine();
        QString str(line);
        if(i==0){setSta.Motor_SetAuto=str.toInt(&OK, 10);}
        if(i==1){setSta.Motor_SetManu=str.toInt(&OK, 10);}
        if(i==2){setSta.Beng_Set=str.toInt(&OK, 10);}
        if(i==3){setSta.MotoSpeed=str.toInt(&OK, 10);}
        if(i==4){setSta.Fa1_Set=str.toInt(&OK, 10);}
        if(i==5){setSta.Fa2_Set=str.toInt(&OK, 10);}
        if(i==6){setSta.led1Set=str.toInt(&OK, 10);}
        if(i==7){setSta.led2Set=str.toInt(&OK, 10);}
        if(i==8){setSta.led3Set=str.toInt(&OK, 10);}
        if(i==9){setSta.led4Set=str.toInt(&OK, 10);}
        if(i==10){setSta.TempMod=str.toInt(&OK, 10);}
        if(i==11){setSta.TempManuSw=str.toInt(&OK, 10);}
        if(i==12){setSta.TempRech=str.toInt(&OK, 10);}
        i++;
        qDebug()<< str;
    }
    if(i<12)
    {
        memset((char *)&setSta,0,sizeof(SETSTADATA));
    }
    Data2window();
}


//关闭窗体
MainWindow::~MainWindow()
{
    qDebug() << tr("界面关闭成功！");
    delete ui;
}



//清空接受窗口
void MainWindow::on_clearButton_clicked()
{
    qDebug() << tr("清空接受窗口");
    ui->textEdit->clear();
    ui->textEdit_2->clear();

}







