#include "mainwindow.h"
#include "ui_mainwindow.h"
//刷新dpi
void MainWindow::refreshDPI()
{
    //计算dpi
    QList<QScreen*> screens = QApplication::screens();
    QScreen* screen = screens[0];
    double dpi = screen->logicalDotsPerInch();
    //计算dpi对应的缩放比例
    double objectRate = dpi/96.0;
    changeObjectSize(*this, objectRate);
    qDebug()<<"width "<<width() << "height "<< height();
    resize(width()*objectRate,height()*objectRate);
}
//修改所有控件尺寸
void MainWindow::changeObjectSize(const QObject &o, double objectRate)
{
    for (int i=0; i<o.children().size(); ++i) {
        QWidget *pWidget = qobject_cast<QWidget *>(o.children().at(i));
        if (pWidget != nullptr) {
            qDebug() << pWidget->width() << pWidget->height();
            //pWidget->resize(pWidget->width()*objectRate, pWidget->height()*objectRate);
            pWidget->setGeometry(pWidget->x()*objectRate,pWidget->y()*objectRate,
                                 pWidget->width()*objectRate, pWidget->height()*objectRate);
            changeObjectSize(*(o.children().at(i)),objectRate);
        }
    }
}
