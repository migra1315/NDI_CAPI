#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //MainWindow类内，信息传递连接
    connect(this,&MainWindow::sendSignal,this,&MainWindow::receiveLogSlots);

    //初始化NDI连接、描述文件信息
    string hostname = "COM3";
    char *ToolDefine1 = "G:\\projects\\demo\\demo6\\res\\sroms\\probe.rom";
    char *ToolDefine2 = "G:\\projects\\demo\\demo6\\res\\sroms\\cross.rom";

    //0 初始化一些基础的操作
    //0.1 下拉菜单,退出
    connect(ui->actionExit,&QAction::triggered,this,&MainWindow::close);

    //新建并启动线程
    myThread = new MyThread();
    myThread->hostname = hostname;
    myThread->ToolDefine1 = ToolDefine1;
    myThread->ToolDefine2 = ToolDefine2;

    connect(myThread,&MyThread::sendSignal,this,&MainWindow::receiveLogSlots);
    connect(myThread,&MyThread::sendDataSignal,this,&MainWindow::receiveDataSlots);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::receiveDataSlots(QByteArray arr)
{
    NDIData data;
    data.JsontoNDIData(arr);
    data.printData();
}

void MainWindow::receiveLogSlots(QString str)
{
    ui->listWidget->addItem(str);
}

void MainWindow::on_pushButton_clicked()
{
    myThread->start();
}
