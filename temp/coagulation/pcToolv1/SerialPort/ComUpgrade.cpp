#include "mainwindow.h"
#include "ui_mainwindow.h"
#pragma pack(1)
typedef struct {
  unsigned char  head[4];
  unsigned short pack_n;
  unsigned short pack_i;
  unsigned short pack_l;
  unsigned short crcLH;
  unsigned char  tail[4];

}PACKACKDATA;
PACKACKDATA PackRecData;
//延时函数
void MainWindow::delaymsec(int msec) {
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

}
//分包
void MainWindow::on_qPack_clicked()
{
    part_list.clear();//清除分包
    QFile bin_file(ui->FilePath->text());
    QByteArray file_array;
    if(bin_file.open(QIODevice::ReadOnly))
    {//成功读取文件
        ui->textEdit_3->append("文件打开成功! 确认设备处于升级模式，并连接后即可升级");
        file_array = bin_file.readAll();
    }
    else
    {//读取文件失败
        ui->textEdit_3->append("文件打开失败，请检查文件路径");
        return;
    }
    ui->textEdit_3->append("请等待......");
    /************文件分包******************/
    a_part_size=128;//一包纯数据部分大小是1024字节
    allpartsize=file_array.length();
    part_num = file_array.length()/a_part_size + 1;//包的个数
    each_part_bar = 100.0/((double)part_num-1);//每个包占进度条的大小比率
    ui->textEdit_3->append("分包个数: "+QString::number(part_num));
    qDebug()<<"this data has :" << file_array.length() << "could divide:" << part_num << "each part" << each_part_bar;
    if(!part_list.isEmpty())
    {
        qDebug()<< "not empty," << part_list;
    }
    int part=0;
    for(part = 0; part < part_num; part++)//进行分包
    {
        part_list << file_array.mid( part*a_part_size, a_part_size);


    }
    ui->textEdit_3->append("文件分包: "+QString::number(part_num-1)+"个128字节+1个"+QString::number(part_list[part-1].length())+"字节");
    part_index=0;//从第0个开始发
    statusCode=0;

    file_array.clear();

}

//根据收到的 处理update包
unsigned char MainWindow::SendAPart(unsigned char *recp,unsigned int reclen)
{
    count.stop();
    QByteArray DataPack;
    QByteArray SendHeadCode;
    unsigned char *pChar;
    unsigned short CrcTemp;
    unsigned int i=0;
    unsigned int j=0;
    unsigned int k=0;

    unsigned char txbuf[256];
    int Len;

    currprogress=(int)(each_part_bar*((double)part_index+1.0));
    ui->progressBar->setValue(currprogress);

    //delaymsec(200);
    if(reclen==0)//第一次发送接收到的长度是0，就是不管怎么样都是要发包的，第二次以后根据接收到的应答数据判断数据是否出错如果有问题就暂停了
    {
        part_index=0;
        DataPack=part_list[part_index];
        pChar=(unsigned char *)DataPack.data();
        part_curIndexLen=DataPack.length();
        i=0;
        txbuf[i++]=0xAA;
        txbuf[i++]=0xBB;
        txbuf[i++]=0xCC;
        txbuf[i++]=0x21+ui->Box_updateMode->currentIndex();

        txbuf[i++]=part_num>>0;//包个数
        txbuf[i++]=part_num>>8;
        txbuf[i++]=part_index>>0;//包序号
        txbuf[i++]=part_index>>8;
        txbuf[i++]=part_curIndexLen>>0;//这个包的长度
        txbuf[i++]=part_curIndexLen>>8;
        for(j=0;j<part_curIndexLen;)//数据部分
        {
            txbuf[i++]=pChar[j++];
        }
        appendCrcLH(txbuf+4,i-4);//追加校验
        i++;
        i++;
        txbuf[i++]=0xAB;
        txbuf[i++]=0xBC;
        txbuf[i++]=0xCD;
        txbuf[i++]=0xDE;
        for(k=0;k<i;k++)
        {
               SendPack[k]= txbuf[k];
        }
        SendPack.resize(i);
        ui->textEdit_2->append(SendAppend(SendPack));
        serial->write(SendPack);
        return 1;

    }else//收到后再发送都是有长度的
    {
        //得到数值
        pChar=(unsigned char *)&PackRecData;//
        for(j=0;j<sizeof(PACKACKDATA);)
        {
          *pChar++=recp[j++];
        }

        //计算校验
        CrcTemp=CRC16x(recp+4,6);

        if(PackRecData.head[0]!=0xAA        ||
           PackRecData.head[1]!=0xBB        ||
           PackRecData.head[2]!=0xCC        ||
           PackRecData.tail[0]!=0xAB        ||
           PackRecData.tail[1]!=0xBC        ||
           PackRecData.tail[2]!=0xCD        ||
           PackRecData.tail[3]!=0xDE        ||
           PackRecData.crcLH!=CrcTemp       ||
           PackRecData.pack_n!=part_num     ||
           PackRecData.pack_i!=part_index   ||
           PackRecData.pack_l!=part_curIndexLen
           )
        {
            failedflag=1;//有问题但是不打断进程
            part_index=part_num;//有问题直接跳到最后
            ui->textEdit_3->append("第"+QString::number(part_index)+"个Pack下发失败size:"+QString::number(PackRecData.pack_l)+"Bytes");
        }else
        {
            ui->textEdit_3->append("第"+QString::number(part_index)+"个Pack下发成功size:"+QString::number(PackRecData.pack_l)+"Bytes");
        }


        if(part_num>part_index+1)//还有包需要发送
        {
            part_index++;
            DataPack=part_list[part_index];
            pChar=(unsigned char *)DataPack.data();
            part_curIndexLen=DataPack.length();
            i=0;
            txbuf[i++]=0xAA;
            txbuf[i++]=0xBB;
            txbuf[i++]=0xCC;
            txbuf[i++]=0x21+ui->Box_updateMode->currentIndex();
            txbuf[i++]=part_num>>0;//包个数
            txbuf[i++]=part_num>>8;
            txbuf[i++]=part_index>>0;//包序号
            txbuf[i++]=part_index>>8;
            txbuf[i++]=part_curIndexLen>>0;//这个包的长度
            txbuf[i++]=part_curIndexLen>>8;
            for(j=0;j<part_curIndexLen;)//数据部分
            {
                txbuf[i++]=pChar[j++];
            }
            appendCrcLH(txbuf+4,i-4);//追加校验
            i++;
            i++;
            txbuf[i++]=0xAB;
            txbuf[i++]=0xBC;
            txbuf[i++]=0xCD;
            txbuf[i++]=0xDE;
            for(k=0;k<i;k++)
            {
                   SendPack[k]= txbuf[k];
            }
            SendPack.resize(i);
            ui->textEdit_2->append(SendAppend(SendPack));
            serial->write(SendPack);
            return 1;

        }else //发完了
        {
            on_escBoot_clicked();
            return 1;
        }
    }
    return 0;
}
//读取版本号
void MainWindow::on_readVersion_clicked()
{
        //使得进入iap模式
        QByteArray msg;
        int i=0;
        msg[i++]=0xDD;
        msg[i++]=0xCC;
        msg[i++]=0xBB;
        msg[i++]=ui->Box_updateMode->currentIndex()+0x11;
        msg[i++]=0x00;
        msg[i++]=0x01;
        msg[i++]=0x02;
        msg[i++]=0x03;
        msg[i++]=0x04;
        msg[i++]=0x05;
        msg[i++]=0x06;
        msg[i++]=0x07;
        msg[i++]=0x08;
        msg[i++]=0x09;
        serial->write(msg);
        ui->textEdit_3->append("正在读取版本信息");
        ui->textEdit_2->append(SendAppend(msg));

}
//确认一键升级
void MainWindow::on_UpdateCode_clicked()
{
    ui->textEdit_3->clear();
    QMessageBox::StandardButton rb ;
    ui->escBoot->setEnabled(false);
    fileName=ui->FilePath->text();//获取升级文件的路径
    if(fileName.contains(".bin",Qt::CaseSensitive))//如果是bin文件
    {
        ui->textEdit_3->append("文件正在分包...");
        on_qPack_clicked();//分包
        delaymsec(100);
        SendAPart(0,0);//串口模式升级自身
    }else//非bin文件
    {
        ui->textEdit_3->append("文件无效请重新选择");
        rb= QMessageBox::question(NULL, "提示", "无效文件，请重新选择！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if(rb == QMessageBox::Yes)
        {
            on_ChooseFile_clicked();//选择文件
        }
    }
}

//进入升级模式
void MainWindow::on_EnterBoot_clicked()
{
    {
        ui->textEdit_3->append("进入Bootloader指令已发送");
        ui->textEdit_3->append("新版本信息已经保存");
        QByteArray bytes = ui->Edit_Version->text().toUtf8();
        //使得进入iap模式
        QByteArray msg;
        int i=0;
        msg[i++]=0xAA;
        msg[i++]=0xBB;
        msg[i++]=0xCC;
        msg[i++]=ui->Box_updateMode->currentIndex()+0x11;
        msg[i++]=bytes[0];//0x37;//下面版本号从这里
        msg[i++]=bytes[1];//0x41;
        msg[i++]=bytes[2];//0x2E;
        msg[i++]=bytes[3];//0x62;
        msg[i++]=bytes[4];//0x69;
        msg[i++]=bytes[5];//0x6E;
        msg[i++]=bytes[6];//0x00;
        msg[i++]=bytes[7];//0x6F;
        msg[i++]=bytes[8];//0x63;
        msg[i++]=bytes[9];//0x74;//
        msg[i++]=0xAA;
        msg[i++]=0xBB;
        msg[i++]=0xCC;
        msg[i++]=0xDD;
        serial->write(msg);
        ui->textEdit_2->append(SendAppend(msg));
        failedflag=0;
        part_curIndexLen=0;
        part_index=0;
    }
}

//退出boot进入APP
void MainWindow::on_escBoot_clicked()
{
    if(failedflag==0)
    {

        ui->textEdit_3->append("退出Boot进入APP指令已发送");
        //使得进入iap模式
        QByteArray msg;
        int i=0;
        msg[i++]=0xA0;
        msg[i++]=0xA1;
        msg[i++]=0xA2;
        msg[i++]=ui->Box_updateMode->currentIndex()+0x11;
        msg[i++]=0;//0x37;//下面版本号从这里
        msg[i++]=0;//0x41;
        msg[i++]=0;//0x2E;
        msg[i++]=0;//0x62;
        msg[i++]=0;//0x69;
        msg[i++]=0;//0x6E;
        msg[i++]=0;//0x00;
        msg[i++]=0;//0x6F;
        msg[i++]=0;//0x63;
        msg[i++]=0;//0x74;//
        msg[i++]=0xA0;
        msg[i++]=0xA1;
        msg[i++]=0xA2;
        msg[i++]=0xA3;
        serial->write(msg);
        ui->textEdit_2->append(SendAppend(msg));
        failedflag=0;
        part_curIndexLen=0;
        part_index=0;

    }else
    {
        ui->escBoot->setEnabled(false);
        ui->textEdit_3->append("升级失败，请按照第①步 使设备进入升级模式，如设备已处于升级模式，请断电重启设备并重新尝试升级。");
    }
}


//遍历文件夹
QString MainWindow::Traversal(QString path)                //遍历文件夹里面的 .h 文件的方法
{
    QDir dir(path);
    QString  retpath;
    int flag=0;
    QFileInfoList list = dir.entryInfoList();
    QDateTime dtcompare=QDateTime::fromString(QString("2020-01-01 23:59:59"), "yyyy-MM-dd hh:mm:ss");

    for (int i = 0; i < list.size(); i++) {

        QFileInfo info = list.at(i);
        if(info.suffix()=="bin")
        {
            QString file_size = tr("%1").arg(info.size());

            QDateTime dt = info.created();

            QString create_time = tr("%1").arg(dt.toString("yyyy-MM-dd hh:mm:ss"));

            dt = info.lastModified();
            if(dt>dtcompare)
            {
                dtcompare=dt;//取得最新的时间
                retpath=info.path()+"/"+info.fileName();;
                flag=1;
            }

            QString modify_time = tr("%1").arg(dt.toString("yyyy-MM-dd hh:mm:ss"));
            qDebug() << "文件名:" << info.fileName()  << " 大小(Byte) :" << file_size << " 创建时间 : " << create_time << " 创建时间 : "<<modify_time ;
        }

    }
    if(flag==1)
    {
        ui->textEdit_3->append(tr("获取文件成功，当前文件路径：%1").arg(retpath));
        return retpath;
    }else
    {
        retpath="Get xxxxxxxxxx.Bin File Failed";
        ui->textEdit_3->append(tr("获取文件失败，当前文件路径 %1").arg(path));
        return retpath;
    }
}
//获取当前路径下升级文件
void MainWindow::on_GetNewFile_clicked()
{
    QString curretFiepath = QDir::currentPath();
    ui->FilePath->setText(Traversal(curretFiepath));
}

//手动获取升级文件
void MainWindow::on_ChooseFile_clicked()
{
    ui->progressBar->setValue(0);
    fileName = QFileDialog::getOpenFileName( this,tr("文件对话框！"),"D:",tr("烧录文件(*bin);;""所有文件(*)"));
    ui->FilePath->setText(fileName);//输入框中放文件路径的字符串
}
