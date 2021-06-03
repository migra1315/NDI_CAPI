# Introduction

在NDI官方提供的API demo基础上做了改写，使用Qt实现了：

1. 在子线程MyThtread内实现对NDI设备的连接、数据读取和处理；
2. 以Json格式将需求数据（包括tool_1的尖端坐标、位姿，tool_2的标志物坐标）emit至主线程，在主线程中实现对数据的解析；
3. 以QString格式将打印的（大部分）调试信息emit至主线程，在UI界面的ListWedgit上显示；
4. 构建NDIData类，类中储存tool_1尖端坐标，tool_2共4个标志物坐标，以及数据是否有效的bool 标示

# MyThread流程

## NDI返回数据格式

发送指令 TX 0x0001|0x0008

```
<# of Handles><Handle 1><Reply Opt 0001 Data><Reply Opt 0008 Data><LF>
<Handle 2><Reply Option 0001 Data>...<Reply Option 0008 Data><LF>
<System Status><CRC16><CR>
```

当有两个工具时，返回正常数据长度为320，如下：

```
"0201+07082-04513-01855-05101+041595-057080-190190+02477000000310000014D040+032681-029273-196806+038254-031402-195542+036870-036707-194443+037965-041486-193249\n02+02762-01462-00466+09487+033067-022295-195013+01564000000310000014D040+033067-022310-195004+027787-023550-195330+030728-017309-196164+023842-018192-196965\n0000"
```

无法追踪工具时，相应Handle的数据会变为MISSING或DISABLE

## 虚拟数据

为方便在无NDI连接时，方便接入系统测试，提供一组虚拟坐标，可以返回和真实情况相同的数值。

```c++
void MyThread::run(){
    bool ProduceORSimulate = false;
    ……
}

```

### 处理数据

run()方法经判断后，执行 bool MyThread::startTrackingSimulate() 。

1. 调用processTrackingData类对定义的虚拟数据string outdata进行处理；

```c++
processTrackingData track(outdata);
```

processTrackingData类实现方法：初始化类时传入待处理的数据，构造函数完成

- 调用函数1，判断数据是否有效，依据，传入数据的长度==320且不存在MISSING、DISABLE
- 调用函数2，3，4，根据相应的信息位置，提取出tool_1（探针）尖端坐标、位姿，tool_2标准物坐标，存入相应成员变量中

```c++
class processTrackingData
{
public:
    processTrackingData(string str);
    string data;
    bool dataIsValid;
    //获取尖端的坐标(x,y,z)
    double probeTx;
    double probeTy;
    double probeTz;
    //探针的位姿四元数(Q0, Qx, Qy,Qz)
    double probeQ0;
    double probeQx;
    double probeQy;
    double probeQz;
    //刚性体的坐标，4*（x,y,z）
    double rigidx1;double rigidy1;double rigidz1;
    double rigidx2;double rigidy2;double rigidz2;
    double rigidx3;double rigidy3;double rigidz3;
    double rigidx4;double rigidy4;double rigidz4;

    //开始处理数据
    //1. 判断NDI返回的数据是否符合要求
    void evaluateData();
    //2. 第一个工具是探针，获取尖端的坐标(x,y,z)
    void getProbeCoordinate();
    //3. 探针的位姿四元数(Q0, Qx, Qy,Qz)
    void getProbeAttitude();
    //4. 刚性体的坐标，4*（x,y,z）
    void getRigidCoordinate();

};
```

### 构建Json体，并emit至主线程

1. 当processTrackingData.dataIsValid==true，即NDI返回数据有效时，构建Json体：

```c++
dataJson.insert("dataIsValid",track.dataIsValid);
dataJson.insert("probeTx",track.probeTx);
dataJson.insert("probeTy",track.probeTy);
dataJson.insert("probeTz",track.probeTz);

dataJson.insert("probeQ0",track.probeQ0);
dataJson.insert("probeQx",track.probeQx);
dataJson.insert("probeQy",track.probeQy);
dataJson.insert("probeQz",track.probeQz);

dataJson.insert("rigidx1",track.rigidx1);
dataJson.insert("rigidy1",track.rigidy1);
dataJson.insert("rigidz1",track.rigidz1);

dataJson.insert("rigidx2",track.rigidx2);
dataJson.insert("rigidy2",track.rigidy2);
dataJson.insert("rigidz2",track.rigidz2);

dataJson.insert("rigidx3",track.rigidx3);
dataJson.insert("rigidy3",track.rigidy3);
dataJson.insert("rigidz3",track.rigidz3);

dataJson.insert("rigidx4",track.rigidx4);
dataJson.insert("rigidy4",track.rigidy4);
dataJson.insert("rigidz4",track.rigidz4);
```

2. 当processTrackingData.dataIsValid==false，即NDI返回数据有问题时，构建Json体：

```c++
dataJson.insert("dataIsValid",track.dataIsValid);
```

3. 以QByteArry类型经 sendDataSignal(QByteArry)emit至主函数；

```c++
emit sendDataSignal(dataJsonDocBA);//MyThread
//--------
 connect(myThread,&MyThread::sendDataSignal,this,&MainWindow::receiveDataSlots);//MainWindow
```

## 真实数据

```c++
bool ProduceORSimulate = true;
```

### 调用startConfig()

注意：该过程包括了以下几个步骤，当某一步骤出现错误时，bool startConfig()函数直接返回false，子线程中止。

- 与NDI连接
- 初始化系统
- 加载工具描述文件
- 激活工具描述文件
- 开启追踪状态

### 获取数据

执行 bool MyThread::startTracking(int replyOption)，int replyOption即通过TX指令送入NDI的option，决定了NDI的返回数据（见NDI API Guide）

之后同2.2.1和2.2.2



## 定时器操作

```c++
timer = new QTimer();
connect(timer,&QTimer::timeout,[=](){
    startTrackingSimulate();
});
timer->start(2000);
```

在timer->start(2000)中设置以毫秒为单位的TimeEvent调用时长。

# 方法中的通信机制

1. 主线程UI中调用MyThread.run()，启动子线程

```c++
connect(ui->actionExit,&QAction::triggered,this,&MainWindow::close);
```

2. 主线程中slots响应子线程中signal，分别处理：
   1. 子线程中以QString传来的Log/调试信息，打印在ListWedgit中
   2. 子线程中以QByteArry传来的NDI数据信息，在NDIData类中进行解析

```c++
connect(myThread,&MyThread::sendSignal,this,&MainWindow::receiveLogSlots);
   connect(myThread,&MyThread::sendDataSignal,this,&MainWindow::receiveDataSlots);
```

3. CombineAPI类（NDI的通信主要实现类）产生调试信息时，与MyThread通信，slot函数emit (2)中的函数，将调试信息送至主线程

```c++
connect(capi,&CombinedApi::sendSignalApi,this,&MyThread::receiveApiMsgSlot);
connect(capi,&CombinedApi::sendSignalApiConst,this,&MyThread::receiveApiMsgSlot);
```

# 主线程NDIData类

## 数据结构和实现方法

NDIData类是定义在MainWindow中的子类（或许不是这个名字），同时定义的还有NDIPoint类和NDILine类

```c++
    class NDIPoint
    {
    public:
        double x;
        double y;
        double z;
    };

    class NDILine
    {
    public:
        NDIPoint p1;
        NDIPoint p2;
        NDIPoint p3;
        NDIPoint p4;
    };

    class NDIData
    {
        public:
        bool NDIdataIsValid;
        NDIPoint pt;
        NDILine line;
        void JsontoNDIData(QByteArray arr)
        {
            ……
        }

        void printData()
        {
            ……
        }

    };
```

在主线程中接收到QByteArray类型的Json数据时，初始化NDIData类，并调用NDIData.JsontoNDIData(QByteArray)方法，完成成员变量的赋值。

NDIData类中存储了1个NDIPoint类（存储tool_1探针尖端坐标），1个NDILine类（内有4个NDIPoint类，存储tool_2共4个标志球坐标），以及一个 bool NDIdataIsValid，**标志获取的数据是不是有效**。

## Json解析方法

```c++
 void JsontoNDIData(QByteArray arr)
    {
        QJsonParseError dataJsonError;
        //QTJSON所有的元素
        QJsonDocument document = QJsonDocument::fromJson(arr, &dataJsonError);
        QJsonObject  dataJson= document.object();
        //检查json是否有错误
        if (dataJsonError.error != QJsonParseError::NoError)
        {
            NDIdataIsValid = false;
            qDebug()<<"格式错误";
            return;
        }
        //解析json
        else
        {
            QJsonValue dataIsValid = dataJson.value("dataIsValid");
            if(dataIsValid.toBool())
            {
                //给点坐标pt赋值
                QJsonValue probeTx = dataJson.value("probeTx");
                pt.x = probeTx.toDouble();
                QJsonValue probeTy = dataJson.value("probeTy");
                pt.y = probeTy.toDouble();
                QJsonValue probeTz = dataJson.value("probeTz");
                pt.z = probeTz.toDouble();
                //给线/框坐标赋值
                QJsonValue rigidx1 = dataJson.value("rigidx1");
                line.p1.x = rigidx1.toDouble();
                QJsonValue rigidy1 = dataJson.value("rigidx1");
                line.p1.y = rigidy1.toDouble();
                QJsonValue rigidz1 = dataJson.value("rigidx1");
                line.p1.z = rigidz1.toDouble();

                QJsonValue rigidx2 = dataJson.value("rigidx1");
                line.p2.x = rigidx2.toDouble();
                QJsonValue rigidy2 = dataJson.value("rigidx1");
                line.p2.y = rigidy2.toDouble();
                QJsonValue rigidz2 = dataJson.value("rigidx1");
                line.p2.z = rigidz2.toDouble();

                QJsonValue rigidx3 = dataJson.value("rigidx1");
                line.p3.x = rigidx3.toDouble();
                QJsonValue rigidy3 = dataJson.value("rigidx1");
                line.p3.y = rigidy3.toDouble();
                QJsonValue rigidz3 = dataJson.value("rigidx1");
                line.p3.z = rigidz3.toDouble();

                QJsonValue rigidx4 = dataJson.value("rigidx1");
                line.p4.x = rigidx4.toDouble();
                QJsonValue rigidy4 = dataJson.value("rigidx1");
                line.p4.y = rigidy4.toDouble();
                QJsonValue rigidz4 = dataJson.value("rigidx1");
                line.p4.z = rigidz4.toDouble();

            }

            else
            {
                NDIdataIsValid = false;
            }

        }

    }

```

1. 数据解析错误时，NDIdataIsValid = false，返回；
2. 数据解析正确，但数据无效时，即JSON中“dataIsValid”Key的值为falses时，NDIdataIsValid = false，返回；
3. 数据解析正确，但数据有效时，给NDIData类中参数们赋值。

# 其他要注意的地方

该线程中需要交互的地方主要有以下几个：

1. NDI连接端口和工具描述文件，在主线程中定义

```c++
string hostname = "COM3";
char *ToolDefine1 = "G:\\projects\\demo\\demo6\\res\\sroms\\probe.rom";
char *ToolDefine2 = "G:\\projects\\demo\\demo6\\res\\sroms\\cross.rom";
```

并赋值给MyThread类中的成员变量

```c++
myThread = new MyThread();
myThread->hostname = hostname;
myThread->ToolDefine1 = ToolDefine1;
myThread->ToolDefine2 = ToolDefine2;
```

2. 获取坐标的时候，传输给NDI的指令

```c++
bool MyThread::startTracking(int replyOption)
```

在MyThread.run()中，该函数是以固定值调用的，没有预留与主线程交互接口

```c++
startTracking(0x0001|0x0008);
```

3. 使用NDI追踪真实数据或调用虚拟数据也是固定的，没有预留与主线程交互接口

```c++
bool ProduceORSimulate = true;//调用NDI接口，使用真实数据
bool ProduceORSimulate = false;//使用模拟数据
```

另，模拟数据 bool MyThread::startTrackingSimulate() 方法内部，定义了有效和无效数据各一行，可以根据需要注释掉一组。在./res/collectData.txt中提供了一组NDI采集的真实数据，可供使用。

4. 开发环境：

Qt 5.10.1+MSVC2013，NDI_CAPI.pro是其项目文件；

另外，在VS2013下，项目也通过了运行测试，NDI_CAPI.sln是其项目文件