#ifndef PROCESSTRACKINGDATA_H
#define PROCESSTRACKINGDATA_H

#include <string>
#include <iostream>
using namespace std;

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

#endif // PROCESSTRACKINGDATA_H
