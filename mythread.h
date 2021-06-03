#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QByteArray>
#include "processtrackingdata.h"

#ifdef _WIN32
    // For Windows Sleep(ms)
    #include <windows.h>
#else
    // For POSIX sleep(sec)
    #include <unistd.h>
#endif

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "CombinedApi.h"
#include "PortHandleInfo.h"
#include "ToolData.h"

using namespace std;

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = nullptr);

    //连接和配置信息
    string hostname;
    const char *ToolDefine1;
    const char *ToolDefine2;

//------------------------以下是NDI相关函数------------------------//
    bool startConfig();
    bool startTracking(int);
    bool startTrackingSimulate();
    void sleepSeconds(unsigned numSeconds);
    bool onErrorPrintDebugMessage(std::string methodName, int errorCode);
    std::string getToolInfo(std::string toolHandle);
    std::string toolDataToCSV(const ToolData& toolData);
    void writeCSV(std::string fileName, int numberOfLines);
    void printToolData(const ToolData& toolData);
    void printTrackingData();
    bool initializeAndEnableTools();
    bool loadTool(const char* toolDefinitionFilePath);
    bool configurePassiveTools();
    void configureUserParameters();
    void simulateAlerts(uint32_t simulatedAlerts = 0x00000000);
    void determineApiSupportForBX2();

private:
    CombinedApi *capi;
    bool apiSupportsBX2;
    int option_id;
    uint16_t options;

    QTimer *timer;

signals:
    void sendSignal(QString);
    void sendDataSignal(QByteArray);

public slots:
    void receiveApiMsgSlot(QString);

protected:
    void run();
};

#endif // MYTHREAD_H
