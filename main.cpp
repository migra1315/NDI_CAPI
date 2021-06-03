#include "mainwindow.h"
#include <QApplication>
#include "processtrackingdata.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    string str = "0201+07082-04513-01855-05101+041595-057080-190190+02477000000310000014D040+032681-029273-196806+038254-031402-195542+036870-036707-194443+037965-041486-193249\n02+02762-01462-00466+09487+033067-022295-195013+01564000000310000014D040+033067-022310-195004+027787-023550-195330+030728-017309-196164+023842-018192-196965\n0000";
//    processTrackingData track(str);
//    if(track.evaluateData()){
//        qDebug()<<QString::fromStdString(track.getProbeAttitude());
//        qDebug()<<QString::fromStdString(track.getRigidCoordinate());
//        qDebug()<<"out";
//    }

    return a.exec();
}
