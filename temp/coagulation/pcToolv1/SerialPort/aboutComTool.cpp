#include "mainwindow.h"
#include "ui_mainwindow.h"



//上行 数据填充到窗体
void MainWindow::Data2window()
{
    float a;
    if(setSta.Motor_SetAuto)//复位锁定的设定
    {
        ui->setMotorReset->setEnabled(false);
        ui->setMotorRelease->setEnabled(true);
    }else
    {
        ui->setMotorReset->setEnabled(false);
        ui->setMotorRelease->setEnabled(true);
    }

    if(setSta.Motor_SetManu)//手动启动电机
    {
        ui->setMotorOn->setEnabled(false);//已启动按钮变灰
        ui->setMotorOff->setEnabled(true);
        ui->Box_MotorSta->setCurrentIndex(1);
    }else
    {
        ui->setMotorOn->setEnabled(true);
        ui->setMotorOff->setEnabled(false);//已关闭按钮变灰
        ui->Box_MotorSta->setCurrentIndex(0);
    }
    if(setSta.MotorSta)
    {
        ui->Box_MotorDianSta->setCurrentIndex(1);
    }else
    {
        ui->Box_MotorDianSta->setCurrentIndex(0);
    }
    if(setSta.kong1Sta)
    {
        ui->Kong1->setCurrentIndex(1);
    }else
    {
        ui->Kong1->setCurrentIndex(0);
    }

    if(setSta.kong2Sta)
    {
        ui->Kong2->setCurrentIndex(1);
    }else
    {
        ui->Kong2->setCurrentIndex(0);
    }

    if(setSta.kong3Sta)
    {
        ui->Kong3->setCurrentIndex(1);
    }else
    {
        ui->Kong3->setCurrentIndex(0);
    }

    if(setSta.kong4Sta)
    {
        ui->Kong4->setCurrentIndex(1);
    }else
    {
        ui->Kong4->setCurrentIndex(0);
    }
    if(setSta.KwSta)
    {
        ui->Ka->setCurrentIndex(1);
    }else
    {
        ui->Ka->setCurrentIndex(0);
    }

    if(setSta.TempSWSta)
    {
        ui->TempHeatSta->setCurrentIndex(1);
    }else
    {
        ui->TempHeatSta->setCurrentIndex(0);
    }
    if(setSta.Fa1_Set)
    {
        ui->setFa1off->setEnabled(true);
        ui->setFa1on->setEnabled(false);
    }else
    {
        ui->setFa1on->setEnabled(true);
        ui->setFa1off->setEnabled(false);
    }
    if(setSta.Fa2_Set)
    {
        ui->setFa2off->setEnabled(true);
        ui->setFa2on->setEnabled(false);
    }else
    {
        ui->setFa2on->setEnabled(true);
        ui->setFa2off->setEnabled(false);
    }

    ui->Edit_MotorSpeed->setText(QString::number(setSta.MotoSpeed));
    ui->Edit_BengSpeed->setText(QString::number(setSta.Beng_Set));

    a=(float)setSta.TempRech/10;
    ui->reachTemp->setText(QString::number(a,'f', 1));

    a=(float)setSta.CurretTemp/10;
    ui->currentTemp->setText(QString::number(a,'f', 1));

    ui->led1AD->setText(QString::number(setSta.DP1light));
    ui->led2AD->setText(QString::number(setSta.DP2light));
    ui->led3AD->setText(QString::number(setSta.DP3light));
    ui->led4AD->setText(QString::number(setSta.DP4light));
    ui->YaliValue->setText(QString::number(setSta.CurrentPress));

    ui->setLed1->setText(QString::number(setSta.led1Set));
    ui->setLed2->setText(QString::number(setSta.led2Set));
    ui->setLed3->setText(QString::number(setSta.led3Set));
    ui->setLed4->setText(QString::number(setSta.led4Set));
}


//下行设置
void MainWindow::window2Data()
{
    QString tmp;
    float   ftmp;
    char*  ch;
    QByteArray bytedata ;


    tmp=ui->Edit_MotorSpeed->text();
    bytedata= tmp.toLatin1(); // must
    ch=bytedata.data();
    setSta.MotoSpeed=(unsigned short)str2inthandle(ch,0);

    tmp=ui->Edit_BengSpeed->text();
    bytedata= tmp.toLatin1(); // must
    ch=bytedata.data();
    setSta.Beng_Set=(unsigned short)str2inthandle(ch,0);

    tmp=ui->reachTemp->text();
    bytedata= tmp.toLatin1(); // must
    ch=bytedata.data();
    setSta.TempRech=(unsigned short)str2inthandle(ch,1);

    tmp=ui->setLed1->text();
    bytedata= tmp.toLatin1(); // must
    ch=bytedata.data();
    setSta.led1Set=(unsigned char)str2inthandle(ch,0);
    if(setSta.led1Set>=100)
    {
        setSta.led1Set=100;
        ui->setLed1->setText("100");
    }
    tmp=ui->setLed2->text();
    bytedata= tmp.toLatin1(); // must
    ch=bytedata.data();
    setSta.led2Set=(unsigned char)str2inthandle(ch,0);
    if(setSta.led2Set>=100)
    {
        setSta.led2Set=100;
        ui->setLed2->setText("100");
    }
    tmp=ui->setLed3->text();
    bytedata= tmp.toLatin1(); // must
    ch=bytedata.data();
    setSta.led3Set=(unsigned char)str2inthandle(ch,0);
    if(setSta.led3Set>=100)
    {
        setSta.led3Set=100;
        ui->setLed3->setText("100");
    }
    tmp=ui->setLed4->text();
    bytedata= tmp.toLatin1(); // must
    ch=bytedata.data();
    setSta.led4Set=(unsigned char)str2inthandle(ch,0);
    if(setSta.led4Set>=100)
    {
        setSta.led4Set=100;
        ui->setLed4->setText("100");
    }

}

//发送前数据准备
//目标 长度 指令 数据源
void MainWindow::ComQYPreSend(unsigned char *TxBufU3,unsigned short Len,unsigned short Cmd,unsigned char *pData )
{
    TxBufU3[0]=0x5e;//头部
    TxBufU3[1]=0xe5;
    TxBufU3[2]=Len>>0;//长度
    TxBufU3[3]=Len>>8;
    TxBufU3[4]=Cmd>>0;//命令
    TxBufU3[5]=Cmd>>8;
    if(Len>10)//data
    {
        memcpy(&TxBufU3[6],pData,Len-6);
    }
    if(Len>6)
    {
        TxBufU3[Len-4]=0xF1;//crc_l
        TxBufU3[Len-3]=0x1F;
        TxBufU3[Len-2]=0xF0;//tail_l
        TxBufU3[Len-1]=0x0F;
    }
    //设置运行数据拷贝到接收
}


//发送读取指令数据
void MainWindow::on_sendButton_clicked()
{
    unsigned char sendReadCode[200];//读取设备信息
    unsigned short len;
    unsigned short cmd;
    len=10;//总长度
    cmd=0x1000;//读取指令
    ComQYPreSend(sendReadCode,len,cmd,0);
    QByteArray QSsendData((char*)sendReadCode, len);

    //获取发送区的数据
    serial->write(QSsendData);
    ui->textEdit_2->append("读取指令：已发送指令");
    ui->textEdit_2->append(SendAppend(QSsendData));
}

//发送设置指令的数据
void MainWindow::on_setButton_clicked()
{

    QString tmp;
    float   ftmp;
    char*  ch;
    unsigned short TxLen;
    unsigned short TxCmd=0x1001;//设置指令
    QByteArray bytedata ;
    unsigned char sendReadCode[200];//设置

    TxLen=sizeof(SETSTADATA)+10;//6+data+4

    window2Data();

    ComQYPreSend(sendReadCode,TxLen,TxCmd,(unsigned char *)&setSta);//目标 长度 指令 数据源

    QByteArray QSsendData((char*)sendReadCode, TxLen);
    serial->write(QSsendData);
    ui->textEdit_2->append("设置指令：已发送指令");
    ui->textEdit_2->append(SendAppend(QSsendData));


    //设置的话保存创建设置项目
    QString     FilePath = qApp->applicationDirPath();//获取app当前路径
    QString     wrFile=FilePath+"/settings.ini";
    QFile writefile( wrFile );
    writefile.open( QIODevice::ReadWrite | QIODevice::Text );
    QTextStream stream(&writefile);

    QString Settings=QString("%1\n%2\n%3\n%4\n%5\n%6\n%7\n%8\n%9\n%10\n%11\n%12\n%13\n")
    .arg(setSta.Motor_SetAuto)
    .arg(setSta.Motor_SetManu)
    .arg(setSta.Beng_Set)
    .arg(setSta.MotoSpeed)
    .arg(setSta.Fa1_Set)
    .arg(setSta.Fa2_Set)
    .arg(setSta.led1Set)
    .arg(setSta.led2Set)
    .arg(setSta.led3Set)
    .arg(setSta.led4Set)
    .arg(setSta.TempMod)
    .arg(setSta.TempManuSw)
    .arg(setSta.TempRech);

    stream << Settings<< endl;

    writefile.close();



}

//收到一条数据
void MainWindow::ReceivedTimeUpdate()
{
    unsigned char *buff;
    unsigned short ReCmd;
    float a;
    m_timer->stop();
    QString RecQStr = QString(ComRecBuf);
    if(ComRecBuf.length()!=0)
    {
        unsigned int rlen=ComRecBuf.length();
        buff=(unsigned char *)ComRecBuf.data();

        if(buff[0]==0xAA&&buff[1]==0xBB&&buff[2]==0xCC&&(buff[3]==0x21||buff[3]==0x24))//升级过程中
        {
            SendAPart(buff,rlen);//在升级包处理函数里处理
        }else if(buff[0]==0xDD&&buff[1]==0xCC&&buff[2]==0xBB&&(buff[3]==0x11||buff[3]==0x14))//读取版本信息的返回
        {
            char st[10];
            st[0]=buff[4];
            st[1]=buff[5];
            st[2]=buff[6];
            st[3]=buff[7];
            st[4]=buff[8];
            st[5]=buff[9];
            st[6]=buff[10];
            st[7]=buff[11];
            st[8]=buff[12];
            st[9]=buff[13];
            unsigned char bootflag=buff[14];
            QString string = QString(QLatin1String(st));
            ui->Edit_Version->setText(string);
            if(bootflag==0)
            {
                ui->BoxMode->setCurrentIndex(0);
                if(buff[3]==0x11)ui->textEdit_3->append("本机版本号："+string+" Bootloader");
                if(buff[3]==0x14)ui->textEdit_3->append("远程CAN版本号："+string+" Bootloader");

            }else
            {
                ui->BoxMode->setCurrentIndex(1);
                if(buff[3]==0x11)ui->textEdit_3->append("本机版本号："+string+" Application");
                if(buff[3]==0x14)ui->textEdit_3->append("远程CAN版本号："+string+" Application");
            }
        }
        else if(//boot 的返回
                buff[0]==0xAA&&//升级码
                buff[1]==0xBB&&
                buff[2]==0xCC&&
                (buff[3]==0x11||buff[3]==0x14)&&
                buff[14]==0xAA&&
                buff[15]==0xBB&&
                buff[16]==0xCC&&
                buff[17]==0xDD)
        {
            char st[10];
            st[0]=buff[4];
            st[1]=buff[5];
            st[2]=buff[6];
            st[3]=buff[7];
            st[4]=buff[8];
            st[5]=buff[9];
            st[6]=buff[10];
            st[7]=buff[11];
            st[8]=buff[12];
            st[9]=buff[13];
            unsigned char bootflag=buff[14];
            QString string = QString(QLatin1String(st));
            if(buff[3]==0x11)ui->textEdit_3->append("已经下发本机版本号："+string+" 本机重启");
            if(buff[3]==0x14)ui->textEdit_3->append("远程下发CAN版本号："+string+" CAN设备重启");

            failedflag=0;
            part_curIndexLen=0;
            part_index=0;

        }
        else if(//退出boot 的返回
                buff[0]==0xA0&&
                buff[1]==0xA1&&
                buff[2]==0xA2&&
                (buff[3]==0x11||buff[3]==0x14)&&
                buff[14]==0xA0&&
                buff[15]==0xA1&&
                buff[16]==0xA2&&
                buff[17]==0xA3)
        {
            char st[10];
            st[0]=buff[4];
            st[1]=buff[5];
            st[2]=buff[6];
            st[3]=buff[7];
            st[4]=buff[8];
            st[5]=buff[9];
            st[6]=buff[10];
            st[7]=buff[11];
            st[8]=buff[12];
            st[9]=buff[13];
            unsigned char bootflag=buff[14];
            QString string = QString(QLatin1String(st));
            if(buff[3]==0x11)ui->textEdit_3->append("本机已经退出Boot进入app版本："+string+" 本机重启");
            if(buff[3]==0x14)ui->textEdit_3->append("远程CAN已经退出Boot进入app版本"+string+" CAN设备重启");

            failedflag=0;
            part_curIndexLen=0;
            part_index=0;

        }else if(decodeRec(buff,&ReCmd,rlen)==1)//数据返回基本正常
        {
            switch(ReCmd)
            {
            //读取指令的数据返回
            case 0x1000:
                memcpy((unsigned char *)&setSta,&buff[6],rlen-4);

                ui->textEdit->append("读取指令：数据返回");
                Data2window();
                ui->textEdit->append("读取指令：显示刷新");
                break;
            case 0x1001:
                ui->textEdit->append("设置指令：数据返回");
                break;
            }
        }else if(RecQStr.startsWith("sample:",Qt::CaseInsensitive))
        {
            QStringList parts = RecQStr.split(" ");
            if (parts.size() == 5)//被分隔成了5个 其中第0个为sample:
            {
                GGvalue1= parts.at(1).toDouble();
                GGvalue2= parts.at(2).toDouble();
                GGvalue3= parts.at(3).toDouble();
                GGvalue4= parts.at(4).toDouble();
                Show_Plot(pPlot1, GGvalue1,GGvalue2,GGvalue3,GGvalue4);
                ui->saveEdit->append(RecQStr);
            }

        }
    }

    ui->textEdit->append(SendAppend(ComRecBuf));//显示收到数据
    ComRecBuf.clear();
}


//锁定按钮
void MainWindow::on_setMotorReset_clicked()
{
    setSta.Motor_SetAuto=1;
    ui->setMotorOn->setEnabled(false);
    ui->setMotorOff->setEnabled(false);
    ui->setMotorReset->setEnabled(false);
    ui->setMotorRelease->setEnabled(true);
    on_setButton_clicked();
}
void MainWindow::on_setMotorRelease_clicked()
{
    setSta.Motor_SetAuto=0;
    ui->setMotorReset->setEnabled(true);
    ui->setMotorRelease->setEnabled(false);
    ui->setMotorOn->setEnabled(true);
    ui->setMotorOff->setEnabled(false);
    on_setButton_clicked();
}


void MainWindow::on_setMotorOn_clicked()
{
    setSta.Motor_SetManu=1;
    ui->setMotorOn->setEnabled(false);
    ui->setMotorOff->setEnabled(true);
    ui->Box_MotorSta->setCurrentIndex(1);
    on_setButton_clicked();
}

void MainWindow::on_setMotorOff_clicked()
{
    setSta.Motor_SetManu=0;
    ui->setMotorOn->setEnabled(true);
    ui->setMotorOff->setEnabled(false);
    ui->Box_MotorSta->setCurrentIndex(0);
    on_setButton_clicked();
}



void MainWindow::on_setBengOn_clicked()
{

    on_setButton_clicked();
    if(setSta.Beng_Set)
    {
        ui->Box_BengSta->setCurrentIndex(1);
    }else
    {
        ui->Box_BengSta->setCurrentIndex(0);
    }
}



void MainWindow::on_setFa1on_clicked()
{
    setSta.Fa1_Set=1;
    ui->setFa1on->setEnabled(false);
    ui->setFa1off->setEnabled(true);
    on_setButton_clicked();
    ui->Fa1Sta->setCurrentIndex(1);
}

void MainWindow::on_setFa1off_clicked()
{
    setSta.Fa1_Set=0;
    ui->setFa1on->setEnabled(true);
    ui->setFa1off->setEnabled(false);
    on_setButton_clicked();
    ui->Fa1Sta->setCurrentIndex(0);
}

void MainWindow::on_setFa2on_clicked()
{
    setSta.Fa2_Set=1;
    ui->setFa2on->setEnabled(false);
    ui->setFa2off->setEnabled(true);
    on_setButton_clicked();
    ui->Fa2Sta->setCurrentIndex(1);
}

void MainWindow::on_setFa2off_clicked()
{
    setSta.Fa2_Set=0;
    ui->setFa2on->setEnabled(true);
    ui->setFa2off->setEnabled(false);
    on_setButton_clicked();
    ui->Fa2Sta->setCurrentIndex(0);
}




void MainWindow::on_setL1OnOff_clicked()
{
    on_setButton_clicked();
    if(setSta.led1Set!=0)ui->led1Sw->setCurrentIndex(1);
    if(setSta.led1Set==0)ui->led1Sw->setCurrentIndex(0);
}

void MainWindow::on_setL2OnOff_clicked()
{
    on_setButton_clicked();
    if(setSta.led2Set!=0)ui->led2Sw->setCurrentIndex(1);
    if(setSta.led2Set==0)ui->led2Sw->setCurrentIndex(0);
}

void MainWindow::on_setL3OnOff_clicked()
{
    on_setButton_clicked();
    if(setSta.led3Set!=0)ui->led3Sw->setCurrentIndex(1);
    if(setSta.led3Set==0)ui->led3Sw->setCurrentIndex(0);
}

void MainWindow::on_setL4OnOff_clicked()
{
    on_setButton_clicked();
    if(setSta.led4Set!=0)ui->led4Sw->setCurrentIndex(1);
    if(setSta.led4Set==0)ui->led4Sw->setCurrentIndex(0);
}



void MainWindow::on_Box_TempMode_activated(const QString &arg1)
{
    if(ui->Box_TempMode->currentIndex()==0)//手动
    {
        setSta.TempMod=0;
        ui->manuTempHeatOn->setEnabled(true);
        ui->manuTempHeatOff->setEnabled(true);

    }else
    {
        ui->manuTempHeatOn->setEnabled(false);
        ui->manuTempHeatOff->setEnabled(false);
        setSta.TempMod=1;
    }
    on_setButton_clicked();
}

void MainWindow::on_manuTempHeatOn_clicked()//手动
{

    ui->Box_TempMode->setCurrentIndex(0);
    ui->manuTempHeatOn->setEnabled(false);
    ui->manuTempHeatOff->setEnabled(true);
    setSta.TempMod=0;
    setSta.TempManuSw=1;
    setSta.TempSWSta=1;
    on_setButton_clicked();
}

void MainWindow::on_manuTempHeatOff_clicked()
{
    ui->Box_TempMode->setCurrentIndex(0);
    ui->manuTempHeatOff->setEnabled(false);
    ui->manuTempHeatOn->setEnabled(true);
    setSta.TempMod=0;
    setSta.TempManuSw=0;
    setSta.TempSWSta=0;
    on_setButton_clicked();
}

