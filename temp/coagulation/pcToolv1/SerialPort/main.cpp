#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    //QGuiApplication::setAttribute(Qt::AA_DisableHighDpiScaling,true);
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Qingyun TOOL 220305");
    w.show();

    return a.exec();

}
