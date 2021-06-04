#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "mythread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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
    };

    class NDIData
    {
    public:
    bool NDIdataIsValid;
    NDILine line1;
    NDILine line2;
    void JsontoNDIData(QByteArray arr)
    {
        QJsonParseError dataJsonError;
        //QTJSON所有的元素
        QJsonDocument document = QJsonDocument::fromJson(arr, &dataJsonError);
        QJsonObject  dataJson= document.object();
        //检查json是否有错误
        if (dataJsonError.error != QJsonParseError::NoError)
        {
            qDebug()<<"格式错误";
            NDIdataIsValid = false;
            return;
        }
        //解析json
        else
        {

            QJsonValue dataIsValid = dataJson.value("dataIsValid");
            if(dataIsValid.toBool())
            {
                NDIdataIsValid = true;
                //探针线赋值
                QJsonValue probeTx = dataJson.value("probeTx");
                line1.p1.x = probeTx.toDouble();
                QJsonValue probeTy = dataJson.value("probeTy");
                line1.p1.y = probeTy.toDouble();
                QJsonValue probeTz = dataJson.value("probeTz");
                line1.p1.z = probeTz.toDouble();

                QJsonValue probeMx = dataJson.value("probeMx");
                line1.p2.x = probeMx.toDouble();
                QJsonValue probeMy = dataJson.value("probeMy");
                line1.p2.y = probeMy.toDouble();
                QJsonValue probeMz = dataJson.value("probeMz");
                line1.p2.z = probeMz.toDouble();

                //标志架线赋值
                QJsonValue rigidx1 = dataJson.value("rigidx1");
                line2.p1.x = rigidx1.toDouble();
                QJsonValue rigidy1 = dataJson.value("rigidy1");
                line2.p1.y = rigidy1.toDouble();
                QJsonValue rigidz1 = dataJson.value("rigidz1");
                line2.p1.z = rigidz1.toDouble();

                QJsonValue rigidx2 = dataJson.value("rigidx2");
                line2.p2.x = rigidx2.toDouble();
                QJsonValue rigidy2 = dataJson.value("rigidy2");
                line2.p2.y = rigidy2.toDouble();
                QJsonValue rigidz2 = dataJson.value("rigidz2");
                line2.p2.z = rigidz2.toDouble();

            }

            else
            {
                NDIdataIsValid = false;
            }

        }

    }

    void printData()
    {
        if(NDIdataIsValid)
        {
            std::cout<<"----------------------------------"<<std::endl;
            std::cout<<"point data 1X :"<<line1.p1.x<<std::endl;
            std::cout<<"point data 1Y :"<<line1.p1.y<<std::endl;
            std::cout<<"point data 1Z :"<<line1.p1.z<<std::endl;

            std::cout<<"point data 2X :"<<line1.p2.x<<std::endl;
            std::cout<<"point data 2Y :"<<line1.p2.y<<std::endl;
            std::cout<<"point data 2Z :"<<line1.p2.z<<std::endl;

            std::cout<<"line data 1X :"<<line2.p1.x<<std::endl;
            std::cout<<"line data 1Y :"<<line2.p1.y<<std::endl;
            std::cout<<"line data 1Z :"<<line2.p1.z<<std::endl;

            std::cout<<"line data 2X :"<<line2.p2.x<<std::endl;
            std::cout<<"line data 2Y :"<<line2.p2.y<<std::endl;
            std::cout<<"line data 2Z :"<<line2.p2.z<<std::endl;
        }
        else if(!NDIdataIsValid)
        {
            std:cout<<"collect error"<<std::endl;
        }
    }

    };


signals:
    void sendSignal(QString);

public slots:
    void receiveLogSlots(QString);
    void receiveDataSlots(QByteArray);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    MyThread *myThread;
};

#endif // MAINWINDOW_H
