#include "mainwindow.h"
#include "ui_mainwindow.h"

QString MainWindow::SendAppend(QByteArray appQbyte)
{
    QString txstr(appQbyte.toHex());
    int len = txstr.length()/2;
    for(int i=1;i<len;i++)
    {
        txstr.insert(2*i+i-1," ");//每2个插入空格
    }
    return txstr;
}

// 将16进制字符串转换为对应的字节序列
QByteArray MainWindow::HexStringToByteArray(QString HexString)
{
    bool ok;
    QByteArray ret;
    HexString = HexString.trimmed();
    HexString = HexString.simplified();

    QStringList sl = HexString.split(" ");

    foreach (QString s, sl) {
        if(!s.isEmpty())
        {
            char c = s.toInt(&ok,16)&0xFF;
            if(ok){
                ret.append(c);
            }else{

                QMessageBox::warning(0,tr("错误："),QString("非法的16进制字符: \"%1\"").arg(s));
            }
        }
    }
    return ret;
}
