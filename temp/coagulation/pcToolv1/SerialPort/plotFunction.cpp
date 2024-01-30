#include "mainwindow.h"
#include "ui_mainwindow.h"
// 绘图图表初始化
void MainWindow::QPlot_init(QCustomPlot *customPlot)
{

    // 创建定时器，用于定时生成曲线坐标点数据
    QTimer *QPlottimer = new QTimer(this);
    QPlottimer->start(100);
    connect(QPlottimer,SIGNAL(timeout()),this,SLOT(TimeData_Update()));

    // 图表添加两条曲线
    pGraph1_1 = customPlot->addGraph();
    pGraph1_2 = customPlot->addGraph();
    pGraph1_3 = customPlot->addGraph();
    pGraph1_4 = customPlot->addGraph();

    // 设置曲线颜色
    pGraph1_1->setPen(QPen(Qt::red));
    pGraph1_2->setPen(QPen(Qt::black));
    pGraph1_3->setPen(QPen(Qt::blue));
    pGraph1_4->setPen(QPen(Qt::green));

    // 设置坐标轴名称
    customPlot->xAxis->setLabel("X");
    customPlot->yAxis->setLabel("Y");

    // 设置y坐标轴显示范围
    customPlot->yAxis->setRange(-1000,70000);

    // 显示图表的图例
    customPlot->legend->setVisible(true);
    // 添加曲线名称
    pGraph1_1->setName("感光波形1");
    pGraph1_2->setName("感光波形2");
    pGraph1_3->setName("感光波形3");
    pGraph1_4->setName("感光波形4");
    // 设置波形曲线的复选框字体颜色
    ui->checkBox_1->setStyleSheet("QCheckBox{color:rgb(255,0,0)}");//设定前景颜色,就是字体颜色
    ui->checkBox_2->setStyleSheet("QCheckBox{color:rgb(21,5,23)}");//设定前景颜色,就是字体颜色
    ui->checkBox_3->setStyleSheet("QCheckBox{color:rgb(58,216,254)}");//设定前景颜色,就是字体颜色
    ui->checkBox_4->setStyleSheet("QCheckBox{color:rgb(105,247,106)}");//设定前景颜色,就是字体颜色
    // 允许用户用鼠标拖动轴范围，用鼠标滚轮缩放，点击选择图形:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}
// 曲线更新绘图
void MainWindow::Show_Plot(QCustomPlot *customPlot, double num1,double num2,double num3,double num4)
{
    static double cnt;

    cnt++;
    // 给曲线添加数据
    pGraph1_1->addData(cnt, num1);
    pGraph1_2->addData(cnt, num2);
    pGraph1_3->addData(cnt, num3);
    pGraph1_4->addData(cnt, num4);
    // 设置x坐标轴显示范围，使其自适应缩放x轴，x轴最大显示1000个点
    customPlot->xAxis->setRange((pGraph1_1->dataCount()>1000)?(pGraph1_1->dataCount()-1000):0, pGraph1_1->dataCount());
    // 更新绘图，这种方式在高填充下太浪费资源。有另一种方式rpQueuedReplot，可避免重复绘图。
    // 最好的方法还是将数据填充、和更新绘图分隔开。将更新绘图单独用定时器更新。例程数据量较少没用单独定时器更新，实际工程中建议大家加上。
    //customPlot->replot();
    customPlot->replot(QCustomPlot::rpQueuedReplot);

    static QTime time(QTime::currentTime());
    double key = time.elapsed()/1000.0; // 开始到现在的时间，单位秒
    ////计算帧数
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 1) // 每1秒求一次平均值
    {
        //状态栏显示帧数和数据总数
        lastFpsKey = key;
        frameCount = 0;
    }

}
void MainWindow::on_checkBox_1_stateChanged(int arg1)
{
    if(arg1){
        // 显示
        pGraph1_1->setPen(QPen(Qt::red));
    }else{
        // 不显示，透明色
        pGraph1_1->setPen(QColor(0,0,0,0));
    }
    pPlot1->replot();
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1){
        pGraph1_2->setVisible(true);
    }else{
        pGraph1_2->setVisible(false);//void QCPLayerable::setVisible(bool on)
    }
    pPlot1->replot();
}


void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    if(arg1){
        pGraph1_3->setVisible(true);
    }else{
        pGraph1_3->setVisible(false);//void QCPLayerable::setVisible(bool on)
    }
    pPlot1->replot();
}

void MainWindow::on_checkBox_4_stateChanged(int arg1)
{
    if(arg1){
        pGraph1_4->setVisible(true);
    }else{
        pGraph1_4->setVisible(false);//void QCPLayerable::setVisible(bool on)
    }
    pPlot1->replot();
}
