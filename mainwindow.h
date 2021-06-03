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

    void printData()
    {
        if(NDIdataIsValid)
        {
            std::cout<<"point data test print :"<<pt.x<<std::endl;
            std::cout<<"line data test print :"<<line.p1.x<<std::endl;
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
