#include "mythread.h"
using namespace std;

MyThread::MyThread(QObject *parent) : QThread(parent)
{
    //执行构造函数的时候初始化相关的信息
    capi = new CombinedApi();//初始化一个API连接类，大部分操作都在此完成
    apiSupportsBX2 = false;
    option_id = 0;
    options = 0;
    //capi中传出的信号，激发处理slot，该函数作用是激发sendSignal(str)
    connect(capi,&CombinedApi::sendSignalApi,this,&MyThread::receiveApiMsgSlot);
    //capi中const函数传出的信号，激发处理slot
    connect(capi,&CombinedApi::sendSignalApiConst,this,&MyThread::receiveApiMsgSlot);

}

void MyThread::receiveApiMsgSlot(QString str)
{
    emit sendSignal(str);
}

//------------------------以下是线程运行、数据处理与传输相关函数------------------------//
bool MyThread::startConfig()
{
    //建立连接，capi->connect(hostname)方法

    if(capi->connect(hostname)!=0)
    {
       emit sendSignal("connect fail");
       return false;
    }

    emit sendSignal("connect succes");

    sleepSeconds(2);

    //打印固件信息和初始化系统
    string firmwareVersion = capi->getUserParameter("Features.Firmware.Version");
    emit sendSignal(QString::fromStdString(firmwareVersion));

    //出现错误，跳出函数
    if(!onErrorPrintDebugMessage("capi.initialize()", capi->initialize()))
        return false;

    //加载工具描述文件并激活
    if(!configurePassiveTools())
        return false;

    if(!initializeAndEnableTools())
        return false;

    //开启追踪
    if(!onErrorPrintDebugMessage("capi.startTracking()", capi->startTracking()))
        return false;

    return true;

}

bool MyThread::startTracking(int replyOption)
{
    //见Polaris_API_Guide.pdf TX指令
    string outdata = capi->getTrackingDataTX(replyOption);
//----------------------------自此以下写一个json出来-----------------------//
    processTrackingData track(outdata);
    //    qDebug()<<QString::fromStdString(outdata);

    QJsonObject dataJson;
    if(track.dataIsValid)//当NDI发来的数据符合需求
    {
        dataJson.insert("dataIsValid",track.dataIsValid);
        dataJson.insert("probeTx",track.probeTx);
        dataJson.insert("probeTy",track.probeTy);
        dataJson.insert("probeTz",track.probeTz);

        dataJson.insert("probeMx",track.probeMx);
        dataJson.insert("probeMy",track.probeMy);
        dataJson.insert("probeMz",track.probeMz);

//        dataJson.insert("probeQ0",track.probeQ0);
//        dataJson.insert("probeQx",track.probeQx);
//        dataJson.insert("probeQy",track.probeQy);
//        dataJson.insert("probeQz",track.probeQz);

        dataJson.insert("rigidx1",track.rigidx1);
        dataJson.insert("rigidy1",track.rigidy1);
        dataJson.insert("rigidz1",track.rigidz1);

        dataJson.insert("rigidx2",track.rigidx2);
        dataJson.insert("rigidy2",track.rigidy2);
        dataJson.insert("rigidz2",track.rigidz2);

//        dataJson.insert("rigidx3",track.rigidx3);
//        dataJson.insert("rigidy3",track.rigidy3);
//        dataJson.insert("rigidz3",track.rigidz3);

//        dataJson.insert("rigidx4",track.rigidx4);
//        dataJson.insert("rigidy4",track.rigidy4);
//        dataJson.insert("rigidz4",track.rigidz4);

    }
    else
    {
        dataJson.insert("dataIsValid",track.dataIsValid);
    }

    QJsonDocument dataJsonDoc;
    dataJsonDoc.setObject(dataJson);
    QByteArray dataJsonDocBA = dataJsonDoc.toJson(QJsonDocument::Compact);

//----------------------------自此以上写一个json出来-----------------------//
    emit sendDataSignal(dataJsonDocBA);
    return true;
}

bool MyThread::startTrackingSimulate()
{
    //见Polaris_API_Guide.pdf TX指令
    string outdata = "0201+07082-04513-01855-05101+041595-057080-190190+02477000000310000014D040+032681-029273-196806+038254-031402-195542+036870-036707-194443+037965-041486-193249\n02+02762-01462-00466+09487+033067-022295-195013+01564000000310000014D040+033067-022310-195004+027787-023550-195330+030728-017309-196164+023842-018192-196965\n0000";
//    string outdata = "0201MISSING000000310000039300\n02+09771-02075-00066+00445+016351+006174-194689+021770000003100000393040+016339+006186-194670+019622+010498-195074+021434+003880-193975+026270+008889-194547\n0000";
    //----------------------------自此以下写一个json出来-----------------------//
    processTrackingData track(outdata);

    QJsonObject dataJson;
    if(track.dataIsValid)//当NDI发来的数据符合需求
    {
        dataJson.insert("dataIsValid",track.dataIsValid);
        dataJson.insert("probeTx",track.probeTx);
        dataJson.insert("probeTy",track.probeTy);
        dataJson.insert("probeTz",track.probeTz);

        dataJson.insert("probeMx",track.probeMx);
        dataJson.insert("probeMy",track.probeMy);
        dataJson.insert("probeMz",track.probeMz);

//        dataJson.insert("probeQ0",track.probeQ0);
//        dataJson.insert("probeQx",track.probeQx);
//        dataJson.insert("probeQy",track.probeQy);
//        dataJson.insert("probeQz",track.probeQz);

        dataJson.insert("rigidx1",track.rigidx1);
        dataJson.insert("rigidy1",track.rigidy1);
        dataJson.insert("rigidz1",track.rigidz1);

        dataJson.insert("rigidx2",track.rigidx2);
        dataJson.insert("rigidy2",track.rigidy2);
        dataJson.insert("rigidz2",track.rigidz2);

//        dataJson.insert("rigidx3",track.rigidx3);
//        dataJson.insert("rigidy3",track.rigidy3);
//        dataJson.insert("rigidz3",track.rigidz3);

//        dataJson.insert("rigidx4",track.rigidx4);
//        dataJson.insert("rigidy4",track.rigidy4);
//        dataJson.insert("rigidz4",track.rigidz4);

    }
    else
    {
        dataJson.insert("dataIsValid",track.dataIsValid);
    }

    QJsonDocument dataJsonDoc;
    dataJsonDoc.setObject(dataJson);
    QByteArray dataJsonDocBA = dataJsonDoc.toJson(QJsonDocument::Compact);

//----------------------------自此以上写一个json出来-----------------------//
    emit sendDataSignal(dataJsonDocBA);
    return true;
}

void MyThread::run()
{
    bool ProduceORSimulate = false;
    /*
      如果和NDI连接，bool值置为true,执行连接、传输数据操作
      否则，直接发送预定的模拟数据
    */
    if(ProduceORSimulate)
    {
        emit sendSignal("the thread start");
        //配置文件
        if (!startConfig())
        {
            emit sendSignal("FAIL,EXIT");
            exec();
        }
        //定时器
        timer = new QTimer();
        connect(timer,&QTimer::timeout,[=](){
            startTracking(0x0001|0x0008);
        });
        timer->start(200);
    }
    else
    {
        timer = new QTimer();
        connect(timer,&QTimer::timeout,[=](){
            startTrackingSimulate();
        });
        timer->start(200);
    }


    //结束追踪
//    if(!onErrorPrintDebugMessage("capi.stopTracking()", capi->stopTracking()))
//        exec();

//    emit sendSignal("FINISH COMMUNICATION");
    this->exec();
}


//------------------------以下是NDI相关函数------------------------//
/**
 * @brief There's no standard cross platform sleep() method prior to C++11
 */
void MyThread::sleepSeconds(unsigned numSeconds)
{
#ifdef _WIN32
    Sleep((DWORD)1000 * numSeconds); // Sleep(ms)
#else
    sleep(numSeconds); // sleep(sec)
#endif
}

/**
 * @brief Prints a debug message if a method call failed.
 * @details To use, pass the method name and the error code returned by the method.
 *          Eg: onErrorPrintDebugMessage("capi.initialize()", capi.initialize());
 *          If the call succeeds, this method does nothing.
 *          If the call fails, this method prints an error message to stdout.
 */
//当触发返回error时，该函数打印错误信息，并返回 bool false
bool MyThread::onErrorPrintDebugMessage(std::string methodName, int errorCode)
{
    if (errorCode < 0)
    {
        string str = methodName + " failed: " + capi->errorToString(errorCode);
        emit sendSignal(QString::fromStdString(str));
        return false;
    }
    return true;
}

bool MyThread::configurePassiveTools()
{
    QString str = "Configuring Passive Tools - Loading .rom Files...";
    emit sendSignal(str);

    bool config1 = loadTool(ToolDefine1);//探针设备架
    bool config2 = loadTool(ToolDefine2);//十字架设备架
    sleepSeconds(1);
    return config1&config2;
//    return config1;

}

bool MyThread::initializeAndEnableTools()
{
    std::cout << std::endl << "Initializing and enabling tools..." << std::endl;

    // Initialize and enable tools
    std::vector<PortHandleInfo> portHandles = capi->portHandleSearchRequest(PortHandleSearchRequestOption::NotInit);
    for (int i = 0; i < portHandles.size(); i++)
    {
        if(!onErrorPrintDebugMessage("capi.portHandleInitialize()", capi->portHandleInitialize(portHandles[i].getPortHandle())))
        {
            std::cout <<  "mark1" << std::endl;
            return false;
        }
        if(!onErrorPrintDebugMessage("capi.portHandleEnable()", capi->portHandleEnable(portHandles[i].getPortHandle()))){
            std::cout <<  "mark2" << std::endl;
            //此处最好是有个弹窗，打印：如果不满足独特性要求，是否继续
        }
    }

    // Print all enabled tools
    portHandles = capi->portHandleSearchRequest(PortHandleSearchRequestOption::Enabled);
    for (int i = 0; i < portHandles.size(); i++)
    {
        std::cout << portHandles[i].toString() << std::endl;
    }

    return true;
}

/**
* @brief Returns the string: "[tool.id] s/n:[tool.serialNumber]" used in CSV output
*/
std::string MyThread::getToolInfo(std::string toolHandle)
{
    // Get the port handle info from PHINF
    PortHandleInfo info = capi->portHandleInfo(toolHandle);

    // Return the ID and SerialNumber the desired string format
    std::string outputString = info.getToolId();
    outputString.append(" s/n:").append(info.getSerialNumber());
    return outputString;
}

std::string MyThread::toolDataToCSV(const ToolData &toolData)
{
    std::stringstream stream;
    stream << std::setprecision(toolData.PRECISION) << std::setfill('0');
    stream << "" << static_cast<unsigned>(toolData.frameNumber) << ","
           << "Port:" << static_cast<unsigned>(toolData.transform.toolHandle) << ",";
    stream << static_cast<unsigned>(toolData.transform.getFaceNumber()) << ",";

    if (toolData.transform.isMissing())
    {
        stream << "Missing,,,,,,,,";
    }
    else
    {
        stream << TransformStatus::toString(toolData.transform.getErrorCode()) << ","
               << toolData.transform.q0 << "," << toolData.transform.qx << "," << toolData.transform.qy << "," << toolData.transform.qz << ","
               << toolData.transform.tx << "," << toolData.transform.ty << "," << toolData.transform.tz << "," << toolData.transform.error;
    }

    // Each marker is printed as: status,tx,ty,tz
    stream << "," << toolData.markers.size();
    for ( int i = 0; i < toolData.markers.size(); i++)
    {
        stream << "," << MarkerStatus::toString(toolData.markers[i].status);
        if (toolData.markers[i].status == MarkerStatus::Missing)
        {
            stream << ",,,";
        }
        else
        {
            stream << "," << toolData.markers[i].x << "," << toolData.markers[i].y << "," << toolData.markers[i].z;
        }
    }
    return stream.str();
}

void MyThread::writeCSV(std::string fileName, int numberOfLines)
{
    // Assumption: tools are not enabled/disabled during CSV output
    std::vector<PortHandleInfo> portHandles = capi->portHandleSearchRequest(PortHandleSearchRequestOption::Enabled);
    if (portHandles.size() < 1)
    {
        std::cout << "Cannot write CSV file when no tools are enabled!" << std::endl;
        return;
    }

    // Lookup and store the serial number for each enabled tool
    std::vector<ToolData> enabledTools;
    for (int i = 0; i < portHandles.size(); i++)
    {
        enabledTools.push_back(ToolData());
        enabledTools.back().transform.toolHandle = (uint16_t) capi->stringToInt(portHandles[i].getPortHandle());
        enabledTools.back().toolInfo = getToolInfo(portHandles[i].getPortHandle());
    }

    // Start tracking
    std::cout << std::endl << "Entering tracking mode and collecting data for CSV file..." << std::endl;
    onErrorPrintDebugMessage("capi.startTracking()", capi->startTracking());

    // Print header information to the first line of the output file
    std::cout << std::endl << "Writing CSV file..." << std::endl;
    std::ofstream csvFile(fileName.c_str());
    csvFile << "#Tools,ToolInfo,Frame#,PortHandle,Face#,TransformStatus,Q0,Qx,Qy,Qz,Tx,Ty,Tz,Error,Markers,State,Tx,Ty,Tz" << std::endl;

    // Loop to gather tracking data and write to the file
    int linesWritten = 0;
    int previousFrameNumber = 0; // use this variable to avoid printing duplicate data with BX
    while (linesWritten < numberOfLines)
    {
        // Get new tool data using BX2
        std::vector<ToolData> newToolData = apiSupportsBX2 ? capi->getTrackingDataBX2("--6d=tools --3d=tools --sensor=none --1d=buttons") :
                                                             capi->getTrackingDataBX(TrackingReplyOption::TransformData | TrackingReplyOption::AllTransforms);

        // Update enabledTools array with new data
        for (int t = 0; t < enabledTools.size(); t++)
        {
            for (int j = 0; j < newToolData.size(); j++)
            {
                if (enabledTools[t].transform.toolHandle == newToolData[j].transform.toolHandle)
                {
                    // Copy the new tool data
                    newToolData[j].toolInfo = enabledTools[t].toolInfo; // keep the serial number
                    enabledTools[t] = newToolData[j]; // use the new data
                }
            }
        }

        // If we're using BX2 there's extra work to do because BX2 and BX use opposite philosophies.
        // BX will always return data for all enabled tools, but some of the data may be old: #linesWritten == # BX commands
        // BX2 never returns old data, but cannot guarantee new data for all enabled tools with each call: #linesWritten <= # BX2 commands
        // We want a CSV file with data for all enabled tools in each line, but this requires multiple BX2 calls.
        if (apiSupportsBX2)
        {
            // Count the number of tools that have new data
            int newDataCount = 0;
            for (int t = 0; t < enabledTools.size(); t++)
            {
                if (enabledTools[t].dataIsNew)
                {
                    newDataCount++;
                }
            }

            // Send another BX2 if some tools still have old data
            if (newDataCount < enabledTools.size())
            {
                continue;
            }
        }
        else
        {
            if (previousFrameNumber == enabledTools[0].frameNumber)
            {
                // If the frame number didn't change, don't print duplicate data to the CSV, send another BX
                continue;
            }
            else
            {
                // This frame number is different, so we'll print a line to the CSV, but remember it for next time
                previousFrameNumber = enabledTools[0].frameNumber;
            }
        }

        // Print a line of the CSV file if all enabled tools have new data
        csvFile << std::dec << enabledTools.size();
        for (int t = 0; t < enabledTools.size(); t++)
        {
            csvFile << "," << enabledTools[t].toolInfo << "," << toolDataToCSV(enabledTools[t]);
            enabledTools[t].dataIsNew = false; // once printed, the data becomes "old"
        }
        csvFile << std::endl;
        linesWritten++;
    }

    // Stop tracking and return to configuration mode
    onErrorPrintDebugMessage("capi.stopTracking()", capi->stopTracking());
}

void MyThread::printToolData(const ToolData &toolData)
{
    if (toolData.systemAlerts.size() > 0)
    {
        std::cout << "[" << toolData.systemAlerts.size() << " alerts] ";
        for (int a = 0; a < toolData.systemAlerts.size(); a++)
        {
            std::cout << toolData.systemAlerts[a].toString() << std::endl;
        }
    }

    if (toolData.buttons.size() > 0)
    {
        std::cout << "[buttons: ";
        for (int b = 0; b < toolData.buttons.size(); b++)
        {
            std::cout << ButtonState::toString(toolData.buttons[b]) << " ";
        }
        std::cout << "] ";
    }
    std::cout << toolDataToCSV(toolData) << std::endl;
}

void MyThread::printTrackingData()
{
    options = 0x0001;
    std::string continueornot = "Y";
    while(continueornot == "Y")
    {
        // Demonstrate TX command: ASCII command sent, ASCII reply received
        std::cout << capi->getTrackingDataTX(options) << std::endl;

        // Demonstrate BX or BX2 command

        std::vector<ToolData> toolData =  capi->getTrackingDataBX(options);
        // Print to stdout in similar format to CSV
        std::cout << "[alerts] [buttons] Frame#,ToolHandle,Face#,TransformStatus,Q0,Qx,Qy,Qz,Tx,Ty,Tz,Error,#Markers,State,Tx,Ty,Tz" << std::endl;
        for (int i = 0; i < toolData.size(); i++)
        {

            printToolData(toolData[i]);
        }
        std::cout << "press \"Y\"to continue"<<std::endl;
        std::cin >> continueornot;
    }

    // Stop tracking (back to configuration mode)
    std::cout << std::endl << "Leaving tracking mode and returning to configuration mode..." << std::endl;
    onErrorPrintDebugMessage("capi.stopTracking()", capi->stopTracking());
}

bool MyThread::loadTool(const char *toolDefinitionFilePath)
{
    // Request a port handle to load a passive tool into
    int portHandle = capi->portHandleRequest();
    if(!onErrorPrintDebugMessage("capi.portHandleRequest()", portHandle))
        return false;

    // Load the .rom file using the previously obtained port handle
    return capi->loadSromToPort(toolDefinitionFilePath, portHandle);
}

void MyThread::configureUserParameters()
{
    std::cout << capi->getUserParameter("Param.User.String0") << std::endl;
    onErrorPrintDebugMessage("capi.setUserParameter(Param.User.String0, customString)", capi->setUserParameter("Param.User.String0", "customString"));
    std::cout << capi->getUserParameter("Param.User.String0") << std::endl;
    onErrorPrintDebugMessage("capi.setUserParameter(Param.User.String0, emptyString)", capi->setUserParameter("Param.User.String0", ""));
}

void MyThread::simulateAlerts(uint32_t simulatedAlerts)
{
    // Simulate alerts if any were requested
    if (simulatedAlerts > 0x0000)
    {
        std::cout << std::endl << "Simulating system alerts..." << std::endl;
        std::stringstream stream;
        stream << simulatedAlerts;
        onErrorPrintDebugMessage("capi.setUserParameter(Param.Simulated Alerts, alerts)", capi->setUserParameter("Param.Simulated Alerts", stream.str()));
        std::cout << capi->getUserParameter("Param.Simulated Alerts") << std::endl;
    }
}

void MyThread::determineApiSupportForBX2()
{
    // Lookup the API revision
    std::string response = capi->getApiRevision();

    // Refer to the API guide for how to interpret the APIREV response
    char deviceFamily = response[0];
    int majorVersion = capi->stringToInt(response.substr(2,3));

    // As of early 2017, the only NDI device supporting BX2 is the Vega
    // Vega is a Polaris device with API major version 003
    if ( deviceFamily == 'G' && majorVersion >= 3)
    {
        apiSupportsBX2 = true;
    }
}



