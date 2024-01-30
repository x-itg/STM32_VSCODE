#-------------------------------------------------
#
# Project created by QtCreator 2016-01-27T13:35:50
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialPort
TEMPLATE = app
RC_FILE = exe.rc

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    modbusCRC.cpp \
    qyRecDecode.cpp \
    ComUpgrade.cpp \
    refDPI.cpp \
    plotFunction.cpp \
    str2intUC.cpp \
    fileOut.cpp \
    ComSet.cpp \
    aboutDebug.cpp \
    aboutComTool.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h

FORMS    += mainwindow.ui
