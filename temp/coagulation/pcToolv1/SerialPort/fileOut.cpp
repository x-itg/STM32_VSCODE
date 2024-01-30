#include "mainwindow.h"
#include "ui_mainwindow.h"
//将数据显示区数据导出
void MainWindow::on_Savegp_clicked()
{
    //step1：生成txt文件
    QString filename = QFileDialog::getSaveFileName(this, tr("Save txt"), "",tr("Txt files (*.txt)"));

    if (!filename.isEmpty()) {
        //ui->plot->savePdf(filename);

        QFile file(filename);//文件命名
        if (!file.open(QFile::WriteOnly | QFile::Text))     //检测文件是否打开
        {
            QMessageBox::information(this, "Error Message", "Please Select a Text File!");
            return;
        }
        QTextStream out(&file);                 //分行写入文件
        out << ui->saveEdit->toPlainText();
        ui->saveEdit->clear();
        file.close();//关闭文件
    }

}
