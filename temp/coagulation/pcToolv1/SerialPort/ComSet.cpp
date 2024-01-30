#include "mainwindow.h"
#include "ui_mainwindow.h"

//串口定时器初始化
void MainWindow::ComInitTimer()
{

    //if(NULL == m_timer)
    m_timer = new QTimer;
    //设置定时器是否为单次触发。默认为 false 多次触发
    m_timer->setSingleShot(false);
    //启动或重启定时器, 并设置定时器时间：毫秒
    //m_timer->start(100);
    //定时器触发信号槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(ReceivedTimeUpdate()));

}

//读取接收到的数据
void MainWindow::Read_Data()
{
    m_timer->start(20);//继续延后20ms再结束
    ComRecBuf.append(serial->readAll());

}

//打开串口
void MainWindow::on_openButton_clicked()
{

    if(ui->openButton->text()==tr("打开串口"))
    {
        qDebug() << tr("打开串口");
        serial = new QSerialPort;
        //设置串口名
        serial->setPortName(ui->PortBox->currentText());
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率
        serial->setBaudRate(ui->BaudBox->currentText().toInt());
        //设置数据位数
        switch(ui->BitNumBox->currentIndex())
        {
        case 8: serial->setDataBits(QSerialPort::Data8); break;
        default: break;
        }
        //设置奇偶校验
        switch(ui->ParityBox->currentIndex())
        {
        case 0: serial->setParity(QSerialPort::NoParity); break;
        default: break;
        }
        //设置停止位
        switch(ui->StopBox->currentIndex())
        {
        case 1: serial->setStopBits(QSerialPort::OneStop); break;
        case 2: serial->setStopBits(QSerialPort::TwoStop); break;
        default: break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);

        //关闭设置菜单使能
        ui->PortBox->setEnabled(false);//打开后 box变灰  不能设置
        ui->BaudBox->setEnabled(false);//打开后 box变灰  不能设置
        ui->BitNumBox->setEnabled(false);//打开后 box变灰  不能设置
        ui->ParityBox->setEnabled(false);//打开后 box变灰  不能设置
        ui->StopBox->setEnabled(false);//打开后 box变灰  不能设置
        ui->openButton->setText(tr("关闭串口"));//打开后  变成 关闭串口
        ui->sendButton->setEnabled(true);
        ui->clearButton->setEnabled(true);

        serial->setReadBufferSize(1000);

        //连接信号槽 当串口收到数据并且接收完毕后，会发出一个QSerialPort::readyRead()的信号
        QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::Read_Data);//把信号和槽函数连接起来

        on_sendButton_clicked();
    }
    else
    {
        qDebug() << tr("关闭串口");
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();
        findCom();//重新找下串口
        //恢复设置使能
        ui->PortBox->setEnabled(true);
        ui->BaudBox->setEnabled(true);
        ui->BitNumBox->setEnabled(true);
        ui->ParityBox->setEnabled(true);
        ui->StopBox->setEnabled(true);
        ui->openButton->setText(tr("打开串口"));
        ui->sendButton->setEnabled(false);
        ui->clearButton->setEnabled(false);
    }
}

//
void MainWindow::findCom(void)
{
    int i=0;
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            ui->PortBox->addItem(serial.portName());//
            qDebug() << serial.portName();
            serial.close();
            ui->PortBox->setCurrentIndex(i++);//选择最大的端口号
        }
    }
}
