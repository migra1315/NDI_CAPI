#-------------------------------------------------
#
# Project created by QtCreator 2021-05-31T15:14:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH+="./library/include"
INCLUDEPATH+="./library/src"
INCLUDEPATH+="./library/src/include"
#预宏定义
DEFINES += CAPICOMMON_EXPORTS
#使用多字节字符集
DEFINES  -= UNICODE
DEFINES  += UMBCS

TARGET = NDI_CAPI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    mythread.cpp \
    library/src/BufferedReader.cpp \
    library/src/CombinedApi.cpp \
    library/src/ComConnection.cpp \
    library/src/GbfButton1D.cpp \
    library/src/GbfComponent.cpp \
    library/src/GbfContainer.cpp \
    library/src/GbfData3D.cpp \
    library/src/GbfData6D.cpp \
    library/src/GbfFrame.cpp \
    library/src/GbfSystemAlert.cpp \
    library/src/MarkerData.cpp \
    library/src/PortHandleInfo.cpp \
    library/src/SystemAlert.cpp \
    library/src/SystemCRC.cpp \
#    library/src/TcpConnection.cpp \
    library/src/ToolData.cpp \
    library/src/Transform.cpp \
    mainwindow.cpp \
    processtrackingdata.cpp

HEADERS += \
    mythread.h \
    library/include/CombinedApi.h \
    library/include/MarkerData.h \
    library/include/PortHandleInfo.h \
    library/include/SystemAlert.h \
    library/include/ToolData.h \
    library/include/Transform.h \
    library/src/include/BufferedReader.h \
    library/src/include/ComConnection.h \
    library/src/include/Connection.h \
    library/src/include/GbfButton1D.h \
    library/src/include/GbfComponent.h \
    library/src/include/GbfContainer.h \
    library/src/include/GbfData3D.h \
    library/src/include/GbfData6D.h \
    library/src/include/GbfFrame.h \
    library/src/include/GbfSystemAlert.h \
    library/src/include/qinfofile.h \
    library/src/include/SystemCRC.h \
#    library/src/include/TcpConnection.h
    mainwindow.h \
    processtrackingdata.h

FORMS += \
    mainwindow.ui
