/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *paratab;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_8;
    QLabel *label_45;
    QComboBox *PortBox;
    QVBoxLayout *verticalLayout;
    QLabel *label_7;
    QComboBox *StopBox;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_6;
    QComboBox *ParityBox;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_5;
    QComboBox *BitNumBox;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_8;
    QComboBox *BaudBox;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *openButton;
    QPushButton *clearButton;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QTextEdit *textEdit_2;
    QTextEdit *saveEdit;
    QPushButton *Savegp;
    QTextEdit *textEdit;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLineEdit *Edit_BengSpeed;
    QPushButton *setMotorRelease;
    QComboBox *Box_MotorSta;
    QComboBox *Fa1Sta;
    QLabel *label_27;
    QLineEdit *currentTemp;
    QComboBox *Box_MotorDianSta;
    QPushButton *setFa2off;
    QComboBox *Box_BengSta;
    QPushButton *setMotorOff;
    QComboBox *Fa2Sta;
    QPushButton *setFa1off;
    QPushButton *setMotorOn;
    QPushButton *setMotorReset;
    QLabel *label_15;
    QPushButton *setFa1on;
    QLabel *label_12;
    QLabel *label_11;
    QPushButton *setBengOn;
    QLineEdit *reachTemp;
    QLabel *label_26;
    QPushButton *setFa2on;
    QLabel *label_16;
    QLabel *label_13;
    QComboBox *Box_TempMode;
    QPushButton *manuTempHeatOn;
    QPushButton *manuTempHeatOff;
    QComboBox *TempHeatSta;
    QLabel *label_46;
    QLineEdit *Edit_MotorSpeed;
    QLabel *label_51;
    QPushButton *sendButton;
    QPushButton *setButton;
    QPushButton *recChooseFile;
    QPushButton *startRec;
    QLineEdit *QLineEditrecFilePath;
    QPushButton *line1_2;
    QPushButton *line1_3;
    QFrame *line_2;
    QFrame *line_3;
    QFrame *line_5;
    QWidget *gridLayoutWidget_4;
    QGridLayout *gridLayout_4;
    QLineEdit *shengyuSec;
    QLineEdit *recADFreq;
    QComboBox *Ka;
    QLabel *label_14;
    QLineEdit *YaliValue;
    QLabel *label_17;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QLineEdit *recTime;
    QFrame *line_6;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_3;
    QPushButton *setL4OnOff;
    QPushButton *setL3OnOff;
    QComboBox *led1Sw;
    QLabel *label_55;
    QLabel *label_53;
    QPushButton *setL2OnOff;
    QComboBox *led3Sw;
    QComboBox *led4Sw;
    QPushButton *setL1OnOff;
    QComboBox *led2Sw;
    QComboBox *Kong3;
    QComboBox *Kong1;
    QComboBox *Kong2;
    QLabel *label_52;
    QComboBox *Kong4;
    QLineEdit *led3AD;
    QLineEdit *led1AD;
    QLabel *label_54;
    QLineEdit *led4AD;
    QLineEdit *led2AD;
    QLineEdit *setLed1;
    QLineEdit *setLed2;
    QLineEdit *setLed3;
    QLineEdit *setLed4;
    QLabel *label_56;
    QCustomPlot *plot;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *sendButton_2;
    QPushButton *sendButton_3;
    QPushButton *sendButton_4;
    QTextEdit *textEdit_4;
    QTextEdit *textEdit_5;
    QTextEdit *textEdit_6;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_5;
    QCheckBox *checkBox_1;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QWidget *updatetab;
    QWidget *layoutWidget2;
    QVBoxLayout *verticalLayout_10;
    QLabel *label_44;
    QTextEdit *textEdit_3;
    QHBoxLayout *horizontalLayout_20;
    QHBoxLayout *horizontalLayout_22;
    QLabel *label_48;
    QProgressBar *progressBar;
    QHBoxLayout *horizontalLayout_27;
    QLineEdit *FilePath;
    QPushButton *ChooseFile;
    QPushButton *GetNewFile;
    QPushButton *escBoot;
    QComboBox *Box_updateMode;
    QHBoxLayout *horizontalLayout_23;
    QPushButton *readVersion;
    QLineEdit *Edit_Version;
    QComboBox *BoxMode;
    QPushButton *EnterBoot;
    QHBoxLayout *horizontalLayout_24;
    QPushButton *UpdateCode;
    QHBoxLayout *horizontalLayout_21;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1400, 788);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setGeometry(QRect(0, 0, 1391, 791));
        tabWidget->setCursor(QCursor(Qt::ArrowCursor));
        tabWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
        tabWidget->setStyleSheet(QStringLiteral(""));
        paratab = new QWidget();
        paratab->setObjectName(QStringLiteral("paratab"));
        layoutWidget = new QWidget(paratab);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 741, 101));
        verticalLayout_6 = new QVBoxLayout(layoutWidget);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        label_45 = new QLabel(layoutWidget);
        label_45->setObjectName(QStringLiteral("label_45"));

        verticalLayout_8->addWidget(label_45);

        PortBox = new QComboBox(layoutWidget);
        PortBox->setObjectName(QStringLiteral("PortBox"));
        PortBox->setIconSize(QSize(32, 32));

        verticalLayout_8->addWidget(PortBox);


        horizontalLayout_3->addLayout(verticalLayout_8);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        verticalLayout->addWidget(label_7);

        StopBox = new QComboBox(layoutWidget);
        StopBox->setObjectName(QStringLiteral("StopBox"));
        StopBox->setMinimumSize(QSize(0, 0));
        StopBox->setSizeIncrement(QSize(0, 0));

        verticalLayout->addWidget(StopBox);


        horizontalLayout_3->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        verticalLayout_2->addWidget(label_6);

        ParityBox = new QComboBox(layoutWidget);
        ParityBox->setObjectName(QStringLiteral("ParityBox"));

        verticalLayout_2->addWidget(ParityBox);


        horizontalLayout_3->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout_3->addWidget(label_5);

        BitNumBox = new QComboBox(layoutWidget);
        BitNumBox->setObjectName(QStringLiteral("BitNumBox"));

        verticalLayout_3->addWidget(BitNumBox);


        horizontalLayout_3->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        verticalLayout_4->addWidget(label_8);

        BaudBox = new QComboBox(layoutWidget);
        BaudBox->setObjectName(QStringLiteral("BaudBox"));

        verticalLayout_4->addWidget(BaudBox);


        horizontalLayout_3->addLayout(verticalLayout_4);


        verticalLayout_6->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        openButton = new QPushButton(layoutWidget);
        openButton->setObjectName(QStringLiteral("openButton"));

        horizontalLayout_5->addWidget(openButton);

        clearButton = new QPushButton(layoutWidget);
        clearButton->setObjectName(QStringLiteral("clearButton"));

        horizontalLayout_5->addWidget(clearButton);


        verticalLayout_6->addLayout(horizontalLayout_5);

        gridLayoutWidget = new QWidget(paratab);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(0, 580, 741, 171));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        textEdit_2 = new QTextEdit(gridLayoutWidget);
        textEdit_2->setObjectName(QStringLiteral("textEdit_2"));

        gridLayout->addWidget(textEdit_2, 1, 0, 1, 1);

        saveEdit = new QTextEdit(gridLayoutWidget);
        saveEdit->setObjectName(QStringLiteral("saveEdit"));

        gridLayout->addWidget(saveEdit, 1, 2, 1, 1);

        Savegp = new QPushButton(gridLayoutWidget);
        Savegp->setObjectName(QStringLiteral("Savegp"));
        QFont font;
        font.setFamily(QStringLiteral("Agency FB"));
        font.setPointSize(10);
        Savegp->setFont(font);

        gridLayout->addWidget(Savegp, 0, 2, 1, 1);

        textEdit = new QTextEdit(gridLayoutWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        gridLayout->addWidget(textEdit, 1, 1, 1, 1);

        gridLayoutWidget_2 = new QWidget(paratab);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(0, 140, 511, 271));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setSpacing(8);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        Edit_BengSpeed = new QLineEdit(gridLayoutWidget_2);
        Edit_BengSpeed->setObjectName(QStringLiteral("Edit_BengSpeed"));

        gridLayout_2->addWidget(Edit_BengSpeed, 4, 2, 1, 1);

        setMotorRelease = new QPushButton(gridLayoutWidget_2);
        setMotorRelease->setObjectName(QStringLiteral("setMotorRelease"));

        gridLayout_2->addWidget(setMotorRelease, 0, 2, 1, 1);

        Box_MotorSta = new QComboBox(gridLayoutWidget_2);
        Box_MotorSta->setObjectName(QStringLiteral("Box_MotorSta"));
        Box_MotorSta->setEnabled(false);

        gridLayout_2->addWidget(Box_MotorSta, 2, 3, 1, 1);

        Fa1Sta = new QComboBox(gridLayoutWidget_2);
        Fa1Sta->setObjectName(QStringLiteral("Fa1Sta"));
        Fa1Sta->setEnabled(false);

        gridLayout_2->addWidget(Fa1Sta, 5, 3, 1, 1);

        label_27 = new QLabel(gridLayoutWidget_2);
        label_27->setObjectName(QStringLiteral("label_27"));

        gridLayout_2->addWidget(label_27, 8, 2, 1, 1);

        currentTemp = new QLineEdit(gridLayoutWidget_2);
        currentTemp->setObjectName(QStringLiteral("currentTemp"));

        gridLayout_2->addWidget(currentTemp, 8, 3, 1, 1);

        Box_MotorDianSta = new QComboBox(gridLayoutWidget_2);
        Box_MotorDianSta->setObjectName(QStringLiteral("Box_MotorDianSta"));
        Box_MotorDianSta->setEnabled(false);

        gridLayout_2->addWidget(Box_MotorDianSta, 0, 3, 1, 1);

        setFa2off = new QPushButton(gridLayoutWidget_2);
        setFa2off->setObjectName(QStringLiteral("setFa2off"));
        setFa2off->setEnabled(false);

        gridLayout_2->addWidget(setFa2off, 6, 2, 1, 1);

        Box_BengSta = new QComboBox(gridLayoutWidget_2);
        Box_BengSta->setObjectName(QStringLiteral("Box_BengSta"));
        Box_BengSta->setEnabled(false);

        gridLayout_2->addWidget(Box_BengSta, 4, 3, 1, 1);

        setMotorOff = new QPushButton(gridLayoutWidget_2);
        setMotorOff->setObjectName(QStringLiteral("setMotorOff"));
        setMotorOff->setEnabled(false);

        gridLayout_2->addWidget(setMotorOff, 2, 2, 1, 1);

        Fa2Sta = new QComboBox(gridLayoutWidget_2);
        Fa2Sta->setObjectName(QStringLiteral("Fa2Sta"));
        Fa2Sta->setEnabled(false);

        gridLayout_2->addWidget(Fa2Sta, 6, 3, 1, 1);

        setFa1off = new QPushButton(gridLayoutWidget_2);
        setFa1off->setObjectName(QStringLiteral("setFa1off"));
        setFa1off->setEnabled(false);

        gridLayout_2->addWidget(setFa1off, 5, 2, 1, 1);

        setMotorOn = new QPushButton(gridLayoutWidget_2);
        setMotorOn->setObjectName(QStringLiteral("setMotorOn"));
        setMotorOn->setEnabled(true);

        gridLayout_2->addWidget(setMotorOn, 2, 1, 1, 1);

        setMotorReset = new QPushButton(gridLayoutWidget_2);
        setMotorReset->setObjectName(QStringLiteral("setMotorReset"));
        setMotorReset->setEnabled(false);

        gridLayout_2->addWidget(setMotorReset, 0, 1, 1, 1);

        label_15 = new QLabel(gridLayoutWidget_2);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_2->addWidget(label_15, 2, 0, 1, 1);

        setFa1on = new QPushButton(gridLayoutWidget_2);
        setFa1on->setObjectName(QStringLiteral("setFa1on"));

        gridLayout_2->addWidget(setFa1on, 5, 1, 1, 1);

        label_12 = new QLabel(gridLayoutWidget_2);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_2->addWidget(label_12, 5, 0, 1, 1);

        label_11 = new QLabel(gridLayoutWidget_2);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_2->addWidget(label_11, 4, 0, 1, 1);

        setBengOn = new QPushButton(gridLayoutWidget_2);
        setBengOn->setObjectName(QStringLiteral("setBengOn"));

        gridLayout_2->addWidget(setBengOn, 4, 1, 1, 1);

        reachTemp = new QLineEdit(gridLayoutWidget_2);
        reachTemp->setObjectName(QStringLiteral("reachTemp"));

        gridLayout_2->addWidget(reachTemp, 8, 1, 1, 1);

        label_26 = new QLabel(gridLayoutWidget_2);
        label_26->setObjectName(QStringLiteral("label_26"));

        gridLayout_2->addWidget(label_26, 8, 0, 1, 1);

        setFa2on = new QPushButton(gridLayoutWidget_2);
        setFa2on->setObjectName(QStringLiteral("setFa2on"));

        gridLayout_2->addWidget(setFa2on, 6, 1, 1, 1);

        label_16 = new QLabel(gridLayoutWidget_2);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_2->addWidget(label_16, 0, 0, 1, 1);

        label_13 = new QLabel(gridLayoutWidget_2);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_2->addWidget(label_13, 6, 0, 1, 1);

        Box_TempMode = new QComboBox(gridLayoutWidget_2);
        Box_TempMode->setObjectName(QStringLiteral("Box_TempMode"));

        gridLayout_2->addWidget(Box_TempMode, 7, 0, 1, 1);

        manuTempHeatOn = new QPushButton(gridLayoutWidget_2);
        manuTempHeatOn->setObjectName(QStringLiteral("manuTempHeatOn"));

        gridLayout_2->addWidget(manuTempHeatOn, 7, 1, 1, 1);

        manuTempHeatOff = new QPushButton(gridLayoutWidget_2);
        manuTempHeatOff->setObjectName(QStringLiteral("manuTempHeatOff"));

        gridLayout_2->addWidget(manuTempHeatOff, 7, 2, 1, 1);

        TempHeatSta = new QComboBox(gridLayoutWidget_2);
        TempHeatSta->setObjectName(QStringLiteral("TempHeatSta"));
        TempHeatSta->setEnabled(false);

        gridLayout_2->addWidget(TempHeatSta, 7, 3, 1, 1);

        label_46 = new QLabel(gridLayoutWidget_2);
        label_46->setObjectName(QStringLiteral("label_46"));

        gridLayout_2->addWidget(label_46, 1, 0, 1, 1);

        Edit_MotorSpeed = new QLineEdit(gridLayoutWidget_2);
        Edit_MotorSpeed->setObjectName(QStringLiteral("Edit_MotorSpeed"));

        gridLayout_2->addWidget(Edit_MotorSpeed, 1, 1, 1, 1);

        label_51 = new QLabel(gridLayoutWidget_2);
        label_51->setObjectName(QStringLiteral("label_51"));

        gridLayout_2->addWidget(label_51, 1, 2, 1, 1);

        setMotorOff->raise();
        label_15->raise();
        Box_MotorSta->raise();
        setMotorOn->raise();
        setFa1on->raise();
        setFa1off->raise();
        label_12->raise();
        label_11->raise();
        Fa1Sta->raise();
        setFa2off->raise();
        Fa2Sta->raise();
        setFa2on->raise();
        Box_MotorDianSta->raise();
        label_26->raise();
        setBengOn->raise();
        reachTemp->raise();
        Box_BengSta->raise();
        setMotorRelease->raise();
        label_16->raise();
        label_13->raise();
        currentTemp->raise();
        label_27->raise();
        setMotorReset->raise();
        Edit_BengSpeed->raise();
        Box_TempMode->raise();
        manuTempHeatOn->raise();
        manuTempHeatOff->raise();
        TempHeatSta->raise();
        label_46->raise();
        Edit_MotorSpeed->raise();
        label_51->raise();
        sendButton = new QPushButton(paratab);
        sendButton->setObjectName(QStringLiteral("sendButton"));
        sendButton->setGeometry(QRect(540, 210, 201, 41));
        setButton = new QPushButton(paratab);
        setButton->setObjectName(QStringLiteral("setButton"));
        setButton->setGeometry(QRect(540, 270, 201, 41));
        recChooseFile = new QPushButton(paratab);
        recChooseFile->setObjectName(QStringLiteral("recChooseFile"));
        recChooseFile->setGeometry(QRect(540, 150, 201, 41));
        recChooseFile->setFont(font);
        startRec = new QPushButton(paratab);
        startRec->setObjectName(QStringLiteral("startRec"));
        startRec->setGeometry(QRect(540, 340, 201, 41));
        startRec->setFont(font);
        QLineEditrecFilePath = new QLineEdit(paratab);
        QLineEditrecFilePath->setObjectName(QStringLiteral("QLineEditrecFilePath"));
        QLineEditrecFilePath->setGeometry(QRect(10, 110, 731, 21));
        QFont font1;
        font1.setFamily(QStringLiteral("Agency FB"));
        font1.setPointSize(12);
        QLineEditrecFilePath->setFont(font1);
        line1_2 = new QPushButton(paratab);
        line1_2->setObjectName(QStringLiteral("line1_2"));
        line1_2->setEnabled(false);
        line1_2->setGeometry(QRect(630, 290, 1, 1));
        line1_2->setMaximumSize(QSize(16777215, 1));
        line1_2->setFont(font);
        line1_3 = new QPushButton(paratab);
        line1_3->setObjectName(QStringLiteral("line1_3"));
        line1_3->setEnabled(false);
        line1_3->setGeometry(QRect(750, 460, 1, 1));
        line1_3->setMaximumSize(QSize(1, 16777215));
        line1_3->setFont(font);
        line_2 = new QFrame(paratab);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(510, 160, 20, 231));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(paratab);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setGeometry(QRect(-10, 410, 741, 16));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        line_5 = new QFrame(paratab);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setGeometry(QRect(750, 0, 20, 751));
        line_5->setFrameShape(QFrame::VLine);
        line_5->setFrameShadow(QFrame::Sunken);
        gridLayoutWidget_4 = new QWidget(paratab);
        gridLayoutWidget_4->setObjectName(QStringLiteral("gridLayoutWidget_4"));
        gridLayoutWidget_4->setGeometry(QRect(530, 430, 220, 141));
        gridLayout_4 = new QGridLayout(gridLayoutWidget_4);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        shengyuSec = new QLineEdit(gridLayoutWidget_4);
        shengyuSec->setObjectName(QStringLiteral("shengyuSec"));

        gridLayout_4->addWidget(shengyuSec, 2, 1, 1, 1);

        recADFreq = new QLineEdit(gridLayoutWidget_4);
        recADFreq->setObjectName(QStringLiteral("recADFreq"));

        gridLayout_4->addWidget(recADFreq, 1, 1, 1, 1);

        Ka = new QComboBox(gridLayoutWidget_4);
        Ka->setObjectName(QStringLiteral("Ka"));
        Ka->setEnabled(false);

        gridLayout_4->addWidget(Ka, 3, 1, 1, 1);

        label_14 = new QLabel(gridLayoutWidget_4);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_4->addWidget(label_14, 0, 0, 1, 1);

        YaliValue = new QLineEdit(gridLayoutWidget_4);
        YaliValue->setObjectName(QStringLiteral("YaliValue"));
        YaliValue->setEnabled(false);

        gridLayout_4->addWidget(YaliValue, 4, 1, 1, 1);

        label_17 = new QLabel(gridLayoutWidget_4);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_4->addWidget(label_17, 1, 0, 1, 1);

        label_18 = new QLabel(gridLayoutWidget_4);
        label_18->setObjectName(QStringLiteral("label_18"));

        gridLayout_4->addWidget(label_18, 2, 0, 1, 1);

        label_19 = new QLabel(gridLayoutWidget_4);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_4->addWidget(label_19, 3, 0, 1, 1);

        label_20 = new QLabel(gridLayoutWidget_4);
        label_20->setObjectName(QStringLiteral("label_20"));

        gridLayout_4->addWidget(label_20, 4, 0, 1, 1);

        recTime = new QLineEdit(gridLayoutWidget_4);
        recTime->setObjectName(QStringLiteral("recTime"));

        gridLayout_4->addWidget(recTime, 0, 1, 1, 1);

        line_6 = new QFrame(paratab);
        line_6->setObjectName(QStringLiteral("line_6"));
        line_6->setGeometry(QRect(510, 430, 20, 131));
        line_6->setFrameShape(QFrame::VLine);
        line_6->setFrameShadow(QFrame::Sunken);
        gridLayoutWidget_3 = new QWidget(paratab);
        gridLayoutWidget_3->setObjectName(QStringLiteral("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(0, 420, 511, 161));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_3->setSpacing(4);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        setL4OnOff = new QPushButton(gridLayoutWidget_3);
        setL4OnOff->setObjectName(QStringLiteral("setL4OnOff"));

        gridLayout_3->addWidget(setL4OnOff, 15, 5, 1, 1);

        setL3OnOff = new QPushButton(gridLayoutWidget_3);
        setL3OnOff->setObjectName(QStringLiteral("setL3OnOff"));

        gridLayout_3->addWidget(setL3OnOff, 15, 4, 1, 1);

        led1Sw = new QComboBox(gridLayoutWidget_3);
        led1Sw->setObjectName(QStringLiteral("led1Sw"));
        led1Sw->setEnabled(false);

        gridLayout_3->addWidget(led1Sw, 12, 2, 1, 1);

        label_55 = new QLabel(gridLayoutWidget_3);
        label_55->setObjectName(QStringLiteral("label_55"));

        gridLayout_3->addWidget(label_55, 15, 1, 1, 1);

        label_53 = new QLabel(gridLayoutWidget_3);
        label_53->setObjectName(QStringLiteral("label_53"));

        gridLayout_3->addWidget(label_53, 12, 1, 1, 1);

        setL2OnOff = new QPushButton(gridLayoutWidget_3);
        setL2OnOff->setObjectName(QStringLiteral("setL2OnOff"));

        gridLayout_3->addWidget(setL2OnOff, 15, 3, 1, 1);

        led3Sw = new QComboBox(gridLayoutWidget_3);
        led3Sw->setObjectName(QStringLiteral("led3Sw"));
        led3Sw->setEnabled(false);

        gridLayout_3->addWidget(led3Sw, 12, 4, 1, 1);

        led4Sw = new QComboBox(gridLayoutWidget_3);
        led4Sw->setObjectName(QStringLiteral("led4Sw"));
        led4Sw->setEnabled(false);

        gridLayout_3->addWidget(led4Sw, 12, 5, 1, 1);

        setL1OnOff = new QPushButton(gridLayoutWidget_3);
        setL1OnOff->setObjectName(QStringLiteral("setL1OnOff"));

        gridLayout_3->addWidget(setL1OnOff, 15, 2, 1, 1);

        led2Sw = new QComboBox(gridLayoutWidget_3);
        led2Sw->setObjectName(QStringLiteral("led2Sw"));
        led2Sw->setEnabled(false);

        gridLayout_3->addWidget(led2Sw, 12, 3, 1, 1);

        Kong3 = new QComboBox(gridLayoutWidget_3);
        Kong3->setObjectName(QStringLiteral("Kong3"));
        Kong3->setEnabled(false);

        gridLayout_3->addWidget(Kong3, 6, 4, 1, 1);

        Kong1 = new QComboBox(gridLayoutWidget_3);
        Kong1->setObjectName(QStringLiteral("Kong1"));
        Kong1->setEnabled(false);

        gridLayout_3->addWidget(Kong1, 6, 2, 1, 1);

        Kong2 = new QComboBox(gridLayoutWidget_3);
        Kong2->setObjectName(QStringLiteral("Kong2"));
        Kong2->setEnabled(false);

        gridLayout_3->addWidget(Kong2, 6, 3, 1, 1);

        label_52 = new QLabel(gridLayoutWidget_3);
        label_52->setObjectName(QStringLiteral("label_52"));

        gridLayout_3->addWidget(label_52, 6, 1, 1, 1);

        Kong4 = new QComboBox(gridLayoutWidget_3);
        Kong4->setObjectName(QStringLiteral("Kong4"));
        Kong4->setEnabled(false);

        gridLayout_3->addWidget(Kong4, 6, 5, 1, 1);

        led3AD = new QLineEdit(gridLayoutWidget_3);
        led3AD->setObjectName(QStringLiteral("led3AD"));
        led3AD->setEnabled(false);

        gridLayout_3->addWidget(led3AD, 14, 4, 1, 1);

        led1AD = new QLineEdit(gridLayoutWidget_3);
        led1AD->setObjectName(QStringLiteral("led1AD"));
        led1AD->setEnabled(false);

        gridLayout_3->addWidget(led1AD, 14, 2, 1, 1);

        label_54 = new QLabel(gridLayoutWidget_3);
        label_54->setObjectName(QStringLiteral("label_54"));

        gridLayout_3->addWidget(label_54, 14, 1, 1, 1);

        led4AD = new QLineEdit(gridLayoutWidget_3);
        led4AD->setObjectName(QStringLiteral("led4AD"));
        led4AD->setEnabled(false);

        gridLayout_3->addWidget(led4AD, 14, 5, 1, 1);

        led2AD = new QLineEdit(gridLayoutWidget_3);
        led2AD->setObjectName(QStringLiteral("led2AD"));
        led2AD->setEnabled(false);

        gridLayout_3->addWidget(led2AD, 14, 3, 1, 1);

        setLed1 = new QLineEdit(gridLayoutWidget_3);
        setLed1->setObjectName(QStringLiteral("setLed1"));

        gridLayout_3->addWidget(setLed1, 7, 2, 1, 1);

        setLed2 = new QLineEdit(gridLayoutWidget_3);
        setLed2->setObjectName(QStringLiteral("setLed2"));

        gridLayout_3->addWidget(setLed2, 7, 3, 1, 1);

        setLed3 = new QLineEdit(gridLayoutWidget_3);
        setLed3->setObjectName(QStringLiteral("setLed3"));

        gridLayout_3->addWidget(setLed3, 7, 4, 1, 1);

        setLed4 = new QLineEdit(gridLayoutWidget_3);
        setLed4->setObjectName(QStringLiteral("setLed4"));

        gridLayout_3->addWidget(setLed4, 7, 5, 1, 1);

        label_56 = new QLabel(gridLayoutWidget_3);
        label_56->setObjectName(QStringLiteral("label_56"));

        gridLayout_3->addWidget(label_56, 7, 1, 1, 1);

        plot = new QCustomPlot(paratab);
        plot->setObjectName(QStringLiteral("plot"));
        plot->setGeometry(QRect(780, 290, 601, 451));
        layoutWidget1 = new QWidget(paratab);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(781, 21, 591, 24));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        sendButton_2 = new QPushButton(layoutWidget1);
        sendButton_2->setObjectName(QStringLiteral("sendButton_2"));
        sendButton_2->setEnabled(true);

        horizontalLayout_2->addWidget(sendButton_2);

        sendButton_3 = new QPushButton(layoutWidget1);
        sendButton_3->setObjectName(QStringLiteral("sendButton_3"));
        sendButton_3->setEnabled(true);

        horizontalLayout_2->addWidget(sendButton_3);

        sendButton_4 = new QPushButton(layoutWidget1);
        sendButton_4->setObjectName(QStringLiteral("sendButton_4"));
        sendButton_4->setEnabled(true);

        horizontalLayout_2->addWidget(sendButton_4);

        textEdit_4 = new QTextEdit(paratab);
        textEdit_4->setObjectName(QStringLiteral("textEdit_4"));
        textEdit_4->setGeometry(QRect(782, 52, 191, 131));
        textEdit_4->setMaximumSize(QSize(16777215, 231));
        textEdit_5 = new QTextEdit(paratab);
        textEdit_5->setObjectName(QStringLiteral("textEdit_5"));
        textEdit_5->setGeometry(QRect(980, 50, 201, 131));
        textEdit_5->setMaximumSize(QSize(16777215, 231));
        textEdit_6 = new QTextEdit(paratab);
        textEdit_6->setObjectName(QStringLiteral("textEdit_6"));
        textEdit_6->setGeometry(QRect(1180, 50, 191, 131));
        textEdit_6->setMaximumSize(QSize(16777215, 231));
        verticalLayoutWidget = new QWidget(paratab);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(1200, 180, 91, 100));
        verticalLayout_5 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        checkBox_1 = new QCheckBox(verticalLayoutWidget);
        checkBox_1->setObjectName(QStringLiteral("checkBox_1"));
        checkBox_1->setEnabled(true);
        checkBox_1->setChecked(true);

        verticalLayout_5->addWidget(checkBox_1);

        checkBox_2 = new QCheckBox(verticalLayoutWidget);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setEnabled(true);
        checkBox_2->setChecked(true);

        verticalLayout_5->addWidget(checkBox_2);

        checkBox_3 = new QCheckBox(verticalLayoutWidget);
        checkBox_3->setObjectName(QStringLiteral("checkBox_3"));
        checkBox_3->setEnabled(true);
        checkBox_3->setChecked(true);

        verticalLayout_5->addWidget(checkBox_3);

        checkBox_4 = new QCheckBox(verticalLayoutWidget);
        checkBox_4->setObjectName(QStringLiteral("checkBox_4"));
        checkBox_4->setEnabled(true);
        checkBox_4->setChecked(true);

        verticalLayout_5->addWidget(checkBox_4);

        tabWidget->addTab(paratab, QString());
        updatetab = new QWidget();
        updatetab->setObjectName(QStringLiteral("updatetab"));
        layoutWidget2 = new QWidget(updatetab);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 0, 1021, 801));
        verticalLayout_10 = new QVBoxLayout(layoutWidget2);
        verticalLayout_10->setSpacing(6);
        verticalLayout_10->setContentsMargins(11, 11, 11, 11);
        verticalLayout_10->setObjectName(QStringLiteral("verticalLayout_10"));
        verticalLayout_10->setContentsMargins(0, 0, 0, 0);
        label_44 = new QLabel(layoutWidget2);
        label_44->setObjectName(QStringLiteral("label_44"));
        label_44->setFont(font1);

        verticalLayout_10->addWidget(label_44);

        textEdit_3 = new QTextEdit(layoutWidget2);
        textEdit_3->setObjectName(QStringLiteral("textEdit_3"));
        textEdit_3->setEnabled(true);

        verticalLayout_10->addWidget(textEdit_3);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setSpacing(6);
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));

        verticalLayout_10->addLayout(horizontalLayout_20);

        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setSpacing(6);
        horizontalLayout_22->setObjectName(QStringLiteral("horizontalLayout_22"));
        label_48 = new QLabel(layoutWidget2);
        label_48->setObjectName(QStringLiteral("label_48"));
        label_48->setFont(font1);

        horizontalLayout_22->addWidget(label_48);

        progressBar = new QProgressBar(layoutWidget2);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);

        horizontalLayout_22->addWidget(progressBar);


        verticalLayout_10->addLayout(horizontalLayout_22);

        horizontalLayout_27 = new QHBoxLayout();
        horizontalLayout_27->setSpacing(6);
        horizontalLayout_27->setObjectName(QStringLiteral("horizontalLayout_27"));
        FilePath = new QLineEdit(layoutWidget2);
        FilePath->setObjectName(QStringLiteral("FilePath"));
        FilePath->setFont(font1);

        horizontalLayout_27->addWidget(FilePath);

        ChooseFile = new QPushButton(layoutWidget2);
        ChooseFile->setObjectName(QStringLiteral("ChooseFile"));
        ChooseFile->setFont(font);

        horizontalLayout_27->addWidget(ChooseFile);

        GetNewFile = new QPushButton(layoutWidget2);
        GetNewFile->setObjectName(QStringLiteral("GetNewFile"));
        GetNewFile->setFont(font);

        horizontalLayout_27->addWidget(GetNewFile);

        escBoot = new QPushButton(layoutWidget2);
        escBoot->setObjectName(QStringLiteral("escBoot"));
        escBoot->setFont(font);

        horizontalLayout_27->addWidget(escBoot);


        verticalLayout_10->addLayout(horizontalLayout_27);

        Box_updateMode = new QComboBox(layoutWidget2);
        Box_updateMode->setObjectName(QStringLiteral("Box_updateMode"));
        Box_updateMode->setSizeIncrement(QSize(0, 0));
        Box_updateMode->setBaseSize(QSize(0, 0));
        Box_updateMode->setIconSize(QSize(32, 32));

        verticalLayout_10->addWidget(Box_updateMode);

        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setSpacing(6);
        horizontalLayout_23->setObjectName(QStringLiteral("horizontalLayout_23"));
        readVersion = new QPushButton(layoutWidget2);
        readVersion->setObjectName(QStringLiteral("readVersion"));
        readVersion->setFont(font);

        horizontalLayout_23->addWidget(readVersion);

        Edit_Version = new QLineEdit(layoutWidget2);
        Edit_Version->setObjectName(QStringLiteral("Edit_Version"));
        Edit_Version->setFont(font1);

        horizontalLayout_23->addWidget(Edit_Version);

        BoxMode = new QComboBox(layoutWidget2);
        BoxMode->setObjectName(QStringLiteral("BoxMode"));

        horizontalLayout_23->addWidget(BoxMode);

        EnterBoot = new QPushButton(layoutWidget2);
        EnterBoot->setObjectName(QStringLiteral("EnterBoot"));
        QFont font2;
        font2.setFamily(QStringLiteral("Agency FB"));
        font2.setPointSize(16);
        font2.setBold(false);
        font2.setUnderline(true);
        font2.setWeight(50);
        EnterBoot->setFont(font2);

        horizontalLayout_23->addWidget(EnterBoot);


        verticalLayout_10->addLayout(horizontalLayout_23);

        horizontalLayout_24 = new QHBoxLayout();
        horizontalLayout_24->setSpacing(6);
        horizontalLayout_24->setObjectName(QStringLiteral("horizontalLayout_24"));
        UpdateCode = new QPushButton(layoutWidget2);
        UpdateCode->setObjectName(QStringLiteral("UpdateCode"));
        UpdateCode->setFont(font2);

        horizontalLayout_24->addWidget(UpdateCode);


        verticalLayout_10->addLayout(horizontalLayout_24);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setSpacing(6);
        horizontalLayout_21->setObjectName(QStringLiteral("horizontalLayout_21"));

        verticalLayout_10->addLayout(horizontalLayout_21);

        tabWidget->addTab(updatetab, QString());
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);
        BaudBox->setCurrentIndex(0);
        Box_MotorSta->setCurrentIndex(0);
        Fa1Sta->setCurrentIndex(0);
        Box_MotorDianSta->setCurrentIndex(0);
        Box_BengSta->setCurrentIndex(0);
        Fa2Sta->setCurrentIndex(0);
        Box_TempMode->setCurrentIndex(0);
        TempHeatSta->setCurrentIndex(0);
        Ka->setCurrentIndex(0);
        led1Sw->setCurrentIndex(0);
        led3Sw->setCurrentIndex(0);
        led4Sw->setCurrentIndex(0);
        led2Sw->setCurrentIndex(0);
        Kong3->setCurrentIndex(0);
        Kong1->setCurrentIndex(0);
        Kong2->setCurrentIndex(0);
        Kong4->setCurrentIndex(0);
        Box_updateMode->setCurrentIndex(0);
        BoxMode->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        label_45->setText(QApplication::translate("MainWindow", "\344\270\262\345\217\243\351\200\211\346\213\251\357\274\232", 0));
        label_7->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\344\275\215\357\274\232", 0));
        StopBox->clear();
        StopBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1", 0)
         << QApplication::translate("MainWindow", "2", 0)
        );
        label_6->setText(QApplication::translate("MainWindow", "\346\240\241\351\252\214\344\275\215\357\274\232", 0));
        ParityBox->clear();
        ParityBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "0", 0)
        );
        label_5->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\344\275\215\357\274\232", 0));
        BitNumBox->clear();
        BitNumBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "8", 0)
        );
        label_8->setText(QApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207\357\274\232", 0));
        BaudBox->clear();
        BaudBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1200", 0)
         << QApplication::translate("MainWindow", "2400", 0)
         << QApplication::translate("MainWindow", "4800", 0)
         << QApplication::translate("MainWindow", "9600", 0)
         << QApplication::translate("MainWindow", "19200", 0)
         << QApplication::translate("MainWindow", "38400", 0)
         << QApplication::translate("MainWindow", "115200", 0)
        );
        openButton->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", 0));
        clearButton->setText(QApplication::translate("MainWindow", "\346\270\205\347\251\272\346\216\245\346\224\266", 0));
        Savegp->setText(QApplication::translate("MainWindow", " \345\257\274\345\207\272\346\225\260\346\215\256", 0));
        setMotorRelease->setText(QApplication::translate("MainWindow", " \351\207\212\346\224\276", 0));
        Box_MotorSta->clear();
        Box_MotorSta->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\345\267\262\345\201\234\346\255\242", 0)
         << QApplication::translate("MainWindow", "\346\255\243\345\234\250\350\275\254", 0)
        );
        Fa1Sta->clear();
        Fa1Sta->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\351\230\2001\346\226\255\345\274\200\347\212\266\346\200\201", 0)
         << QApplication::translate("MainWindow", "\351\230\2001\351\227\255\345\220\210\347\212\266\346\200\201", 0)
        );
        label_27->setText(QApplication::translate("MainWindow", "           \345\275\223\345\211\215\346\270\251\345\272\246\357\274\232", 0));
        Box_MotorDianSta->clear();
        Box_MotorDianSta->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\347\246\273\344\275\215", 0)
         << QApplication::translate("MainWindow", "\345\210\260\344\275\215", 0)
        );
        setFa2off->setText(QApplication::translate("MainWindow", "\346\226\255\345\274\200", 0));
        Box_BengSta->clear();
        Box_BengSta->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\345\267\262\345\201\234\346\255\242", 0)
         << QApplication::translate("MainWindow", "\346\255\243\345\234\250\350\275\254", 0)
        );
        setMotorOff->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242", 0));
        Fa2Sta->clear();
        Fa2Sta->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\351\230\2002\346\226\255\345\274\200\347\212\266\346\200\201", 0)
         << QApplication::translate("MainWindow", "\351\230\2002\351\227\255\345\220\210\347\212\266\346\200\201", 0)
        );
        setFa1off->setText(QApplication::translate("MainWindow", "\346\226\255\345\274\200", 0));
        setMotorOn->setText(QApplication::translate("MainWindow", "\345\220\257\345\212\250", 0));
        setMotorReset->setText(QApplication::translate("MainWindow", "\345\244\215\344\275\215\351\224\201\345\256\232", 0));
        label_15->setText(QApplication::translate("MainWindow", "   \347\224\265\346\234\272\357\274\232", 0));
        setFa1on->setText(QApplication::translate("MainWindow", "\351\227\255\345\220\210", 0));
        label_12->setText(QApplication::translate("MainWindow", " \347\224\265\347\243\201\351\230\2001\357\274\232", 0));
        label_11->setText(QApplication::translate("MainWindow", "   \346\263\265\346\234\272\357\274\232", 0));
        setBengOn->setText(QApplication::translate("MainWindow", "  \346\263\265\346\234\272\350\256\276\347\275\256", 0));
        reachTemp->setText(QApplication::translate("MainWindow", "37.0", 0));
        label_26->setText(QApplication::translate("MainWindow", " \346\270\251\345\272\246\347\233\256\346\240\207\357\274\232", 0));
        setFa2on->setText(QApplication::translate("MainWindow", "\351\227\255\345\220\210", 0));
        label_16->setText(QApplication::translate("MainWindow", " \347\224\265\346\234\272\345\244\215\344\275\215\357\274\232", 0));
        label_13->setText(QApplication::translate("MainWindow", " \347\224\265\347\243\201\351\230\2002\357\274\232", 0));
        Box_TempMode->clear();
        Box_TempMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\346\211\213\345\212\250\346\250\241\345\274\217", 0)
         << QApplication::translate("MainWindow", "\350\207\252\345\212\250\346\250\241\345\274\217", 0)
        );
        manuTempHeatOn->setText(QApplication::translate("MainWindow", " \346\211\213\345\212\250\345\212\240\347\203\255\345\274\200", 0));
        manuTempHeatOff->setText(QApplication::translate("MainWindow", " \346\211\213\345\212\250\345\212\240\347\203\255\345\205\263", 0));
        TempHeatSta->clear();
        TempHeatSta->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\346\227\240\345\212\240\347\203\255", 0)
         << QApplication::translate("MainWindow", "\346\255\243\345\234\250\345\212\240\347\203\255", 0)
        );
        label_46->setText(QApplication::translate("MainWindow", " \347\224\265\346\234\272\351\200\237\345\272\246\357\274\232", 0));
        label_51->setText(QApplication::translate("MainWindow", "%", 0));
        sendButton->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226\346\225\260\346\215\256", 0));
        setButton->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256\345\217\202\346\225\260", 0));
        recChooseFile->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\344\277\235\345\255\230\350\267\257\345\276\204", 0));
        startRec->setText(QApplication::translate("MainWindow", "\345\220\257\345\212\250\346\243\200\346\265\213", 0));
        QLineEditrecFilePath->setText(QApplication::translate("MainWindow", "D:/1.txt", 0));
        line1_2->setText(QString());
        line1_3->setText(QString());
        shengyuSec->setText(QApplication::translate("MainWindow", "0", 0));
        recADFreq->setText(QApplication::translate("MainWindow", "10", 0));
        Ka->clear();
        Ka->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\345\215\241 None", 0)
         << QApplication::translate("MainWindow", "\345\215\241 \345\210\260\344\275\215", 0)
        );
        label_14->setText(QApplication::translate("MainWindow", " \351\207\207\351\233\206\346\227\266\351\227\264\357\274\210\347\247\222\357\274\211", 0));
        label_17->setText(QApplication::translate("MainWindow", "\351\207\207\351\233\206\351\242\221\347\216\207\357\274\210 \346\254\241/ \347\247\222\357\274\211", 0));
        label_18->setText(QApplication::translate("MainWindow", "  \345\200\222\350\256\241\346\227\266\357\274\210\347\247\222\357\274\211", 0));
        label_19->setText(QApplication::translate("MainWindow", " \350\257\225\345\211\202\345\215\241\345\210\260\344\275\215\346\203\205\345\206\265", 0));
        label_20->setText(QApplication::translate("MainWindow", "\345\216\213\345\212\233AD\345\200\274:", 0));
        recTime->setText(QApplication::translate("MainWindow", "15", 0));
        setL4OnOff->setText(QApplication::translate("MainWindow", " \347\201\2574\345\274\200", 0));
        setL3OnOff->setText(QApplication::translate("MainWindow", " \347\201\2573\345\274\200", 0));
        led1Sw->clear();
        led1Sw->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\347\201\2571\347\201\255", 0)
         << QApplication::translate("MainWindow", "\347\201\2571\344\272\256", 0)
        );
        label_55->setText(QApplication::translate("MainWindow", "  \347\201\257\346\216\247\345\210\266\357\274\232", 0));
        label_53->setText(QApplication::translate("MainWindow", "  \347\201\257\347\212\266\346\200\201\357\274\232", 0));
        setL2OnOff->setText(QApplication::translate("MainWindow", " \347\201\2572\345\274\200", 0));
        led3Sw->clear();
        led3Sw->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\347\201\2573\347\201\255", 0)
         << QApplication::translate("MainWindow", "\347\201\2573\344\272\256", 0)
        );
        led4Sw->clear();
        led4Sw->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\347\201\2574\347\201\255", 0)
         << QApplication::translate("MainWindow", "\347\201\2574\344\272\256", 0)
        );
        setL1OnOff->setText(QApplication::translate("MainWindow", " \347\201\2571\345\274\200", 0));
        led2Sw->clear();
        led2Sw->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\347\201\2572\347\201\255", 0)
         << QApplication::translate("MainWindow", "\347\201\2572\344\272\256", 0)
        );
        Kong3->clear();
        Kong3->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\345\255\2243 None", 0)
         << QApplication::translate("MainWindow", "\345\255\2243 \345\210\260\344\275\215", 0)
        );
        Kong1->clear();
        Kong1->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\345\255\2241 None", 0)
         << QApplication::translate("MainWindow", "\345\255\2241 \345\210\260\344\275\215", 0)
        );
        Kong2->clear();
        Kong2->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\345\255\2242 None", 0)
         << QApplication::translate("MainWindow", "\345\255\2242 \345\210\260\344\275\215", 0)
        );
        label_52->setText(QApplication::translate("MainWindow", "  \345\255\224\344\275\215:", 0));
        Kong4->clear();
        Kong4->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\345\255\2244 None", 0)
         << QApplication::translate("MainWindow", "\345\255\2244 \345\210\260\344\275\215", 0)
        );
        label_54->setText(QApplication::translate("MainWindow", "  \345\205\211\345\274\272AD\345\200\274\357\274\232", 0));
        label_56->setText(QApplication::translate("MainWindow", "  \344\272\256\345\272\246\350\256\276\347\275\256\357\274\232", 0));
        sendButton_2->setText(QApplication::translate("MainWindow", "   \346\240\267\346\234\254\346\235\241\347\240\201\346\234\272", 0));
        sendButton_3->setText(QApplication::translate("MainWindow", " \350\257\225\345\211\202\346\235\241\347\240\201\346\234\272", 0));
        sendButton_4->setText(QApplication::translate("MainWindow", " \346\211\223\345\215\260\344\277\241\346\201\257", 0));
        checkBox_1->setText(QApplication::translate("MainWindow", "\346\204\237\345\205\211\346\263\242\345\275\2421", 0));
        checkBox_2->setText(QApplication::translate("MainWindow", "\346\204\237\345\205\211\346\263\242\345\275\2422", 0));
        checkBox_3->setText(QApplication::translate("MainWindow", "\346\204\237\345\205\211\346\263\242\345\275\2423", 0));
        checkBox_4->setText(QApplication::translate("MainWindow", "\346\204\237\345\205\211\346\263\242\345\275\2424", 0));
        tabWidget->setTabText(tabWidget->indexOf(paratab), QApplication::translate("MainWindow", " \346\216\247\345\210\266", 0));
        label_44->setText(QApplication::translate("MainWindow", "Device interaction information:", 0));
        label_48->setText(QApplication::translate("MainWindow", "\345\215\207\347\272\247\350\277\233\345\272\246\n"
"Upgrade Progress:", 0));
        FilePath->setText(QApplication::translate("MainWindow", "D:/ QY_COA_V1.101.bin", 0));
        ChooseFile->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\346\226\207\344\273\266\n"
"File Choose", 0));
        GetNewFile->setText(QApplication::translate("MainWindow", "\350\216\267\345\217\226\345\275\223\345\211\215\350\267\257\345\276\204\344\270\213\346\234\200\346\226\260bin\346\226\207\344\273\266\n"
"Get New Bin File", 0));
        escBoot->setText(QApplication::translate("MainWindow", "\350\277\233\345\205\245APP\n"
"ENTER APP", 0));
        Box_updateMode->clear();
        Box_updateMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "\344\270\262\345\217\243\345\215\207\347\272\247 ", 0)
         << QApplication::translate("MainWindow", "\350\275\254\345\217\221CAN ", 0)
        );
        readVersion->setText(QApplication::translate("MainWindow", "\350\257\273\345\217\226APP\347\211\210\346\234\254\345\217\267\n"
"Read APP Verion", 0));
        Edit_Version->setText(QString());
        BoxMode->clear();
        BoxMode->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Boot", 0)
         << QApplication::translate("MainWindow", "App", 0)
        );
        EnterBoot->setText(QApplication::translate("MainWindow", "\350\277\233\345\205\245\345\215\207\347\272\247\346\250\241\345\274\217\n"
" Enter Bootloader", 0));
        UpdateCode->setText(QApplication::translate("MainWindow", "\347\241\256\350\256\244\345\215\207\347\272\247\345\274\200\345\247\213\345\217\221\351\200\201\346\226\207\344\273\266\n"
"Confirm the upgrade Start Send data", 0));
        tabWidget->setTabText(tabWidget->indexOf(updatetab), QApplication::translate("MainWindow", " \345\210\267\346\234\272", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
